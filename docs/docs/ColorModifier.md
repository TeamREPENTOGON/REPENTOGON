---
tags:
  - Class
---
# Class "ColorModifier"

An example mod using the ColorModifier class can be found [here.](./examples/ColorModifiers.md)

???+ info
    This class can be accessed by using its constructor:
    ???+ example "Example Code"
        ```lua
        local tintRed = ColorModifier(1,0,0,0.33,0,1)
        ```

## Constructors
### ColorModifier () {: aria-label='Constructors' }
#### [ColorModifier](ColorModifier.md) ColorModifier ( float R = 1, float G = 1, float B = 1, float A = 0, float Brightness = 0, float Contrast = 1 ) {: .copyable aria-label='Constructors' }

## Variables
### A {: aria-label='Variables' }
#### float A {: .copyable aria-label='Variables'}
???+ warning "Warning"
    This acts as a strength multiplier and must be non-zero in order for RGB to have any effect!
___
### B {: aria-label='Variables' }
#### float B {: .copyable aria-label='Variables'}

___
### Brightness {: aria-label='Variables' }
#### float Brightness {: .copyable aria-label='Variables'}

___
### Contrast {: aria-label='Variables' }
#### float Contrast {: .copyable aria-label='Variables'}

___
### G {: aria-label='Variables' }
#### float G {: .copyable aria-label='Variables'}

___
### R {: aria-label='Variables' }
#### float R {: .copyable aria-label='Variables'}

___
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
