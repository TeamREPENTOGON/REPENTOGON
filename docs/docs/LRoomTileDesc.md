---
tags:
  - Class
---
# Class "LRoomTileDesc"

???+ info
    You can get this class by using the following functions:

    * [Room:GetLRoomTileDesc()](Room.md#getlroomTiledesc)

    ???+ example "Example Code"
        ```lua
        local TileDesc = Game():GetRoom():GetLRoomTileDesc()
        ```
        
## Functions

### GetHighBottomRight () {: aria-label='Functions' }
#### int[2] GetHighBottomRight ( ) {: .copyable aria-label='Functions' }
Returns the grid coordinates of the high half's bottom right corner.

___
### GetHighTopLeft () {: aria-label='Functions' }
#### int[2] GetHighTopLeft ( ) {: .copyable aria-label='Functions' }
Returns the grid coordinates of the high half's top left corner.

___
### GetLowBottomRight () {: aria-label='Functions' }
#### int[2] GetLowBottomRight ( ) {: .copyable aria-label='Functions' }
Returns the grid coordinates of the low half's bottom right corner.

___
### GetLowTopLeft () {: aria-label='Functions' }
#### int[2] GetLowTopLeft ( ) {: .copyable aria-label='Functions' }
Returns the grid coordinates of the low half's top left corner.

___
### GetRandomTile ( int Seed ) {: aria-label='Functions' }
#### int[2] GetRandomTile ( int Seed ) {: .copyable aria-label='Functions' }
Returns the grid coordinates of a random tile in this L-room.

___
