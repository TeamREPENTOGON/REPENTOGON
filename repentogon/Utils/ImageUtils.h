#pragma once

#include "IsaacRepentance.h"
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