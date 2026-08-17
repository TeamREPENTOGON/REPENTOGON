"""Signature re-derivation, v2 - anchor-guided local search.

v1 (`sig_reporter.py`) searched the whole new binary for k-grams that were *globally* unique.
That is a harsh filter: most grams from a function body also appear elsewhere, so most
functions produced no anchor at all and were reported unsolvable (86.7%).

v2 narrows the search first. `name_anchors.py` yields a set of functions whose identity is
known on both builds (matched by the binary's own debug strings). Sorting those by address
gives a piecewise map of how far code drifted between versions - the drift is monotonic but
not uniform, so we interpolate between the two bracketing anchors to predict where a given
function landed, then search only a window around that prediction.

Inside a small window, a gram no longer needs to be globally unique - only locally
unambiguous - which is a far weaker requirement and recovers many more functions.

Structural validation from v1 is kept: prologue must survive, candidate must sit at a real
function boundary, emitted pattern must match exactly once in the whole binary.
"""
import sys, os, re, struct
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import name_anchors as NA

K = 10
WINDOW = 96
MIN_VOTES = 2
# +/- window around the interpolated prediction. With only the ~40 debug-string anchors this
# needed to be large (128 KB); the RTTI anchors make predictions far tighter, and a smaller
# window also means fewer spurious gram matches.
SEARCH_RADIUS = 0x8000    # 32 KB


def extract_signatures(zhl_dir):
    import glob
    sig_re = re.compile(r'^\s*"([0-9a-fA-F?]{8,})"\s*:')
    out = []
    for path in sorted(glob.glob(os.path.join(zhl_dir, "*.zhl"))):
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            for lineno, line in enumerate(f, 1):
                s = line.lstrip()
                if s.startswith("//") or s.startswith("\\"):
                    continue
                m = sig_re.match(line)
                if m:
                    out.append((os.path.basename(path), lineno, m.group(1)))
    return out


def patt_to_regex(patt):
    parts, i = [], 0
    while i < len(patt):
        if patt[i] == '?':
            i += 2 if i + 1 < len(patt) and patt[i+1] == '?' else 1
            parts.append(b'.')
        else:
            if i + 1 >= len(patt):
                return None
            parts.append(re.escape(bytes([int(patt[i:i+2], 16)])))
            i += 2
    return re.compile(b''.join(parts), re.DOTALL)


def build_drift_table(old_path, new_path, rtti_file=None):
    """Sorted [(old_va, new_va)] anchors.

    Two independent sources, merged:
      - debug-string names (few, very trustworthy)
      - RTTI vtable slots (many; `rtti_vtables.py --dump` writes rtti_anchors.txt)
    """
    pairs = []
    old_map = NA.build_name_map(old_path)
    new_map = NA.build_name_map(new_path)
    for n in set(old_map) & set(new_map):
        if len(old_map[n]) == 1 and len(new_map[n]) == 1:
            pairs.append((next(iter(old_map[n])), next(iter(new_map[n]))))

    if rtti_file and os.path.exists(rtti_file):
        with open(rtti_file) as f:
            for line in f:
                parts = line.split()
                if len(parts) >= 2:
                    try:
                        pairs.append((int(parts[0], 16), int(parts[1], 16)))
                    except ValueError:
                        pass

    # de-duplicate by old VA, then sort
    seen = {}
    for ova, nva in pairs:
        seen.setdefault(ova, nva)
    pairs = sorted(seen.items())

    # Real code drift is monotonic, so the consistent anchors are the longest strictly
    # increasing subsequence of new VAs. A greedy "drop anything that goes backwards" pass
    # is wrong here: vtable slots point at functions scattered across the image, so one early
    # outlier would discard almost everything after it (observed: 1910 anchors -> 5).
    return longest_increasing(pairs)


def longest_increasing(pairs):
    """Longest strictly-increasing-by-new-VA subsequence (patience sorting, O(n log n))."""
    import bisect
    if not pairs:
        return []
    tails_new, tails_idx, prev = [], [], [-1] * len(pairs)
    for i, (_ova, nva) in enumerate(pairs):
        j = bisect.bisect_left(tails_new, nva)
        if j == len(tails_new):
            tails_new.append(nva)
            tails_idx.append(i)
        else:
            tails_new[j] = nva
            tails_idx[j] = i
        prev[i] = tails_idx[j - 1] if j > 0 else -1
    out, k = [], tails_idx[-1]
    while k >= 0:
        out.append(pairs[k])
        k = prev[k]
    return out[::-1]


def predict(drift, old_va):
    """Interpolate the expected new VA for an old VA using the drift table."""
    if not drift:
        return None
    if old_va <= drift[0][0]:
        return old_va + (drift[0][1] - drift[0][0])
    if old_va >= drift[-1][0]:
        return old_va + (drift[-1][1] - drift[-1][0])
    lo, hi = 0, len(drift) - 1
    while lo + 1 < hi:
        mid = (lo + hi) // 2
        if drift[mid][0] <= old_va:
            lo = mid
        else:
            hi = mid
    (o0, n0), (o1, n1) = drift[lo], drift[hi]
    if o1 == o0:
        return old_va + (n0 - o0)
    frac = (old_va - o0) / (o1 - o0)
    return int(n0 + frac * (n1 - n0))


def rederive_local(old, new, old_off, lo_off, hi_off):
    """Vote for a function start, searching only within [lo_off, hi_off) of `new`."""
    window = old[old_off:old_off + WINDOW]
    region = new[lo_off:hi_off]
    votes = Counter()
    for i in range(0, len(window) - K):
        gram = window[i:i+K]
        if len(set(gram)) < 5:
            continue
        pos = region.find(gram)
        if pos < 0:
            continue
        if region.find(gram, pos + 1) >= 0:
            continue   # ambiguous even locally
        votes[lo_off + pos - i] += 1
    if not votes:
        return None, 0
    best, n = votes.most_common(1)[0]
    return (best, n) if n >= MIN_VOTES else (None, n)


SNAP_RADIUS = 32
MAX_WILDCARD_RATIO = 0.40   # reject patterns that are mostly ??


def snap_to_boundary(old, new, old_off, new_off):
    """Find the nearest offset to `new_off` that looks like the same function's start.

    Requires the old function's first bytes to reappear and the candidate to sit at a real
    boundary. Returns None when nothing nearby qualifies - i.e. the function's prologue
    genuinely changed, which is a real semantic difference and a job for a human.
    """
    head = old[old_off:old_off + 4]
    best = None
    for delta in range(0, SNAP_RADIUS + 1):
        for cand in ({new_off} if delta == 0 else {new_off - delta, new_off + delta}):
            if cand < 4 or cand + WINDOW > len(new):
                continue
            if new[cand:cand + 4] != head:
                continue
            if not NA_is_start(new, cand):
                continue
            if best is None:
                best = cand
        if best is not None:
            return best
    return None


def make_pattern(old, new, old_off, new_off, length):
    if old[old_off:old_off+3] != new[new_off:new_off+3]:
        return None
    if not (NA_is_start(old, old_off) and NA_is_start(new, new_off)):
        return None
    out = []
    for i in range(length):
        ob, nb = old[old_off + i], new[new_off + i]
        out.append("%02x" % nb if ob == nb else "??")
    if out[0] == "??":
        return None
    # A pattern that is mostly wildcards is not evidence of anything - it means the two
    # bodies barely agree, and "grow until unique" will happily pad with ?? until some
    # match falls out. Semantic verification showed these are usually plain wrong.
    if out.count("??") > MAX_WILDCARD_RATIO * len(out):
        return None
    return "".join(out)


def NA_is_start(buf, off):
    if off < 4:
        return False
    if buf[off-4:off] in (b"\xcc\xcc\xcc\xcc", b"\x90\x90\x90\x90"):
        return True
    return any(buf.startswith(p, off) for p in
               (b"\x55\x8b\xec", b"\x53\x8b\xdc", b"\x56\x8b\xf1", b"\x8b\xff\x55"))


def main():
    zhl_dir, old_path, new_path = sys.argv[1], sys.argv[2], sys.argv[3]
    limit = int(sys.argv[4]) if len(sys.argv) > 4 else 0

    old, old_ib, old_secs = NA.load_pe(old_path)
    new, new_ib, new_secs = NA.load_pe(new_path)
    old_o2v, _ = NA.make_converters(old_ib, old_secs)
    new_o2v, new_v2o = NA.make_converters(new_ib, new_secs)

    rtti_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), "rtti_anchors.txt")
    print("building drift table (debug-string names + RTTI vtable slots)...")
    drift = build_drift_table(old_path, new_path, rtti_file)
    print(f"  usable monotonic anchors: {len(drift)}\n")

    sigs = extract_signatures(zhl_dir)
    broken = []
    for fname, lineno, patt in sigs:
        rx = patt_to_regex(patt)
        if rx is None:
            continue
        if len(rx.findall(new)) != 1:
            m = rx.search(old)
            if m and len(rx.findall(old)) == 1:
                broken.append((fname, lineno, patt, m.start()))
    if limit:
        broken = broken[:limit]
    print(f"Broken-on-new signatures to attempt: {len(broken)}\n")

    solved = failed = 0
    why = Counter()
    rows = []
    for fname, lineno, patt, old_off in broken:
        ova = old_o2v(old_off)
        pred_va = predict(drift, ova) if ova else None
        pred_off = new_v2o(pred_va) if pred_va else None
        if pred_off is None:
            failed += 1; why["no prediction possible"] += 1
            continue
        lo = max(0, pred_off - SEARCH_RADIUS)
        hi = min(len(new), pred_off + SEARCH_RADIUS)
        new_off, votes = rederive_local(old, new, old_off, lo, hi)
        if new_off is None:
            failed += 1
            why["no locally-unambiguous grams" if votes == 0
                else "too few agreeing votes"] += 1
            continue
        # A gram vote yields `pos - i`, which assumes the new function's internal layout is
        # byte-identical up to that gram. One extra/missing byte earlier in the body (a wider
        # immediate, say) shifts that estimate and lands us mid-function. So snap to the
        # nearest real boundary whose prologue matches the old one.
        snapped = snap_to_boundary(old, new, old_off, new_off)
        if snapped is None:
            failed += 1; why["prologue changed / not a function boundary"] += 1
            continue
        new_off = snapped

        # Grow the pattern until it is unique rather than giving up at a fixed length.
        sig_len = max(16, len(patt.replace("?", "")) // 2)
        newpatt = None
        for length in range(min(sig_len + 8, WINDOW), WINDOW + 1, 8):
            cand = make_pattern(old, new, old_off, new_off, length)
            if cand is None:
                break
            rx = patt_to_regex(cand)
            if rx and len(rx.findall(new)) == 1:
                newpatt = cand
                break
        if newpatt is not None:
            solved += 1
            rows.append((fname, lineno, patt, newpatt, votes,
                         abs(new_off - pred_off), old_o2v(old_off), new_o2v(new_off)))
        else:
            failed += 1; why["emitted pattern not unique"] += 1

    # Collision check: two distinct source signatures must not resolve to the same function.
    # Overloads are the usual culprit (observed with AnimationState.zhl:19 / :22) - each
    # pattern verifies as unique on its own, yet they cannot both be correct.
    by_target = {}
    for r in rows:
        by_target.setdefault(r[3], []).append(r)
    collided = [g for g in by_target.values() if len(g) > 1]
    if collided:
        bad = {id(r) for g in collided for r in g}
        rows = [r for r in rows if id(r) not in bad]
        n = sum(len(g) for g in collided)
        solved -= n
        failed += n
        why["collides with another signature (overload?)"] += n

    total = len(broken)
    print("=== RESULT (v2, anchor-guided) ===")
    print(f"  auto-rederived & verified unique : {solved:4d}  ({100.0*solved/total:5.1f}%)")
    print(f"  needs manual RE                  : {failed:4d}  ({100.0*failed/total:5.1f}%)")
    print("\n  why the rest failed:")
    for reason, n in why.most_common():
        print(f"    {n:4d}  ({100.0*n/total:5.1f}%)  {reason}")
    print()
    # Emit candidates so they can be semantically verified and human-reviewed.
    out_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                            "rederived_candidates.txt")
    with open(out_path, "w") as f:
        f.write("# old_va new_va source_file:line | old_pattern -> new_pattern\n")
        f.write("# UNVERIFIED machine output: each line is a hypothesis, not a result.\n")
        for fname, lineno, oldp, newp, votes, err, ova, nva in rows:
            if ova and nva:
                f.write(f"{ova:08X} {nva:08X} {fname}:{lineno} | {oldp} -> {newp}\n")
    print(f"wrote {len(rows)} candidates to {os.path.basename(out_path)}\n")

    print("Sample:")
    for fname, lineno, oldp, newp, votes, err, _ova, _nva in rows[:10]:
        print(f"\n  {fname}:{lineno}  (votes {votes}, prediction off by {err} bytes)")
        print(f"    old: {oldp[:64]}")
        print(f"    new: {newp[:64]}")


if __name__ == "__main__":
    main()
