# Class "Sprite"

Currently, this class does not have any new additions in REPENTOGON, only reimplemented versions of functions already in the API with added functionality.

## Modified Functions

### Replace·Spritesheet () {: aria-label='Functions' }
#### void ReplaceSpritesheet ( int LayerId, string PngFilename, bool︎ LoadGraphics = false ) {: .copyable aria-label='Functions' }
Now accepts an optional `bool` to determine if Sprite:LoadGraphics should be called after replacing the spritesheet. In most cases, you'll want to do this.

___

## Functions

### Get·Color () {: aria-label='Functions' }
#### [KColor](https://wofsauge.github.io/IsaacDocs/rep/Color.html) GetColor () {: .copyable aria-label='Functions' }

___
### Get·Crop·Offset () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetCropOffset ( ) {: .copyable aria-label='Functions' }

___
### Get·Layer () {: aria-label='Functions' }
#### [LayerState](LayerState.md) GetLayer ( int LayerId ) {: .copyable aria-label='Functions' }

___
### Get·Null·Frame () {: aria-label='Functions' }
#### [NullFrame](NullFrame.md) GetNullFrame ( int LayerId ) {: .copyable aria-label='Functions' }

___
### Get·Pos () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetPos () {: .copyable aria-label='Functions' }

___
### Get·Rotation () {: aria-label='Functions' }
#### int GetRotation () {: .copyable aria-label='Functions' }

___
### Get·Size () {: aria-label='Functions' }
#### [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) GetSize () {: .copyable aria-label='Functions' }

___
### Get·Spritesheet·Path () {: aria-label='Functions' }
#### string GetSpritesheetPath () {: .copyable aria-label='Functions' }

___
### Is·Visible () {: aria-label='Functions' }
#### boolean IsVisible () {: .copyable aria-label='Functions' }

___
### Set·Color () {: aria-label='Functions' }
#### void SetColor ( [KColor](https://wofsauge.github.io/IsaacDocs/rep/Color.html) Color ) {: .copyable aria-label='Functions' }

___
### Set·Crop·Offset () {: aria-label='Functions' }
#### void SetCropOffset ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) CropOffset ) {: .copyable aria-label='Functions' }

___
### Set·Pos () {: aria-label='Functions' }
#### void SetPos ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Position ) {: .copyable aria-label='Functions' }

___
### Set·Rotation () {: aria-label='Functions' }
#### void SetRotation ( int Rotation ) {: .copyable aria-label='Functions' }

___
### Set·Size () {: aria-label='Functions' }
#### void SetSize ( [Vector](https://wofsauge.github.io/IsaacDocs/rep/Vector.html) Size ) {: .copyable aria-label='Functions' }

___
### Set·Visible () {: aria-label='Functions' }
#### void SetVisible (boolean Visible) {: .copyable aria-label='Functions' }

___
