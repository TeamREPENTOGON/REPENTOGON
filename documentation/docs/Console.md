# Class "Console"

???+ info
    You can get this class by using the Console global table.
    ???+ example "Example Code"
        ```lua
        local cmdhistory = Console.GetCommandHistory ()
        ```
        
        
## Functions

### Get·Command·History () {: aria-label='Functions' }
#### string[] GetCommandHistory ( ) {: .copyable aria-label='Functions' }
Returns a table containing current command history.

___
### Get·History () {: aria-label='Functions' }
#### string[] GetHistory ( ) {: .copyable aria-label='Functions' }
Returns a table containing every previous entry printed to the console this run.

This is ordered last-to-first- the first entry will be the currently blank line awaiting user input, followed by the previous print, and so on. The last line will always be `Repentance Console`.

___
### Pop·History () {: aria-label='Functions' }
#### void PopHistory ( int amount = 1 ) {: .copyable aria-label='Functions' }
Removes previous lines from history. Optionally, use amount to define how many entries should be removed. The line currently awaiting user input in the console counts as a part of the history, but this is already accounted for on the C++ side.

___
### Print·Error () {: aria-label='Functions' }
#### void PrintError ( string Error ) {: .copyable aria-label='Functions' }
Prints an error to the console, errors display in red text.

___
### Print·Warning () {: aria-label='Functions' }
#### void PrintWarning ( string Warning ) {: .copyable aria-label='Functions' }
Prints a warning to the console, warnings display in yellow text.

___
### Register·Command () {: aria-label='Functions' }
#### void RegisterCommand ( string Name, string Desc, string HelpText, boolean ShowOnMenu, [AutocompleteType](enums/AutocompleteType.md) Type ) {: .copyable aria-label='Functions' }
Registers a command in the new console. These will show up in the new console's autocomplete.

* `Desc` will show when typing the `help` command.
* `HelpText` will show when typing `help (Name)`.
* `AutocompleteType` will make the command inherit that autocomplete type. If the command doesn't fit into any of the standard types, use `CUSTOM` combined with [MC_CONSOLE_AUTOCOMPLETE](enums/ModCallbacks.md#mc_console_autocomplete) to create a bespoke one for this command.

___
### Register·Macro () {: aria-label='Functions' }
#### void RegisterMacro ( string Name, string[] Commands ) {: .copyable aria-label='Functions' }
Registers a macro in the new console. These will show up in the new console's autocomplete for the `macro` command.

* `Commands` is a table of strings containing the commands that should be executed, in order.

___