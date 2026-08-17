"""Measure how many REPENTOGON ZHL signatures still match a different game build.

Extracts every active (non-commented) byte-pattern signature from libzhl/functions/*.zhl
and searches for it in each supplied isaac-ng.exe. A ZHL signature is expected to match
exactly once; 0 matches = broken, >1 = ambiguous (also broken in practice).
"""
import sys, os, re, glob

def extract_signatures(zhl_dir):
    """Return list of (file, pattern_string). Skips commented-out lines."""
    sigs = []
    # a signature line looks like:  "558bec83ec08??5657":
    sig_re = re.compile(r'^\s*"([0-9a-fA-F?]{8,})"\s*:')
    for path in sorted(glob.glob(os.path.join(zhl_dir, "*.zhl"))):
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            for line in f:
                stripped = line.lstrip()
                # ZHL files comment things out with // and, in this repo, sometimes \
                if stripped.startswith("//") or stripped.startswith("\\"):
                    continue
                m = sig_re.match(line)
                if m:
                    sigs.append((os.path.basename(path), m.group(1)))
    return sigs

def pattern_to_regex(patt):
    """Convert a ZHL hex pattern with ?? wildcards into a compiled bytes regex."""
    out = []
    i = 0
    while i < len(patt):
        c = patt[i]
        if c == '?':
            # consume '??' or a single '?'
            i += 2 if i + 1 < len(patt) and patt[i+1] == '?' else 1
            out.append(b'.')
        else:
            if i + 1 >= len(patt):
                return None  # odd trailing nibble -> malformed
            out.append(re.escape(bytes([int(patt[i:i+2], 16)])))
            i += 2
    return re.compile(b''.join(out), re.DOTALL)

def main():
    zhl_dir = sys.argv[1]
    exes = sys.argv[2:]
    sigs = extract_signatures(zhl_dir)
    print(f"Active signatures extracted: {len(sigs)}\n")

    compiled = []
    malformed = 0
    for fname, patt in sigs:
        rx = pattern_to_regex(patt)
        if rx is None:
            malformed += 1
            continue
        compiled.append((fname, patt, rx))
    if malformed:
        print(f"(skipped {malformed} malformed patterns)\n")

    results = {}
    for exe in exes:
        data = open(exe, "rb").read()
        unique = ambiguous = missing = 0
        broken_by_file = {}
        for fname, patt, rx in compiled:
            n = len(rx.findall(data))
            if n == 1:
                unique += 1
            elif n == 0:
                missing += 1
                broken_by_file[fname] = broken_by_file.get(fname, 0) + 1
            else:
                ambiguous += 1
                broken_by_file[fname] = broken_by_file.get(fname, 0) + 1
        results[exe] = (unique, ambiguous, missing, broken_by_file)

        total = len(compiled)
        label = os.path.basename(os.path.dirname(exe)) + "/isaac-ng.exe"
        print(f"=== {label} ({len(data):,} bytes) ===")
        print(f"  unique match (OK) : {unique:5d}  ({100.0*unique/total:5.1f}%)")
        print(f"  ambiguous (>1)    : {ambiguous:5d}")
        print(f"  NOT FOUND (broken): {missing:5d}  ({100.0*missing/total:5.1f}%)")
        print()

    # show worst-hit files on the last exe scanned
    if len(exes) > 1:
        _, _, _, broken = results[exes[-1]]
        print("Worst-affected signature files (last exe):")
        for fname, cnt in sorted(broken.items(), key=lambda kv: -kv[1])[:15]:
            print(f"  {cnt:4d} broken  {fname}")

if __name__ == "__main__":
    main()
