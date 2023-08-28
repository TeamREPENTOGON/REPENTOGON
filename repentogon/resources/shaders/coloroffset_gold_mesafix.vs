attribute vec3 Position;
attribute vec4 Color;
attribute vec2 TexCoord;
attribute vec4 ColorizeIn;
attribute vec3 ColorOffsetIn;
attribute vec2 TextureSize;
attribute float PixelationAmount;
attribute vec3 ClipPlane;

varying vec4 Color0;
varying vec2 TexCoord0;
varying vec4 ColorizeOut;
varying vec3 ColorOffsetOut;
varying vec2 TextureSizeOut;
varying float PixelationAmountOut;
varying vec3 ClipPlaneOut;

uniform mat4 Transform;

void main(void)
{
	ColorizeOut = ColorizeIn;
	ColorOffsetOut = ColorOffsetIn;
	
	Color0 = Color;
	TextureSizeOut = TextureSize;
	PixelationAmountOut = PixelationAmount;
	ClipPlaneOut = ClipPlane;
	gl_Position = Transform * vec4(Position.xyz, 1.0);
	TexCoord0 = TexCoord;
}
