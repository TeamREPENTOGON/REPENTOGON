#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include <cstddef>
#include <algorithm>

static inline void* get_sig_address(const char* signature, const char* location)
{
    SigScan scanner(signature);
    if (!scanner.Scan())
    {
        ZHL::Log("[ASSERT] [ASMPatch] Did not find \"%s\" for %s patch\n", signature, location);
        assert(false);
    }
    return scanner.GetAddress();
}

/* This bug causes all modded items to have a craftingQuality of 0, if it is not explicitly set it in the XML or the mod doesn't use items_metadata.xml
* After an item node is parsed and the ItemConfig is created, if the craftingQuality is equal to a special value then the craftingQuality is set to match the quality value.
* However unlike what happens in items_metadata.xml the craftingQuality is never set to this special value before parsing the node, as such this check becomes unused.
* All we do is properly set the craftingQuality to this special value right after constructing the ItemConfig::Item object.
*/
static void fix_modded_crafting_quality(const char* signature, const char* location)
{
    void* address = get_sig_address(signature, location);

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

    void* address = get_sig_address(signature, location);

    ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

    ASMPatch patch;
    patch.AddBytes("\x8B\x41\x04") // mov eax, dword ptr [ecx + 0x4] (first->upperBound)
        .AddBytes("\x83\xC0\x02") // inc eax, 2
        .AddBytes("\x3B\x02"); // cmp eax, dword ptr [edx] (next->lowerBound)
    sASMPatcher.FlatPatch(address, &patch);
}

/* This bug causes the ENTCOLL_PLAYERONLY entity collision class to behave like ENTCOLL_NONE, if set by the entity that initiated collision.
* Unlike what happens with the other collider, the game does not check if the colliding entity is an ENTITY_PLAYER and just skips collision early.
* This bug has no impact on vanilla, since players are always sorted to be the initiating entity, and players do not use the collision class in the first place.
*/
static void fix_handle_collisions_playeronly_entity_class(const char* signature, const char* location)
{
    /* Original assembly:
    * cmp eax, ENTCOLL_PLAYERONLY
    * jz SKIP_COLLISION (if entityCollisionClass == ENTCOLL_PLAYERONLY)
    */

    void* address = get_sig_address(signature, location);

    ZHL::Log("[REPENTOGON] Patching %s at %p\n", location, address);

    ByteBuffer byteBuffer = ByteBuffer();

    size_t typeOffset = offsetof(Entity, _type);
    uint32_t playerType = ENTITY_PLAYER;
    int8_t* rawJumpOffset = (int8_t*)byteBuffer.AddAny((char*)address + 0x4, 1).GetData();
    int jumpOffset = 0x5 + *rawJumpOffset;

    ASMPatch patch;
    patch.AddBytes(ByteBuffer().AddAny((char*)address, 0x3)) // restore cmp eax, ENTCOLL_PLAYERONLY
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)address + 0x5) // resume
        .AddBytes("\x81\xBF") // cmp [edi + _type], ENTITY_PLAYER
            .AddBytes(ByteBuffer().AddAny((char*)&typeOffset, sizeof(uint32_t)))
            .AddBytes(ByteBuffer().AddAny((char*)&playerType, sizeof(uint32_t)))
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)address + jumpOffset) // SKIP_COLLISION
        .AddRelativeJump((char*)address + 0x5); // resume
    sASMPatcher.PatchAt(address, &patch);
}

// nop's out an check in TryRemoveSmeltedTrinket that requires the trinket ID to be a vanilla one, fixing removal of smelted modded trinkets.
// The check is entirely unnecessary anyway because the code also checks if the trinket has a valid ItemConfig entry.
// Note: This is a Rep+ bug on v1.7.9.12 (https://github.com/epfly6/RepentanceAPIIssueTracker/issues/605)
static void fix_try_remove_smelted_trinket()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryRemoveSmeltedTrinketIdCheck);
    ASMPatch patch;
    ByteBuffer buffer;
    buffer.AddByte('\x90', 8);
    patch.AddBytes(buffer);
    sASMPatcher.FlatPatch(addr, &patch, true);
}

void ASMFixes()
{
    fix_modded_crafting_quality("8b0eba????????85c9c745", "ItemConfig::Load");
    fix_variant_set_add_unique("8b0283c002", "ModManager::UpdateRooms (inline RoomConfig::VariantSet::AddUnique)");
    fix_handle_collisions_playeronly_entity_class("83f80174??83f901", "Entity::handle_collisions");
    fix_try_remove_smelted_trinket();
}