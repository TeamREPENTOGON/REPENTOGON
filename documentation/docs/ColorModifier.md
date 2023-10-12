# Class "ColorModifier"

???+ info
    You can get this class by using the following functions:

    * [FXParams:GetColorModifier()](FXParams.md#getcolormodifier)

    ???+ example "Example Code"
        ```lua
        local colorMod = Game():GetCurrentRoom():GetFXParams():GetColorModifier()
        ```

## Variables
### R {: aria-label='Variables' }
#### float R {: .copyable aria-label='Variables'}


___
### G {: aria-label='Variables' }
#### float G {: .copyable aria-label='Variables'}

___
### B {: aria-label='Variables' }
#### float B {: .copyable aria-label='Variables'}

___
### A {: aria-label='Variables' }
#### float A {: .copyable aria-label='Variables'}
???+ warning "Warning"
    This acts as a strength multiplier and must be non-zero in order for RGB to have any effect!
___
### Brightness {: aria-label='Variables' }
#### float Brightness {: .copyable aria-label='Variables'}

___
### Contrast {: aria-label='Variables' }
#### float Contrast {: .copyable aria-label='Variables'}

## Operators
### __add () {: aria-label='Operators' }
#### [ColorModifier](ColorModifier.md) __add ( [ColorModifier](ColorModifier.md) right ) {: .copyable aria-label='Operators' }

Defines the addition of two [ColorModifier](ColorModifier.md) objects using the `+` operator.
___
### __div () {: aria-label='Operators' }
#### [ColorModifier](ColorModifier.md) __div ( [ColorModifier](ColorModifier.md) right ) {: .copyable aria-label='Operators' }

Defines the division of a [ColorModifier](ColorModifier.md) object and a `float` using the `/` operator. The `ColorModifier` must be on the left side.
___
### __eq () {: aria-label='Operators' }
#### [ColorModifier](ColorModifier.md) __eq ( [ColorModifier](ColorModifier.md) right ) {: .copyable aria-label='Operators' }

Defines equal of two [ColorModifier](ColorModifier.md) objects using the `==` operator.
___
### __mul () {: aria-label='Operators' }
#### [ColorModifier](ColorModifier.md) __mul ( [ColorModifier](ColorModifier.md) right ) {: .copyable aria-label='Operators' }

Defines the multiplication of a [ColorModifier](ColorModifier.md) object and a `float` using the `*` operator. The `ColorModifier` must be on the left side.
___
### __sub () {: aria-label='Operators' }
#### [ColorModifier](ColorModifier.md) __sub ( [ColorModifier](ColorModifier.md) right ) {: .copyable aria-label='Operators' }

Defines the subtraction of two [ColorModifier](ColorModifier.md) objects using the `-` operator.
___
