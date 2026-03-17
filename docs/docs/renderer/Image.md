---
tags:
  - Class
---
# Class "Image"

???+ info
    You can get this class by using the following function:

    * [Renderer.LoadImage()](Renderer.md#loadimage)
    * [Renderer.CreateImage()](Renderer.md#createimage)

## Functions

### GetWidth () {: aria-label='Functions' }
#### number GetWidth ( ) {: .copyable aria-label='Functions' }    

The logical width of the image.

UV coordinates for a `SourceQuad` are based on this dimension.

___
### GetHeight () {: aria-label='Functions' }
#### number GetHeight ( ) {: .copyable aria-label='Functions' }    

The logical height of the image.

UV coordinates for a `SourceQuad` are based on this dimension.

___
### GetPaddedWidth () {: aria-label='Functions' }
#### number GetPaddedWidth ( ) {: .copyable aria-label='Functions' }    

The number of pixels stored per row in memory, including any extra padding added for alignment. This value may be greater than Width.

UV coordinates used in the shader are based on this dimension.

___
### GetPaddedHeight () {: aria-label='Functions' }
#### number GetPaddedHeight ( ) {: .copyable aria-label='Functions' }    

The number of pixels rows in memory, including any extra padding added for alignment. This value may be greater than Height.

UV coordinates used in the shader are based on this dimension.

___
### GetName () {: aria-label='Functions' }
#### string GetName ( ) {: .copyable aria-label='Functions' }    

___
### GetTexelRegion () {: aria-label='Functions' }
#### string GetTexelRegion ( integer x, integer y, integer width, integer height ) {: .copyable aria-label='Functions' }    

Returns the image content for the specified quad.

The returned string contains raw binary data representing 32-bit floating-point values.
Every group of 4 consecutive floats represents one pixel in RGBA order:

___
### Render () {: aria-label='Functions' }
#### void Render ( [SourceQuad](SourceQuad.md) sourceQuad, [DestinationQuad](DestinationQuad.md) destinationQuad, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) color, [Color](../Color.md) colorMod = Color() ) {: .copyable aria-label='Functions' }    

___
### RenderWithShader () {: aria-label='Functions' }
#### void RenderWithShader ( [SourceQuad](SourceQuad.md) sourceQuad, [DestinationQuad](DestinationQuad.md) destinationQuad, [KColor](https://wofsauge.github.io/IsaacDocs/rep/KColor.html) color, [Shader](Shader.md) shader, table shaderParams ) {: .copyable aria-label='Functions' }   

The shaderParams are represented as a `table<string, (number | number[])>`.

  * Use a number for float attributes.
  * Use a numeric array (`number[]`) for vecN attributes, with N being the length.

The function throws an error if any attribute is missing or has an invalid type.

___