---
tags:
  - Class
---
# Class "Point"

Used for [Beam](Beam.md).

## Constructors
### Beam () {: aria-label='Constructors' }
#### [Point](Point.md) Point ( [Vector](../Vector.md) Position, float SpritesheetCoordinate, float Width = 1.0 ) {: .copyable aria-label='Constructors' }

???+ info "Info"
    `SpritesheetCoordinate` is, to our current understanding, the `Y` position of the spritesheet that should be drawn by the time this Point is reached. For example, two points of `0` and `64` SpritesheetCoordinate will render the spritesheet starting from `y 0` to `y 64`, while an additional third point of `0` will draw it in reverse from `y 64` to `y 0`.
	`Width` acts as a multiplier for how wide the beam should be. A non-zero value will scale the spritesheet width accordingly. This is interpolated between points.

___

## Functions

### GetSpritesheetCoordinate  () {: aria-label='Functions' }
#### float GetSpritesheetCoordinate ( ) {: .copyable aria-label='Functions' }   

___
### GetWidth () {: aria-label='Functions' }
#### float GetWidth ( ) {: .copyable aria-label='Functions' }

___
### GetPosition () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetPosition ( ) {: .copyable aria-label='Functions' }   

___
### SetSpritesheetCoordinate () {: aria-label='Functions' }
#### void SetSpritesheetCoordinate ( float Height ) {: .copyable aria-label='Functions' }   

___
### SetWidth () {: aria-label='Functions' }
#### void SetWidth ( float Width ) {: .copyable aria-label='Functions' }

___
### SetPosition () {: aria-label='Functions' }
#### void SetPosition ( [Vector](../Vector.md) Position ) {: .copyable aria-label='Functions' }   

___
