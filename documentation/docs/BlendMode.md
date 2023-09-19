# Class "BlendMode"

???+ info
    You can get this class by using the following functions:

    * [LayerState:GetBlendMode()](LayerState.md#getblendmode)

    ???+ example "Example Code"
        ```lua
        local blendMode = Sprite():GetLayer(0):GetBlendMode()
        ```
 
???+ warning "Warning"
    Largely undocumented. The `Flag` variables are likely OpenGL enums, but they are currently unknown.  

## Functions

### Set·Mode () {: aria-label='Functions' }
#### void SetMode ( int mode ) {: .copyable aria-label='Functions' } 
Ranges from 0-2. Invalid modes are ignored. The game generally uses `1`, but uses `2` for multiplicative blending.
???+ info
    TODO: The game manually sets blend modes other than what's provided here. Document and add to this function.
___
## Variables
### Flag1 {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### int Flag1 {: .copyable aria-label='Variables'}

___
### Flag2 {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### int Flag2 {: .copyable aria-label='Variables'}

___
### Flag3 {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### int Flag3 {: .copyable aria-label='Variables'}

___
### Flag4 {: aria-label='Variables' }
[ ](#){: .tooltip .badge }
#### int Flag4 {: .copyable aria-label='Variables'}

___
