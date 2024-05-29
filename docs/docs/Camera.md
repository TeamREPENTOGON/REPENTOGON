---
tags:
  - Class
---
# Class "Camera"

???+ info
    You can get this class by using the following functions:

    * [Room:GetCamera()](Room.md#getcamera)

    ???+ example "Example Code"
        ```lua
        local camera = Game():GetRoom():GetCamera()
        ```
        
## Functions

### IsPosVisible () {: aria-label='Functions' }
#### boolean IsPosVisible ( [Vector](Vector.md) Pos ) {: .copyable aria-label='Functions' }
Returns if the in-world position is visible to the camera.

___
### SetFocusPosition () {: aria-label='Functions' }
#### void SetFocusPosition ( [Vector](Vector.md) Pos ) {: .copyable aria-label='Functions' }
Sets the camera's current focus position, causing it to shift towards the specified position

The camera will only move if the current room size is larger than 1x1. If the room size is 1x1 or smaller, the camera will remain stationary and this function will do nothing. 

This function must be called on every game update, otherwise the game will override the camera's position.

___
### SnapToPosition () {: aria-label='Functions' }
#### void SnapToPosition ( [Vector](Vector.md) Pos ) {: .copyable aria-label='Functions' }
Sets the camera's position instantly to the specified position.

The camera will only move if the current room size is larger than 1x1. If the room size is 1x1 or smaller, the camera will remain stationary and this function will do nothing.

This function must be called on every game update, otherwise the game will override the camera's position.

???+ bug "Bug"
	This function seems to only work when Active Camera is off.

___
