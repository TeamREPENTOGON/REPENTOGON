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
???+ info "Mode usages"
	The game uses:
	
	* `CONSTANT` for the flat blue Planetarium backdrop layer and the flat color layers of the Hell backdrop
	* `NORMAL` as the default value
	* `ADDITIVE` for Planetarium backdrop starfield layers, `8bitclouds_utero` backdrop layer, the `lava_add` layer of the Hell backdrop, Beast death spotlight, and various other misc effects
	* `MULTIPLICATIVE` for EffectVariant.HALO (specifically the one used by Cursed Death's Head), EffectVariant.PURGATORY, and the `lava_multiply` layer of the Hell backdrop
	* `OVERLAY` for EffectVariant.HUSH_ASHES & the overlay layer of the [Boss Intro versus Sprite](../RoomTransition.md#getversusscreensprite)
	
???- info "Nonstandard modes"
    There have also been a couple nonstandard modes seen used for specific situations:

    * EffectVariant.BACKDROP_DECORATION, as used by Mother for the fake backdrop wall, uses `{ RGBSourceFactor.ZERO, RGBDestinationFactor.SRC_COLOR, AlphaSourceFactor.ONE, AlphaDestinationFactor.ONE }`
	* Light rendering uses `{ RGBSourceFactor.DST_COLOR, RGBDestinationFactor.DST_ALPHA, AlphaSourceFactor.DST_COLOR, AlphaDestinationFactor.ONE_MINUS_SRC_ALPHA }`

___
## Variables
### AlphaDestinationFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) AlphaDestinationFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `CONSTANT`, `ONE_MINUS_SRC_ALPHA` in mode `NORMAL` and `OVERLAY`, `ONE` in mode `ADDITIVE`, and `SRC_COLOR` in `MULTIPLICATIVE`.

???- warning "Depreciation history"
    This variable was previously known as `Flag4`.

___
### AlphaSourceFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) AlphaSourceFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `MULTIPLICATIVE`, `ONE` in all other modes.

???- warning "Depreciation history"
    This variable was previously known as `Flag3`.

___
### RGBDestinationFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) RGBDestinationFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `CONSTANT`, `ONE_MINUS_SRC_ALPHA` in mode `NORMAL` and `OVERLAY`, `ONE` in mode `ADDITIVE`, and `SRC_COLOR` in `MULTIPLICATIVE`.

???- warning "Depreciation history"
    This variable was previously known as `Flag2`.

___
### RGBSourceFactor {: aria-label='Variables' }
#### [BlendFactor](../enums/BlendFactor.md) RGBSourceFactor {: .copyable aria-label='Variables'}
`ZERO` in mode `MULTIPLICATIVE`, `ONE` in all other modes (there's a possibility for this to be `SRC_ALPHA` if an internal `UsePremultipliedAlpha` variable in the graphics manager class isn't set, but this has not yet been observed)

???- warning "Depreciation history"
    This variable was previously known as `Flag1`.

___