#pragma once

#include "ShaderUtils.h"

inline size_t ShaderUtils::GetNumVertexAttributes(KAGE_Graphics_ImageBase_VertexAttributeDescriptor *descriptor)
{
    size_t i = 0;
    unsigned int format = descriptor[i].format;
    
    while (format != 0)
    {
        ++i;
        format = descriptor[i].format;
    }
    
    return i;
}

inline uint32_t ShaderUtils::GetVertexSize(KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor, size_t numAttributes)
{
    uint32_t stride = 0;

    for (size_t i = 0; i < numAttributes; i++)
    {
        stride += GetFormatSize(descriptor[i].format);
    }

    return stride;
}

inline uint32_t ShaderUtils::GetVertexStride(KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor, size_t numAttributes)
{
    return GetVertexSize(descriptor, numAttributes) * sizeof(float);
}

inline void ShaderUtils::ToImageVertexDescriptor(uint8_t *result, KAGE_Graphics_ImageBase_VertexAttributeDescriptor *descriptor, size_t numAttributes)
{
    for (size_t i = 0; i < numAttributes; i++)
    {
        result[i] = (uint8_t)descriptor[i].format;
    }
}

inline void ShaderUtils::ColorOffset::FillVertices(float* vertexBuffer, KAGE_Graphics_ImageBase& image, const ColorMod& colorMod)
{
    assert(vertexBuffer != nullptr);

    float width = (float)image.GetPaddedWidth();
    float height = (float)image.GetPaddedHeight();
    float pixelationAmount = g_ANM2_PixelationAmount;
    Vector clipPaneNormal = g_ANM2_ClipPaneNormal;
    float clipPaneThreshold = g_ANM2_ClipPaneThreshold;

    // for each vertex of the quad
    for (int i = 0; i < 4; i++)
    {
        // vec4 ColorizeIn
        vertexBuffer[COLORIZE_OFFSET + 0] = colorMod._colorize[0];
        vertexBuffer[COLORIZE_OFFSET + 1] = colorMod._colorize[1];
        vertexBuffer[COLORIZE_OFFSET + 2] = colorMod._colorize[2];
        vertexBuffer[COLORIZE_OFFSET + 3] = colorMod._colorize[3];

        // vec3 ColorOffsetIn
        vertexBuffer[COLOR_OFFSET_OFFSET + 0] = colorMod._offset[0];
        vertexBuffer[COLOR_OFFSET_OFFSET + 1] = colorMod._offset[1];
        vertexBuffer[COLOR_OFFSET_OFFSET + 2] = colorMod._offset[2];

        // vec2 TextureSize
        vertexBuffer[TEXTURE_SIZE_OFFSET + 0] = width;
        vertexBuffer[TEXTURE_SIZE_OFFSET + 1] = height;

        // float PixelationAmount
        vertexBuffer[PIXELATION_AMOUNT_OFFSET + 0] = pixelationAmount;

        // vec3 ClipPane
        vertexBuffer[CLIP_PANE_OFFSET + 0] = clipPaneNormal.x;
        vertexBuffer[CLIP_PANE_OFFSET + 1] = clipPaneNormal.y;
        vertexBuffer[CLIP_PANE_OFFSET + 2] = clipPaneThreshold;

        vertexBuffer += VERTEX_SIZE;
    }
}

inline void ShaderUtils::ColorOffsetChampion::FillVertices(float* vertexBuffer, KAGE_Graphics_ImageBase& image, const ColorMod& colorMod, const ColorMod& championColor)
{
    assert(vertexBuffer != nullptr);

    float width = (float)image.GetPaddedWidth();
    float height = (float)image.GetPaddedHeight();
    float pixelationAmount = g_ANM2_PixelationAmount;
    Vector clipPaneNormal = g_ANM2_ClipPaneNormal;
    float clipPaneThreshold = g_ANM2_ClipPaneThreshold;

    // for each vertex of the quad
    for (int i = 0; i < 4; i++)
    {
        // vec4 ColorizeIn
        vertexBuffer[COLORIZE_OFFSET + 0] = colorMod._colorize[0];
        vertexBuffer[COLORIZE_OFFSET + 1] = colorMod._colorize[1];
        vertexBuffer[COLORIZE_OFFSET + 2] = colorMod._colorize[2];
        vertexBuffer[COLORIZE_OFFSET + 3] = colorMod._colorize[3];

        // vec3 ColorOffsetIn
        vertexBuffer[COLOR_OFFSET_OFFSET + 0] = colorMod._offset[0];
        vertexBuffer[COLOR_OFFSET_OFFSET + 1] = colorMod._offset[1];
        vertexBuffer[COLOR_OFFSET_OFFSET + 2] = colorMod._offset[2];

        // vec2 TextureSize
        vertexBuffer[TEXTURE_SIZE_OFFSET + 0] = width;
        vertexBuffer[TEXTURE_SIZE_OFFSET + 1] = height;

        // float PixelationAmount
        vertexBuffer[PIXELATION_AMOUNT_OFFSET + 0] = pixelationAmount;

        // vec3 ClipPane
        vertexBuffer[CLIP_PANE_OFFSET + 0] = clipPaneNormal.x;
        vertexBuffer[CLIP_PANE_OFFSET + 1] = clipPaneNormal.y;
        vertexBuffer[CLIP_PANE_OFFSET + 2] = clipPaneThreshold;

        // vec4 ChampionColorIn
        vertexBuffer[CHAMPION_COLOR_OFFSET + 0] = championColor._tint[0];
        vertexBuffer[CHAMPION_COLOR_OFFSET + 1] = championColor._tint[1];
        vertexBuffer[CHAMPION_COLOR_OFFSET + 2] = championColor._tint[2];
        vertexBuffer[CHAMPION_COLOR_OFFSET + 3] = championColor._tint[3];

        vertexBuffer += VERTEX_SIZE;
    }
}