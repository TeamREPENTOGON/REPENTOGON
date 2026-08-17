# Version-port tooling

Scratch tooling built while assessing what it would take to move REPENTOGON from its pinned
build (**v1.9.7.12.J273**) to a newer one (**v1.9.7.17.J460**). See
`docs/ONLINE_COOP_INVESTIGATION.md` §6 for the findings.

These are analysis tools only — they read binaries and `.zhl` files and print reports. They
never modify the game, the repo, or any signature file.

## `sig_port_test.py` — how much breaks?

Extracts every active byte-pattern signature from `libzhl/functions/*.zhl` and searches for it
in each supplied executable. A ZHL signature is expected to match exactly once; 0 matches means
broken, >1 means ambiguous (broken in practice too).

```bash
python sig_port_test.py <libzhl/functions> <old-isaac-ng.exe> <new-isaac-ng.exe>
```

## `sig_reporter.py` — can they be re-derived automatically?

For each signature that breaks on the new build, tries to relocate the same function there:

1. Locate the function in the old build via its existing signature.
2. Take a window of its bytes; find rare 12-grams that occur exactly once in the new build.
3. Each unique gram votes for a candidate function start; require ≥3 agreeing votes.
4. Validate structurally — the prologue must survive and the candidate must sit at a real
   function boundary (preceded by `CC`/`NOP` padding, or opening with a known prologue).
5. Emit a new pattern with `??` where bytes differ, and verify it matches exactly once.

```bash
python sig_reporter.py <libzhl/functions> <old-exe> <new-exe> [limit]
```

Anything without consensus is reported as *needs manual RE*. The structural validation in
step 4 exists because without it the tool emitted verified-unique-but-wrong signatures
anchored mid-function — a silently broken hook is far worse than a reported failure.

**Its output is a starting point, not a result.** A byte pattern matching does not prove the
function is semantically the same; every emitted signature still needs a human to confirm it
points at the right function. And neither tool touches struct field offsets at all.

## `name_anchors.py` — ground truth by name

Some functions identify themselves in their own debug strings (e.g.
`"Menu_OnlineLobby::send_game_start()"`). Such a string is referenced by exactly the function
it names, so `string → code xref → walk back to prologue` yields that function's address.
Doing it on both builds gives a name-keyed correspondence — real identity, not byte similarity.

```bash
python name_anchors.py <old-exe> <new-exe>
```

Coverage is small (42 usable pairs on J273→J460) because few functions log their own name, but
those pairs are trustworthy, which makes them useful as anchors rather than as results.

## `sig_rederive2.py` — anchor-guided local search

Uses the name anchors to build a drift table (code shifted monotonically but not uniformly
between builds), interpolates where a given function should have landed, and searches only a
±128 KB window around that prediction. Inside a small window a k-gram only needs to be
*locally* unambiguous instead of globally unique, which recovers many more functions.

```bash
python sig_rederive2.py <libzhl/functions> <old-exe> <new-exe> [limit]
```

Anchors come from two sources: debug-string names (40, very trustworthy) and RTTI vtable slots
(1,870 — see `rtti_vtables.py`). They are reconciled by taking the **longest increasing
subsequence** of new VAs, since real drift is monotonic. Do not use a greedy "drop anything
that goes backwards" filter: vtable slots point at functions scattered across the image, so a
single early outlier discards the rest (observed: 1,910 anchors → 5).

Two refinements matter more than the anchors themselves:

- **Boundary snapping** — a gram vote yields `pos - i`, which assumes the new function is
  byte-identical up to that gram. One extra/missing byte earlier shifts the estimate
  mid-function, so snap to the nearest boundary whose prologue matches the old function.
- **Adaptive pattern length** — grow the emitted pattern until unique instead of giving up.

Measured progression, full run over all 485 broken signatures:

| Step | Auto-re-derived |
|---|---|
| v1, global unique-gram search | 13.3% |
| v2, anchor-guided (40 anchors) | 19.3% |
| + RTTI anchors | 19.3% (precision up, recall unchanged) |
| + boundary snapping + adaptive length | **43.7% — 212 of 485** |

Output goes to `rederived_candidates.txt` (`old_va new_va source:line | old -> new`), which is
input for `verify_semantics.py`. Patterns that end up more than 40% wildcards are rejected:
"grow until unique" will otherwise pad with `??` until something matches, and semantic
verification showed those are usually wrong.

## `verify_semantics.py` — does the candidate still do the same thing?

```bash
python verify_semantics.py <old-exe> <new-exe> rederived_candidates.txt
```

Scores each candidate on evidence that survives recompilation:

- **Shared strings** — string VAs move between builds, contents do not. The strongest signal.
- **Call graph** — do calls land on the counterparts of the new function's callees, per the
  known anchor map?
- **Shape** — instruction and call counts in the same ballpark.

Reports STRONG / PLAUSIBLE / WEAK / SUSPECT with the evidence, rather than a boolean, so a
human signs off on ~10 lines of evidence instead of two disassembly listings.

This is what revealed that the raw 43.7% solve rate was inflated: `PersistentGameData.zhl:29`
had been paired 2,473 instructions against 138. Current distribution over 172 candidates:
13 STRONG, 79 PLAUSIBLE, 72 WEAK, 8 SUSPECT.

## `rtti_vtables.py` — dense anchors from RTTI

```bash
python rtti_vtables.py <old-exe> <new-exe> --dump   # writes rtti_anchors.txt
```

Walks `.?AVClass@@` type descriptors → Complete Object Locators → vftables, then pairs classes
slot-by-slot. Only classes whose vtables are the *same length* on both builds are emitted
(175 of 220 common classes), so silent slot reordering within an equal-length vtable is the
main residual risk.

### Known failure modes (all observed, none fully solved)

1. **Mid-function anchoring** — fixed in v1 by requiring the prologue to survive and the
   candidate to sit at a function boundary. Without it the tool emitted a `BossPool.zhl`
   signature starting `0f28da…`, which is not a function start.
2. **Overload collision** — `AnimationState.zhl:19` and `:22` are distinct functions but v2
   emitted an *identical* pattern for both. Each "verified unique", yet they cannot both be
   right. Emitted patterns are not currently checked for collisions against each other.
3. **Semantic drift** — nothing here proves the relocated function still does the same thing.
   A refactor that preserves the prologue and most of the body will pass every check.

Treat every emitted signature as a hypothesis to verify by disassembly, never as a result.
