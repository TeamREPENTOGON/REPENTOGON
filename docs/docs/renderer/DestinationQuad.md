---
tags:
  - Class
---
# Class "DestinationQuad"

## Constructors
### DestinationQuad () {: aria-label='Constructors' }
#### [DestinationQuad](DestinationQuad.md) DestinationQuad ( [Vector](../Vector.md) TopLeft, [Vector](../Vector.md) TopRight, [Vector](../Vector.md) BottomLeft, [Vector](../Vector.md) BottomRight ) {: .copyable aria-label='Constructors' }

## Functions

### __tostring () {: aria-label='Functions' }
#### string __tostring ( ) {: .copyable aria-label='Operators' }
Creates a string representation of the quad object.

This enables direct printing of the object by doing `print(quad)`

### GetBottomLeft () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetBottomLeft ( ) {: .copyable aria-label='Functions' }      

___
### GetBottomRight () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetBottomRight ( ) {: .copyable aria-label='Functions' }     

___
### GetTopLeft () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetTopLeft ( ) {: .copyable aria-label='Functions' }

___
### GetTopRight () {: aria-label='Functions' }
#### [Vector](../Vector.md) GetTopRight ( ) {: .copyable aria-label='Functions' }        

___
### SetBottomLeft () {: aria-label='Functions' }
#### void SetBottomLeft ( [Vector](../Vector.md) Vector ) {: .copyable aria-label='Functions' }

___
### SetBottomRight () {: aria-label='Functions' }
#### void SetBottomRight ( [Vector](../Vector.md) Vector ) {: .copyable aria-label='Functions' }

___
### SetTopLeft () {: aria-label='Functions' }
#### void SetTopLeft ( [Vector](../Vector.md) Vector ) {: .copyable aria-label='Functions' }

___
### SetTopRight () {: aria-label='Functions' }
#### void SetTopRight ( [Vector](../Vector.md) Vector ) {: .copyable aria-label='Functions' }

___
### Copy () {: aria-label='Functions' }
#### [DestinationQuad](DestinationQuad.md) Copy ( ) {: .copyable aria-label='Functions' }

___
### Translate () {: aria-label='Functions' }
#### void Translate ( [Vector](../Vector.md) offset ) {: .copyable aria-label='Functions' }

___
### Scale () {: aria-label='Functions' }
#### void Scale ( [Vector](../Vector.md) scale, [Vector](../Vector.md) anchor ) {: .copyable aria-label='Functions' }

___
### Rotate () {: aria-label='Functions' }
#### void Rotate ( number rotation, [Vector](../Vector.md) anchor ) {: .copyable aria-label='Functions' }

The rotation is in Degrees.

___
### Shear () {: aria-label='Functions' }
#### void Shear ( [Vector](../Vector.md) shear, [Vector](../Vector.md) anchor ) {: .copyable aria-label='Functions' }

___
### ApplyMatrix () {: aria-label='Functions' }
#### void ApplyMatrix ( table renderMatrix, [Vector](../Vector.md) anchor ) {: .copyable aria-label='Functions' }

Applies a 2x3 render matrix to the image, effectively applying a `Translate`, `Scale`, `Rotate` and `Shear` all at once.

The table must have at least 2 rows, each with at least 3 columns.

___
### Flip () {: aria-label='Functions' }
#### void Flip ( boolean flipX, boolean flipY ) {: .copyable aria-label='Functions' }

___