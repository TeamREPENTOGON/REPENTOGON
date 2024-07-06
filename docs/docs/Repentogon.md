---
tags:
  - Global
---
# Global Variable "REPENTOGON"

This global variable exposes functions and variables about Repentogon, such as
the current version, the changelog etc. It is a **table**.

This variable can be accessed anywhere.

## Functions

All functions in the table are static: they are accessed using the dot (`.`) 
operator, rather than the colon (`:`) operator.

### MeetsVersion () {: aria-label='Functions' }
#### bool MeetsVersion ( string version ) {: .copyable aria-label='Functions' }
Checks whether the specified `version` is greater or equal to the currently 
installed Repentogon version.

The function splits the `version` parameter alongside numbers boudaries. Each 
token produced by the split is compared against the corresponding token in 
the Repentogon version string. The function returns as soon as:
* The token in `version` is strictly lower than the corresponding token in 
`REPENTOGON.Version`. The function returns `false`.
* The token in `version` is strictly greater than the corresponding token in 
`REPENTOGON.Version`. The function returns `true`.
* All tokens are consumed. The function returns `true` as the requested `version`
exactly matches the current Repentogon version.

If `REPENTOGON.Version` equals "dev build", the function always returns `true`.

???+ example
    ```lua
    -- Assume REPENTOGON.Version = "1.0.9a"
    REPENTOGON.MeetsVersion("1.0.8b") -- False
    --[[ 1 is compared against 1: they are equal so continue.
    0 is compared against 0: they are equal so continue.
    8 is compared against 9: it's lower so return false. --]]

    REPENTOGON.MeetsVersion("1.0.9") -- True
    --[[ 1 is compared against 1, 0 is compared against 0, 9 is compared against 
    9: all are equals, all tokens are consumed: return true.
    Note that the letter is silently discarded. --]]

    REPENTOGON.MeetsVersion("2") -- True
    --[[ 2 is compared against 1: it's higher, so return true. --]]

    REPENTOGON.MeetsVersion("1.12") -- True
    --[[ 1 is compared against 1: they are equal so continue.
    12 is compared against 0: it's greater so return true. --]]

    REPENTOGON.MeetsVersion("1c1") -- True
    --[[ 1 is compared against 1: they are equal so continue.
    1 is compared against 0: it's greater so return true. --]]
    ```

???+ bug
    Up until Repentogon version 1.0.10b, this function is bugged and will always
    return `true`.

___
