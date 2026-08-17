"""Normalised side-by-side comparison of a function across two builds.

Manual verification of a candidate means answering two questions:

  1. Is it the same code? Absolute addresses change between builds (call targets, string
     pointers, globals), so comparing raw disassembly drowns in noise. Normalising those
     operands to a placeholder leaves the actual instruction sequence, which should match
     almost exactly if it really is the same function.

  2. Did the struct layout change? Every `[reg + 0xNN]` displacement is a field offset. If
     the same offsets appear in both versions, the layout that this function touches survived
     - which is direct evidence about the 1,820 struct offsets the signature tooling cannot
     check.

Reports a similarity ratio, the first differing instructions, and any struct offsets that
appear on one side only.
"""
import sys, os, re

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import capstone
from rtti_vtables import Image

ADDR_RE = re.compile(r'0x[0-9a-f]{5,8}')
DISP_RE = re.compile(r'\[(?:e[a-z]{2})(?: \+ (?:e[a-z]{2}(?:\*\d)?)?)?(?: \+ (0x[0-9a-f]+))\]')


def disasm(img, va, max_insns=4000):
    off = img.v2o(va)
    if off is None:
        return []
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out, ret_seen = [], False
    for insn in md.disasm(img.data[off:off + 0x4000], va):
        if insn.mnemonic == "int3" and ret_seen:
            break
        out.append(insn)
        if len(out) >= max_insns:
            break
        if insn.mnemonic == "ret":
            ret_seen = True
        elif insn.mnemonic not in ("int3", "nop"):
            ret_seen = False
    return out


def normalise(insn):
    """Instruction text with absolute addresses replaced, so relocation is not a diff."""
    return f"{insn.mnemonic} {ADDR_RE.sub('<a>', insn.op_str)}"


def struct_offsets(insns):
    offs = set()
    for i in insns:
        for m in DISP_RE.finditer(i.op_str):
            offs.add(int(m.group(1), 16))
    return offs


def compare(old_img, new_img, old_va, new_va, label, show=6):
    a, b = disasm(old_img, old_va), disasm(new_img, new_va)
    na, nb = [normalise(i) for i in a], [normalise(i) for i in b]

    same = sum(1 for x, y in zip(na, nb) if x == y)
    n = max(len(na), len(nb)) or 1
    ratio = same / n

    oa, ob = struct_offsets(a), struct_offsets(b)
    only_old, only_new = sorted(oa - ob), sorted(ob - oa)

    print(f"\n=== {label}   0x{old_va:X} -> 0x{new_va:X} ===")
    print(f"  instructions      : {len(na)} vs {len(nb)}")
    print(f"  normalised match  : {same}/{n}  ({ratio:.1%})")
    print(f"  struct offsets    : {len(oa)} vs {len(ob)}, shared {len(oa & ob)}")
    if only_old or only_new:
        print(f"    only in old: {[hex(x) for x in only_old][:10]}")
        print(f"    only in new: {[hex(x) for x in only_new][:10]}")
    else:
        print("    all struct offsets identical")

    diffs = [(i, na[i], nb[i]) for i in range(min(len(na), len(nb))) if na[i] != nb[i]]
    if diffs:
        print(f"  first differing instructions ({len(diffs)} total):")
        for i, x, y in diffs[:show]:
            print(f"    #{i:<4} {x:34s} | {y}")

    if ratio >= 0.95 and not only_old and not only_new:
        verdict = "VERIFIED - same code, same struct layout"
    elif ratio >= 0.90:
        verdict = "LIKELY SAME - minor codegen differences, review the diffs above"
    elif ratio >= 0.70:
        verdict = "PARTIAL - same function but meaningfully changed"
    else:
        verdict = "MISMATCH - do not trust"
    print(f"  => {verdict}")
    return ratio, verdict


def main():
    old_path, new_path = sys.argv[1], sys.argv[2]
    old_img, new_img = Image(old_path), Image(new_path)
    # remaining args: old_va,new_va,label triples as "OLD:NEW:LABEL"
    results = []
    for spec in sys.argv[3:]:
        o, n, label = spec.split(":", 2)
        results.append(compare(old_img, new_img, int(o, 16), int(n, 16), label))

    print("\n=== SUMMARY ===")
    for (ratio, verdict), spec in zip(results, sys.argv[3:]):
        label = spec.split(":", 2)[2]
        print(f"  {ratio:6.1%}  {label:34s} {verdict}")


if __name__ == "__main__":
    main()
