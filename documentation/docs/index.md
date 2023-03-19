---
search:
  boost: -999
---
# Lua API Documentation for The Binding of Isaac - REPENTOGON
___

This documentation covers the additions and modifications made to the Lua API by REPENTOGON. If you stumbled on this website inadvertently... nothing to see here, move along 🕵️

In addition to the changes listed on the pages, REPENTOGON makes the following general behavior changes to the API:

## Lua 5.4
The API has been ported to use Lua 5.4 instead of 5.3.
  
  * The generational garbage collector from 5.4 has been enabled by default, allowing for a significant performance boost.

  * We have (tentative) plans to migrate to LuaJIT in the future, which is *not* fully compatible with 5.4 (or even 5.2, for that matter). As a result, we ask that mods refrain from relying on any Lua 5.4-specific functionality.

## Callback Typechecking
Still a work in progress for the new callbacks added by REPENTOGON, but all vanilla callbacks are now typechecked.

  * An error is thrown when a callback return type is invalid.
  
  * A traceback will be printed into the debug console when a callback error is thrown.
  
  * This fixes some vanilla callbacks crashing the game when given a bad return type (MC_POST_CURSE_EVAL, MC_GET_CARD, MC_EXECUTE_CMD, etc);
  
## Old Callback Error Behavior
Callback error behavior has been reverted back to how it was before Repentance's 1.7.9b patch. 

  * A callback with an error will no longer stop every future callback from running.
  
  * This deprecates [Mod Error Container](https://steamcommunity.com/sharedfiles/filedetails/?id=2917616737).
  
## Error Display
The first mod to cause a Lua error will trigger a message at the top of the screen stating the mod is causing errors. This is in the hopes that unrelated mods stop being blamed for errors.
  
  * This can be clicked by the user to open a read-only (unless the console has already been opened by the player) version of the debug console to help show players what exactly is wrong.

## Console Error Flattening
 Multiple sequential instances of the same error will be flattened in the console (not the logs). Duplicate entries will be denoted with an (x2) (x3) etc next to the error.