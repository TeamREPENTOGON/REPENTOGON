---
tags:
  - Class
---
# Class "BlendMode"

Determines how an image (either a [Sprite](../Sprite.md) as a whole or a [LayerState](../LayerState.md))should blend with the elements rendered behind it. Specifically, these parameters are used in calls to [glBlendFuncSeparate](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFuncSeparate.xml).

???+ info
    You can get this class by using the following functions:

    * [LayerState:GetBlendMode()](../LayerState.md#getblendmode)

    ???+ example "Example Code"
        ```lua
        local blendMode = Sprite():GetLayer(0):GetBlendMode()
        ```

???+ warning "Warning"
    The variables in this class refer to the enums used by [glBlendFuncSeparate](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFuncSeparate.xml) (with a starting index of `0`). Official enums will be added for this next release.

## Functions

### SetMode () {: aria-label='Functions' }
#### void SetMode ( [BlendFactor](../enums/BlendType.md) Mode ) {: .copyable aria-label='Functions' } 
This defaults to `NORMAL` (or `{ 1, 7, 1, 7 }`) as a default value, `ADDITIVE` (or `{ 0, 1, 0, 1 }`) for the blue base of the Planetarium backdrop, and `MULTIPLICATIVE` (or `{ 1, 1, 1, 1 }`) for Planetarium backdrop starfield layers, `8bitclouds_utero` backdrop layer, Beast death spotlight, and various other misc effects.

???- info "Nonstandard modes"
    The game additionally manually sets modes for various effects:

    * EffectVariant.BACKDROP_DECORATION, as used by Mother for the fake backdrop wall, uses `{ 0, 2, 1, 1 }`
    * EffectVariant.HUSH_ASHES & the [Boss Intro versus Sprite](../RoomTransition.md#getversusscreensprite) uses `{ 4, 7, 4, 7 }`
    * EffectVariant.HALO (specifically the one used by Cursed Death's Head), EffectVariant.PURGATORY, and one of special Hell backdrop layers for the Beast fight use `{ 0, 2, 0, 2 }`

___
## Variables
### Flag1 {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) Flag1 {: .copyable aria-label='Variables'}
This is generally always `ONE`. There's a possibility for this to be `SRC_ALPHA` if an internal `UsePremultipliedAlpha` variable in the graphics manager class isn't set, but it has so far always been.

???+ warning "Depreciation Warning"
    This variable will be depreciated in favor of `RGBSourceFactor` next release.

___
### Flag2 {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) Flag2 {: .copyable aria-label='Variables'}
`ZERO` in "Mode 0", `ONE_MINUS_SRC_ALPHA` in "Mode 1", and `ONE` in "Mode 2".

???+ warning "Depreciation Warning"
    This variable will be depreciated in favor of `RGBDestinationFactor` next release.

___
### Flag3 {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) Flag3 {: .copyable aria-label='Variables'}
Set to `ONE` in all modes.

???+ warning "Depreciation Warning"
    This variable will be depreciated in favor of `AlphaSourceFactor` next release.

___
### Flag4 {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) Flag4 {: .copyable aria-label='Variables'}
`ZERO` in "Mode 0", `ONE_MINUS_SRC_ALPHA` in "Mode 1", and `ONE` in "Mode 2".

???+ warning "Depreciation Warning"
    This variable will be depreciated in favor of `AlphaDestinationFactor` next release.
___
