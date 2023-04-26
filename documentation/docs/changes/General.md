# General changes to the game or game engine

In addition to the API changes listed in the docs, REPENTOGON makes the following general behavior changes to the game:

## Lua 5.4
The API has been ported to use Lua 5.4 instead of 5.3.
  
  * The generational garbage collector from 5.4 has been enabled by default, allowing for a significant performance boost.

  * We have (tentative) plans to migrate to LuaJIT in the future, which is *not* fully compatible with 5.4 (or even 5.2, for that matter). As a result, we ask that mods refrain from relying on any Lua 5.4-specific functionality.

## Game Window
The Game window now displays the current build version and commit hash of REPENTOGON in its name.