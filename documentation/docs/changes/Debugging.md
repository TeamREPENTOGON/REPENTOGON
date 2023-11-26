# Changes done to debugging and the Console

## Callback Typechecking

All callbacks are now typechecked.

- An error is thrown when a callback return type is invalid.
- A traceback will be printed into the debug console when a callback error is thrown.
- This fixes some vanilla callbacks crashing the game when given a bad return type (MC_POST_CURSE_EVAL, MC_GET_CARD, MC_EXECUTE_CMD, etc);

## Old Callback Error Behavior

Callback error behavior has been reverted back to how it was before Repentance's 1.7.9b patch.

- A callback with an error will no longer stop every future callback from running.
- This deprecates [Mod Error Container](https://steamcommunity.com/sharedfiles/filedetails/?id=2917616737).

## Error Display

The first mod to cause a Lua error will trigger a message at the top of the screen stating the mod is causing errors. This is in the hopes that unrelated mods stop being blamed for errors.

- This can be clicked by the user to open a read-only (unless the console has already been opened by the player) version of the debug console to help show players what exactly is wrong.

## Console Error Flattening

Multiple sequential instances of the same error will be flattened in the console (not the logs). Duplicate entries will be denoted with an (x2) (x3) etc. next to the error.

## External console window
Adding the `-console` launch parameter to the game will open a second window that displays debug messages created by calling `printf()` inside of the C++ code.

## Dear ImGui

We have integrated the Dear ImGui user interface library into the game. This can be brought up at any time using the key that previously brought up the vanilla debug console. Mods can add their own user interfaces into the menu with the [ImGui](../ImGui.md) class.

## In-game Log Viewer

A log viewer has been built-in to the Dear ImGui menu.

- Game, Console and REPENTOGON logs can be filtered individually.
- The log can be searched.

## Completely Overhauled Console

The debug console has been completely rebuilt from the ground up in Dear ImGui.

- The new console has an intelligent autocomplete system, featuring proper tab completion and a more compact autocomplete layout.
- The `macro` command has been remade from scratch. Mods can now [register macros](../Console.md#registermacro) and these will show up in the new autocomplete system.
- Mods can [register commands](../Console.md#registercommand). These will show up in the new help menu and in autocomplete.
  - Autocomplete presets can be used by Lua commands.
  - Mods can define their own custom autocomplete with [MC_CONSOLE_AUTOCOMPLETE](../enums/ModCallbacks.md#mc_console_autocomplete) and the `CUSTOM` [AutocompleteType](../enums/AutocompleteType.md).
- The new console works on the main menu. Commands marked as "unsafe" on the main menu when registered are not shown in autocomplete there, and will intentionally error if attempted.
- The new console saves command history every time a command is entered. The vanilla one only saved on console or game exit. This prevents history from being lost on a game crash.
- A `help` command has been added. This lists all registered commands. Commands marked as "unsafe" on the main menu when registered are not shown there.