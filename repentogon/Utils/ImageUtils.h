#pragma once

#include "IsaacRepentance.h"
#include "ShaderUtils.h"
#include <memory>

namespace ImageUtils
{
    struct QuadColor
    {
        KAGE_Graphics_Color topLeft;
        KAGE_Graphics_Color topRight;
        KAGE_Graphics_Color bottomLeft;
        KAGE_Graphics_Color bottomRight;

        QuadColor() {};
        QuadColor(const KAGE_Graphics_Color& color)
            : topLeft(color), topRight(color), bottomLeft(color), bottomRight(color) {};
        QuadColor(const KAGE_Graphics_Color& topLeft, const KAGE_Graphics_Color& topRight, const KAGE_Graphics_Color& bottomLeft, const KAGE_Graphics_Color& bottomRight)
            : topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight) {};
    };

    struct ShaderRenderData
    {
        const std::unique_ptr<uint8_t[]> imageVertexDescriptor;
        const uint16_t vertexStride = 0;

        ShaderRenderData(const KAGE_Graphics_Shader& shader);
    };

    /// @brief Queues the image for rendering with the specified shader and reserves space in the vertex buffer for one quad.
    /// due to how the engine works, some data is elaborated during queuing, as such the returned vertex buffer will already have all data
    /// regarding POSITION, COLOR and TEXTURE COORDINATES have already been assigned, and should not be reassigned.
    /// @return Vertex buffer (4 vertices long) in which to submit shader specific quad data, the caller is responsible for filling the buffer with data.
    /// Can return nullptr if the engine chooses to cull this specific image render.
    float* SubmitQuadForShader(KAGE_Graphics_ImageBase& image, KAGE_Graphics_Shader& shader, const ShaderRenderData& shaderData, const SourceQuad& sourceQuad, const DestinationQuad& destinationQuad, const QuadColor& color);

    /// @brief This version of the function calculates the extra shader data needed to render.
    /// Since this data remains constant once the shader is loaded, it is generally preferred to pass it directly instead.
    /// Queues the image for rendering with the specified shader and reserves space in the vertex buffer for one quad.
    /// due to how the engine works, some data is elaborated during queuing, as such the returned vertex buffer will already have all data
    /// regarding POSITION, COLOR and TEXTURE COORDINATES have already been assigned, and should not be reassigned.
    /// @return Vertex buffer (4 vertices long) in which to submit shader specific quad data, the caller is responsible for filling the buffer with data.
    /// Can return nullptr if the engine chooses to cull this specific image render.
    inline float* SubmitQuadForShader(KAGE_Graphics_ImageBase& image, KAGE_Graphics_Shader& shader, const SourceQuad& sourceQuad, const DestinationQuad& destinationQuad, const QuadColor& color)
    {
        return ImageUtils::SubmitQuadForShader(image, shader, ShaderRenderData(shader), sourceQuad, destinationQuad, color);
    }
}

namespace ImageUtils
{
    /// @internal
    namespace _detail
    {
        constexpr uint32_t NO_PREDEFINED_SHADER = 3;
    }
}

inline ImageUtils::ShaderRenderData::ShaderRenderData(const KAGE_Graphics_Shader& shader)
    : imageVertexDescriptor(std::make_unique<uint8_t[]>(shader._numVertexAttributes)),
    vertexStride(ShaderUtils::GetVertexStride(shader._vertexAttributes, shader._numVertexAttributes))
{
    ShaderUtils::ToImageVertexDescriptor(this->imageVertexDescriptor.get(), shader._vertexAttributes, shader._numVertexAttributes);
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
    image._numVertexAttributes = shader._numVertexAttributes;

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