---
search:
  boost: -999
---
# Lua API Documentation for The Binding of Isaac - REPENTOGON
___

This documentation covers the additions and modifications made to the Lua API by REPENTOGON. If you stumbled on this website inadvertently... nothing to see here, move along 🕵️

In addition to the changes listed on the pages, REPENTOGON makes the following general behavior changes to the API:

## Callback Typechecking
Still a work in progress for the new callbacks added by REPENTOGON, but all vanilla callbacks are now typechecked.

  * An error is thrown when a callback return type is invalid.
  
  * A traceback will be printed into the debug console when a callback error is thrown.
  
  * This fixes some vanilla callbacks crashing the game when given a bad return type (MC_POST_CURSE_EVAL, MC_GET_CARD, MC_EXECUTE_CMD, etc);
  
## Old Callback Error Behavior
Callback error behavior has been reverted back to how it was before Repentance's 1.7.9b patch. 

  * A callback with an error will no longer stop every future callback from running.
  
  * This deprecates [Mod Error Container](https://steamcommunity.com/sharedfiles/filedetails/?id=2917616737).