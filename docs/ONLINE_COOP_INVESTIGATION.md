# REPENTOGON + Online Co-op — investigation notes

**Branch:** `feature/online-coop-investigation` — experimental, **not** merge-ready.
**Game analysed:** `Repentogon/isaac-ng.exe` — `v1.9.7.12.J273`, ImageBase `0x400000`.
**Method:** live instrumented builds (ZHL hooks + `ZHL::Log`) + static x86 disassembly
(Python + capstone) of the shipped binary. Every VA below was verified against this exact
executable; they will not survive a game update.

> **Status legend** — `CONFIRMED` = proven by live log or by disassembly read directly.
> `UNCONFIRMED` = inference, explicitly not proven.

---

## 1. TL;DR

REPENTOGON blocks Online in several independent layers. We identified and neutralised the
**menu-navigation** layer, and confirmed live that the whole chain
Title → Online → MULTIPLAYER → Friend Match → Create Lobby → Ready → **real gameplay**
now works end to end, single player, with no crash.

**However** — see §6 — the REPENTOGON build ships a game executable that is **five patch
versions behind** the current Steam build. That, not the menu guards, is the dominant
blocker for actually playing with another person.

---

## 2. The blocking layers

| # | Layer | Where | What we did |
|---|-------|-------|-------------|
| 1 | Menu cursor lock | `ASMPatchOnlineMenu()` in `ASMPatches.cpp` | Call commented out (ASM funcs in `ASMMenu.cpp` untouched) |
| 2 | `_modBanStatus` forced to `3` | `ModManager::ListMods` hook, `MiscFixes.cpp` | Changed to preserve the game's own computed value |
| 3 | Native `online_mods_check()` | game code, not hooked | **Untouched.** Still returns `false` |
| 4 | `_selectedMenuID` guard | `XMLData.cpp` | Values `19/18/21/17` removed from the reset list |
| 5 | Cosmetic "disabled" styling | `Menu_Online::Render` hook, `MiscFixes.cpp` | Dimming + "switch to Vanilla" text removed |

Layer 4 was the one that actually caused the "bounces back to Press Start" symptom.

### The `XMLData.cpp` guard

Original REPENTOGON code, comment verbatim:
`Menu Bug Crash fix and backwards compat (be careful when removing this, could cause savedata corruption)`

```cpp
if (_selectedMenuID == 4 || _selectedMenuID == 19 || _selectedMenuID == 17
 || _selectedMenuID == 18 || _selectedMenuID == 21) { _selectedMenuID = 1; }  // 1 = TITLE
```

`4` (DAILYRUN) is still guarded — it is unrelated to online and we never had a reason to
touch it. **The savedata-corruption warning above was never investigated and still stands.**

---

## 3. Native menu dispatch (CONFIRMED by disassembly)

`MenuManager::Update` @ **`0x961CB0`**; jump table @ **`0x962360`**, indexed by
`_selectedMenuID` (field offset `0x40`, documented in `libzhl/functions/MenuManager.zhl`).

| ID | Enum | Trampoline | Handler |
|----|------|-----------|---------|
| 17 | ONLINELOBBY   | `0x962220` | `0x8C6D80` |
| 18 | FRIENDLOBBIES | `0x962230` | `0x8BC8C0` |
| 19 | MULTIPLAYER   | `0x962240` | `0x8C3970` = `Menu_Online::Update` |
| 21 | CREATELOBBY   | `0x96225D` | `0x8B2410` |

Index 19 → `0x8C3970` matches the ZHL signature for `Menu_Online::Update`, which
independently validates the whole table read.

Writes to `_selectedMenuID` go through a navigation-history helper: `0x963210` →
`0x962E20`, which performs the real `mov dword ptr [ebx+0x40], edi`.

### `Menu_Online::Update` (`0x8C3970`) — the "PLAY ONLINE!" screen

Ends in a 5-case jump table, one per visible button. Confirmed by the description strings
each case pushes (`'Quick Match'` `0xB26B68`, `'Public Match'` `0xB26A78`,
`'Friend Match'` `0xB26A68`, `'Daily Run'` `0xB26938`, `'Options'` `0xB1E204`):

| Button | Handler | Action |
|--------|---------|--------|
| Quick Match  | `0x8C4100` | **No menu change** — async matchmaking (see §4) |
| Public Match | `0x8C4320` | `RequestMenuChange(18)`, mode flag `0` |
| Friend Match | `0x8C43D0` | `RequestMenuChange(18)`, mode flag `1` |
| Daily Run    | inline     | `RequestMenuChange(4)` |
| Options      | inline     | `RequestMenuChange(20)` |

Public and Friend Match target the *same* screen (18), differing only by an internal flag.

---

## 4. Quick Match (CONFIRMED)

`0x8C4100` does not navigate. It builds an async request: callback `0x8C4C10`, dispatcher
`0x9F40F0`, sentinel `0xDEADBEEF`.

`0x9F40F0` resolves Steam interfaces via `SteamInternal_ContextInit` (import table entry
`0xAAA7C0`, verified against `steam_api.dll` imports) and issues vtable calls — a lobby-list
request with filters. The callback `0x8C4C10` then walks the returned lobbies and reads
per-lobby metadata keys:

- `lobby_mode` (`0xB1F3FC`) — must equal the requested mode
- `lobby_password` (`0xB1F430`) — length must be 0 (skip password-protected lobbies)
- `current_stage` (`0xB26344`)

Other keys present in the binary: `lobby_tag`, `lobby_seed`, `lobby_challenge`, `lobby_banned`.

**Notable:** none of these filters mention mods. Mod gating happens elsewhere
(`online_mods_check` / `_modBanStatus`), not in lobby matchmaking.

**Quick Match was never modified and never tested live.**

---

## 5. Correction — an earlier claim of ours was wrong

An earlier round of this investigation reported that `0x8B37C0` (inside
`Menu_CreateLobby::Update`) contained a *network message queue* reached by pressing Ready,
based on `call 0x9EEC40` + `call eax` + `operator new(0x10)`.

**That was wrong.** String evidence shows:

- The three `call 0x9EEC40` / `call eax` pairs are a loop over `'A'..'Z'` and `'0'..'9'`
  polling key presses — it is the **text input** for the *Select a Tag* / *Input a Seed*
  fields.
- `0x905170` logs `'Seed %d added to SaveState'` (`0xB2BA50`) and `0x903460` logs
  `'PersistentGameData no filename supplied'` (`0xB2B758`) — both are **save/seed**
  machinery, not networking.

The real game-start path is elsewhere, in `Menu_OnlineLobby`:
`send_game_start()` referenced at `0x8CB536`, `notify_game_start()` at `0x8F18B4`.
This also explains the second `MenuManager::Init` we observed in the log when Ready was
pressed: the menu system reinitialises as the real run starts.

---

## 6. ⚠ The version gap — the actual blocker

| Executable | Version |
|---|---|
| `The Binding of Isaac Rebirth/isaac-ng.exe` (Steam, current) | **v1.9.7.17.J460** |
| `The Binding of Isaac Rebirth/Repentogon/isaac-ng.exe` | **v1.9.7.12.J273** |

REPENTOGON 1.1.2g is built against **J273**; Steam has since moved the base game to **J460**.
Playing online through REPENTOGON therefore puts you on a five-patch-old build. Anyone on the
current Steam version is running different game logic, content and RNG. **No amount of menu
patching fixes that** — it is an independent, and probably decisive, source of desync.

This is **deliberate**, not an oversight. Per `docs/docs/install.md`, the team pinned the
project to J273 because Repentance+'s release cycle made porting "an endless task", and the
launcher intentionally *downgrades* the game — which is why `Repentogon/` carries its own
J273 `isaac-ng.exe` (8,764,368 bytes) separate from Steam's J460 one (9,362,440 bytes).

> ⚠ The same doc warns that savefile structures differ between versions, and that loading a
> REPENTOGON-compatible savefile on the latest release **can obliterate all achievements**.
> Do not hand-swap executables between the two installs.

### Measured cost of porting to J460

All 1,155 active byte-pattern signatures in `libzhl/functions/*.zhl` were extracted and
searched against both executables (script: `sig_port_test.py`; a ZHL signature is expected
to match exactly once):

| | J273 (`Repentogon/`) | J460 (Steam) |
|---|---|---|
| Unique match (OK) | 1,151 — **99.7%** | 666 — **57.7%** |
| Ambiguous (>1 match) | 1 | 74 |
| Not found | 3 — 0.3% | **415 — 35.9%** |
| **Total broken** | 4 | **489 — 42.3%** |

Breakage is concentrated in the core classes, not the periphery: `EntityPlayer.zhl` (80),
`Room.zhl` (19), `Game.zhl` (18), `EntityFamiliar.zhl` / `EntityPickup.zhl` (14 each),
`Entity.zhl` / `EntityNPC.zhl` / `Manager.zhl` (12 each), `Level.zhl` (11).

**Caveat — the 57.7% that match are not automatically safe.** This test covers byte patterns
only. The `.zhl` files also declare **struct field offsets** (e.g. `_selectedMenuID : 0x40`,
relied on throughout this investigation), which were **not checked at all**. A signature that
still matches while its struct layout changed is more dangerous than a clean miss: the hook
attaches successfully and then reads/writes the wrong offset.

A port therefore means re-deriving ~489 signatures in the most central classes, re-verifying
struct offsets project-wide, and redoing the ASM patches that use hardcoded addresses.

### Can the re-derivation be automated?

Partly, and less than hoped. `tools/version-port/sig_reporter.py` relocates a broken function
in the new build by anchoring on rare 12-grams from its old body, requiring ≥3 agreeing votes,
validating that the candidate sits at a real function boundary with its prologue intact, and
finally verifying the emitted pattern is unique. Measured over a 150-signature sample:

| Outcome | Count | Share |
|---|---|---|
| Auto-re-derived and verified unique | 20 | **13.3%** |
| Needs manual RE | 130 | 86.7% |

A second iteration (`sig_rederive2.py`) improves on this. `name_anchors.py` recovers 42
functions whose identity is known on *both* builds, by matching the binary's own debug strings
(`"Menu_OnlineLobby::send_game_start()"` and similar) rather than bytes. Sorting those by
address shows code drifted monotonically but unevenly (`+0x4B60` low in the image, `+0x284F0`
high), so the expected location of any other function can be interpolated between anchors and
searched locally — inside a ±128 KB window a k-gram need only be locally unambiguous, not
globally unique.

Anchors were then densified using RTTI. Both builds carry MSVC RTTI, so
`class name → Complete Object Locator → vftable → slot i` identifies a virtual function by
identity rather than similarity. Restricting to the 175 classes whose vtables are the same
length on both builds yields **1,870 anchor pairs**, versus 40 from debug strings.

Feeding those into the drift table required care: real drift is monotonic, so the consistent
subset is the **longest increasing subsequence** of new VAs. A greedy "discard anything that
goes backwards" pass collapsed 1,910 anchors to 5, because vtable slots point at functions
scattered across the image and one early outlier discards everything after it.

Two further fixes mattered more than the anchors themselves:

- **Boundary snapping.** A gram vote yields `pos - i`, which assumes the new function is
  byte-identical up to that gram; a single extra or missing byte earlier in the body lands the
  estimate mid-function. Snapping to the nearest real boundary whose prologue matches turned
  the largest failure bucket (40.3%) into successes.
- **Adaptive pattern length.** Growing the emitted pattern until it is unique, instead of
  giving up at a fixed length.

| Approach | Auto-re-derived |
|---|---|
| v1, global unique-gram search | 13.3% |
| v2, anchor-guided local search (40 anchors) | 19.3% |
| + RTTI anchors (550 usable) | 19.3% — precision up, recall unchanged |
| + boundary snapping + adaptive length | 46.7% (sample) |
| **full run over all 485 broken signatures** | **212 solved — 43.7%** |

Remaining failures: no locally-unambiguous grams (17.7%), prologue genuinely changed (17.7%),
emitted pattern not unique (10.5%), too few agreeing votes (8.7%), collides with another
signature (1.6%).

That last category matters: two distinct source signatures resolving to the same function.
Overloads are the usual cause (`AnimationState.zhl:19` and `:22`), and each pattern verifies
as unique on its own, so only a cross-signature check catches it. Eight were found and
rejected. The other silent-failure modes are documented in `tools/version-port/README.md`.

### Semantic verification — and why the 43.7% was inflated

Byte agreement cannot detect semantic drift, so `verify_semantics.py` scores each candidate on
evidence that survives recompilation: **shared referenced strings** (string VAs move between
builds, contents do not), **call-graph consistency** against the 642 known correspondences, and
**shape** (instruction and call counts).

Running it over the 212 candidates was sobering — only 6.1% scored STRONG, and the SUSPECT
cases were plainly wrong: `PersistentGameData.zhl:29` paired a 2,473-instruction function with
a 138-instruction one; `EntityPlayer.zhl:595` paired 2,045 against 605.

The cause was the "grow the pattern until unique" fix: it inflated the solve rate by padding
patterns with wildcards until something matched. Rejecting patterns that are more than 40%
wildcards drops the count but raises the quality:

| | candidates | STRONG | PLAUSIBLE | WEAK | SUSPECT |
|---|---|---|---|---|---|
| before quality filter | 212 (43.7%) | 13 (6.1%) | 92 (43.4%) | 93 (43.9%) | 14 (6.6%) |
| **after quality filter** | **172 (35.5%)** | **13 (7.6%)** | **79 (45.9%)** | 72 (41.9%) | 8 (4.7%) |

**Honest bottom line: of 485 broken signatures, 13 are high-confidence, ~79 more are worth a
human's review, and roughly 390 still require manual RE.**

### All 13 STRONG candidates verified by hand

Each was compared instruction-by-instruction with absolute addresses normalised (relocation is
not a difference), and every `[reg + 0xNN]` displacement was extracted to check struct layout
(`compare_functions.py`). **All 13 addresses are correct** — the re-derived signature finds the
right function in every case. But they split into two groups:

| Candidate | Code | Struct offsets |
|---|---|---|
| `RNG.zhl:13` — `RNG::Next` | identical | identical (0/4/8/0xC) |
| `AnimationState.zhl:19` / `:22` | identical | identical (0x6C / 0x54 sizes) |
| `AnimationState.zhl:4` | identical | identical |
| `EntityPickup.zhl:97` — `Init` | grew 1154→1363 | identical |
| `PersistentGameData.zhl:65` | 99.1% | identical (34/34) |
| `EntityNPC.zhl:61` | 99.2% | identical (18/18) |
| `EntityNPC.zhl:88` | 89.3% — `esi`/`edi` swapped throughout | identical (10/10) |
| `LuaEngine.zhl:70` | 92.2% — register allocation | identical |
| `MenuCharacter.zhl:5` | 85.8% | **shifted** — `0xf734`→`0xf72c`, `0x25b01`→`0x26589`, plus two new fields |
| `AchievementOverlay.zhl:1` | 98.8% | **shifted** — `0x8368`→`0x839c`, `0x2a160`→`0x2a324`, `0x4a8b8`→`0x4abbc` |
| `Music.zhl:4` | 98.1% | **shifted** — `0x2a120`→`0x2a2e4`, `0x2a190`→`0x2a354` |
| `EntityFamiliar.zhl:34` | 92.2% | **shifted** — `0xee0`→`0xef4` |

Two lessons. First, a low similarity score is often benign: `EntityNPC.zhl:88` scores 89.3%
purely because the compiler swapped `esi` and `edi`, and `LuaEngine.zhl:70` likewise. Register
reallocation is not semantic change.

Second, and much more important — **the struct drift is real, and it follows a pattern.**
Small self-contained structs (RNG state, AnimationState, per-entity NPC fields) are byte-for-byte
unchanged. The offsets that moved are all large displacements into the big aggregate objects
(the Manager/global block around `0x2a000`–`0x4b000`, and the menu structures), and they moved
by varying amounts (`+0x1C4`, `+0x304`, `+0x420`), meaning fields were inserted at several
points rather than the whole block shifting.

This is direct evidence on the question the signature tooling could not answer. Signatures are
the tractable half of a port: they can be found, re-derived and verified. **The struct offsets
are the real hazard** — a hook whose signature resolves correctly but whose field offsets are
stale attaches cleanly and then reads or writes the wrong memory, silently.

### Measuring struct drift across the whole binary

`struct_drift.py` scales that hand-verification up. For every pair of functions already known
to correspond (the 1,870 RTTI vtable slots), it aligns the two disassemblies with `difflib` on
normalised instruction text — absolute addresses *and* displacements blanked, so neither
relocation nor a moved field breaks alignment — and, inside blocks that match exactly, reads
off what displacement each side used:

```
old:  mov eax, [edi + 0x2a160]
new:  mov eax, [edi + 0x2a324]     ==>  vote 0x2a160 -> 0x2a324
```

Over 1,755 comparable function pairs this yields a voted offset-correspondence table.

| Observed offsets (1,026 distinct) | |
|---|---|
| unchanged | 559 (57.4%) |
| **moved** | **415 (42.6%)** |
| ambiguous (conflicting votes, excluded) | 52 |

Cross-referencing against what the `.zhl` files actually declare:

| Declared struct offsets | |
|---|---|
| total | 1,820 |
| covered by evidence | 749 (41.2%) |
| — of those, unchanged | 594 |
| — of those, **moved** | **155** |
| no evidence either way | 1,071 |

**155 declared offsets are confirmed to have moved**, and they are not obscure ones
(`struct_drift.txt` has the full list):

| Field | J273 | J460 |
|---|---|---|
| `Game::_frameCount` | `0x25A70` | `0x264F8` |
| `Manager::_netplayManager` | `0x4AFB8` | `0x4B3D8` |
| `Manager::_sfxManager` | `0x2A160` | `0x2A324` |
| `EntityPlayer::_playerType` | `0x13BC` | `0x13C0` |
| `EntityPlayer::_isCoopGhost` | `0x207D` | `0x20A9` |
| `EntityPlayer::_controllerIndex` | `0x160C` | `0x1618` |
| `EntityPlayer::_twinPlayer` | `0x1E4C` | `0x1E68` |
| `Game::_challenge` | `0x25AFC` | `0x26584` |

These are among the most-referenced fields in the entire project. `EntityPlayer` alone shows
fields shifting by `+4`, `+0xC`, `+0x14`, `+0x1C` at increasing offsets — fields were inserted
at several points, so no single adjustment fixes the class.

The useful corollary: because the table gives the *new* offset with vote counts, correcting
those 155 is largely mechanical rather than research. The unresolved part is the **1,071
declared offsets with no evidence at all** — fields no anchored function happens to touch.
Those need either more anchor coverage or manual RE, and until they are resolved a port cannot
be considered safe.

The structural validation is not optional. An earlier version without it scored a nominally
better 16.7%, but among those "successes" was `BossPool.zhl:4`, whose re-derived pattern began
`0f28da…` — not a function start at all. It passed the uniqueness check while pointing at the
wrong code. **A verified-unique signature is not a correct signature**, which is why the tool
now refuses anything whose prologue does not survive.

### The larger, unquantified half: struct offsets

`libzhl/functions/*.zhl` also declares **1,820 struct field offsets** (`_selectedMenuID : 0x40`
and friends). None of the tooling above touches them, and no automated byte-level method can
verify them — a field that moved while its containing function's signature still matches
produces a hook that attaches cleanly and then reads or writes the wrong memory.

### Overall assessment

Signatures alone: ~424 manual re-derivations in the game's core classes. Plus 1,820 struct
offsets to re-verify, plus ASM patches, plus the savefile-format countermeasures the launcher
ties to J273 — and J460 will not be the last release either. This is precisely the treadmill
described in `install.md`, and it is a multi-person, multi-month effort rather than something
achievable in the course of this investigation.

### Practical alternative

The version gap only matters if the *other* player is on a different build. If both players
install REPENTOGON, both are on J273 — identical clients, no gap, nothing to port. For the
still-untested two-player run this is the realistic setup (the second player also needs the
same experimental `zhlREPENTOGON.dll`, since a stock build still blocks the Online menu).

---

## 7. The game's native desync system (CONFIRMED)

The base game already ships a full desync detect-and-recover subsystem — this is *not*
REPENTOGON code:

- **Per-frame checksums**, compared between clients: framestate checksum, save checksum,
  and a global RNG checksum.
- **Detection** at `0x8DB1CF`: `[Frame: %d] Desync detected! desyncFrame = %d`
- **Protocol**: `MSG_DESYNC_NOTIFY` → `MSG_DESYNC_CONFIRM` → `MSG_DESYNC_RECOVERY_START`
  → `MSG_DESYNC_RECOVERY_COMPLETE`, with a resync counter, handled by `NetDesyncHandler`.
- **Recovery** can go as far as `Desync Handler creating new lobby`.
- **Diagnostics dumped to disk** under
  `Documents/My Games/Binding of Isaac Repentance+/online_logs/desyncs/<timestamp>__<user>/`:
  `desync_diff.txt`, `desync_framestate.txt`, `desync_rng_history.txt`, `desync_log.txt`,
  `desync_shared_save.dat`, `desync_screenshot.png`.

Also confirmed: `##PLATFORM###ONLINE_MODS_PERMABANNED` / `##PLATFORM###ONLINE_MODS_BANNED`
are the localisation keys behind `_modBanStatus` — the mod-ban system is entirely separate
from the desync system.

### Real desyncs already on this machine — from *before* this investigation

`online_logs/desyncs/` contains **9 recorded desyncs dated 2026-08-10**, all with
`--repentogonoff` in the command line and `Game Version: J460` — i.e. **plain vanilla,
no REPENTOGON involved**. A representative `desync_diff.txt`:

```
Desync detected on frame: 0
Number of players: 2
 - Player0: Checksum (04a3bdd8), Global RNG checksum (f9609185)
 - Player1: Checksum (06a4ae76), Global RNG checksum (e1147275)
Entity 4: Player0 Type (1000.68.0) (Type Mismatch!) / Player1 Type (1000.21.0)
Entity 6: Player0 Missing! / Player1 Type (1000.33.0)
```

Divergence **at frame 0**, with entity type mismatches and different global seeds — the two
clients never agreed on an initial state at all. This user was already getting desyncs in
vanilla online co-op before any of this work started.

**UNCONFIRMED:** the cause of those particular vanilla desyncs (version skew between the two
players, differing content, or something else) was not determined.

---

## 8. On "optimising" the desync system

Worth stating plainly: **the desync detector should not be weakened.** Suppressing checksum
comparisons or short-circuiting the recovery protocol would not remove desyncs — it would
convert a *detected, recoverable, logged* desync into *silent state divergence*, which is
precisely the `could cause savedata corruption` scenario REPENTOGON's own comment warns
about. No such change was made.

Constructive directions instead:

1. **Close the version gap (§6)** — the highest-value fix by far, and a prerequisite for any
   meaningful two-player test.
2. **Determinism auditing** — REPENTOGON adds Lua callbacks and hooks in gameplay paths; any
   that consume RNG or mutate entity state divergently across clients are desync sources.
   Not audited here.
3. **Visibility** — the native system already writes rich diagnostics to
   `online_logs/desyncs/`; that is the place to look after any co-op test, and it costs
   nothing to use.

---

## 9. Live test results

All tests: user launched the game manually; each build installed with a timestamped backup
and SHA-256 verification of `zhlREPENTOGON.dll`, with `libzhl.dll` / `zhlLoader.dll` /
`isaac-ng.exe` verified unchanged. Reports in
`<game>/REPENTOGON_BACKUP_ONLINE_TEST/*_READY.txt`.

| Step | Result |
|---|---|
| Reach & hold MULTIPLAYER (19) | CONFIRMED — stable 12 s, no reset |
| Hook order on `MenuManager::Update` | CONFIRMED — `XMLData.cpp` is outermost of the 4 |
| Friend Match → FRIENDLOBBIES (18) | CONFIRMED — reached "FRIEND MATCHES" screen |
| TAB/Create → CREATELOBBY (21) | CONFIRMED — live log caught the `18 → 21` transition |
| CREATE! → lobby room | CONFIRMED — real Steam lobby, invite slots, "TAB to READY" |
| Ready → gameplay | CONFIRMED by user — entered and played a normal run, exited cleanly |
| Two real players | **NEVER TESTED** |
| Quick Match | **NEVER TESTED** |
| ONLINELOBBY (17) | Guard removed, but never exercised by this flow |

No crash, no `ERROR 127`, no `0xC0000409` in any session.

---

## 10. Instrumentation left in the tree

`[ONLINE]`, `[ONLINE-TRACE]`, `[ONLINE-GUARD]` and `[HOOK-ORDER]` tags in
`MiscFixes.cpp`, `XMLData.cpp`, `CutsceneSkip.cpp`, `ModsMenuStuff.cpp`,
`GameStateManagement.cpp`. Per-frame `[HOOK-ORDER]` printing has been reduced to a
`WINDOW_TIMEOUT` marker, and `[ONLINE-GUARD]` now logs only on actual `_selectedMenuID`
changes. **All of it is diagnostic scaffolding and must be removed before this branch could
be considered for merge.**

One unrelated real fix was made along the way: `#include <algorithm>` added to
`LuaPlayerHUD.cpp` for `std::clamp` (pre-existing build break).

---

## 11. Unresolved

- Version gap J273 vs J460 (§6) — quantified (42.3% of signatures break on J460) but not
  addressed; porting is a project-scale effort the upstream team deliberately declined.
  Sidestepped rather than solved by putting both players on J273.
- Two-player co-op — the central question, still unanswered.
- Whether REPENTOGON's own hooks introduce cross-client nondeterminism.
- The savedata-corruption risk the original `XMLData.cpp` comment warns about.
- Quick Match end to end.
- A `selected=0` anomaly seen in early `[HOOK-ORDER]` traces for
  `MiscFixes.cpp`/`CutsceneSkip.cpp` — never explained. It did not affect the primary
  finding, which was fully demonstrated within `XMLData.cpp`'s own ENTER→BEFORE_SUPER pair.
