# Class "Sprite"

## Modified Functions

### Replace·Spritesheet () {: aria-label='Modified Functions' }
#### void ReplaceSpritesheet ( int LayerId, string PngFilename, boolean LoadGraphics = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

___
### Set·Overlay·Frame () {: aria-label='Functions' }
#### void SetOverlayFrame ( int FrameNum ) {: .copyable aria-label='Functions' }
New overload for `SetOverlayFrame()` that sets the frame for the current animation without stopping it, akin to the one for `SetFrame()`.

___
### Stop () {: aria-label='Functions' }
#### void Stop ( boolean StopOverlay = true ) {: .copyable aria-label='Functions' }
Now accepts an optional `bool` for whether to also stop the overlay animation. Defaults to true.

___

## Functions

### Get·All·Animation·Data () {: aria-label='Functions' }
#### table GetAllAnimationData ( ) {: .copyable aria-label='Functions' }
Returns a table of AnimationData representing all animations in this anm2 file.

___
### Get·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetAnimationData ( string AnimationName ) {: .copyable aria-label='Functions' }

___
### Get·Current·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetCurrentAnimationData ( ) {: .copyable aria-label='Functions' }

___
### Get·Layer () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }

___
### Get·Layer () {: aria-label='Functions' }
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
### Continue () {: aria-label='Functions' }
#### void Continue ( boolean ContinueOverlay = true ) {: .copyable aria-label='Functions' }
If the animation is currently stopped, makes it start playing again from the current frame. Will not restart a finished, non-looping animation.

___
### Continue·Overlay () {: aria-label='Functions' }
#### void ContinueOverlay ( ) {: .copyable aria-label='Functions' }
Same as above, but only for the overlay animation.

___
### Stop·Overlay () {: aria-label='Functions' }
#### void StopOverlay ( ) {: .copyable aria-label='Functions' }

___
### Is·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean IsOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
### Was·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean WasOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
### Get·Bit·Flags() {: aria-label='Functions' }
#### [AnimRenderFlags](enums/AnimRenderFlags.md) GetBitFlags ( ) {: .copyable aria-label='Functions' }

___
### Set·Bit·Flags () {: aria-label='Functions' }
#### void SetBitFlags ( [AnimRenderFlags](enums/AnimRenderFlags.md) Flags ) {: .copyable aria-label='Functions' }

___
