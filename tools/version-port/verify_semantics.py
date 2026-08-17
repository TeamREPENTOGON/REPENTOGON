"""Semantic verification of re-derived function correspondences.

`sig_rederive2.py` matches bytes. Bytes cannot tell you whether the function you landed on
still *does the same thing* - that was its documented blind spot. This verifies a candidate
pair (old_fn, new_fn) on evidence that survives recompilation:

  1. **String references.** Absolute string VAs differ between builds, but the string
     *contents* do not. A function that referenced "Failed to create lobby, result = %d"
     should still reference it. This is the strongest single signal.
  2. **Call-graph consistency.** Using already-known correspondences (RTTI vtable slots,
     debug-name anchors), check that calls made by the old function land on the counterparts
     of the functions called by the new one.
  3. **Shape.** Instruction count and call count should be in the same ballpark; a wildly
     different size means we probably landed on a different function.

None of these is conclusive alone, so the tool reports a graded verdict and the evidence
behind it rather than a boolean. A human still signs off - but on ~10 lines of evidence
instead of two disassembly listings.
"""
import sys, os, re, struct

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import capstone
from rtti_vtables import Image

MAX_FN_BYTES = 0x2000


def disasm_function(img, va):
    """Linearly disassemble from `va` until the function clearly ends."""
    off = img.v2o(va)
    if off is None:
        return []
    code = img.data[off:off + MAX_FN_BYTES]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    ret_seen = False
    for insn in md.disasm(code, va):
        # int3 padding after a ret is the end of the function
        if insn.mnemonic == "int3" and ret_seen:
            break
        out.append(insn)
        if insn.mnemonic == "ret":
            ret_seen = True
        elif insn.mnemonic not in ("int3", "nop"):
            ret_seen = False
    return out


def read_cstring(img, va, limit=200):
    off = img.v2o(va)
    if off is None:
        return None
    end = img.data.find(b"\0", off, off + limit)
    if end < 0:
        return None
    s = img.data[off:end]
    if len(s) < 4 or not all(32 <= c < 127 or c in (9, 10, 13) for c in s):
        return None
    return s.decode(errors="replace")


def features(img, va):
    """Extract (call targets, referenced string contents, instruction count)."""
    insns = disasm_function(img, va)
    calls, strings = [], set()
    for insn in insns:
        if insn.mnemonic == "call":
            op = insn.op_str
            if op.startswith("0x"):
                try:
                    calls.append(int(op, 16))
                except ValueError:
                    pass
        # any immediate that points at a plausible C string
        for m in re.finditer(r'0x[0-9a-f]{6,8}', insn.op_str):
            try:
                s = read_cstring(img, int(m.group(), 16))
            except Exception:
                s = None
            if s:
                strings.add(s)
    return calls, strings, len(insns)


def load_anchor_map(path):
    m = {}
    if os.path.exists(path):
        with open(path) as f:
            for line in f:
                p = line.split()
                if len(p) >= 2:
                    try:
                        m[int(p[0], 16)] = int(p[1], 16)
                    except ValueError:
                        pass
    return m


def verify(old_img, new_img, old_va, new_va, anchors):
    oc, os_, on = features(old_img, old_va)
    nc, ns, nn = features(new_img, new_va)

    ev = []
    score = 0.0

    # 1. strings
    shared = os_ & ns
    only_old = os_ - ns
    if os_ or ns:
        if shared:
            score += 2.0 * (len(shared) / max(len(os_ | ns), 1))
            ev.append(f"strings: {len(shared)} shared" +
                      (f", {len(only_old)} only in old" if only_old else ""))
            for s in list(shared)[:2]:
                ev.append(f"    shared string: {s[:60]!r}")
        elif os_ and ns:
            score -= 1.0
            ev.append(f"strings: NONE shared ({len(os_)} old vs {len(ns)} new) <-- suspicious")
    else:
        ev.append("strings: none referenced by either")

    # 2. call graph via known correspondences
    mapped = hit = 0
    for t in oc:
        if t in anchors:
            mapped += 1
            if anchors[t] in nc:
                hit += 1
    if mapped:
        ratio = hit / mapped
        score += 2.0 * ratio
        ev.append(f"call graph: {hit}/{mapped} known callees line up ({ratio:.0%})")
    else:
        ev.append("call graph: no callees are known anchors (uninformative)")

    # 3. shape
    if on and nn:
        r = min(on, nn) / max(on, nn)
        score += r
        ev.append(f"shape: {on} vs {nn} instructions, {len(oc)} vs {len(nc)} calls")
        if r < 0.5:
            ev.append("    size differs by >2x <-- suspicious")

    if score >= 3.0:
        verdict = "STRONG"
    elif score >= 1.5:
        verdict = "PLAUSIBLE"
    elif score >= 0.7:
        verdict = "WEAK"
    else:
        verdict = "SUSPECT"
    return verdict, score, ev


def main():
    old_path, new_path, cand_path = sys.argv[1], sys.argv[2], sys.argv[3]
    old_img, new_img = Image(old_path), Image(new_path)
    anchors = load_anchor_map(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "rtti_anchors.txt"))
    print(f"known anchor correspondences: {len(anchors)}\n")

    cands = []
    with open(cand_path) as f:
        for line in f:
            p = line.split()
            if len(p) >= 3 and not line.startswith("#"):
                try:
                    cands.append((int(p[0], 16), int(p[1], 16), " ".join(p[2:])))
                except ValueError:
                    pass

    tally = {}
    detail = []
    for old_va, new_va, label in cands:
        verdict, score, ev = verify(old_img, new_img, old_va, new_va, anchors)
        tally[verdict] = tally.get(verdict, 0) + 1
        detail.append((verdict, score, old_va, new_va, label, ev))

    total = len(cands) or 1
    print("=== VERDICTS ===")
    for v in ("STRONG", "PLAUSIBLE", "WEAK", "SUSPECT"):
        n = tally.get(v, 0)
        print(f"  {v:10s} {n:4d}  ({100.0*n/total:5.1f}%)")

    print("\n=== SUSPECT / WEAK (review these first) ===")
    shown = 0
    for verdict, score, o, n, label, ev in sorted(detail, key=lambda d: d[1]):
        if verdict not in ("SUSPECT", "WEAK") or shown >= 8:
            continue
        shown += 1
        print(f"\n  [{verdict} {score:.2f}] {label}   0x{o:X} -> 0x{n:X}")
        for line in ev:
            print(f"      {line}")

    print("\n=== STRONG samples ===")
    shown = 0
    for verdict, score, o, n, label, ev in sorted(detail, key=lambda d: -d[1]):
        if verdict != "STRONG" or shown >= 3:
            continue
        shown += 1
        print(f"\n  [{verdict} {score:.2f}] {label}   0x{o:X} -> 0x{n:X}")
        for line in ev:
            print(f"      {line}")


if __name__ == "__main__":
    main()
