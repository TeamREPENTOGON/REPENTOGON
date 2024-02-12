---
tags:
  - Class
---
# Class "BlendMode"

???+ info
    You can get this class by using the following functions:

    * [LayerState:GetBlendMode()](../LayerState.md#getblendmode)

    ???+ example "Example Code"
        ```lua
        local blendMode = Sprite():GetLayer(0):GetBlendMode()
        ```
 
???+ warning "Warning"
    Largely undocumented. I believe the `Flag` variables correspond to RGBA, with `Flag1` being `A`, though i'm not sure about the order of others. I also suspect that these flags correspond with the enums used for [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml).

## Functions

### SetMode () {: aria-label='Functions' }
#### void SetMode ( int mode ) {: .copyable aria-label='Functions' } 
Ranges from 0-2. Invalid modes are ignored. The game generally uses `1`, but uses `2` for multiplicative blending.
???+ info
    TODO: The game manually sets blend modes other than what's provided here. Document and add to this function.
___
## Variables
### Flag1 {: aria-label='Variables' }
#### int Flag1 {: .copyable aria-label='Variables'}

___
### Flag2 {: aria-label='Variables' }
#### int Flag2 {: .copyable aria-label='Variables'}

___
### Flag3 {: aria-label='Variables' }
#### int Flag3 {: .copyable aria-label='Variables'}

___
### Flag4 {: aria-label='Variables' }
#### int Flag4 {: .copyable aria-label='Variables'}

___
