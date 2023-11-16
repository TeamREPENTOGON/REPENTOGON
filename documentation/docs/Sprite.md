# Class "Sprite"

## Modified Functions

### Replace·Spritesheet () {: aria-label='Modified Functions' }
#### void ReplaceSpritesheet ( int LayerId, string PngFilename, boolean LoadGraphics = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

___
### Set·Overlay·Frame () {: aria-label='Modified Functions' }
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
### Continue·Overlay () {: aria-label='Functions' }
#### void ContinueOverlay ( ) {: .copyable aria-label='Functions' }
Same as above, but only for the overlay animation.

___
### Get·All·Animation·Data () {: aria-label='Functions' }
#### table GetAllAnimationData ( ) {: .copyable aria-label='Functions' }
Returns a table of [AnimationData](AnimationData.md) representing all animations in this anm2 file.

___
### Get·All·Layers () {: aria-label='Functions' }
#### table GetAllLayers ( ) {: .copyable aria-label='Functions' }
Returns a table of all [LayerStates](LayerState.md) in this Sprite.

___
### Get·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetAnimationData ( string AnimationName ) {: .copyable aria-label='Functions' }

___
### Get·Current·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetCurrentAnimationData ( ) {: .copyable aria-label='Functions' }

___
### Get·Layer () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Get·Null·Frame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Get·Overlay·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetOverlayAnimationData ( ) {: .copyable aria-label='Functions' }

___
### Get·Overlay·Null·Frame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetOverlayNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Get·Render·Flags() {: aria-label='Functions' }
#### [AnimRenderFlags](enums/AnimRenderFlags.md) GetRenderFlags ( ) {: .copyable aria-label='Functions' }

___
### Is·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean IsOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
### Set·Render·Flags () {: aria-label='Functions' }
#### void SetRenderFlags ( [AnimRenderFlags](enums/AnimRenderFlags.md) Flags ) {: .copyable aria-label='Functions' }

___
### Stop·Overlay () {: aria-label='Functions' }
#### void StopOverlay ( ) {: .copyable aria-label='Functions' }

___
### Was·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean WasOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
