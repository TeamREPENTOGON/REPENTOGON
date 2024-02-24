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
    Largely undocumented. I believe the `Flag` variables are actually two sets, with the first set being for Alpha and the second being for RGB. I also suspect that these flags correspond with the enums used for [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml).

## Functions

### SetMode () {: aria-label='Functions' }
#### void SetMode ( int mode ) {: .copyable aria-label='Functions' } 
Ranges from 0-2. Invalid modes are ignored. The game uses `1` as a default value, `0` for the blue base of the Planetarium backdrop, and `2` for multiplicative blending (Planetarium backdrop starfield layers, `8bitclouds_utero` backdrop layer, Beast death spotlight, and various other misc effects.

???- info "Nonstandard modes"
    The game additionally manually sets modes for various effects:

    * EffectVariant.BACKDROP_DECORATION, as used by Mother for the fake backdrop wall, uses `{ 0, 2, 1, 1 }`
    * Level 2 Willo, Necro, Visage (seemingly including the heart, mask, and plasma ball), Bulbs (on death), Fire variant projectiles & tears, EffectVariant.CRACK_THE_SKY (as spawned by the active, at least), EffectVariant.DUST_CLOUD (as spawned by Book of Belial), EffectVariant.DUST_CLOUD (as spawned by Ultra War) use `{ 1, 1, 1 ,1 }`
    * EffectVariant.HUSH_ASHES & the [Boss Intro versus Sprite](../RoomTransition.md#getversusscreensprite) uses `{ 4, 7, 4, 7 }`
    * EffectVariant.HALO (specifically the one used by Cursed Death's Head), EffectVariant.PURGATORY, and one of special Hell backdrop layers for the Beast fight use `{ 0, 2, 0, 2 }`

___
## Variables
### Flag1 {: aria-label='Variables' }
#### int Flag1 {: .copyable aria-label='Variables'}
Used for alpha blending. In most cases this is `1`.

Based on newer knowledge, a better name for this might be `AlphaSourceFactor`, with the `int` being one of the enums used by [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml). More research is required before this can be confirmed.

___
### Flag2 {: aria-label='Variables' }
#### int Flag2 {: .copyable aria-label='Variables'}
`0` in "Mode 0", `7` in "Mode 1", and `1` in "Mode 2".

Based on newer knowledge, a better name for this might be `AlphaDestinationFactor`, with the `int` being one of the enums used by [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml). More research is required before this can be confirmed.

___
### Flag3 {: aria-label='Variables' }
#### int Flag3 {: .copyable aria-label='Variables'}
Set to `1` in all modes.

Based on newer knowledge, a better name for this might be `RGBSourceFactor`, with the `int` being one of the enums used by [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml). More research is required before this can be confirmed.

___
### Flag4 {: aria-label='Variables' }
#### int Flag4 {: .copyable aria-label='Variables'}
`0` in "Mode 0", `7` in "Mode 1", and `1` in "Mode 2".

Based on newer knowledge, a better name for this might be `RGBDestinationFactor`, with the `int` being one of the enums used by [glBlendFunc](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFunc.xml). More research is required before this can be confirmed.
___
