"""Automated ZHL signature re-derivation from one game build to another.

Idea: a signature that breaks across versions usually points at a function whose *body*
is still largely intact - only embedded absolute addresses/immediates moved. So:

  1. Locate the function in the OLD build using its existing (working) signature.
  2. Take a window of bytes from that location.
  3. Find rare k-grams from that window that occur exactly once in the NEW build.
     Each such gram votes for a candidate function start in the new build.
  4. If enough grams agree on the same start, align old vs new windows and emit a new
     pattern with ?? wildcards wherever the bytes differ.
  5. Verify the emitted pattern matches exactly once in the new build.

Anything without a clear consensus is reported as needing manual RE - the tool never
guesses silently.
"""
import sys, os, re, glob
from collections import Counter

K = 12            # k-gram length used for anchoring
WINDOW = 96       # bytes of function body to consider
MIN_VOTES = 3     # distinct grams that must agree on the same start
MAX_GRAMS = 24    # cap grams tested per function (runtime)


def extract_signatures(zhl_dir):
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


def count_upto2(hay, needle):
    """Occurrences of needle, capped at 2 (we only care about unique vs not)."""
    first = hay.find(needle)
    if first < 0:
        return 0
    return 1 if hay.find(needle, first + 1) < 0 else 2


def rederive(old, new, old_off):
    """Try to locate the same function in `new`. Returns (new_off, votes) or (None, 0)."""
    window = old[old_off:old_off + WINDOW]
    votes = Counter()
    tested = 0
    for i in range(0, len(window) - K):
        if tested >= MAX_GRAMS:
            break
        gram = window[i:i+K]
        # skip low-entropy grams (padding, zero runs) - they anchor nothing
        if len(set(gram)) < 5:
            continue
        tested += 1
        pos = new.find(gram)
        if pos < 0:
            continue
        if new.find(gram, pos + 1) >= 0:
            continue  # not unique, useless as an anchor
        votes[pos - i] += 1
    if not votes:
        return None, 0
    best, n = votes.most_common(1)[0]
    if n < MIN_VOTES or best < 0 or best + WINDOW > len(new):
        return None, n
    return best, n


def is_function_start(buf, off):
    """Heuristic: a real function start is preceded by padding, or opens with a
    recognised x86 prologue. Guards against anchoring mid-function."""
    if off < 4:
        return False
    pad = buf[off-4:off]
    if pad in (b"\xcc\xcc\xcc\xcc", b"\x90\x90\x90\x90"):
        return True
    prologues = (b"\x55\x8b\xec", b"\x53\x8b\xdc", b"\x56\x8b\xf1",
                 b"\x8b\xff\x55", b"\x55\x8b\xe5")
    return any(buf.startswith(p, off) for p in prologues)


def make_pattern(old, new, old_off, new_off, length):
    """Emit a hex pattern covering `length` bytes, wildcarding bytes that differ.

    Returns None if the candidate fails structural validation - we would rather report
    'needs manual RE' than emit a plausible-looking signature pointing at the wrong code.
    """
    # The prologue must survive. If the first bytes disagree we are almost certainly
    # anchored mid-function (this is what produced false positives before this check).
    if old[old_off:old_off+3] != new[new_off:new_off+3]:
        return None
    if not (is_function_start(old, old_off) and is_function_start(new, new_off)):
        return None
    out = []
    for i in range(length):
        ob = old[old_off + i]
        nb = new[new_off + i]
        out.append("%02x" % nb if ob == nb else "??")
    if out and out[0] == "??":
        return None
    return "".join(out)


def main():
    zhl_dir, old_path, new_path = sys.argv[1], sys.argv[2], sys.argv[3]
    limit = int(sys.argv[4]) if len(sys.argv) > 4 else 0

    old = open(old_path, "rb").read()
    new = open(new_path, "rb").read()
    sigs = extract_signatures(zhl_dir)

    broken = []
    for fname, lineno, patt in sigs:
        rx = patt_to_regex(patt)
        if rx is None:
            continue
        if len(rx.findall(new)) != 1:          # broken on the new build
            m = rx.search(old)
            if m and len(rx.findall(old)) == 1:  # but unambiguous on the old one
                broken.append((fname, lineno, patt, m.start()))

    if limit:
        broken = broken[:limit]
    print(f"Broken-on-new signatures to attempt: {len(broken)}\n")

    solved = failed = 0
    solved_rows = []
    for fname, lineno, patt, old_off in broken:
        new_off, votes = rederive(old, new, old_off)
        if new_off is None:
            failed += 1
            continue
        # emit a pattern the same length as the original, then verify uniqueness
        sig_len = max(16, len(patt.replace("?", "")) // 2)
        newpatt = make_pattern(old, new, old_off, new_off, min(sig_len + 8, WINDOW))
        if newpatt is None:
            failed += 1
            continue
        rx = patt_to_regex(newpatt)
        if rx and len(rx.findall(new)) == 1:
            solved += 1
            solved_rows.append((fname, lineno, patt, newpatt, votes))
        else:
            failed += 1

    total = len(broken)
    print(f"=== RESULT ===")
    print(f"  auto-rederived & verified unique : {solved:4d}  ({100.0*solved/total:5.1f}%)")
    print(f"  needs manual RE                  : {failed:4d}  ({100.0*failed/total:5.1f}%)\n")

    print("Sample of auto-rederived signatures:")
    for fname, lineno, oldp, newp, votes in solved_rows[:8]:
        print(f"\n  {fname}:{lineno}  (anchor votes: {votes})")
        print(f"    old: {oldp[:72]}")
        print(f"    new: {newp[:72]}")


if __name__ == "__main__":
    main()
