# Class "Console"

???+ info
    You can get this class by using the following functions:

    * [Game:GetConsole](Game.md#getconsole)

    ???+ example "Example Code"
        ```lua
        local console = Game():GetConsole()
        ```
        
## Functions

### Print·Error () {: aria-label='Functions' }
#### void PrintError ( string Error ) {: .copyable aria-label='Functions' }
Prints an error to the console.

___
### Show () {: aria-label='Functions' }
#### void Show ( ) {: .copyable aria-label='Functions' }
Opens the debug console. This works even if the debug console is disabled.

???+ note "Console input"
    If the console has not been manually opened by the end-user since game start, this will be a read-only copy. This is intentional- we skip some initialization steps, solely setting the console's state to force it to open.
	
___