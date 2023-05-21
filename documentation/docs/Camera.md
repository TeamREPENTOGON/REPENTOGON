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
The true nature of this function is not yet clearly understood.

* With active cam OFF, this seems to do what you'd expect, and focus the camera on the given coordinates.
* With active cam ON, this seems to *offset* the camera by the coordinates.

Needless to say, that doesn't seem right, and is being looked into.

Must be called every frame.

___
### Snap·To·Position () {: aria-label='Functions' }
#### void SnapToPosition ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Pos ) {: .copyable aria-label='Functions' }
Seems to only work with active cam OFF (although this doesn't seem right, I'm looking into this.)

Must be called every frame.

___
