---
tags:
  - Class
---
# Class "LayerState"

## Functions

### ClearCustomChampionShader () {: aria-label='Functions' }
#### void ClearCustomChampionShader ( ) {: .copyable aria-label='Functions' }
Removes any custom `coloroffset_champion` shader applied by `layer:SetCustomChampionShader(path)`.

___
### ClearCustomShader () {: aria-label='Functions' }
#### void ClearCustomShader ( ) {: .copyable aria-label='Functions' }
Removes any custom `coloroffset` shader applied by `layer:SetCustomShader(path)`.

___
### GetBlendMode () {: aria-label='Functions' }
#### [BlendMode](renderer/BlendMode.md) GetBlendMode ( ) {: .copyable aria-label='Functions' }

___
### GetColor () {: aria-label='Functions' }
#### [Color](Color.md) GetColor ( ) {: .copyable aria-label='Functions' }

___
### GetCropOffset () {: aria-label='Functions' }
#### [Vector](Vector.md) GetCropOffset ( ) {: .copyable aria-label='Functions' }

___
### GetDefaultSpritesheetPath () {: aria-label='Functions' }
#### string GetDefaultSpritesheetPath ( ) {: .copyable aria-label='Functions' }

___
### GetFlipX () {: aria-label='Functions' }
#### boolean GetFlipX ( ) {: .copyable aria-label='Functions' }

___
### GetFlipY () {: aria-label='Functions' }
#### boolean GetFlipY ( ) {: .copyable aria-label='Functions' }

___
### GetLayerID () {: aria-label='Functions' }
#### int GetLayerID ( ) {: .copyable aria-label='Functions' }

___
### GetName () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }

___
### GetPos () {: aria-label='Functions' }
#### [Vector](Vector.md) GetPos ( ) {: .copyable aria-label='Functions' }

___
### GetRenderFlags () {: aria-label='Functions' }
#### [AnimRenderFlags](enums/AnimRenderFlags.md) GetRenderFlags ( ) {: .copyable aria-label='Functions' }

___
### GetRotation () {: aria-label='Functions' }
#### float GetRotation ( ) {: .copyable aria-label='Functions' }

___
### GetSize () {: aria-label='Functions' }
#### [Vector](Vector.md) GetSize ( ) {: .copyable aria-label='Functions' }

___
### GetSpritesheetPath () {: aria-label='Functions' }
#### string GetSpritesheetPath ( ) {: .copyable aria-label='Functions' }

___
### GetWrapSMode () {: aria-label='Functions' }
#### int GetWrapSMode ( ) {: .copyable aria-label='Functions' }
Returns the wrap mode for the `x` coordinate. See [this website](https://open.gl/textures) for a general overview.

???+ info "Info"
  Enums for these are pending.

___
### GetWrapTMode () {: aria-label='Functions' }
#### int GetWrapTMode ( ) {: .copyable aria-label='Functions' }
Returns the wrap mode for the `y` coordinate. See [this website](https://open.gl/textures) for a general overview.

???+ info "Info"
  Enums for these are pending.

___
### HasCustomChampionShader () {: aria-label='Functions' }
#### boolean HasCustomChampionShader ( string ShaderPath ) {: .copyable aria-label='Functions' }
Returns true if the specified custom champion shader is currently loaded (see `SetCustomChampionShader` below). If no string is provided, returns true if ANY custom champion shader is applied.

___
### HasCustomShader () {: aria-label='Functions' }
#### boolean HasCustomShader ( string ShaderPath ) {: .copyable aria-label='Functions' }
Returns true if the specified custom shader is currently loaded (see `SetCustomShader` below). If no string is provided, returns true if ANY custom shader is applied.

___
### IsVisible () {: aria-label='Functions' }
#### boolean IsVisible ( ) {: .copyable aria-label='Functions' }

___
### SetColor () {: aria-label='Functions' }
#### void SetColor ( [Color](Color.md) Color ) {: .copyable aria-label='Functions' }

___
### SetCropOffset () {: aria-label='Functions' }
#### void SetCropOffset ( [Vector](Vector.md) Offset ) {: .copyable aria-label='Functions' }

___
### SetCustomChampionShader () {: aria-label='Functions' }
#### void SetCustomChampionShader ( string ShaderPath ) {: .copyable aria-label='Functions' }
Specify a custom champion shader file for this layer to use instead of the usual `coloroffset_champion` shader. Provided path is expected to start at `.../resources/` and to find both a .vs and .fs file at that location. For example: `layer:SetCustomChampionShader("shaders/my_shader")` will load `.../resources/shaders/my_shader.vs` and `.../resources/shaders/my_shader.fs`.

The custom champion shader will only be used by the game if the entity is actually a champion.

Note that the custom shader must take the exact same inputs as the default `coloroffset_champion` shader the game uses (it has one additional input compared to `coloroffset`).

___
### SetCustomShader () {: aria-label='Functions' }
#### void SetCustomShader ( string ShaderPath ) {: .copyable aria-label='Functions' }
Specify a custom shader file for this layer to use instead of the default `coloroffset` shader. Provided path is expected to start at `.../resources/` and to find both a .vs and .fs file at that location. For example: `layer:SetCustomShader("shaders/my_shader")` will load `.../resources/shaders/my_shader.vs` and `.../resources/shaders/my_shader.fs`.

This custom shader will not be used by the game if the entity is a champion, or if it has the gold/dogma shader applied.

Note that the custom shader must take the exact same inputs as the default `coloroffset` shader the game uses. The gold and dogma shaders also use the same inputs, and can be a good reference.

___
### SetFlipX () {: aria-label='Functions' }
#### void SetFlipX ( boolean FlipX ) {: .copyable aria-label='Functions' }

___
### SetFlipY () {: aria-label='Functions' }
#### void SetFlipY ( boolean FlipY ) {: .copyable aria-label='Functions' }

___
### SetPos () {: aria-label='Functions' }
#### void SetPos ( [Vector](Vector.md) Pos ) {: .copyable aria-label='Functions' }

___
### SetRenderFlags () {: aria-label='Functions' }
#### void SetRenderFlags ( [AnimRenderFlags](enums/AnimRenderFlags.md) Flags ) {: .copyable aria-label='Functions' }

___
### SetRotation () {: aria-label='Functions' }
#### void SetRotation ( float Rotation ) {: .copyable aria-label='Functions' }

___
### SetSize () {: aria-label='Functions' }
#### void SetSize ( [Vector](Vector.md) Size ) {: .copyable aria-label='Functions' }

___
### SetVisible () {: aria-label='Functions' }
#### void SetVisible ( boolean Visible ) {: .copyable aria-label='Functions' }

___
### SetWrapSMode () {: aria-label='Functions' }
#### void SetWrapSMode ( int Mode ) {: .copyable aria-label='Functions' }
Sets the wrap mode for the `x` coordinate. See [this website](https://open.gl/textures) for a general overview.

???+ info "Info"
  Enums for these are pending.

___
### SetWrapTMode () {: aria-label='Functions' }
#### void SetWrapTMode ( int Mode ) {: .copyable aria-label='Functions' }
Sets the wrap mode for the `y` coordinate. See [this website](https://open.gl/textures) for a general overview.

???+ info "Info"
  Enums for these are pending.

___
