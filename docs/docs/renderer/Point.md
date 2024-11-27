---
tags:
  - Class
---
# Class "Point"

Used for [Beam](Beam.md).

## Constructors
### Beam () {: aria-label='Constructors' }
#### [Point](Point.md) Point ( [Vector](../Vector.md) Position, float SpritesheetCoordinate, float Width = 1.0, [Color](../Color.md) Color = Default, boolean IsWorldSpace = false ) {: .copyable aria-label='Constructors' }


## Functions

### GetColor  () {: aria-label='Functions' }
#### [Color](../Color.md) GetColor ( ) {: .copyable aria-label='Functions' }   

The color the Beam should be at this Point. This is interpolated between points.

___
### GetIsWorldSpace () {: aria-label='Functions' }
#### boolean GetIsWorldSpace ( ) {: .copyable aria-label='Functions' }   

If this is set, `Position` will be automatically converted from worldspace to screenspace during rendering.

___
### GetSpritesheetCoordinate  () {: aria-label='Functions' }
#### float GetSpritesheetCoordinate ( ) {: .copyable aria-label='Functions' }   

The `Y` position of the spritesheet that should be drawn by the time this Point is reached. For example, two points of `0` and `64` SpritesheetCoordinate will render the spritesheet starting from `y 0` to `y 64`, while an additional third point of `0` will draw it in reverse from `y 64` to `y 0`.

___
### GetWidth () {: aria-label='Functions' }
#### float GetWidth ( ) {: .copyable aria-label='Functions' }

A multiplier for how wide the beam should be. A non-zero value will scale the spritesheet width accordingly. This is interpolated between points.

___
### GetPosition () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetPosition ( ) {: .copyable aria-label='Functions' }   

Where (in screenspace) to render the Point.

___
### SetColor  () {: aria-label='Functions' }
#### void SetColor ( [Color](../Color.md) Color ) {: .copyable aria-label='Functions' }   

___
### SetIsWorldSpace () {: aria-label='Functions' }
#### void SetIsWorldSpace ( boolean Value ) {: .copyable aria-label='Functions' }   

___
### SetSpritesheetCoordinate () {: aria-label='Functions' }
#### void SetSpritesheetCoordinate ( float Coordinate ) {: .copyable aria-label='Functions' }   

___
### SetWidth () {: aria-label='Functions' }
#### void SetWidth ( float Width ) {: .copyable aria-label='Functions' }

___
### SetPosition () {: aria-label='Functions' }
#### void SetPosition ( [Vector](../Vector.md) Position ) {: .copyable aria-label='Functions' }   

___
