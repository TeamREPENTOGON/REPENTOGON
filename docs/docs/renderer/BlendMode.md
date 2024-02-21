---
tags:
  - Class
---
# Class "BlendMode"

Determines how an image (either a [Sprite](../Sprite.md) as a whole or one of its [LayerStates](../LayerState.md))should blend with the elements rendered behind it. Used by [fxlayers.xml](https://wofsauge.github.io/IsaacDocs/rep/xml/fxlayers.html?h=blendMode) and various effects for multiplicative blending, but potentially has other uses. 

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
Ranges from 0-2. Invalid modes are ignored. The game uses `1` as a default value, `0` for the blue base of the Planetarium backdrop, and `2` for multiplicative blending (Planetarium backdrop starfield layers, `8bitclouds_utero` backdrop layer, Beast death spotlight, and various other misc effects.
???+ info
    TODO: The game manually sets blend modes other than what's provided here. Find them again, then document and add to this function.
___
## Variables
### Flag1 {: aria-label='Variables' }
#### int Flag1 {: .copyable aria-label='Variables'}
Used for alpha blending. In most cases this is `1`.

___
### Flag2 {: aria-label='Variables' }
#### int Flag2 {: .copyable aria-label='Variables'}
`0` in "Mode 0", `7` in "Mode 1", and `1` in "Mode 2".

___
### Flag3 {: aria-label='Variables' }
#### int Flag3 {: .copyable aria-label='Variables'}
Set to `1` in all modes.

___
### Flag4 {: aria-label='Variables' }
#### int Flag4 {: .copyable aria-label='Variables'}
`0` in "Mode 0", `7` in "Mode 1", and `1` in "Mode 2".

___
