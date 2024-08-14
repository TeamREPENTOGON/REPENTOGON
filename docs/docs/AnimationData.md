---
tags:
  - Class
---
# Class "AnimationData"

Cached data for one animation from a loaded ANM2 file. Shared by all Sprites using the same ANM2 and cannot be modified.

Can be obtained via [Sprite:GetAnimationData()](Sprite.md#getanimationdata), [Sprite:GetCurrentAnimationData()](Sprite.md#getanimationdata) or [Sprite:GetOverlayAnimationData()](Sprite.md#getanimationdata).

## Functions

### GetAllLayers () {: aria-label='Functions' }
#### [AnimationLayer](AnimationLayer.md)[] GetAllLayers ( ) {: .copyable aria-label='Functions' }
Returns a table of AnimationLayers, in order from bottom to top (NOT ordered by layer ID).

___
### GetLayer () {: aria-label='Functions' }
#### [AnimationLayer](AnimationLayer.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }
Gets an AnimationLayer by that layer's ID number.

___
### GetLength () {: aria-label='Functions' }
#### int GetLength ( ) {: .copyable aria-label='Functions' }
Length of this animation in frames.

___
### GetName () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### IsLoopingAnimation () {: aria-label='Functions' }
#### boolean IsLoopingAnimation ( ) {: .copyable aria-label='Functions' }

___

