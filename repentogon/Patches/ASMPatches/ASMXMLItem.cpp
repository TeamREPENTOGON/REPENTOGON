#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include <cstddef>
#include <algorithm>

void ASMFixModdedCraftingQuality()
{
    SigScan scanner("8b0eba????????85c9c745");
    scanner.Scan();
    void* postItemConstructorSig = scanner.GetAddress();

    ZHL::Log("[REPENTOGON] Patching ItemConfig::Load at %p\n", postItemConstructorSig);

    const size_t craftingQualityOffset = offsetof(ItemConfig_Item, craftingQuality);
    const uint32_t specialCraftingQuality = 0x8FFFFFFF; // After parsing an Item node, if the craftingQuality is equal to this value then the quality value is used as the craftingQuality

    ASMPatch patch;
    patch.AddBytes("\xC7\x81") // Mov [ECX + craftingQualityOffset], specialCratingQuality
    .AddBytes(ByteBuffer().AddAny((char*)&craftingQualityOffset, sizeof(craftingQualityOffset)))
    .AddBytes(ByteBuffer().AddAny((char*)&specialCraftingQuality, sizeof(specialCraftingQuality)))
    .AddBytes(ByteBuffer().AddAny((char*)postItemConstructorSig, 0x7))
    .AddRelativeJump((char*)postItemConstructorSig + 0x7);
    sASMPatcher.PatchAt(postItemConstructorSig, &patch);
}

void ASMPatchesForXMLItem()
{
    ASMFixModdedCraftingQuality();
}