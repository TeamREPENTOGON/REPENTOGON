---
tags:
  - Class
---
# Class "FXParams"

???+ info
    You can get this class by using the following functions:

    * [Room:GetFXParams()](Room.md#getfxparams)

    ???+ example "Example Code"
        ```lua
        local fxparams = Game():GetRoom():GetFXParams()
        ```

## Variables
### Color·Modifier {: aria-label='Variables' }
#### [ColorModifier](ColorModifier.md) ColorModifier {: .copyable aria-label='Variables'}
Gets a modifiable copy of the color correction introduced in Repentance. This stores the values used in `fxlayers.xml` and not the raw values (see [GetCurrentColorModifier](Game.md#getcurrentcolormodifier) for this).

Changes made here are _not_ automatically applied, use [UpdateColorModifier](Room.md#updatecolormodifier) to do this.
___
### Light·Color {: aria-label='Variables' }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) LightColor {: .copyable aria-label='Variables'}

___
### Shadow·Alpha {: aria-label='Variables' }
#### float ShadowAlpha {: .copyable aria-label='Variables'}

___
### Shadow·Color {: aria-label='Variables' }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) ShadowColor {: .copyable aria-label='Variables'}

___
### Use·Water·V2 {: aria-label='Variables' }
#### boolean UseWaterV2 {: .copyable aria-label='Variables'}
If set, water will use the reflective shader featured in Downpour and Dross.

___
### Water·Color {: aria-label='Variables' }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) WaterColor {: .copyable aria-label='Variables'}

___
### Water·Color·Multiplier {: aria-label='Variables' }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) WaterColorMultiplier {: .copyable aria-label='Variables'}

___
### Water·Effect·Color {: aria-label='Variables' }
#### [Color](https://wofsauge.github.io/IsaacDocs/rep/Color.html) WaterEffectColor {: .copyable aria-label='Variables'}

___
