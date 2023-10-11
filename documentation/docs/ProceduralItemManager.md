# Class "ProceduralItemManager"

???+ info
    You can get this class by using the following functions:

    * [Game:GetProceduralItemManager](Game.md#getproceduralitemmanager)

    ???+ example "Example Code"
        ```lua
        local manager = Game():GetProceduralItemManager()
        ```
        
		
## Functions
### Create·Procedural·Item () {: aria-label='Functions' }
#### int CreateProceduralItem ( int Seed, int Unknown ) {: .copyable aria-label='Functions' }
Creates a glitch item based on a given seed. 
Returns the negative ID of the created item.

___
### Get·Procedural·Item () {: aria-label='Functions' }
#### [ProceduralItem](ProceduralItem.md) GetProcedualItem ( int index ) {: .copyable aria-label='Functions' }
Get the glitch item at the given index.

___
### Get·Procedural·Item·Count () {: aria-label='Functions' }
#### int GetProcedualItemCount ( ) {: .copyable aria-label='Functions' }
___