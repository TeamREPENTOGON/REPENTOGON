---
tags:
  - Global
  - Class
---
# Global Class "ProceduralItemManager"

???+ info
    You can get this class by using the `ProceduralItemManager` global table.

    **Note that to call these functions, you must use a `.` (period) instead of a `:` (colon)!**
    
    ???+ example "Example Code"
        ```lua
        local pItem = ProceduralItemManager.GetProceduralItem(0)
        ```

## Functions
### CreateProceduralItem () {: aria-label='Functions' }
#### int CreateProceduralItem ( int Seed, int Unknown ) {: .copyable aria-label='Functions' }
Creates a glitch item based on a given seed. 
Returns the negative ID of the created item.

___
### GetProceduralItem () {: aria-label='Functions' }
#### [ProceduralItem](ProceduralItem.md) GetProceduralItem ( int Index ) {: .copyable aria-label='Functions' }
Get the glitch item at the given index.

___
### GetProceduralItemCount () {: aria-label='Functions' }
#### int GetProceduralItemCount ( ) {: .copyable aria-label='Functions' }
___