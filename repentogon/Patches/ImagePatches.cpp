#include "ImagePatches.h"

#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "Log.h"

#pragma region Fix incorrect last transparent batch invalidation

/** This patches the part of ImageBase::begin_quad_batch that invalidates the last transparent batch
 *  of the last queued transparent image. This in order to avoid that following renders on that image
 *  will reuse that batch, leading to duplicate batches in the queue, and out of order rendering.
 *
 *  This is done by taking the image of the previous batch, and invalidating (setting to nullptr)
 *  the _lastTransparentRenderBatch field.
 *
 *  However this does not account for the case in which the previous image is the same as
 *  the currently queued one, leading to the current image's last render batch being incorrectly invalidated.
 *
 *  This temporarily breaks batching for this image, as now every render will be treated as it's own separate batch;
 *  even if the previous one matches the current shader and blend mode (this fixes itself when a different image is rendered).
 *
 *  Additionally it causes the creation of unnecessary render batches, which persist until the image is fully unloaded,
 *  leading to a waste of memory.
 *
 *  To fix this we simply add an additional check to see if the previous image is not the current image, before
 *  invalidating the last batch.
*/

static void patch_invalidate_last_transparent_render_batch()
{
    ASMPatch patch;

    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Image_BeginQuadBatch_InvalidateLastTransparentBatch);
    ZHL::Log("[REPENTOGON] Patching ImageBase::begin_quad_batch for InvalidateLastTransparentRenderBatch at %p\n", addr);

    intptr_t invalidateAddr = (addr + 4);
    intptr_t resumeAddr = (addr + 11);

    // ECX holds the previous image
    // ESI holds the current image
    patch.AddBytes("\x85\xc9") // TEST, ECX, ECX
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (void*)resumeAddr)
        .AddBytes("\x3B\xCE") // CMP ECX, ESI
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (void*)resumeAddr)
        .AddBytes(ByteBuffer().AddAny((void*)invalidateAddr, 7))
        .AddRelativeJump((void*)resumeAddr);
    
    sASMPatcher.PatchAt((void*)addr, &patch);
}

#pragma endregion

#pragma region Find Reusable Transparent Batch

/** This patches the part of ImageBase::get_batch that gets a transparent batch from the vector of reusable batches,
 *  in order to avoid creating a new render batch.
 *  In the original code, this process merely involves checking if the vector was not empty, and if so pop_back the vector.
 *
 *  However this means that the code never checks if the Batch's vertex buffer is compatible with the current shader,
 *  leading to heap corruption if that happens to not be the case.
 *
 *  We override this behavior to instead search within the available transparent batches for the first one that has
 *  an element size equal to the vertex stride, and we swap it with the last one, leaving the rest to the original code.
 *  If we don't find one, we take the same code path that would have been taken if the vector was empty 
 *
 *  The swap is safe, as the order of the entries is not important.
 *  It is also safe for us to act as if the vector was empty, as the code path does not make assumption based on this fact.
*/

static bool __stdcall try_find_reusable_transparent_batch(KAGE_Graphics_ImageBase& image)
{
    uint16_t vertexStride = image._vertexFormatStride;
    std::vector<KAGE_Graphics_RenderBatch*>& batches = image._reusableTransparentRenderBatches;

    for (int i = (int)batches.size() - 1; i >= 0; i--)
    {
        KAGE_Graphics_RenderBatch* batch = batches[i];
        if (batch->_vertexBuffer.elementSize == vertexStride)
        {
            std::swap(batches[i], batches.back());
            return true;
        }
    }

    return false;
}

static void patch_try_find_reusable_transparent_batch()
{
    constexpr size_t JE_REL32_LENGTH = 6;
    constexpr size_t REUSABLE_TRANSPARENT_BATCHES_OFFSET = offsetof(KAGE_Graphics_ImageBase, _reusableTransparentRenderBatches);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;

    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Image_GetBatch_TryGetReusableTransparentBatch);
    ZHL::Log("[REPENTOGON] Patching ImageBase::get_batch for TryGetReusableTransparentBatch at %p\n", addr);

    int32_t jmpFailure_rel32 = *(int32_t*)(addr + 2);
    intptr_t jmpFailureTarget = (addr + JE_REL32_LENGTH) + jmpFailure_rel32;
    intptr_t jmpSuccessTarget = addr + JE_REL32_LENGTH;

    patch.PreserveRegisters(savedRegisters)
        .AddBytes("\x83\xEE") // SUB ESI, imm8
        .AddBytes(ByteBuffer().AddByte(REUSABLE_TRANSPARENT_BATCHES_OFFSET, 1))
        .Push(ASMPatch::Registers::ESI) // image
        .AddInternalCall(try_find_reusable_transparent_batch)
        .AddBytes("\x84\xC0") // TEST AL, AL
        .RestoreRegisters(savedRegisters)
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (void*)jmpFailureTarget)
        .AddRelativeJump((void*)jmpSuccessTarget);
    
    sASMPatcher.PatchAt((void*)addr, &patch);
}

#pragma endregion

void ImagePatches::ApplyPatches()
{
    patch_invalidate_last_transparent_render_batch();
    patch_try_find_reusable_transparent_batch();
}