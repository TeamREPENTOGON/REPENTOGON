"""Extract MSVC RTTI class vtables and pair them between two builds.

Debug-string anchors (`name_anchors.py`) are trustworthy but rare - only functions that log
their own name are covered. RTTI is far denser: an MSVC 32-bit binary lays out, for every
polymorphic class,

    TypeDescriptor:  [vfptr][spare][".?AVClassName@@"]
    CompleteObjectLocator (COL):
        +0x00 signature (0 for 32-bit)
        +0x04 offset
        +0x08 cdOffset
        +0x0C -> TypeDescriptor VA
        +0x10 -> ClassHierarchyDescriptor VA
    vftable:  [-4] -> COL VA, then the virtual function pointers

so `class name -> vftable -> slot i` identifies a virtual function by *identity*: same class,
same slot. Pairing slot-by-slot across two builds yields anchors that byte matching cannot
produce, and many more of them than debug strings do.

Caveat: a slot only corresponds across versions if the class did not gain, lose or reorder
virtuals. This tool pairs classes whose vtables are the *same length* on both builds, which
makes reordering the main residual risk - see README.
"""
import sys, re, struct
from collections import OrderedDict

TYPE_NAME_RE = re.compile(rb'\.\?A[VU][A-Za-z0-9_@?$]{2,160}@@')


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


class Image:
    def __init__(self, path):
        self.data, self.base, self.secs = load_pe(path)
        self.text = None
        for name, va, vs, rp, rs in self.secs:
            if name.startswith(".text"):
                self.text = (self.base + va, self.base + va + max(vs, rs))
        if self.text is None:                      # fall back to first exec-looking section
            name, va, vs, rp, rs = self.secs[0]
            self.text = (self.base + va, self.base + va + max(vs, rs))

    def o2v(self, off):
        for _n, va, vs, rp, rs in self.secs:
            if rp <= off < rp + rs:
                return self.base + va + (off - rp)
        return None

    def v2o(self, v):
        r = v - self.base
        for _n, va, vs, rp, rs in self.secs:
            if va <= r < va + max(vs, rs):
                return rp + (r - va)
        return None

    def dword(self, off):
        if off is None or off < 0 or off + 4 > len(self.data):
            return None
        return struct.unpack_from("<I", self.data, off)[0]

    def is_code(self, va):
        return self.text[0] <= va < self.text[1]


def extract_vtables(img):
    """Return OrderedDict: class name -> list of virtual function VAs."""
    # 1. type descriptors: name string starts 8 bytes into the descriptor
    td_by_va = {}
    for m in TYPE_NAME_RE.finditer(img.data):
        name_off = m.start()
        td_off = name_off - 8
        if td_off < 0:
            continue
        td_va = img.o2v(td_off)
        if td_va:
            td_by_va[td_va] = m.group().decode(errors="replace")

    # 2. COLs: a dword equal to a type-descriptor VA sitting at COL+0x0C, signature 0 at +0x00
    col_to_name = {}
    for td_va, name in td_by_va.items():
        needle = struct.pack("<I", td_va)
        start = 0
        while True:
            i = img.data.find(needle, start)
            if i < 0:
                break
            start = i + 1
            col_off = i - 0x0C
            if col_off < 0:
                continue
            if img.dword(col_off) != 0:            # 32-bit COL signature
                continue
            col_va = img.o2v(col_off)
            if col_va:
                col_to_name[col_va] = name

    # 3. vftables: a dword equal to a COL VA; the table starts right after it
    vtables = OrderedDict()
    for col_va, name in col_to_name.items():
        needle = struct.pack("<I", col_va)
        start = 0
        while True:
            i = img.data.find(needle, start)
            if i < 0:
                break
            start = i + 1
            vt_off = i + 4
            funcs = []
            off = vt_off
            while True:
                v = img.dword(off)
                if v is None or not img.is_code(v):
                    break
                funcs.append(v)
                off += 4
                if len(funcs) > 400:
                    break
            if len(funcs) >= 1:
                # keep the longest table found for a class
                if name not in vtables or len(funcs) > len(vtables[name]):
                    vtables[name] = funcs
    return vtables


def main():
    old_path, new_path = sys.argv[1], sys.argv[2]
    old, new = Image(old_path), Image(new_path)

    print("extracting RTTI vtables...")
    vo = extract_vtables(old)
    vn = extract_vtables(new)
    print(f"  classes with vtables (old): {len(vo)}")
    print(f"  classes with vtables (new): {len(vn)}")

    common = [c for c in vo if c in vn]
    same_len = [c for c in common if len(vo[c]) == len(vn[c])]
    pairs = sum(len(vo[c]) for c in same_len)
    print(f"  classes in both           : {len(common)}")
    print(f"  same vtable length (safe) : {len(same_len)}")
    print(f"  => anchor pairs           : {pairs}\n")

    if "--dump" in sys.argv:
        out = []
        for c in same_len:
            for i, (a, b) in enumerate(zip(vo[c], vn[c])):
                out.append(f"{a:08X} {b:08X} {c}#{i}")
        path = "rtti_anchors.txt"
        open(path, "w").write("\n".join(out))
        print(f"wrote {len(out)} anchor pairs to {path}")

    print("Sample classes:")
    for c in same_len[:12]:
        print(f"  {len(vo[c]):3d} slots  {c[:80]}")


if __name__ == "__main__":
    main()
