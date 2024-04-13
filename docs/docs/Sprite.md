---
tags:
  - Class
---
# Class "Sprite"

## Modified Constructors

### Sprite () {: aria-label='Modified Constructors' }
#### [Sprite](Sprite.md),bool Sprite ( string ANM2Path, bool LoadGraphics = true ) {: .copyable aria-label='Modified Constructors' }

Added two optional arguments, the function now returns two values: [Sprite](Sprite.md) object and the bool signifying whether the sprite was loaded successfully or not.
___
## Modified Functions

### ReplaceSpritesheet () {: aria-label='Modified Functions' }
#### void ReplaceSpritesheet ( int LayerId, string PngFilename, boolean LoadGraphics = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

___
### SetOverlayFrame () {: aria-label='Modified Functions' }
#### void SetOverlayFrame ( int FrameNum ) {: .copyable aria-label='Modified Functions' }
New overload for `SetOverlayFrame()` that sets the frame for the current animation without stopping it, akin to the one for `SetFrame()`.

___
### Stop () {: aria-label='Modified Functions' }
#### void Stop ( boolean StopOverlay = true ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` for whether to also stop the overlay animation. Defaults to true.

___

## Functions

### Continue () {: aria-label='Functions' }
#### void Continue ( boolean ContinueOverlay = true ) {: .copyable aria-label='Functions' }
If the animation is currently stopped, makes it start playing again from the current frame. Will not restart a finished, non-looping animation.

___
### ContinueOverlay () {: aria-label='Functions' }
#### void ContinueOverlay ( ) {: .copyable aria-label='Functions' }
Same as above, but only for the overlay animation.

___
### GetAllAnimationData () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md)[] GetAllAnimationData ( ) {: .copyable aria-label='Functions' }
Returns a table of [AnimationData](AnimationData.md) representing all animations in this anm2 file.

___
### GetAllLayers () {: aria-label='Functions' }
#### [LayerStates](LayerState.md)[] GetAllLayers ( ) {: .copyable aria-label='Functions' }
Returns a table of all [LayerStates](LayerState.md) in this Sprite.

___
### GetAnimationData () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetAnimationData ( string AnimationName ) {: .copyable aria-label='Functions' }

___
### GetCurrentAnimationData () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetCurrentAnimationData ( ) {: .copyable aria-label='Functions' }

___
### GetLayer () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( string LayerName ) {: .copyable aria-label='Functions' }
Returns the layer data from the provided layer id.

___
### GetNullFrame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }
Returns the [NullFrame](NullFrame.md) from the provided layer name.

___
### GetOverlayAnimationData () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetOverlayAnimationData ( ) {: .copyable aria-label='Functions' }
Returns the [AnimationData](AnimationData.md) of the currently playing overlay.

___
### GetOverlayNullFrame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetOverlayNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }
Returns the [NullFrame](NullFrame.md) from the provided layer name of the overlay animation.

___
### GetRenderFlags () {: aria-label='Functions' }
#### [AnimRenderFlags](enums/AnimRenderFlags.md) GetRenderFlags ( ) {: .copyable aria-label='Functions' }

___
### IsOverlayEventTriggered () {: aria-label='Functions' }
#### boolean IsOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }
Returns `true` if the currently playing overlay animation just reached the event with the provided name.

___
### SetRenderFlags () {: aria-label='Functions' }
#### void SetRenderFlags ( [AnimRenderFlags](enums/AnimRenderFlags.md) Flags ) {: .copyable aria-label='Functions' }

___
### StopOverlay () {: aria-label='Functions' }
#### void StopOverlay ( ) {: .copyable aria-label='Functions' }

___
### WasOverlayEventTriggered () {: aria-label='Functions' }
#### boolean WasOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
