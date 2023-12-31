---
tags:
  - Class
---
# Class "Backdrop"

???+ info
    You can get this class by using the following functions:

    * [Room:GetBackdrop()](Room.md#getbackdrop)

    ???+ example "Example Code"
        ```lua
        local backdrop = Game():GetRoom():GetBackdrop()
        ```
        
## Functions

### GetFloorImage () {: aria-label='Functions' }
#### [Image](renderer/Image.md) GetFloorImage ( ) {: .copyable aria-label='Functions' }
Returns the buffer containing the floor texture of the current room.

___
### GetWallImage () {: aria-label='Functions' }
#### [Image](renderer/Image.md) GetWallImage ( ) {: .copyable aria-label='Functions' }
Returns the buffer containing the wall texture of the current room.

___
