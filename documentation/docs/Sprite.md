# Class "Sprite"

## Modified Functions

### Replace·Spritesheet () {: aria-label='Modified Functions' }
#### void ReplaceSpritesheet ( int LayerId, string PngFilename, bool︎ LoadGraphics = false ) {: .copyable aria-label='Modified Functions' }
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

___

## Functions

### Get·All·Animation·Data () {: aria-label='Functions' }
#### table GetAnimationData ( ) {: .copyable aria-label='Functions' }
Returns a table of AnimationData representing all animations in this anm2 file.

___
### Get·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetAnimationData ( string AnimationName ) {: .copyable aria-label='Functions' }

___
### Get·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetAnimationState ( ) {: .copyable aria-label='Functions' }

___
### Get·Current·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetCurrentAnimationData ( ) {: .copyable aria-label='Functions' }

___
### Get·Layer () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }

___
### Get·Layer·By·Name () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayerByName ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Get·Null·Frame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Get·Overlay·Animation·Data () {: aria-label='Functions' }
#### [AnimationData](AnimationData.md) GetOverlayAnimationData ( ) {: .copyable aria-label='Functions' }

___
### Get·Overlay·Animation·State () {: aria-label='Functions' }
#### [AnimationState](AnimationState.md) GetOverlayAnimationState ( ) {: .copyable aria-label='Functions' }

___
### Get·Overlay·Null·Frame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetOverlayNullFrame ( string LayerName ) {: .copyable aria-label='Functions' }

___
### Is·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean IsOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___
### Was·Overlay·Event·Triggered () {: aria-label='Functions' }
#### boolean WasOverlayEventTriggered ( string EventName ) {: .copyable aria-label='Functions' }

___

