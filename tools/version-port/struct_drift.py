"""Empirically map struct field offsets from one build to another.

The signature tooling answers "where did this function go". It cannot answer "did the fields
this code touches move", which is the more dangerous half of a port: a hook whose signature
resolves correctly but whose offsets are stale attaches cleanly and then reads or writes the
wrong memory.

This derives the answer from evidence rather than assumption. For every pair of functions
already known to correspond (RTTI vtable slots), it aligns the two disassemblies and, wherever
an instruction pair lines up, records the displacement each side used:

    old:  mov eax, [edi + 0x2a160]
    new:  mov eax, [edi + 0x2a324]      ==>  vote 0x2a160 -> 0x2a324

Aggregated over thousands of instructions this produces an offset correspondence table with
vote counts, which is then checked against the offsets declared in libzhl/functions/*.zhl.

Alignment uses difflib on the *normalised* instruction text (absolute addresses stripped), so
relocation does not break it, and only 'equal' blocks are trusted - inside those, the two
sides are executing the same instruction sequence, so positional pairing is sound.
"""
import sys, os, re, glob, difflib
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import capstone
from rtti_vtables import Image

ADDR_RE = re.compile(r'0x[0-9a-f]{5,8}')
# [reg + 0xNN] or [reg + reg*n + 0xNN]
DISP_RE = re.compile(r'\[(e[a-z]{2})(?: \+ e[a-z]{2}(?:\*\d)?)? \+ (0x[0-9a-f]+)\]')
MAX_INSNS = 1500
MIN_OFFSET = 0x8          # ignore tiny displacements: stack frames, not struct fields


def disasm(img, va):
    off = img.v2o(va)
    if off is None:
        return []
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out, ret_seen = [], False
    for insn in md.disasm(img.data[off:off + 0x3000], va):
        if insn.mnemonic == "int3" and ret_seen:
            break
        out.append(insn)
        if len(out) >= MAX_INSNS:
            break
        if insn.mnemonic == "ret":
            ret_seen = True
        elif insn.mnemonic not in ("int3", "nop"):
            ret_seen = False
    return out


def norm(insn):
    """Instruction text with absolute addresses AND displacements blanked.

    Displacements must be blanked too, otherwise a moved field would make the instructions
    compare unequal and difflib would never align exactly the pairs we want to learn from.
    """
    s = ADDR_RE.sub('<a>', insn.op_str)
    s = DISP_RE.sub(lambda m: f"[{m.group(1)}+<d>]", s)
    return f"{insn.mnemonic} {s}"


def disp_of(insn):
    m = DISP_RE.search(insn.op_str)
    return int(m.group(2), 16) if m else None


def collect(old_img, new_img, pairs, limit=None):
    votes = defaultdict(Counter)
    used = 0
    for i, (ova, nva) in enumerate(pairs):
        if limit and used >= limit:
            break
        a, b = disasm(old_img, ova), disasm(new_img, nva)
        if not a or not b or abs(len(a) - len(b)) > max(len(a), len(b)) * 0.5:
            continue
        used += 1
        na, nb = [norm(x) for x in a], [norm(x) for x in b]
        sm = difflib.SequenceMatcher(a=na, b=nb, autojunk=False)
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag != "equal":
                continue
            for k in range(i2 - i1):
                da, db = disp_of(a[i1 + k]), disp_of(b[j1 + k])
                if da is None or db is None:
                    continue
                if da < MIN_OFFSET or db < MIN_OFFSET:
                    continue
                votes[da][db] += 1
    return votes, used


def parse_zhl_offsets(zhl_dir):
    """Declared struct field offsets: {file: [(field, offset)]}"""
    field_re = re.compile(
        r'^\s*[A-Za-z_][A-Za-z0-9_:<>*&\s]*?\s([A-Za-z_][A-Za-z0-9_]*)\s*:\s*(0x[0-9a-fA-F]+)')
    out = {}
    for path in sorted(glob.glob(os.path.join(zhl_dir, "*.zhl"))):
        rows = []
        for line in open(path, encoding="utf-8", errors="replace"):
            s = line.lstrip()
            if s.startswith("//") or s.startswith("\\"):
                continue
            m = field_re.match(line)
            if m:
                rows.append((m.group(1), int(m.group(2), 16)))
        if rows:
            out[os.path.basename(path)] = rows
    return out


def main():
    old_path, new_path, zhl_dir = sys.argv[1], sys.argv[2], sys.argv[3]
    limit = int(sys.argv[4]) if len(sys.argv) > 4 else None

    old_img, new_img = Image(old_path), Image(new_path)

    anchors_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "rtti_anchors.txt")
    pairs = []
    for line in open(anchors_path):
        p = line.split()
        if len(p) >= 2:
            try:
                pairs.append((int(p[0], 16), int(p[1], 16)))
            except ValueError:
                pass
    print(f"anchor function pairs available: {len(pairs)}")

    votes, used = collect(old_img, new_img, pairs, limit)
    print(f"function pairs actually compared : {used}")
    print(f"distinct old offsets observed    : {len(votes)}\n")

    stable = moved = ambiguous = 0
    mapping = {}
    for old_off, c in votes.items():
        best, n = c.most_common(1)[0]
        total = sum(c.values())
        if n / total < 0.75:
            ambiguous += 1
            continue
        mapping[old_off] = (best, n)
        if best == old_off:
            stable += 1
        else:
            moved += 1

    tot = stable + moved or 1
    print("=== OBSERVED OFFSETS ===")
    print(f"  unchanged : {stable:5d}  ({100.0*stable/tot:5.1f}%)")
    print(f"  MOVED     : {moved:5d}  ({100.0*moved/tot:5.1f}%)")
    print(f"  ambiguous : {ambiguous:5d}  (conflicting votes, excluded)\n")

    print("=== sample of MOVED offsets (old -> new, votes) ===")
    movers = sorted(((o, v[0], v[1]) for o, v in mapping.items() if v[0] != o),
                    key=lambda t: -t[2])
    for o, n, v in movers[:15]:
        print(f"  0x{o:<8X} -> 0x{n:<8X}  (delta {n-o:+#x}, {v} votes)")

    # cross-reference with what the .zhl files declare
    declared = parse_zhl_offsets(zhl_dir)
    total_decl = sum(len(v) for v in declared.values())
    covered = ok = bad = 0
    risky = []
    for fname, rows in declared.items():
        for field, off in rows:
            if off in mapping:
                covered += 1
                if mapping[off][0] == off:
                    ok += 1
                else:
                    bad += 1
                    risky.append((fname, field, off, mapping[off][0], mapping[off][1]))
    print(f"\n=== DECLARED .zhl OFFSETS ===")
    print(f"  declared total          : {total_decl}")
    print(f"  observed in evidence    : {covered}  ({100.0*covered/max(total_decl,1):5.1f}%)")
    print(f"    of those, unchanged   : {ok}")
    print(f"    of those, MOVED       : {bad}")
    print(f"  no evidence either way  : {total_decl - covered}")

    if risky:
        print("\n=== DECLARED OFFSETS THAT MOVED (these would silently corrupt) ===")
        for fname, field, o, n, v in sorted(risky, key=lambda r: -r[4])[:25]:
            print(f"  {fname:28s} {field:28s} 0x{o:<6X} -> 0x{n:<6X}  ({v} votes)")
        out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "struct_drift.txt")
        with open(out, "w") as f:
            f.write("# file field old_offset new_offset votes\n")
            for fname, field, o, n, v in sorted(risky):
                f.write(f"{fname} {field} 0x{o:X} 0x{n:X} {v}\n")
        print(f"\nwrote {len(risky)} moved declared offsets to {os.path.basename(out)}")


if __name__ == "__main__":
    main()
