---
tags:
  - Class
---
# Class "AnimationData"

Cached data for one animation from a loaded Anm2 file. Shared by all Sprites using the same Anm2 and cannot be modified.

Can be obtained via [Sprite:GetAnimationData()](Sprite.md#getanimationdata), [Sprite:GetCurrentAnimationData()](Sprite.md#getanimationdata) or [Sprite:GetOverlayAnimationData()](Sprite.md#getanimationdata).

## Functions

### Get·All·Layers () {: aria-label='Functions' }
#### [AnimationLayer](AnimationLayer.md)[] GetAllLayers ( ) {: .copyable aria-label='Functions' }
Returns a table of AnimationLayers, in order from bottom to top (NOT ordered by layer ID).

___
### Get·Layer () {: aria-label='Functions' }
#### [AnimationLayer](AnimationLayer.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }
Gets an AnimationLayer by that layer's ID number.

___
### Get·Length () {: aria-label='Functions' }
#### int GetLength ( ) {: .copyable aria-label='Functions' }
Length of this animation in frames.

___
### Get·Name () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### Is·Looping·Animation () {: aria-label='Functions' }
#### boolean IsLoopingAnimation ( ) {: .copyable aria-label='Functions' }

___

