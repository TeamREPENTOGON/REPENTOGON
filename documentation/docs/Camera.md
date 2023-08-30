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
Seems to only work with active cam OFF (although this doesn't seem right, I'm looking into this.)

Must be called every frame.

___
