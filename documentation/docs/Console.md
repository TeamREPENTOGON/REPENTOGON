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
#### void GetHistory ( ) {: .copyable aria-label='Functions' }
Returns a table containing every previous entry printed to the console this run.

This is ordered last-to-first- the first entry will be the currently blank line awaiting user input, followed by the previous print, and so on. The last line will always be `Repentance Console`.

___
### Pop·History () {: aria-label='Functions' }
#### void PopHistory ( ) {: .copyable aria-label='Functions' }
Removes previous lines from history. The line currently awaiting user input in the console counts as a part of the history, but this is already accounted for on the C++ side.

___
### Print·Error () {: aria-label='Functions' }
#### void PrintError ( string Error ) {: .copyable aria-label='Functions' }
Prints an error to the console.

___
### Print·Warning () {: aria-label='Functions' }
#### void PrintWarning ( string Warning ) {: .copyable aria-label='Functions' }
Prints a warning to the console.

___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Opens the debug console. This works even if the debug console is disabled.

???+ note "Console input"
    If the console has not been manually opened by the end-user since game start, this will be a read-only copy. This is intentional- we skip some initialization steps, solely setting the console's state to force it to open.
	
___
