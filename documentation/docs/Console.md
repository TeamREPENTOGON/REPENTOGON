# Class "Console"

???+ info
    You can get this class by using the following functions:

    * [Game:GetConsole](Game.md#getconsole)

    ???+ example "Example Code"
        ```lua
        local console = Game():GetConsole()
        ```
        
## Functions

### Get·History () {: aria-label='Functions' }
#### table GetHistory ( ) {: .copyable aria-label='Functions' }
Returns a table containing every previous entry printed to the console this run.

This is ordered last-to-first- the first entry will be the currently blank line awaiting user input, followed by the previous print, and so on. The last line will always be `Repentance Console`.

___
### Get·Command·History () {: aria-label='Functions' }
#### table GetCommandHistory ( ) {: .copyable aria-label='Functions' }
Returns a table containing current command history.

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