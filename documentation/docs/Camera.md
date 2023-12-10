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
        local camera = Game():GetCurrentRoom():GetCamera()
        ```
        
## Functions

### Set·Focus·Position () {: aria-label='Functions' }
#### void SetFocusPosition ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos ) {: .copyable aria-label='Functions' }
The camera will attempt to focus on the specified position. Currently, this must be called every frame or else the game will stop trying to focus on it.

___
### Snap·To·Position () {: aria-label='Functions' }
#### void SnapToPosition ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos ) {: .copyable aria-label='Functions' }
Immediately moves the camera to the specified position. As of now, this function seems to be either bugged or not made to work with Active Cam as this mode will snap back to its intended position on the next camera update.

___
