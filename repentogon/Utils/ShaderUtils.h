#pragma once

#include <IsaacRepentance.h>
#include <glad/glad.h>

namespace ShaderUtils
{
    constexpr bool IsGLtypeCompatible(GLenum glType, uint32_t format);
    constexpr uint32_t GLtypeToFormat(GLenum glType);
    // size is expressed in number of floats
    constexpr size_t GetFormatSize(uint32_t format);
    constexpr size_t GetFormatStride(uint32_t format);
    // This method expects a descriptor with a Terminator format.
    size_t GetNumVertexAttributes(const KAGE_Graphics_VertexAttributeDescriptor* descriptor);
    // size is expressed in number of floats
    uint32_t GetVertexSize(const KAGE_Graphics_VertexAttributeDescriptor* descriptor, size_t numAttributes);
    uint32_t GetVertexStride(const KAGE_Graphics_VertexAttributeDescriptor* descriptor, size_t numAttributes);
    void ToImageVertexDescriptor(uint8_t* result, KAGE_Graphics_VertexAttributeDescriptor* descriptor, size_t numAttributes);
    bool AreVerticesEqual(const KAGE_Graphics_VertexAttributeDescriptor* desc, const KAGE_Graphics_VertexAttributeDescriptor* other, size_t numAttributes);
    bool UsesVertexDescriptor(const KAGE_Graphics_ShaderBase& shader, const KAGE_Graphics_VertexAttributeDescriptor* vertexDescriptor, size_t numAttributes);
}

inline constexpr uint32_t ShaderUtils::GLtypeToFormat(GLenum glType)
{
    switch (glType)
    {
    case GL_FLOAT:
        return (uint32_t)eVertexAttributeFormat::FLOAT;
    case GL_FLOAT_VEC2:
        return (uint32_t)eVertexAttributeFormat::VEC_2;
    case GL_FLOAT_VEC3:
        return (uint32_t)eVertexAttributeFormat::VEC_3;
    case GL_FLOAT_VEC4:
        return (uint32_t)eVertexAttributeFormat::VEC_4;
    default:
        return (uint32_t)eVertexAttributeFormat::TERMINATOR;
    }
}

inline constexpr bool ShaderUtils::IsGLtypeCompatible(GLenum glType, uint32_t format)
{
    switch (format)
    {
    case (uint32_t)eVertexAttributeFormat::FLOAT:
    case 8: // unknown
        return glType == GL_FLOAT;
    case (uint32_t)eVertexAttributeFormat::VEC_2:
    case (uint32_t)eVertexAttributeFormat::TEX_COORD:
        return glType == GL_FLOAT_VEC2;
    case (uint32_t)eVertexAttributeFormat::VEC_3:
    case (uint32_t)eVertexAttributeFormat::POSITION:
        return glType == GL_FLOAT_VEC3;
    case (uint32_t)eVertexAttributeFormat::VEC_4:
    case (uint32_t)eVertexAttributeFormat::COLOR:
        return glType == GL_FLOAT_VEC4;
    default:
        return false;
    }
}

inline constexpr size_t ShaderUtils::GetFormatSize(uint32_t format)
{
    switch (format)
    {
    case (uint32_t)eVertexAttributeFormat::FLOAT:
    case 8: // unknown
        return 1;
    case (uint32_t)eVertexAttributeFormat::VEC_2:
    case (uint32_t)eVertexAttributeFormat::TEX_COORD:
        return 2;
    case (uint32_t)eVertexAttributeFormat::VEC_3:
    case (uint32_t)eVertexAttributeFormat::POSITION:
        return 3;
    case (uint32_t)eVertexAttributeFormat::VEC_4:
    case (uint32_t)eVertexAttributeFormat::COLOR:
        return 4;
    default:
        return 0;
    }
}

inline constexpr size_t ShaderUtils::GetFormatStride(uint32_t format)
{
    return ShaderUtils::GetFormatSize(format) * sizeof(float);
}

namespace ShaderUtils
{
    namespace ColorOffset
    {
        constexpr size_t POSITION_OFFSET = 0;
        constexpr size_t COLOR_OFFSET = POSITION_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::POSITION);
        constexpr size_t TEX_COORD_OFFSET = COLOR_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::COLOR);
        constexpr size_t COLORIZE_OFFSET = TEX_COORD_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::TEX_COORD);
        constexpr size_t COLOR_OFFSET_OFFSET = COLORIZE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_4);
        constexpr size_t TEXTURE_SIZE_OFFSET = COLOR_OFFSET_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_3);
        constexpr size_t PIXELATION_AMOUNT_OFFSET = TEXTURE_SIZE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_2);
        constexpr size_t CLIP_PANE_OFFSET = PIXELATION_AMOUNT_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::FLOAT);
        constexpr size_t VERTEX_SIZE = CLIP_PANE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_3);
        constexpr size_t VERTEX_STRIDE = VERTEX_SIZE * sizeof(float);
        constexpr size_t NUM_ATTRIBUTES = 8;

        void FillVertices(float* vertexBuffer, KAGE_Graphics_ImageBase& image, const ColorMod& colorMod);
    }

    namespace ColorOffsetChampion
    {
        constexpr size_t POSITION_OFFSET = 0;
        constexpr size_t COLOR_OFFSET = POSITION_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::POSITION);
        constexpr size_t TEX_COORD_OFFSET = COLOR_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::COLOR);
        constexpr size_t COLORIZE_OFFSET = TEX_COORD_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::TEX_COORD);
        constexpr size_t COLOR_OFFSET_OFFSET = COLORIZE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_4);
        constexpr size_t TEXTURE_SIZE_OFFSET = COLOR_OFFSET_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_3);
        constexpr size_t PIXELATION_AMOUNT_OFFSET = TEXTURE_SIZE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_2);
        constexpr size_t CLIP_PANE_OFFSET = PIXELATION_AMOUNT_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::FLOAT);
        constexpr size_t CHAMPION_COLOR_OFFSET = CLIP_PANE_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_3);
        constexpr size_t VERTEX_SIZE = CHAMPION_COLOR_OFFSET + ShaderUtils::GetFormatSize((uint32_t)eVertexAttributeFormat::VEC_4);
        constexpr size_t VERTEX_STRIDE = VERTEX_SIZE * sizeof(float);
        constexpr size_t NUM_ATTRIBUTES = 9;

        void FillVertices(float* vertexBuffer, KAGE_Graphics_ImageBase& image, const ColorMod& colorMod, const ColorMod& championColor);
    }
}

inline size_t ShaderUtils::GetNumVertexAttributes(const KAGE_Graphics_VertexAttributeDescriptor *descriptor)
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

inline uint32_t ShaderUtils::GetVertexSize(const KAGE_Graphics_VertexAttributeDescriptor* descriptor, size_t numAttributes)
{
    uint32_t stride = 0;

    for (size_t i = 0; i < numAttributes; i++)
    {
        stride += GetFormatSize(descriptor[i].format);
    }

    return stride;
}

inline uint32_t ShaderUtils::GetVertexStride(const KAGE_Graphics_VertexAttributeDescriptor* descriptor, size_t numAttributes)
{
    return GetVertexSize(descriptor, numAttributes) * sizeof(float);
}

inline void ShaderUtils::ToImageVertexDescriptor(uint8_t *result, KAGE_Graphics_VertexAttributeDescriptor *descriptor, size_t numAttributes)
{
    for (size_t i = 0; i < numAttributes; i++)
    {
        result[i] = (uint8_t)descriptor[i].format;
    }
}

inline bool ShaderUtils::AreVerticesEqual(const KAGE_Graphics_VertexAttributeDescriptor* desc, const KAGE_Graphics_VertexAttributeDescriptor* other, size_t numAttributes)
{
    for (size_t i = 0; i < numAttributes; i++)
    {
        if (desc[i] != other[i])
        {
            return false;
        }
    }

    return true;
}

inline bool ShaderUtils::UsesVertexDescriptor(const KAGE_Graphics_ShaderBase& shader, const KAGE_Graphics_VertexAttributeDescriptor* vertexDescriptor, size_t numAttributes)
{
    if (shader._numVertexAttributes != numAttributes)
    {
        return false;
    }

    return ShaderUtils::AreVerticesEqual(shader._vertexAttributes, vertexDescriptor, numAttributes);
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