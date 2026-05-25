#include "MinimapPatches.h"

#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "Log.h"

/**
 * This patch properly sets the shader to ColorOffset right before the minimap image is rendered.
 * Currently we patch right after the border is rendered, but any place before the image render is valid.
 * 
 * The issue stems from the fact that the question mark render does not set the shader prior to the
 * image render. This ends up not being a problem in vanilla because AnimationLayer::RenderFrame ends up leaving
 * the last used shader as the currently set shader as a side effect.
 * 
 * However due to the changes made to RenderFrame, which aims to make the function side effect free,
 * besides queuing render operations, this causes issues if the shader does not end up being ColorOffset.
 */

static void patch_set_shader_pre_render_image()
{
    ASMPatch patch;

    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Minimap_Render_SetShaderPreImageRender);
    ZHL::Log("[REPENTOGON] Patching Minimap::Render for SetShaderPreImageRender at %p\n", addr);

    intptr_t resumeAddr = addr + 5;
    intptr_t managerShaderAddr = (intptr_t)&(g_KAGE_Graphics_Manager._shader);
    KAGE_Graphics_Shader* colorOffsetPtr = __ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET];
    int32_t restoreCall_rel32 = *(int32_t*)(addr + 1);
    intptr_t restoreCallAddr = addr + 5 + restoreCall_rel32;

    patch.AddInternalCall((void*)restoreCallAddr)
        .AddBytes("\xC7\x05") // MOV [imm32], imm32
        .AddBytes(ByteBuffer().AddAny((void*)&managerShaderAddr, sizeof(managerShaderAddr)))
        .AddBytes(ByteBuffer().AddAny((void*)&colorOffsetPtr, sizeof(colorOffsetPtr)))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void MinimapPatches::ApplyPatches()
{
    patch_set_shader_pre_render_image();
}