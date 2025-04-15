#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include <cstddef>
#include <algorithm>

/* This bug causes all modded items to have a craftingQuality of 0, if it is not explicitly set it in the XML or the mod doesn't use items_metadata.xml
* After an item node is parsed and the ItemConfig is created, if the craftingQuality is equal to a special value then the craftingQuality is set to match the quality value.
* However unlike what happens in items_metadata.xml the craftingQuality is never set to this special value before parsing the node, as such this check becomes unused.
* All we do is properly set the craftingQuality to this special value right after constructing the ItemConfig::Item object.
*/
static void fix_modded_crafting_quality(const char* signature, const char* location)
{
    SigScan scanner(signature);
    scanner.Scan();
    void* address = scanner.GetAddress();

    ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

    const size_t craftingQualityOffset = offsetof(ItemConfig_Item, craftingQuality);
    const uint32_t specialCraftingQuality = 0x8FFFFFFF;

    ASMPatch patch;
    patch.AddBytes("\xC7\x81") // Mov [ECX + craftingQualityOffset], specialCratingQuality
    .AddBytes(ByteBuffer().AddAny((char*)&craftingQualityOffset, sizeof(craftingQualityOffset))) // add bytes for craftingQualityOffset
    .AddBytes(ByteBuffer().AddAny((char*)&specialCraftingQuality, sizeof(specialCraftingQuality))) // add bytes for specialCraftingQuality
    .AddBytes(ByteBuffer().AddAny((char*)address, 0x7)) // restore
    .AddRelativeJump((char*)address + 0x7); // resume
    sASMPatcher.PatchAt(address, &patch);
}

/* This bug leaves the variant set in an invalid state, alongside returning a duplicate room variant rather than a unique one,
* This only occurs when the gap between the first disjointed set and the next one is exactly 1, and the room's originalVariant is not unique.
* The code should add the new variant to the first set and then merge the two sets together due to there being no gap between them,
* however the check to see if the merge should occur is incorrect (next->lowerBound + 2 == first->upperBound) when it should be (first->upperBound + 2 == next->lowerBound)
*/ 
static void fix_variant_set_add_unique(const char* signature, const char* location)
{
    /* Original assembly:
    * mov eax, dword ptr [edx] (next->lowerBound)
    * inc eax, 2
    * cmp eax, dword ptr [ecx + 0x4] (first->upperBound)
    */

    SigScan scanner(signature);
    scanner.Scan();
    void* address = scanner.GetAddress();

    ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

    ASMPatch patch;
    patch.AddBytes("\x8B\x41\x04") // mov eax, dword ptr [ecx + 0x4] (first->upperBound)
        .AddBytes("\x83\xC0\x02") // inc eax, 2
        .AddBytes("\x3B\x02"); // cmp eax, dword ptr [edx] (next->lowerBound)
    sASMPatcher.FlatPatch(address, &patch);
}

void ASMFixes()
{
    fix_modded_crafting_quality("8b0eba????????85c9c745", "ItemConfig::Load");
    fix_variant_set_add_unique("8b0283c002", "ModManager::UpdateRooms (inline RoomConfig::VariantSet::AddUnique)");
}