
#pragma once

#include "IsaacRepentance.h"
#include "ImageUtils.h"
#include "ShaderUtils.h"

namespace ImageUtils
{
    /// @internal
    namespace _detail
    {
        constexpr uint32_t NO_PREDEFINED_SHADER = 3;
    }
}

inline ImageUtils::ShaderRenderData::ShaderRenderData(const KAGE_Graphics_Shader& shader)
    : imageVertexDescriptor(std::make_unique<uint8_t[]>(shader._shaderBase._numVertexAttributes)),
    vertexStride(ShaderUtils::GetVertexStride(shader._shaderBase._vertexAttributes, shader._shaderBase._numVertexAttributes))
{
    ShaderUtils::ToImageVertexDescriptor(this->imageVertexDescriptor.get(), shader._shaderBase._vertexAttributes, shader._shaderBase._numVertexAttributes);
}

inline float* ImageUtils::SubmitQuadForShader(KAGE_Graphics_ImageBase &image, KAGE_Graphics_Shader& shader, const ImageUtils::ShaderRenderData& shaderData, const SourceQuad& sourceQuad, const DestinationQuad& destinationQuad, const ImageUtils::QuadColor& color)
{
    KAGE_Graphics_Manager& manager = g_KAGE_Graphics_Manager;

    // supress predefined shader
    uint32_t oldPredefinedShader = image._predefinedShader;
    image._predefinedShader = ImageUtils::_detail::NO_PREDEFINED_SHADER;

    // change vertex format so position, color and texture coordinates are properly set in the vertex buffer
    uint8_t* oldVertexFormat = image._vertexFormat;
    uint8_t oldNumVertexAttributes = image._numVertexAttributes;
    image._vertexFormat = shaderData.imageVertexDescriptor.get();
    image._numVertexAttributes = shader._shaderBase._numVertexAttributes;

    // change the vertexStride so that the vertex buffer is allocated with the proper size, when creating a new RenderBatch.
    uint16_t oldVertexStride = image._vertexFormatStride;
    image._vertexFormatStride = shaderData.vertexStride;

    KAGE_Graphics_Shader* oldShader = manager._shader;
    manager._shader = &shader;

    float* vertexBuffer = image.Render(sourceQuad, destinationQuad, color.topLeft, color.topRight, color.bottomLeft, color.bottomRight);

    // restore changed fields
    manager._shader = oldShader;
    image._predefinedShader = oldPredefinedShader;
    image._vertexFormat = oldVertexFormat;
    image._numVertexAttributes = oldNumVertexAttributes;
    image._vertexFormatStride = oldVertexStride;

    return vertexBuffer;
}