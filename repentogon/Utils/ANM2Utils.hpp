#pragma once

#include "ANM2Utils.h"
#include "ShaderUtils.h"

namespace ANM2Utils
{
    /** @internal */
    namespace _detail
    {
        ColorMod get_frame_color(const ANM2& anm2, const LayerState& layer, const AnimationFrame& frame, uint32_t flags);
    }
}

inline ColorMod ANM2Utils::_detail::get_frame_color(const ANM2& anm2, const LayerState& layer, const AnimationFrame& frame, uint32_t flags)
{
    if ((flags & (uint32_t)eAnimationFlag::IGNORE_COLOR_MODIFIERS))
    {
        // does not apply anm2 color
        ColorMod color = layer._color;
        color *= frame.color;
        return color;
    }

    if (g_ColorOverrideStack.size() > 0)
    {
        return g_ColorOverrideStack.back();
    }

    ColorMod color = anm2._color;
    color *= layer._color;
    color *= frame.color;
    return color;
}

inline BlendMode ANM2Utils::GetFrameBlendMode(const LayerState& layer)
{
    return layer._blendMode;
}

inline ColorMod ANM2Utils::GetFrameColor(const ANM2& anm2, const LayerState& layer, const AnimationFrame& frame, uint32_t flags)
{
    ColorMod color = ANM2Utils::_detail::get_frame_color(anm2, layer, frame, flags);
    
    if (g_ANM2_ReflectionRendering)
    {
        color._tint[3] *= 0.5f * g_Game->_room->_waterAmount; // alpha
    }

    return color;
}

inline SourceQuad ANM2Utils::GetFrameSourceQuad(const AnimationFrame& frame, LayerState& layerState, const Vector& topLeftClamp, const Vector& bottomRightClamp, const ANM2& anm2, const KAGE_Graphics_ImageBase* texture)
{
    SourceQuad sourceQuad;
    AnimationLayer::GetSourceQuad(&sourceQuad, nullptr, frame, layerState, topLeftClamp, bottomRightClamp, anm2);

    if (g_ANM2_GlitchRendering)
    {
        Vector pivot = Vector(
            frame.width / 2 + frame.crop.x,
            frame.height / 2 + frame.crop.y
        );

        const KAGE_Graphics_ImageBase* proceduralItemTexture = g_Game->GetProceduralItemManager()->_proceduralItemTexture.image;
        if (texture == proceduralItemTexture)
        {
            pivot.x += layerState._cropOffset.x;
            pivot.y += layerState._cropOffset.y;
        }

        RNG rng = RNG();
        rng.SetSeed((uint32_t)&anm2, 35);

        float randomAngle = rng.RandomInt(2) == 0 ? 90.0f : -90.0f;
        bool shouldFlipX = rng.RandomInt(2) != 0;

        sourceQuad.RotateDegrees(pivot, randomAngle);
        if (shouldFlipX)
        {
            sourceQuad.FlipX();
        }
    }

    return sourceQuad;
}

inline DestinationQuad ANM2Utils::GetFrameDestinationQuad(const AnimationFrame& frame, const Vector& position, LayerState& layerState, const Vector& topLeftClamp, const Vector& bottomRightClamp, const ANM2& anm2)
{
    DestinationQuad destinationQuad;
    AnimationLayer::GetDestQuad(&destinationQuad, position, frame, layerState, topLeftClamp, bottomRightClamp, anm2);
    return destinationQuad;
}