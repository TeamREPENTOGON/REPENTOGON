---
tags:
  - Class
---
# Class "BlendMode"

Determines how a [LayerState](../LayerState.md) should blend with the elements rendered behind it. Specifically, these parameters are used in calls to [glBlendFuncSeparate](https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBlendFuncSeparate.xml).

???+ info
    You can get this class by using the following functions:

    * [LayerState:GetBlendMode()](../LayerState.md#getblendmode)

    ???+ example "Example Code"
        ```lua
        local blendMode = Sprite():GetLayer(0):GetBlendMode()
        ```

## Functions

### SetMode () {: aria-label='Functions' }
#### void SetMode ( [BlendType](../enums/BlendType.md) Type ) {: .copyable aria-label='Functions' } 
`NORMAL` is used as the default value, `ADDITIVE` for the blue base of the Planetarium backdrop, and `MULTIPLICATIVE` for Planetarium backdrop starfield layers, `8bitclouds_utero` backdrop layer, Beast death spotlight, and various other misc effects.

???- info "Nonstandard modes"
    The game additionally manually sets modes for various effects:

    * EffectVariant.BACKDROP_DECORATION, as used by Mother for the fake backdrop wall, uses `{ RGBSourceFactor.ZERO, RGBDestinationFactor.SRC_COLOR, AlphaSourceFactor.ONE, AlphaDestinationFactor.ONE }`
    * EffectVariant.HUSH_ASHES & the [Boss Intro versus Sprite](../RoomTransition.md#getversusscreensprite) uses `{ RGBSourceFactor.DST_COLOR, RGBDestinationFactor.ONE_MINUS_SRC_ALPHA, AlphaSourceFactor.DST_COLOR, AlphaDestinationFactor.ONE_MINUS_SRC_ALPHA }`
    * EffectVariant.HALO (specifically the one used by Cursed Death's Head), EffectVariant.PURGATORY, and one of special Hell backdrop layers for the Beast fight use `{ RGBSourceFactor.ZERO, RGBDestinationFactor.SRC_COLOR, AlphaSourceFactor.ZERO, AlphaDestinationFactor.SRC_COLOR }`

___
## Variables
### AlphaDestinationFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) AlphaDestinationFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `ADDITIVE`, `ONE_MINUS_SRC_ALPHA` in mode `NORMAL`, and `ONE` in mode `MULTIPLICATIVE`.

???- warning "Depreciation history"
    This variable was previously known as `Flag4`.

___
### AlphaSourceFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) AlphaSourceFactor {: .copyable aria-label='Variables'}
Set to `ONE` in all modes.

???- warning "Depreciation history"
    This variable was previously known as `Flag3`.

___
### RGBDestinationFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) RGBDestinationFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `ADDITIVE`, `ONE_MINUS_SRC_ALPHA` in mode `NORMAL`, and `ONE` in mode `MULTIPLICATIVE`.

???- warning "Depreciation history"
    This variable was previously known as `Flag2`.

___
### RGBSourceFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) RGBSourceFactor {: .copyable aria-label='Variables'}
This is generally always `ONE`. There's a possibility for this to be `SRC_ALPHA` if an internal `UsePremultipliedAlpha` variable in the graphics manager class isn't set, but it has so far always been.

???- warning "Depreciation history"
    This variable was previously known as `Flag1`.

___