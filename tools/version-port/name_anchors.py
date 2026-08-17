"""Build `Class::method` -> function-address maps for two game builds, by name.

Byte-pattern matching is a weak way to relocate a function across versions: it has no notion
of what the function *is*. This uses a much stronger anchor - the binary's own self-identifying
debug strings (e.g. "Menu_OnlineLobby::send_game_start()") and RTTI type descriptors.

A string like that is referenced by exactly the function it names, so:
    string -> code xref -> walk back to function start -> address of that named function

Doing this on both builds yields a name-keyed correspondence between them, which is ground
truth for a port: no guessing, no byte similarity, just "this is the same named function".
"""
import sys, re, struct
from collections import defaultdict

MAX_WALKBACK = 0x900   # how far back to search for a function prologue


def load_pe(path):
    data = open(path, "rb").read()
    e = struct.unpack_from("<I", data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", data, coff + 2)[0]
    optsize = struct.unpack_from("<H", data, coff + 16)[0]
    opt = coff + 20
    image_base = struct.unpack_from("<I", data, opt + 28)[0]
    secs = []
    for i in range(nsec):
        en = data[opt + optsize + i * 40: opt + optsize + (i + 1) * 40]
        name = en[0:8].rstrip(b"\0").decode(errors="replace")
        vs, va, rs, rp = struct.unpack_from("<IIII", en, 8)
        secs.append((name, va, vs, rp, rs))
    return data, image_base, secs


def make_converters(image_base, secs):
    def off_to_va(off):
        for _n, va, vs, rp, rs in secs:
            if rp <= off < rp + rs:
                return image_base + va + (off - rp)
        return None

    def va_to_off(v):
        r = v - image_base
        for _n, va, vs, rp, rs in secs:
            if va <= r < va + max(vs, rs):
                return rp + (r - va)
        return None

    return off_to_va, va_to_off


# Strings that look like a C++ qualified name, optionally with () on the end.
NAME_RE = re.compile(rb'[A-Za-z_][A-Za-z0-9_]{2,40}::[A-Za-z_][A-Za-z0-9_]{2,60}')

PROLOGUES = (b"\x55\x8b\xec", b"\x53\x8b\xdc", b"\x56\x8b\xf1", b"\x8b\xff\x55")


def find_function_start(data, off):
    """Walk backwards from a code offset to the most plausible function start."""
    for back in range(0, MAX_WALKBACK):
        p = off - back
        if p < 4:
            break
        # padding immediately before a prologue is the strongest signal
        if data[p - 4:p] in (b"\xcc\xcc\xcc\xcc", b"\x90\x90\x90\x90"):
            if any(data.startswith(pr, p) for pr in PROLOGUES) or data[p] in (0x53, 0x55, 0x56, 0x57):
                return p
    return None


def build_name_map(path):
    data, image_base, secs = load_pe(path)
    off_to_va, va_to_off = make_converters(image_base, secs)

    # 1. collect candidate name strings and their VAs
    names = {}
    for m in NAME_RE.finditer(data):
        # must be a NUL-terminated C string starting here-ish
        s = m.group().decode(errors="replace")
        va = off_to_va(m.start())
        if va:
            names.setdefault(s, []).append(va)

    # 2. find code xrefs (push imm32 / mov reg,imm32) to those string VAs
    name_to_funcs = defaultdict(set)
    for name, vas in names.items():
        for sva in vas:
            needle = struct.pack("<I", sva)
            start = 0
            while True:
                i = data.find(needle, start)
                if i < 0:
                    break
                start = i + 1
                if i == 0:
                    continue
                prev = data[i - 1]
                if prev == 0x68 or 0xB8 <= prev <= 0xBF:  # push imm32 / mov r32,imm32
                    fs = find_function_start(data, i - 1)
                    if fs is not None:
                        fva = off_to_va(fs)
                        if fva:
                            name_to_funcs[name].add(fva)
    return name_to_funcs


def main():
    old_path, new_path = sys.argv[1], sys.argv[2]
    old_map = build_name_map(old_path)
    new_map = build_name_map(new_path)

    print(f"named functions located  (old): {len(old_map)}")
    print(f"named functions located  (new): {len(new_map)}")

    common = set(old_map) & set(new_map)
    # only names that resolve to exactly one function on BOTH sides are usable anchors
    unambiguous = [n for n in common if len(old_map[n]) == 1 and len(new_map[n]) == 1]
    print(f"names present in both          : {len(common)}")
    print(f"unambiguous on both (USABLE)   : {len(unambiguous)}\n")

    print("Sample of name-anchored correspondences (old VA -> new VA):")
    for n in sorted(unambiguous)[:20]:
        o = next(iter(old_map[n]))
        w = next(iter(new_map[n]))
        print(f"  0x{o:X} -> 0x{w:X}   {n}")


if __name__ == "__main__":
    main()
