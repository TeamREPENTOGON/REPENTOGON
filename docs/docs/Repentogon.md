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
#### boolean MeetsVersion ( string version ) {: .copyable aria-label='Functions' }
Checks whether the specified `version` is lower or equal to the currently 
installed Repentogon version. In other words, returns `true` if the specified version is met.

???+ bug
    Up until Repentogon version 1.0.10b, this function is bugged and will always
    return `true`.

___
