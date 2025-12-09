#pragma once

#include <IsaacRepentance.h>

namespace ShaderUtils
{
    // size is expressed in number of floats
    constexpr size_t GetFormatSize(uint32_t format);
    constexpr size_t GetFormatStride(uint32_t format);
    size_t GetNumVertexAttributes(KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor);
    // size is expressed in number of floats
    uint32_t GetVertexSize(KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor, size_t numAttributes);
    uint32_t GetVertexStride(KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor, size_t numAttributes);
    void ToImageVertexDescriptor(uint8_t* result, KAGE_Graphics_ImageBase_VertexAttributeDescriptor* descriptor, size_t numAttributes);
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

        void FillVertices(float* vertexBuffer, KAGE_Graphics_ImageBase& image, const ColorMod& colorMod, const ColorMod& championColor);
    }
}