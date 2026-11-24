#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "../ASMPatches.h"
#include "rapidxml.hpp"

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
// Note: This is a Rep+ bug on v1.9.7.12 (https://github.com/epfly6/RepentanceAPIIssueTracker/issues/605)
// Fixed on v1.9.7.13, so this patch can likely be removed if we port to a new version.
static void fix_try_remove_smelted_trinket()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::TryRemoveSmeltedTrinketIdCheck);
    ZHL::Log("[REPENTOGON] Patching TryRemoveSmeltedTrinket TrinketType check @ %p\n", addr);
    ASMPatch patch;
    ByteBuffer buffer;
    buffer.AddByte('\x90', 8);
    patch.AddBytes(buffer);
    sASMPatcher.FlatPatch(addr, &patch, true);
}

// nop's out a PillEffect validity check added in ForceAddPillEffect that mostly served to prevent modded pill effects from being added.
// Note: This is a Rep+ bug on v1.9.7.12 (https://github.com/epfly6/RepentanceAPIIssueTracker/issues/599)
// Fixed on v1.9.7.13, so this patch can likely be removed if we port to a new version.
static void fix_force_add_pill_effect()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::ForceAddPillEffectIdCheck);
    ZHL::Log("[REPENTOGON] Patching ForceAddPillEffect PillEffect check @ %p\n", addr);
    ASMPatch patch;
    ByteBuffer buffer;
    buffer.AddByte('\x90', 9);
    patch.AddBytes(buffer);
    sASMPatcher.FlatPatch(addr, &patch, true);
}

// Fix handling of invalid PillColors in a few places (the game sometimes treats them as a Gold Pill instead of just skipping/ignoring them).
static void fix_render_pocket_item_pill_identified_check()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::RenderPocketItemPillIdentifiedCheck);
    ZHL::Log("[REPENTOGON] Patching RenderPocketItem identified PillColor check @ %p\n", addr);
    ASMPatch patch;
    patch.AddBytes("\x3B\xC1")  // CMP EAX,ECX
        .CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX)
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JA, (char*)addr + 0x13)  // Skip
        .AddRelativeJump((char*)addr + 0x5);
    sASMPatcher.PatchAt(addr, &patch);
}
static void fix_use_pill_identify_pill()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::UsePillIdentifyPill);
    ZHL::Log("[REPENTOGON] Patching UsePill PillColor identification @ %p\n", addr);
    ASMPatch patch;
    patch.AddBytes("\x3B\xC8")  // CMP ECX,EAX
        .CopyRegister(ASMPatch::Registers::EAX, ASMPatch::Registers::ECX)
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JA, (char*)addr + 0xD)  // Skip
        .AddRelativeJump((char*)addr + 0x5);
    sASMPatcher.PatchAt(addr, &patch);
}
static void fix_itempool_getpilleffect()
{
    void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::ItemPoolGetPillEffect);

    ZHL::Log("[REPENTOGON] Patching ItemPool::GetPillEffect @ %p\n", addr);

    ASMPatch patch;
    patch.Push(ASMPatch::Registers::EAX)
        .MoveImmediate(ASMPatch::Registers::EAX, PILLEFFECT_NULL, true)
        .AddBytes("\x83\xFE").AddBytes(ASMPatch::ToHexString((int8_t)NUM_PILLS))  // CMP ESI,NUM_PILLS
        .AddBytes("\x0F\x43\xF0")  // CMOVAE ESI,EAX
        .Pop(ASMPatch::Registers::EAX)
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JAE, (char*)addr + 0x7)  // Jump for invalid PillColor
        .AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Place corresponding PillEffect in ESI
        .AddRelativeJump((char*)addr + 0x7);
    sASMPatcher.PatchAt(addr, &patch);
}

static void __stdcall asm_log_failed_item_pool_read()
{
    KAGE::_LogMessage(2, "Failed to read item pool data from gamestate \n");
}

/**
 * 
 */

/** This patch aims to fix a problem when reading the game state caused by a missing check.
 *  Currently if the method that reads ItemPool data fails, parsing continues as if it succeeded
 *  causing the parser to misinterpret the rest of the data.
 * 
 *  In practice, this problem only seems to occur when disabling mods that registered trinkets.
 */
static void fix_game_state_read_missing_item_pool_fail_check()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GameState_Read_ItemPoolRead);
    ZHL::Log("[REPENTOGON] Patching GameState::read @ %p\n", addr);

    intptr_t return_addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GameState_Read_Return);
    ASMPatch patch;

    intptr_t resumeAddr = addr + 5;
    constexpr size_t RESTORED_BYTES = 5;

    intptr_t callAddr = addr;
    int32_t call_rel32 = *(int32_t*)(callAddr + 1);
    intptr_t calleeAddress = callAddr + 5 + call_rel32;

    patch.AddInternalCall((void*)calleeAddress) // restore call to assign
        .AddBytes("\x84\xC0") // TEST AL, AL
        .AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)resumeAddr)
        .AddInternalCall(asm_log_failed_item_pool_read)
        .AddBytes("\x32\xC0") // XOR AL, AL
        .AddRelativeJump((void*)return_addr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

// https://github.com/epfly6/RepentanceAPIIssueTracker/issues/603
// I honest to god believe that this log doesn't actually mean anything despite how it sounds, but I will keep ONE in the log.
// If it appears at all, it's probably being spammed constantly.
static bool s_LoggedPushRenderTargetStackOverflow = false;
void __stdcall push_render_target_stack_overflow() {
	if (!s_LoggedPushRenderTargetStackOverflow) {
		s_LoggedPushRenderTargetStackOverflow = true;
		KAGE::LogMessage(3, "PushRenderTarget: stack overflow!\n");
	}
}
static void suppress_push_render_target_stack_overflow() {
	intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Misc_PushRenderTargetStackOverflow);
	ZHL::Log("[REPENTOGON] PushRenderTarget: stack overflow! (at %p)\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(push_render_target_stack_overflow)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xC);
	sASMPatcher.PatchAt((void*)addr, &patch);
}

// Fixes a couple of spots in MoveRandomlyAxisAligned that do not check if the pathfinding entity is a Familiar instead of an EntityNPC (so the result of the dynamic cast was nullptr).
// This function does actually check for nullptr, but not enough apparantly.
static void fix_familiar_pathfinder_move_randomly_axis_aligned(char* patchSig, char* jumpSig, char* testAsm, int patchSize) {
	void* addr = (void*)sASMDefinitionHolder->GetDefinition(patchSig);
	void* jumpAddr = (void*)sASMDefinitionHolder->GetDefinition(jumpSig);

	ASMPatch patch;
	patch.AddBytes(testAsm)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, jumpAddr)
		.AddBytes(ByteBuffer().AddAny((char*)addr, patchSize))
		.AddRelativeJump((char*)addr + patchSize);
	sASMPatcher.PatchAt(addr, &patch);
}

// A malformed inputconfigs.dat crashes the game on v1.9.7.12 but seemingly not on later version (defaults are used instead).
// While unlikely to happen without manually editing the file, avoid the crash for parity.
void __stdcall parse_input_config_xml(xml_document<char>* doc, char* text) {
	try {
		doc->parse<0>(text);
	} catch (const rapidxml::parse_error& e) {
		ZHL::Log("[ERROR] Failed to parse inputconfigs.dat: %s\n", e.what());
		doc->clear();
	}
}
static void fix_load_button_maps_crash() {
	void* addr = (void*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::ButtonMapManager_LoadButtonMaps_RapidXmlParse);

	ZHL::Log("[REPENTOGON] Patching ButtonMapManager::LoadButtonMaps crash on invalid XML @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(parse_input_config_xml)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt((void*)addr, &patch);
}

constexpr int OPENAL_ATTRIBUTES[] = {
	0x19AC, // ALC_OUTPUT_MODE_SOFT
	0x19AE, // ALC_STEREO_BASIC_SOFT
	NULL
};

static void fix_openal_attributes() {
	void* patchAddr = (void*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::KAGE_Sound_Manager_Initialize_OpenAL32_pre_alcCreateContext);
	void* jumpAddr = (void*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::KAGE_Sound_Manager_Initialize_OpenAL32_call_alcCreateContext);

	ZHL::Log("[REPENTOGON] Patching KAGE::Sound::Manager::Initialize to set OPENAL32 attributes (for alcCreateContext) @ %p\n", patchAddr);

	const int* ptr = OPENAL_ATTRIBUTES;

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)patchAddr, 0x5))  // MOV dword ptr[EDI+0x34],EAX; TEST EAX,EAX
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)patchAddr + 0x7)
		.AddBytes("\x68").AddBytes(ByteBuffer().AddAny((char*)&ptr, 4))
		.Push(ASMPatch::Registers::EAX)
		.AddRelativeJump(jumpAddr);
	sASMPatcher.PatchAt((void*)patchAddr, &patch);
}

static int __fastcall asm_door_update_get_secret_exit_type(GridEntity_Door* door)
{
	if ((int)g_Game->_currentRoomIdx == ROOM_SECRET_EXIT_IDX)
	{
		return 0;
	}

	return door->GetSecretExitType();
}

// Rep+ reopens the Mausoleum Mom's Heart door on every frame once it's unlocked.
// This also applies to the other side of the door inside the Mom's Heart room, so that door never stays closed during the fight.
// To work around this, make the check see no secret exit door while inside the secret exit room.
static void fix_mausoleum_heart_door_forced_open()
{
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GridEntityDoor_Update_GetSecretExitType);
	ZHL::Log("[REPENTOGON] Patching GridEntity_Door::Update Mausoleum Mom's Heart door bug %p\n", addr);

	ASMPatch patch;
	patch.AddInternalCall(asm_door_update_get_secret_exit_type)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_evaluate_items_is_own_lil_delirium(Entity_Familiar* familiar, Entity_Player* player)
{
	if (!familiar->_isLilDelirium)
	{
		return false;
	}

	return familiar->_player == player || (familiar->_originalPlayer != nullptr && familiar->_originalPlayer == player);
}

// Fixes a vanilla bug where Lil Delirium disappears if owned by a second player.
// Lil Delirium's EvaluateItems check doesn't actually take player owner into account,
// so the amount of Lil Deliriums in the room is only ever equal to the amount the player with the least Lil Deliriums has.
static void fix_evaluate_items_lil_delirium_owner()
{
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_EvaluateItems_LilDeliriumCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::EvaluateItems Lil Delirium owner check at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8B\xCE") // mov ecx,esi // familiar
		.AddBytes("\x8B\x95\xA8\xFB\xFF\xFF") // mov edx, dword ptr [ebp - 0x458] // player
		.AddInternalCall(asm_evaluate_items_is_own_lil_delirium)
		.AddRelativeJump((char*)addr + 0x7); // test al, al
	sASMPatcher.PatchAt(addr, &patch);
}

static int __fastcall asm_bag_of_crafting_queue_item_pool(ItemConfig_Item* item)
{
	return g_Game->_itemPool.GetFirstItemPoolForCollectible(item->id);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where rerolling Tainted Cain's crafted items turns them into Breakfast.
// 1.9.7.12 passes -1 as the crafted item's pool, but 1.9.7.13 calls GetFirstItemPoolForCollectible on it instead, so D4 and friends work properly.
static void fix_bag_of_crafting_queue_item_pool()
{
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_ControlActiveItem_BagOfCraftingQueueItemPool);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::ControlActiveItem Bag of Crafting QueueItem pool at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8B\xCE") // mov ecx, esi
		.AddInternalCall(asm_bag_of_crafting_queue_item_pool)
		.Push(ASMPatch::Registers::EAX)
        .AddBytes("\x6A").AddZeroes(1).AddBytes("\x6A").AddZeroes(1)
		.AddRelativeJump((char*)addr + 0x6); // push dword ptr [esi + 0xc8]
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Rotgut never drops its thematic item (Yuck Heart).
// The normal code doesn't work because Rotgut is a multi room boss. 1.9.7.13 instead checks for the ROTGUT_CLEARED flag.
static void fix_rotgut_thematic_item()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_GetBossThematicItem_BossIdSwitch);
	ZHL::Log("[REPENTOGON] Patching Room::GetBossThematicItem Rotgut thematic item at %p\n", addr);

	void* defaultCase = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_GetBossThematicItem_BossIdSwitch_JumpTarget);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr, 0x3)) // mov eax, dword ptr [ebp - 0x30] // room
		.AddBytes("\x8B\x50\x04") // mov edx, dword ptr [eax + 0x4] // room->_descriptor
		.AddBytes(ByteBuffer().AddAny(addr + 0x3, 0x6)) // mov eax, dword ptr [eax + 0x1bb0] // room->_bossId
		.AddBytes("\xF7\x42\x44").AddBytes(ByteBuffer().AddInteger<int32_t>(0x10000, false)) // test dword ptr [edx + 0x44], 0x10000 // ROTGUT_CLEARED
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addr + 0x9) // dec eax
		.AddBytes("\xC7\x06").AddBytes(ByteBuffer().AddInteger<int32_t>(COLLECTIBLE_YUCK_HEART, false)) // mov dword ptr [esi], COLLECTIBLE_YUCK_HEART
		.AddRelativeJump(defaultCase);
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_remove_hourglass_blood_puppy(Entity* entity)
{
	if (g_Game->_isRestoringGlowingHourglassState && entity->_type == ENTITY_BLOOD_PUPPY) {
		entity->Remove();
		return true;
	}

	return false;
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Glowing Hourglass could duplicate Blood Puppy.
// A fully grown Blood Puppy is persistent, so it survives the rewind. The restored familiar then gets respawned as well.
// 1.9.7.13 culls the persistent Blood Puppy, fixing the bug.
static void fix_glowing_hourglass_blood_puppy()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_RepositionFriendlyNPCs_Loop);
	ZHL::Log("[REPENTOGON] Patching Glowing Hourglass Blood Puppy removal at %p\n", addr);

	void* nextEntity = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_RepositionFriendlyNPCs_Loop_NextEntity);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr, 0x9)) // mov eax, dword ptr [ebx + 0x124c]; mov ecx, dword ptr [eax + edi * 4] // entity
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(asm_remove_hourglass_blood_puppy)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, nextEntity) // inc edi
		.AddRelativeJump(addr + 0x9); // mov eax, dword ptr [ecx + 0x168]
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Fissure spawners don't persist when leaving an uncleared room.
// 1.9.7.13 adds 1000.192 to the effects that Room::ShouldSaveEntity always saves.
static void fix_fissure_spawner_not_saved()
{
	constexpr int32_t EFFECT_FISSURE_SPAWNER = 192;

	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_ShouldSaveEntity_EffectVariantCheck);
	ZHL::Log("[REPENTOGON] Patching Room::ShouldSaveEntity Fissure spawner effect at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x81\xFA").AddBytes(ByteBuffer().AddInteger<int32_t>(EFFECT_FISSURE_SPAWNER, false)) // cmp edx, EFFECT_FISSURE_SPAWNER
		.AddBytes("\x75\x04") // jne +4
		.AddBytes("\xB0\x01\x5D\xC3") // mov al, 1; pop ebp; ret
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // cmp edx, 0x8d
		.AddRelativeJump(addr + 0x6); // jne
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_filigree_feather_needs_key_piece(bool needsKeyPiece, CollectibleType otherKeyPiece)
{
	if (!needsKeyPiece) {
		return false;
	}

	PlayerManager* playerManager = g_Game->GetPlayerManager();
	return !playerManager->AnyoneHasTrinket(TRINKET_FILIGREE_FEATHERS) || playerManager->FirstCollectibleOwner(otherKeyPiece, nullptr, true) != nullptr;
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Filigree Feather would prioritize Key Piece drops.
// In 1.9.7.12, Uriel/Gabriel drop their missing Key Piece before checking for Filigree Feather.
// 1.9.7.13 reverts this, going back to Repentance behavior.
static void fix_filigree_feather_key_piece_priority()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_TriggerBossDeath_AngelKeyPieceCheck);
	char* addr = sig + 0x11;
	ZHL::Log("[REPENTOGON] Patching Room::TriggerBossDeath Filigree Feather Key Piece check at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EAX)
		.AddBytes("\x0F\xB6\xCA") // movzx ecx, dl
		.AddBytes("\xBA").AddBytes(ByteBuffer().AddInteger<int32_t>(COLLECTIBLE_KEY_PIECE_2, false)) // mov edx, COLLECTIBLE_KEY_PIECE_2
		.AddInternalCall(asm_filigree_feather_needs_key_piece)
		.AddBytes("\x8A\xD0") // mov dl, al
		.AddBytes(ByteBuffer().AddAny(sig, 0x3)) // mov byte ptr [ebp - 0x1d], dl
		.Pop(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x0F\xB6\xC9") // movzx ecx, cl
		.AddBytes("\xBA").AddBytes(ByteBuffer().AddInteger<int32_t>(COLLECTIBLE_KEY_PIECE_1, false)) // mov edx, COLLECTIBLE_KEY_PIECE_1
		.AddInternalCall(asm_filigree_feather_needs_key_piece)
		.Pop(ASMPatch::Registers::EDX)
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // mov ecx, dword ptr [esi + 0x28]; mov byte ptr [ebp - 0x1e], al
		.AddRelativeJump(addr + 0x6); // cmp ecx, 0x10f
	sASMPatcher.PatchAt(addr, &patch);
}

static int __fastcall asm_death_certificate_item_pool(int collectible)
{
	return g_Game->_itemPool.GetFirstItemPoolForCollectible(collectible);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where using Genesis after taking an item from the Death Certificate area can crash the game.
// The Death Certificate area's collectible spawns never set their source pool, leaving it uninitialized.
// Best case, Genesis doesn't spawn a choice of replacement items and breaks. Worst case, the game crashes.
static void fix_death_certificate_item_pool()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_InitDeathCertificateArea_CollectibleSpawn);
	ZHL::Log("[REPENTOGON] Patching Death Certificate area collectible pool at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x8B\x8D\x78\xFF\xFF\xFF") // mov ecx, dword ptr [ebp - 0x88]
		.AddInternalCall(asm_death_certificate_item_pool)
		.AddBytes("\x89\x45\xC0") // mov dword ptr [ebp - 0x40], eax
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // mov dword ptr [ebp - 0x5c], esi; mov eax, dword ptr [edx + ecx - 0x2c]
		.AddRelativeJump(addr + 0x7); // lea edi, [edx + ecx]
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where using Glowing Hourglass in the starting room of Downpour/Dross I would teleport the player to Home.
// The Home easter egg didn't check for STAGETYPE_REPENTANCE(_B), whoops!
static void fix_glowing_hourglass_alt_path_home()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_StartRoomTransition_GlowingHourglassHomeCheck);
	char* addr = sig + 0x5;
	char* skipHome = addr + *(int8_t*)(sig + 0x4);
	ZHL::Log("[REPENTOGON] Patching Game::StartRoomTransition Glowing Hourglass alt path check at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8B\x42\x04") // mov eax, dword ptr [edx + 0x4] // level->_stageType
		.AddBytes("\x83\xF8\x04") // cmp eax, STAGETYPE_REPENTANCE
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipHome)
		.AddBytes("\x83\xF8\x05") // cmp eax, STAGETYPE_REPENTANCE_B
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipHome)
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // mov eax, dword ptr [edx + 0x18304] // level->_roomIdx
		.AddRelativeJump(addr + 0x6); // cmp eax, dword ptr [edx + 0x182d0] // level->_startingRoomIdx
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where using Glowing Hourglass while Tainted Lilith's Gello was out left her unable to shoot.
// Glowing Hourglass removes familiars from their entity list without clearing their references, leaving the whip pointed at a detached Gello.
// 1.9.7.13 instead calls ClearReferences() on each entity before detaching it.
static void fix_glowing_hourglass_detached_entity_references()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_RestoreGlowingHourglassState_DetachPersistentEntity);
	ZHL::Log("[REPENTOGON] Patching Glowing Hourglass restore entity ClearReferences at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EAX)
        .Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x8B\x03") // mov eax, dword ptr [ebx] // entity->vtable
		.AddBytes("\x8B\xCB") // mov ecx, ebx
		.AddBytes("\xFF\x50\x44") // call dword ptr [eax + 0x44] // entity->ClearReferences()
        .Pop(ASMPatch::Registers::EDX)
        .Pop(ASMPatch::Registers::ECX)
        .Pop(ASMPatch::Registers::EAX)
		.AddBytes(ByteBuffer().AddAny(addr, 0xC)) // mov esi, dword ptr [edx + 0x18300]; add esi, 0x1218
		.AddRelativeJump(addr + 0xC);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where the items Soul of Jacob and Esau grants to the spawned Esau were removed from future item pools.
// The GetCollectible call that rolls them passed flags 6 instead of 7 (teehee), missing the "don't remove from pool" bit.
static void fix_soul_of_jacob_esau_item_pool_decrease()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseCard_SoulOfJacobEsauItemFlags) + 0x2;
	ZHL::Log("[REPENTOGON] Patching Soul of Jacob and Esau GetCollectible flags at %p\n", addr);

	sASMPatcher.FlatPatch(addr, "\x07", 1); // push 6 -> push 7
}

static void __stdcall asm_epiphora_reset_fire_weapons(Entity_Player* player, Vector* direction, bool isShooting, bool canFire)
{
	if ((player->_flags & FLAG_INTERPOLATION_UPDATE) == 0 || !canFire)
		return;

	if (!player->IsExtraAnimationFinished())
		return;
	if (g_Game->_stageTransition.IsActive())
		return;

	player->_flags &= ~FLAG_INTERPOLATION_UPDATE;
	player->FireWeapons(direction, isShooting);
	player->_flags |= FLAG_INTERPOLATION_UPDATE;
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where charged weapons could fail to fire with Epiphora.
// Epiphora runs on every player update, but charged weapons only fire on non-interpolated updates.
// When the fire input is released on an interpolated update, the release doesn't happen until the next real frame,
// but Epiphora resets immediately and runs EvaluateItems. The next update then sees the higher MaxFireDelay,
// so the charge doesn't count as full and the shot is dropped.
// 1.9.7.13 instead fires the weapons right after the Epiphora reset.
static void fix_epiphora_charged_weapon_release()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_ControlFire_EpiphoraReset);
	char* addr = sig + 0xF;
	ZHL::Log("[REPENTOGON] Patching Epiphora reset charged weapon release at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x0F\xB6\x85\xBA\xFE\xFF\xFF") // movzx eax, byte ptr [ebp - 0x146] // canFire
		.Push(ASMPatch::Registers::EAX)
		.AddBytes("\x0F\xB6\x85\xA0\xFE\xFF\xFF") // movzx eax, byte ptr [ebp - 0x160] // isShooting
		.Push(ASMPatch::Registers::EAX)
		.AddBytes("\x8D\x85\xA4\xFE\xFF\xFF") // lea eax, [ebp - 0x15c] // direction
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::EDI)
		.AddInternalCall(asm_epiphora_reset_fire_weapons)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.Pop(ASMPatch::Registers::EAX)
		.AddBytes(ByteBuffer().AddAny(addr, 0x10)) // movss xmm5, dword ptr [ebp - 0x158]; movss xmm6, dword ptr [ebp - 0x15c]
		.AddRelativeJump(addr + 0x10);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where spectral tears made Bob's Rotten Head bounce off walls instead of bursting.
// Tear::Update only runs its wall-hit handling (which kills the tear) when the tear isn't spectral.
// 1.9.7.13 instead runs it for Bob's Rotten Head indiscriminately.
static void fix_spectral_bobs_head_wall_bounce()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityTear_Update_WallHitSpectralCheck);
	char* resume = addr + 0x8;
	char* skipWallHit = resume + *(int32_t*)(addr + 0x4);
	ZHL::Log("[REPENTOGON] Patching Entity_Tear::Update spectral Bob's Rotten Head wall check at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x84\xC0") // test al, al // (tearFlags & TEAR_SPECTRAL) is zero
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, resume)
		.AddBytes("\x83\x7F\x2C\x04") // cmp dword ptr [edi + 0x2c], TEAR_BOBS_HEAD
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, skipWallHit)
		.AddRelativeJump(resume);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Star of Bethlehem would move much faster while the player was in an off-grid room.
// The speed algorithm fails with a negative room index, so it falls through to 8.0.
// 1.9.7.13 instead uses the normal speed when the current room index is negative.
static void fix_star_of_bethlehem_off_grid_speed()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_StarOfBethlehemUpdateSpeed_RoomIdx);
	char* addr = sig + 0x21;
	char* normalSpeed = sig + 0x6 + *(int32_t*)(sig + 0x2);
	ZHL::Log("[REPENTOGON] Patching Star of Bethlehem off-grid room speed at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // mov edi, dword ptr [edi + 0x18304] // level->_roomIdx
		.AddBytes("\x85\xFF") // test edi, edi
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JS, normalSpeed)
		.AddBytes(ByteBuffer().AddAny(addr + 0x6, 0x8)) // movss dword ptr [ebp - 0x190], xmm0
		.AddRelativeJump(addr + 0xE);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Belly Jelly and Cone Head could block Dull Razor's fake damage.
// Both blocks ignore DAMAGE_COUNTDOWN; 1.9.7.13 also ignores DAMAGE_FAKE.
static void fix_dull_razor_damage_block()
{
	char* bellyJelly = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_TakeDamage_BellyJellyFlagCheck) + 0x1F;
	char* coneHead = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_TakeDamage_ConeHeadFlagCheck) + 0x1B;
	ZHL::Log("[REPENTOGON] Patching Belly Jelly / Cone Head fake damage checks at %p, %p\n", bellyJelly, coneHead);

	// and eax, 0x2000000 -> and eax, 0x2200000 (DAMAGE_COUNTDOWN | DAMAGE_FAKE)
	sASMPatcher.FlatPatch(bellyJelly, "\x20", 1);
	sASMPatcher.FlatPatch(coneHead, "\x20", 1);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Black Maw was not charging to the right correctly in large rooms.
// 1.9.7.12 hardcoded a turnaround at x = 800, which only matches in a 1-wide room.
// 1.9.7.13 instead uses the room's bottom right x + 200.
static const float s_blackMawChargeRightMargin = 200.0f;

static void fix_black_maw_charge_right_bound()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_AiBlackMaw_ChargeRightBound);
	char* resume = addr + 0x9;
	char* turnAround = resume + *(int8_t*)(addr + 0x8);
	ZHL::Log("[REPENTOGON] Patching Black Maw charge right bound at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\xEC\x04") // sub esp, 4
		.AddBytes("\xF3\x0F\x11\x1C\x24") // movss dword ptr [esp], xmm3
		.Push(ASMPatch::Registers::EAX)
		.AddBytes("\xA1").AddBytes(ByteBuffer().AddPointer(&g_Game)) // mov eax, dword ptr [g_Game]
		.AddBytes("\x8B\x80").AddZeroes(1).AddBytes("\x83\x01\x00").AddZeroes(1) // mov eax, dword ptr [eax + 0x18300] // game->_room
		.AddBytes("\xF3\x0F\x10\x58\x1C") // movss xmm3, dword ptr [eax + 0x1c] // room->_bottomRightPos.x
		.AddBytes("\xF3\x0F\x58\x1D").AddBytes(ByteBuffer().AddPointer((void*)&s_blackMawChargeRightMargin)) // addss xmm3, dword ptr [s_blackMawChargeRightMargin]
		.AddBytes("\x0F\x2F\xC3") // comiss xmm0, xmm3 // npc->_pos.x
		.Pop(ASMPatch::Registers::EAX)
		.AddBytes("\xF3\x0F\x10\x1C\x24") // movss xmm3, dword ptr [esp]
		.AddBytes("\x8D\x64\x24\x04") // lea esp, [esp + 4]
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JA, turnAround)
		.AddRelativeJump(resume);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where a ??? pill could prevent the chest from appearing after defeating Satan in Ultra Hard.
// The ??? pill runs AddCurse which reapplies *every* curse Ultra Hard normally has, including Curse of the Labyrinth.
// SpawnClearAward treats the boss room as one stage deeper when Curse of the Labyrinth is active (or in challenge 44),
// so Sheol (10) took the stage 11 path. 1.9.7.13 only applies the Labyrinth bump below stage 8.
static void fix_satan_chest_labyrinth_stage()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_SpawnClearAward_LabyrinthStageCheck);
	char* incStage = addr + 0xD;
	char* skip = addr + 0x11;
	ZHL::Log("[REPENTOGON] Patching SpawnClearAward Labyrinth stage check at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr + 0x4, 0x7)) // cmp dword ptr [ecx + 0x25afc], 0x2c
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, incStage)
		.AddBytes("\xA8\x02") // test al, 2
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skip)
		.AddBytes("\x83\xFE\x08") // cmp esi, 8
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JL, incStage)
		.AddRelativeJump(skip);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where killing Greed could remove Coupon and Head of the Keeper from future item pools.
// When killing Greed or Super Greed, there was supposed to be a 1/100 or 1/50 (respectively) chance of dropping Coupon or Head of the Keeper.
// But first, when the roll succeeds, ItemPool::RemoveCollectible is called on the item, 
// then CanSpawnCollectible fails because the item isn't in a pool anymore. So it never worked and just removed the items from the pools.
// Nicalis opted to just kill the roll altogether in 1.9.7.13, so we're doing that too.
static void fix_greed_keeper_item_pool_removal()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_TriggerBossDeath_GreedKeeperItemRoll);
	char* spawnCheck = addr + 0x59 + *(int32_t*)(addr + 0x55);
	ZHL::Log("[REPENTOGON] Patching TriggerBossDeath Greed Head of the Keeper / Coupon roll at %p\n", addr);

	ASMPatch patch;
	patch.AddRelativeJump(spawnCheck);
	sASMPatcher.PatchAt(addr, &patch);
}

static void __stdcall asm_dogma_kill_room_bombs()
{
	EntityList_EL* list = g_Game->GetCurrentRoom()->GetEntityList()->GetUpdateEL();
	for (unsigned int i = 0; i < list->_size; i++)
	{
		Entity* entity = list->_data[i];
		if (*entity->GetType() == ENTITY_BOMB)
		{
			EntityRef source;
			entity->Kill(&source);
		}
	}
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where bombs could be used to skip the first phase of Dogma.
// Bombs placed before the living room TV spawns Dogma were still around when the fight started.
// 1.9.7.13 kills every bomb in the room right after the TV spawns Dogma.
// TBH this one seems more theoretical than anything to me, it's a fix, it's documented, it's going in.
static void fix_dogma_room_bombs()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_AiGenericProp_DogmaSpawn);
	char* addr = sig + 0xD;
	ZHL::Log("[REPENTOGON] Patching generic prop Dogma spawn bomb removal at %p\n", addr);

	ASMPatch patch;
	patch.AddInternalCall(asm_dogma_kill_room_bombs)
		.AddBytes(ByteBuffer().AddAny(addr, 0x5)) // mov ecx, 0x724
		.AddRelativeJump(addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where the Little Chubby, Big Chubby and Lil Haunt sound effects could spam.
// Nicalis came to their senses and realized the sounds suck, so they have a 15 second delay now!
static void patch_familiar_sound_delay(char* addr)
{
	ASMPatch patch;
	patch.AddBytes("\x6A").AddZeroes(1) // push 0 // loop
		.AddBytes("\x68\xC2\x01").AddZeroes(2) // push 450 // frameDelay
        .Push(ASMPatch::Registers::ECX) // volume
		.AddRelativeJump(addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

static void fix_familiar_sound_delays()
{
	char* lilHaunt = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_LilHaunt_SoundPlay) + 0x29;
	char* bigChubby = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_BigChubby_SoundPlay) + 0x27;
	char* littleChubby = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_LittleChubby_SoundPlay) + 0x27;
	ZHL::Log("[REPENTOGON] Patching Lil Haunt / Big Chubby / Little Chubby sound delays at %p, %p, %p\n", lilHaunt, bigChubby, littleChubby);

	patch_familiar_sound_delay(lilHaunt);
	patch_familiar_sound_delay(bigChubby);
	patch_familiar_sound_delay(littleChubby);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where consumable popups were miniaturized while holding Found Soul.
// The HUD picked the small per-player co-op pop-up whenever more than one human player existed, and Found Soul counts as one.
// 1.9.7.13 instead changes this so only online play triggers the mini popups.
static unsigned int __stdcall asm_item_text_coop_player_count()
{
	void** peers = (void**)g_Manager->GetNetplayManager();
	return peers[0] != peers[1] ? 2 : 0;
}

static void patch_item_text_coop_player_count(char* addr)
{
	ASMPatch patch;
	patch.AddInternalCall(asm_item_text_coop_player_count)
		.AddRelativeJump(addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

static void fix_found_soul_item_text_popup()
{
	char* showItemText = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HUD_ShowItemText_CoopPlayerCount);
	char* showItemTextCustom = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HUD_ShowItemTextCustomUTF8_CoopPlayerCount);
	ZHL::Log("[REPENTOGON] Patching HUD item text co-op popup checks at %p, %p\n", showItemText, showItemTextCustom);

	patch_item_text_coop_player_count(showItemText);
	patch_item_text_coop_player_count(showItemTextCustom);
}

static bool __fastcall asm_suplex_has_mega_mush(Entity_Player* player)
{
	return player->_temporaryeffects.HasCollectibleEffect(COLLECTIBLE_MEGA_MUSH);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Suplex! could be used during Mega Mush.
// 1.9.7.13 adds a Mega Mush check to the Suplex! use case, failing the same way as when the player is standing still.
static void fix_suplex_mega_mush()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_SuplexCanUse);
	char* addr = sig + 0x33;
	char* fail = addr + *(int32_t*)(sig + 0x2F);
	ZHL::Log("[REPENTOGON] Patching Suplex! Mega Mush check at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x8B\xCE") // mov ecx, esi // player
		.AddInternalCall(asm_suplex_has_mega_mush)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, fail)
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // lea eax, [ebp - 0x934]
		.AddRelativeJump(addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_suplex_damage_fireplace(Entity_Player* player, Entity* collider)
{
	if (*collider->GetType() != ENTITY_FIREPLACE)
		return false;

	EntityRef source(player);
	collider->TakeDamage(1000.0f, 0, &source, 30);
	return true;
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Suplex! could grab fireplaces.
// 1.9.7.13 instead deals 1000 damage to a fireplace the dash runs into and carries on as if nothing was grabbed.
static void fix_suplex_fireplace_grab()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_HandleCollision_SuplexGrab);
	char* addr = sig + 0xB;
	char* noGrab = addr + *(int32_t*)(sig + 0x7);
	char* tryHoldEntity = addr + 0x8 + *(int32_t*)(addr + 0x4);
	ZHL::Log("[REPENTOGON] Patching Suplex! fireplace grab at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8B\xCE") // mov ecx, esi // player
		.AddBytes("\x8B\xD7") // mov edx, edi // collider
		.AddInternalCall(asm_suplex_damage_fireplace)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, noGrab)
		.AddBytes(ByteBuffer().AddAny(addr, 0x3)) // push edi; mov ecx, esi
		.AddInternalCall(tryHoldEntity) // player->TryHoldEntity(collider)
		.AddRelativeJump(addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_camo_undies_is_mouse_shooting(Entity_Player* player)
{
	if (!g_Manager->GetOptions()->_enableMouseControl)
		return false;

	Vector direction(0.0f, 0.0f);
	return player->GetMouseShootingDirection(&direction, false);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Camo Undies would not uncloak when shooting with the mouse.
static void fix_camo_undies_mouse_shooting()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_Update_CamoUndiesShootCheck);
	char* addr = sig + 0x9;
	char* removeEffect = sig + 0xF;
	char* keepEffect = removeEffect + *(int32_t*)(sig + 0xB);
	ZHL::Log("[REPENTOGON] Patching Camo Undies mouse shooting check at %p\n", addr);

	ASMPatch patch;
	patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, removeEffect)
		.AddBytes("\x8B\xCF") // mov ecx, edi // player
		.AddInternalCall(asm_camo_undies_is_mouse_shooting)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, removeEffect)
		.AddRelativeJump(keepEffect);
	sASMPatcher.PatchAt(addr, &patch);
}

static void __fastcall asm_eye_of_the_occult_mouse_direction(Entity_Player* player, Vector* direction)
{
	player->GetMouseShootingDirection(direction, false);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Eye of the Occult not working with mouse controls.
static void fix_eye_of_the_occult_mouse_direction()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityTear_Update_EyeOfTheOccultAimDirection);
	char* addr = sig + 0x13;
	char* vectorAssign = sig + 0x18 + *(int32_t*)(sig + 0x14);
	ZHL::Log("[REPENTOGON] Patching Eye of the Occult mouse direction at %p\n", addr);

	ASMPatch patch;
	patch.AddInternalCall(vectorAssign) // direction = player->GetShootingInput()
		.AddBytes(ByteBuffer().AddAny(sig + 0xD, 0x6)) // lea ecx, [ebp - direction]
		.AddBytes("\x8B\xD1") // mov edx, ecx
		.AddBytes(ByteBuffer().AddAny(sig + 0x18, 0x6)) // mov ecx, [ebp - player]
		.AddInternalCall(asm_eye_of_the_occult_mouse_direction)
		.AddRelativeJump(sig + 0x18);
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __fastcall asm_dark_esau_collision_can_damage_player(Entity_Player* player, Entity* darkEsau)
{
	Entity* playerDarkEsau = player->_darkEsau.GetReference();
	return !playerDarkEsau || playerDarkEsau == darkEsau || playerDarkEsau == darkEsau->GetParent()
		|| playerDarkEsau == darkEsau->GetChild() || darkEsau->GetSpawnerEntity() == player;
}

static bool __fastcall asm_dark_esau_source_can_damage_player(Entity_Player* player, Entity* source)
{
	if (!source || *source->GetType() != ENTITY_DARK_ESAU)
		return true;

	Entity* playerDarkEsau = player->_darkEsau.GetReference();
	if (source == playerDarkEsau)
		return true;
	if (playerDarkEsau && (source->GetParent() == playerDarkEsau || source->GetChild() == playerDarkEsau))
		return true;

	Entity* spawner = source->GetSpawnerEntity();
	return spawner == player || spawner == playerDarkEsau;
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Tainted Jacob could not be damaged by the extra Dark Esau from Birthright.
// Dark Esau's contact damage only applied if it was exactly the player's own Dark Esau, which the birthright Dark Esau doesn't count as.
// 1.9.7.13 replaces that with two checks, and contact damage must pass both:
// * Collision: a Dark Esau can hurt the player if: 
//   - the player has no Dark Esau
//   - it is the player's own Dark Esau or that Dark Esau's parent/child
//   - the player spawned it
// * TakeDamage: damage sourced from a Dark Esau is rejected unless:
//   - it is the player's own Dark Esau or that Dark Esau's parent/child
//   - it was spawned by the player or by the player's own Dark Esau
static void fix_dark_esau_birthright_damage()
{
	char* collisionSig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_HandleCollision_DarkEsauPlayerCheck);
	char* collisionAddr = collisionSig + 0x23;
	char* noCollisionDamage = collisionAddr + 0x12 + *(int32_t*)(collisionAddr + 0xE);
	ZHL::Log("[REPENTOGON] Patching Dark Esau player collision check at %p\n", collisionAddr);

	ASMPatch collisionPatch;
	collisionPatch.AddBytes(ByteBuffer().AddAny(collisionAddr, 0x6)) // mov eax, [ebp - player]
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x8B\xC8") // mov ecx, eax // player
		.AddBytes("\x8B\xD7") // mov edx, edi // dark esau
		.AddInternalCall(asm_dark_esau_collision_can_damage_player)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noCollisionDamage)
		.AddRelativeJump(collisionAddr + 0x12);
	sASMPatcher.PatchAt(collisionAddr, &collisionPatch);

	char* takeDamageSig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_TakeDamage_DarkEsauSourceCheck);
	char* takeDamageAddr = takeDamageSig + 0x10;
	char* noDamage = takeDamageAddr + 0xD + *(int32_t*)(takeDamageAddr + 0x9);
	ZHL::Log("[REPENTOGON] Patching Dark Esau damage source check at %p\n", takeDamageAddr);

	ASMPatch takeDamagePatch;
	takeDamagePatch.AddBytes("\x8B\xCF") // mov ecx, edi // player
		.AddBytes("\x8B\x56\x24") // mov edx, [esi + 0x24] // source->_entity
		.AddInternalCall(asm_dark_esau_source_can_damage_player)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noDamage)
		.AddBytes(ByteBuffer().AddAny(takeDamageAddr, 0x7)) // cmp byte ptr [edi + 0x173], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, noDamage)
		.AddRelativeJump(takeDamageAddr + 0xD);
	sASMPatcher.PatchAt(takeDamageAddr, &takeDamagePatch);
}

static bool __fastcall asm_camillo_jr_can_fire(Entity_NPC* npc)
{
	return g_Game->GetCurrentRoom()->GetCamera()->IsPosVisible(npc->GetPosition());
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Camillo Jr. could fire from offscreen.
// The patch notes say "Greed Mode" but it actually applies in every mode.
static void fix_camillo_jr_offscreen_fire()
{
	char* sig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_AiTumor_CamilloJrShootEvent);
	char* addr = sig + 0x1A;
	char* skipShoot = addr + 0x6 + *(int32_t*)(addr + 0x2);
	ZHL::Log("[REPENTOGON] Patching Camillo Jr. offscreen shoot check at %p\n", addr);

	ASMPatch patch;
	patch.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipShoot)
		.AddBytes("\x8B\xCF") // mov ecx, edi // npc
		.AddInternalCall(asm_camillo_jr_can_fire)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, skipShoot)
		.AddRelativeJump(addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where ice shards from frozen enemies could freeze and damage friendly monsters.
// The shards are tears spawned with no spawner. 1.9.7.13 instead sets each shard's spawner type to ENTITY_PLAYER.
static void fix_frozen_enemy_ice_shard_spawner_type()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_UpdateFrozenEnemy_IceShardSpawn);
	ZHL::Log("[REPENTOGON] Patching frozen enemy ice shard spawner type at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr, 0xA)) // mov dword ptr [esi + 0x418], 0.2f
		.AddBytes("\xC7\x46\x34").AddBytes(ByteBuffer().AddInteger<int32_t>(ENTITY_PLAYER, false)) // mov dword ptr [esi + 0x34], ENTITY_PLAYER // tear->_spawnerType
		.AddRelativeJump(addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Red Poops had a deadzone in the center where they would not damage the player.
// 1.9.7.13 raises the hurt radii from 18/22 to 18.5/22.5.
static const float s_redPoopPlayerHurtRadius = 18.5f;
static const float s_redPoopHurtRadius = 22.5f;

static void fix_red_poop_hurt_radius()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::GridEntityPoop_Update_RedPoopHurtRadius);
	ZHL::Log("[REPENTOGON] Patching red poop hurt radius at %p\n", addr);

	const float* playerRadius = &s_redPoopPlayerHurtRadius;
	const float* radius = &s_redPoopHurtRadius;
	sASMPatcher.FlatPatch(addr + 0x4, (const char*)&playerRadius, sizeof(playerRadius)); // movss xmm2, [18.0f] -> [18.5f]
	sASMPatcher.FlatPatch(addr + 0xE, (const char*)&radius, sizeof(radius)); // movss xmm1, [22.0f] -> [22.5f]
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Consolation Prize always dropped mana for Tainted ???.
// Consolation Prize doesn't account for Tainted ???'s mana, instead only accounting for his 0 bombs.
static void fix_consolation_prize_tainted_blue_baby()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_AddCollectible_ConsolationPrizeBombCount);
	ZHL::Log("[REPENTOGON] Patching Consolation Prize bomb count at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ECX)
		.AddBytes("\x8B\x8E").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Player, _numBombs), false)) // mov ecx, [esi + _numBombs]
		.AddBytes("\x83\xF8").AddBytes(ByteBuffer().AddByte(PLAYER_BLUEBABY_B)) // cmp eax, PLAYER_BLUEBABY_B // eax = player->_playerType
		.AddBytes("\x0F\x44\x8E").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Player, _poopMana), false)) // cmove ecx, [esi + _poopMana]
		.AddBytes("\x66\x0F\x6E\xC1") // movd xmm0, ecx
		.Pop(ASMPatch::Registers::ECX)
		.AddRelativeJump(addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Dream Catcher showed the wrong Treasure Room items on the Alt Path with More Options.
// The Dream Catcher's preview would show the treasure room's layout for More Options, 
// but always picked it from the Special Rooms set, not the Alt Path's.
// 1.9.7.13 instead picks it from the current Alt Path stage's room set.
// (Note that as Dream Catcher only supports two items, one is still missing.)
static int __stdcall asm_dream_catcher_treasure_room_stage()
{
	const int stageType = g_Game->_stageType;
	if (stageType != STAGETYPE_REPENTANCE && stageType != STAGETYPE_REPENTANCE_B)
		return STB_SPECIAL_ROOMS;

	const int offset = stageType == STAGETYPE_REPENTANCE ? 0 : 1;
	switch (g_Game->_stage)
	{
	case 1:
	case 2:
		return STB_DOWNPOUR + offset;
	case 3:
	case 4:
		return STB_MINES + offset;
	case 5:
	case 6:
		return STB_MAUSOLEUM + offset;
	default:
		return STB_SPECIAL_ROOMS;
	}
}

static void fix_dream_catcher_alt_path_treasure_room()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::NightmareScene_Show_MoreOptionsTreasureRoomStage);
	ZHL::Log("[REPENTOGON] Patching NightmareScene::Show More Options treasure room stage at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EAX) // push eax // shape
		.AddBytes("\x6A\x04") // push ROOM_TREASURE
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(asm_dream_catcher_treasure_room_stage)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EAX) // push stage (was STB_SPECIAL_ROOMS)
		.AddRelativeJump(addr + 0x5); // push 0 // reduceWeight
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Tainted Bethany would not start with the correct number of stored red hearts in co-op if she was not player one.
// Her starting red hearts were overwritten by the co-op consumable sync.
// 1.9.7.13 instead uses AddBloodCharge.
static void __stdcall asm_tainted_bethany_starting_blood_charge(Entity_Player* player)
{
	player->AddBloodCharge(*player->GetRedHearts());
}

static void fix_tainted_bethany_coop_starting_blood_charge()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_InitPreLevelInitStats_TaintedBethanyBloodCharge);
	ZHL::Log("[REPENTOGON] Patching Tainted Bethany starting blood charge at %p\n", addr);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EDI) // player
		.AddInternalCall(asm_tainted_bethany_starting_blood_charge)
		.AddRelativeJump(addr + 0x11); // mov dword ptr [edi + _maxHearts], 0
	sASMPatcher.PatchAt(addr + 0x5, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Bethany's starting Book of Virtues did not count towards the Bookworm transformation.
// The innate Book of Virtues doesn't count; 1.9.7.13 instead counts it during PostLevelInitStats.
static void __stdcall asm_bethany_starting_book_of_virtues_bookworm(Entity_Player* player, int playerType)
{
	if (playerType == PLAYER_BETHANY && player->HasCollectible(COLLECTIBLE_BOOK_OF_VIRTUES, false))
		player->IncrementPlayerFormCounter(10, 1);
}

static void fix_bethany_starting_book_of_virtues_bookworm()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_InitPreLevelInitStats_TaintedBethanyBloodCharge);
	ZHL::Log("[REPENTOGON] Patching Bethany starting Book of Virtues Bookworm counter at %p\n", addr);

	char* notTaintedBethany = addr + 0x5 + *(int8_t*)(addr + 0x4);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX) // playerType
		.Push(ASMPatch::Registers::EDI) // player
		.AddInternalCall(asm_bethany_starting_book_of_virtues_bookworm)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x83\xF8\x24") // cmp eax, PLAYER_BETHANY_B
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, notTaintedBethany)
		.AddRelativeJump(addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Keeper and Tainted Keeper could gain infinite health with Mother's Kiss.
// Each Mother's Kiss raises Keeper's health limit with no upper bound.
// 1.9.7.13 instead properly caps Keeper at 12 containers. Birthright and Greed's Gullet can still go past that, though.
static void fix_keeper_mothers_kiss_health_limit()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_GetHealthLimit_KeeperMothersKissLimit);
	ZHL::Log("[REPENTOGON] Patching Keeper Mother's Kiss health limit at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8D\x0C\x4D\x04").AddZeroes(3) // lea ecx, [ecx*2 + 4]
		.AddBytes("\x8D\x1C\x41") // lea ebx, [ecx + eax*2]
		.AddBytes("\xB8\x18").AddZeroes(3) // mov eax, 24
		.AddBytes("\x3B\xD8") // cmp ebx, eax
		.AddBytes("\x0F\x4F\xD8") // cmovg ebx, eax
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp dword ptr [edi + _playerType], PLAYER_KEEPER
		.AddRelativeJump(addr + 0x11); // jne
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where using Soul of Cain in the mirror room would break the mirror.
static void fix_soul_of_cain_mirror_door()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseCard_SoulOfCainDoorUnlock);
	ZHL::Log("[REPENTOGON] Patching Soul of Cain mirror door at %p\n", addr);

	void* skipDoor = addr + 0xA + *(int8_t*)(addr + 0x9);

	ASMPatch patch;
	patch.AddBytes("\xF7\x80").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(GridEntity_Door, _doorFlags), false)) // test dword ptr [eax + _doorFlags], 0x10 // eax = door
		.AddBytes(ByteBuffer().AddInteger<int32_t>(0x10, false))
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, skipDoor)
		.AddBytes(ByteBuffer().AddAny(addr + 0xA, 0x5)) // push 1; push edi; mov ecx, eax
		.AddRelativeJump(addr + 0xF); // call GridEntity_Door::TryUnlock
	sASMPatcher.PatchAt(addr + 0xA, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where a crawl space would not spawn if placed under certain rocks.
// Room::Init picks the rock hiding the crawl space and only rejected walls, so it could pick a rock that can't be broken normally.
// 1.9.7.13 also rejects blocks, bomb rocks and super secret rocks.
static void fix_crawl_space_rock_type()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_Init_CrawlSpaceRockTypeCheck);
	ZHL::Log("[REPENTOGON] Patching crawl space rock type check at %p\n", addr);

	void* rejectRock = addr + 0x6;
	void* acceptRock = addr + 0x6 + *(int8_t*)(addr + 0x5);

	ASMPatch patch;
	patch.AddBytes("\x8B\x40").AddBytes(ByteBuffer().AddAny(addr + 0x2, 0x1)) // mov eax, dword ptr [eax + _type] // eax = grid entity
		.AddBytes("\x83\xF8").AddBytes(ByteBuffer().AddByte(GRID_WALL)) // cmp eax, GRID_WALL
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, rejectRock)
		.AddBytes("\x83\xF8").AddBytes(ByteBuffer().AddByte(GRID_ROCKB)) // cmp eax, GRID_ROCKB
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, rejectRock)
		.AddBytes("\x83\xF8").AddBytes(ByteBuffer().AddByte(GRID_ROCK_SS)) // cmp eax, GRID_ROCK_SS
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, rejectRock)
		.AddBytes("\x83\xF8").AddBytes(ByteBuffer().AddByte(GRID_ROCK_BOMB)) // cmp eax, GRID_ROCK_BOMB
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, acceptRock)
		.AddRelativeJump(rejectRock); // mov dword ptr [edi + 0x768], -1 // crawl space rock index
	sASMPatcher.PatchAt(addr, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Glowing Hourglass would not refund a teleport card if used to undo that teleport.
// In 1.9.7.13, GHG additionally stores right before a pocket item is used, then skips further stores until the player leaves the room.
// The 1.9.7.13 fix has an additional booboo in which alternating between a teleport card and GHG never actually consumes its charge,
// so I fixed that, too.
static bool s_glowingHourglassStoredForPocketItem = false;

HOOK_STATIC(Game, StoreGlowingHourglassState, (int doorSlot) -> void, __stdcall) {
	if (s_glowingHourglassStoredForPocketItem)
		return;

	super(doorSlot);
}

HOOK_METHOD(Game, ResetState, () -> void) {
	s_glowingHourglassStoredForPocketItem = false;
	super();
}

HOOK_STATIC(Game, RestoreGlowingHourglassState, (Entity_Player* player) -> void, __stdcall) {
	s_glowingHourglassStoredForPocketItem = false;
	super(player);
}

static void __stdcall asm_store_glowing_hourglass_state_for_pocket_item()
{
	Game::StoreGlowingHourglassState(-1);
	s_glowingHourglassStoredForPocketItem = true;
}

static void __stdcall asm_clear_glowing_hourglass_pocket_item_store()
{
	s_glowingHourglassStoredForPocketItem = false;
}

static void fix_glowing_hourglass_pocket_item_refund()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_ControlPocketItem_UsePocketItem);
	ZHL::Log("[REPENTOGON] Patching Glowing Hourglass pocket item store at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(asm_store_glowing_hourglass_state_for_pocket_item)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // push "Action_PillCard_Triggered"; push 0
		.AddRelativeJump(addr + 0x7); // call
	sASMPatcher.PatchAt(addr, &patch);

	char* doorSig = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_CheckPlayerExits_DoorRoomTransition);
	char* doorJumps[] = { doorSig + 0x19, doorSig + 0x27 };
	char* startRoomTransitionCall = doorJumps[0] + 0x5 + *(int32_t*)(doorJumps[0] + 0x1);
	void* startRoomTransition = startRoomTransitionCall + 0x5 + *(int32_t*)(startRoomTransitionCall + 0x1);
	ZHL::Log("[REPENTOGON] Patching Glowing Hourglass pocket item store door reset at %p\n", doorSig);

	for (char* jump : doorJumps)
	{
		ASMPatch doorPatch;
		doorPatch.AddInternalCall(startRoomTransition) // call Game::StartRoomTransition
			.AddInternalCall(asm_clear_glowing_hourglass_pocket_item_store)
			.AddRelativeJump(startRoomTransitionCall + 0x5);
		sASMPatcher.PatchAt(jump, &doorPatch);
	}
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Tainted Lazarus could lose story items if respawning as another player
// and they were held by the other Lazarus half.
// Reviving as ??? (Ankh) or Black Judas (Judas' Shadow) left the other half stranded, so its items were lost.
// 1.9.7.13 instead calls MergeTwins when changing away from Tainted Lazarus, which falls back to the stored
// other half (_backupPlayer) when there's no twin, merging its items and then destroying it.
static void __stdcall asm_tainted_lazarus_merge_other_half(Entity_Player* player, int oldPlayerType)
{
	if (oldPlayerType == PLAYER_LAZARUS_B || oldPlayerType == PLAYER_LAZARUS2_B)
		player->MergeTwins();
}

static void fix_tainted_lazarus_type_change_merge()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_TriggerPlayerTypeChanged_MergeTwinsCheck);
	ZHL::Log("[REPENTOGON] Patching Tainted Lazarus type change merge at %p\n", addr);

	void* notTaintedJacob = addr + 0x9 + *(int32_t*)(addr + 0x5);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // old player type
		.Push(ASMPatch::Registers::EBX) // player
		.AddInternalCall(asm_tainted_lazarus_merge_other_half)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x8B\x8B").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Player, _playerType), false)) // mov ecx, dword ptr [ebx + _playerType]
		.AddBytes(ByteBuffer().AddAny(addr, 0x3)) // cmp ecx, PLAYER_JACOB_B
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, notTaintedJacob)
		.AddRelativeJump(addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);

	char* getTwin = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_MergeTwins_GetTwin) + 0xA;
	ZHL::Log("[REPENTOGON] Patching MergeTwins backup player fallback at %p\n", getTwin);

	ASMPatch getTwinPatch;
	getTwinPatch.AddBytes("\xC6\x45\xF4").AddZeroes(1) // mov byte ptr [ebp - 0xC], 0
		.AddBytes(ByteBuffer().AddAny(getTwin, 0x6)) // mov ebx, dword ptr [esi + _twinPlayer]
		.AddBytes("\x85\xDB") // test ebx, ebx
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, getTwin + 0x6)
		.AddBytes("\xC6\x45\xF4\x01") // mov byte ptr [ebp - 0xC], 1
		.AddBytes("\x8B\x9E").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Player, _backupPlayer), false)) // mov ebx, dword ptr [esi + _backupPlayer]
		.AddBytes("\x85\xDB") // test ebx, ebx
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, getTwin + 0x6)
        .Pop(ASMPatch::Registers::ESI)
        .Pop(ASMPatch::Registers::EBX)
		.AddBytes("\x8B\xE5") // mov esp, ebp
        .Pop(ASMPatch::Registers::EBP)
        .AddBytes("\xC3"); // ret
	sASMPatcher.PatchAt(getTwin, &getTwinPatch);

	char* removeTwin = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_MergeTwins_RemoveTwin) + 0xA;
	ZHL::Log("[REPENTOGON] Patching MergeTwins backup player removal at %p\n", removeTwin);

	void* removeCoPlayer = removeTwin + 0x6 + *(int8_t*)(removeTwin + 0x5);
	ByteBuffer backupPlayerOffset = ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Player, _backupPlayer), false);

	ASMPatch removeTwinPatch;
	removeTwinPatch.AddBytes(ByteBuffer().AddAny(removeTwin, 0x4)) // mov ecx, dword ptr [edi]; test ecx, ecx // edi = &_twinPlayer
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, removeTwin + 0x6)
		.AddBytes("\x80\x7D\xF4").AddZeroes(1) // cmp byte ptr [ebp - 0xC], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, removeCoPlayer)
		.AddBytes("\x8B\x8E").AddBytes(backupPlayerOffset) // mov ecx, dword ptr [esi + _backupPlayer]
		.AddBytes("\x8B\x01") // mov eax, dword ptr [ecx]
		.AddBytes("\xFF\x50\x44") // call dword ptr [eax + 0x44]
		.AddBytes("\x8B\x8E").AddBytes(backupPlayerOffset) // mov ecx, dword ptr [esi + _backupPlayer]
		.AddBytes("\x85\xC9") // test ecx, ecx
		.AddBytes("\x74\x10") // je epilogue
		.AddBytes("\x8B\x01") // mov eax, dword ptr [ecx]
		.AddBytes("\x6A\x01") // push 1
		.AddBytes("\xFF\x10") // call dword ptr [eax]
		.AddBytes("\xC7\x86").AddBytes(backupPlayerOffset).AddZeroes(4) // mov dword ptr [esi + _backupPlayer], 0
        .Pop(ASMPatch::Registers::EDI)
        .Pop(ASMPatch::Registers::ESI)
        .Pop(ASMPatch::Registers::EBX)
		.AddBytes("\x8B\xE5") // mov esp, ebp
        .Pop(ASMPatch::Registers::EBP)
        .AddBytes("\xC3"); // ret
	sASMPatcher.PatchAt(removeTwin, &removeTwinPatch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where collecting a Bone Heart could remove charges from Alabaster Box.
// When at the HP limit, AddBoneHearts calls AddSoulHearts with a negative amount to make room.
// AddSoulHearts only skipped Alabaster Box adjustment for a value of 0, not negatives, so a negative amount was "added" to the box.
static void fix_alabaster_box_negative_soul_hearts()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_AddSoulHearts_AlabasterBoxChargeCheck) + 0x3;
	ZHL::Log("[REPENTOGON] Patching AddSoulHearts Alabaster Box charge check at %p\n", addr);

	sASMPatcher.FlatPatch(addr, "\x8E", 1); // je -> jle
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where Bloaty's creep attack could hurt flying characters.
// 1.9.7.13 removes the creep attack's splash damage.
static void fix_bloaty_creep_splash_damage()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_AiBloaty_CreepSplashDamage);
	ZHL::Log("[REPENTOGON] Patching Bloaty creep splash damage at %p\n", addr);

	sASMPatcher.FlatPatch(addr, "\xEB\x3B", 2);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where a Blood Donation Machine would not be destroyed when Swallowed M80 made Isaac explode.
static void fix_blood_donation_machine_swallowed_m80()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntitySlot_HandleCollision_BloodDonationTakeDamage);
	ZHL::Log("[REPENTOGON] Patching Blood Donation Machine Swallowed M80 explosion at %p\n", addr);

	void* payout = addr + 0x1A + *(int8_t*)(addr + 0x19);
	void* skipPayout = addr + 0x27 + *(int32_t*)(addr + 0x23);

	ASMPatch patch;
	patch.AddBytes("\xFF\x50\x20") // call dword ptr [eax + 0x20]
		.AddBytes("\x83\xBF").AddBytes(ByteBuffer().AddInteger<int32_t>(offsetof(Entity_Slot, _state), false)).AddBytes("\x01") // cmp dword ptr [edi + _state], 1 // edi = slot
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, skipPayout)
		.AddRelativeJump(payout);
	sASMPatcher.PatchAt(addr + 0x15, &patch);
}

// Fixes a vanilla bug (fixed in 1.9.7.13) where using Glowing Hourglass immediately after taking Dad's Note resulted in a softlock.
static void __stdcall asm_store_glowing_hourglass_state_for_dads_note(int collectible)
{
	if (collectible == COLLECTIBLE_DADS_NOTE)
		Game::StoreGlowingHourglassState(-1);
}

static void fix_glowing_hourglass_dads_note_softlock()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_HandleCollision_CollectibleRemoveFromPools);
	ZHL::Log("[REPENTOGON] Patching Glowing Hourglass Dad's Note store at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny(addr + 0x4, 0x6)) // push dword ptr [ebp - 0xAC]
		.AddInternalCall(asm_store_glowing_hourglass_state_for_dads_note)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny(addr, 0xA)) // push 0; push 0; push dword ptr [ebp - 0xAC]
		.AddRelativeJump(addr + 0xA); // lea ecx, [eax + _itemPool]
	sASMPatcher.PatchAt(addr, &patch);

	char* stageStore = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Game_StartStageTransition_StoreGlowingHourglassState) + 0x9;
	ZHL::Log("[REPENTOGON] Patching StartStageTransition Glowing Hourglass store at %p\n", stageStore);

	void* storeGlowingHourglassState = stageStore + 0x7 + *(int32_t*)(stageStore + 0x3);

	ASMPatch stagePatch;
	stagePatch.AddBytes("\x83\xFE\x05") // cmp esi, 5
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, stageStore + 0x7)
		.AddBytes("\x6A\xFF") // push -1
		.AddInternalCall(storeGlowingHourglassState)
		.AddRelativeJump(stageStore + 0x7);
	sASMPatcher.PatchAt(stageStore, &stagePatch);
}

static Room* __fastcall asm_room_reset(Room* room)
{
    if (room == nullptr)
    {
        room = (Room*)operator new(sizeof(Room));
    }
    else
    {
        room->destructor();
    }

	// Forced zero-initialization, because for some reason the constructor does not initialize all fields, such as the _roomDescriptor pointer.
	// Helps to prevent some potential crashes at specific points in time, such as MC_PRE_NEW_ROOM.
	std::memset(room, 0, sizeof(Room));

    room->constructor();
    return room;
}

static void patch_in_place_room_reset()
{
    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Reset_RoomReset);
    intptr_t resumeAddr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Level_Reset_PostRoomReset);
    ZHL::Log("[REPENTOGON] Patching Level::Reset for in-place Room reset at %p\n", addr);

    ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
    ASMPatch patch;
    patch.PreserveRegisters(savedRegisters)
        .AddBytes("\x8B\xCF") // MOV ECX, EDI // room
        .AddInternalCall(asm_room_reset)
        .RestoreRegisters(savedRegisters)
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void ASMFixes()
{
    fix_modded_crafting_quality("8b0eba????????85c9c745", "ItemConfig::Load");
    fix_variant_set_add_unique("8b0283c002", "ModManager::UpdateRooms (inline RoomConfig::VariantSet::AddUnique)");
    fix_handle_collisions_playeronly_entity_class("83f80174??83f901", "Entity::handle_collisions");
    fix_try_remove_smelted_trinket();
    fix_force_add_pill_effect();
    fix_render_pocket_item_pill_identified_check();
    fix_use_pill_identify_pill();
    fix_itempool_getpilleffect();
    fix_game_state_read_missing_item_pool_fail_check();
	suppress_push_render_target_stack_overflow();
	fix_familiar_pathfinder_move_randomly_axis_aligned(&AsmDefinitions::MoveRandomlyAxisAligned_FamiliarCheckA, &AsmDefinitions::MoveRandomlyAxisAligned_FamiliarCheckA_JumpTarget, "\x85\xc9", 0x6);  // TEST ECX,ECX
	fix_familiar_pathfinder_move_randomly_axis_aligned(&AsmDefinitions::MoveRandomlyAxisAligned_FamiliarCheckB, &AsmDefinitions::MoveRandomlyAxisAligned_FamiliarCheckB_JumpTarget, "\x85\xc0", 0x7);  // TEST EAX,EAX
	fix_load_button_maps_crash();
	fix_openal_attributes();
	fix_mausoleum_heart_door_forced_open();
	fix_evaluate_items_lil_delirium_owner();
	fix_bag_of_crafting_queue_item_pool();
	fix_rotgut_thematic_item();
	fix_glowing_hourglass_blood_puppy();
	fix_fissure_spawner_not_saved();
	fix_filigree_feather_key_piece_priority();
	fix_death_certificate_item_pool();
	fix_glowing_hourglass_alt_path_home();
	fix_glowing_hourglass_detached_entity_references();
	fix_soul_of_jacob_esau_item_pool_decrease();
	fix_epiphora_charged_weapon_release();
	fix_spectral_bobs_head_wall_bounce();
	fix_star_of_bethlehem_off_grid_speed();
	fix_dull_razor_damage_block();
	fix_black_maw_charge_right_bound();
	fix_satan_chest_labyrinth_stage();
	fix_greed_keeper_item_pool_removal();
	fix_dogma_room_bombs();
	fix_familiar_sound_delays();
	fix_found_soul_item_text_popup();
	fix_suplex_mega_mush();
	fix_suplex_fireplace_grab();
	fix_camo_undies_mouse_shooting();
	fix_eye_of_the_occult_mouse_direction();
	fix_dark_esau_birthright_damage();
	fix_camillo_jr_offscreen_fire();
	fix_frozen_enemy_ice_shard_spawner_type();
	fix_red_poop_hurt_radius();
	fix_consolation_prize_tainted_blue_baby();
	fix_dream_catcher_alt_path_treasure_room();
	fix_tainted_bethany_coop_starting_blood_charge();
	fix_bethany_starting_book_of_virtues_bookworm();
	fix_keeper_mothers_kiss_health_limit();
	fix_soul_of_cain_mirror_door();
	fix_crawl_space_rock_type();
	fix_glowing_hourglass_pocket_item_refund();
	fix_tainted_lazarus_type_change_merge();
	fix_alabaster_box_negative_soul_hearts();
	fix_bloaty_creep_splash_damage();
	fix_blood_donation_machine_swallowed_m80();
	fix_glowing_hourglass_dads_note_softlock();
    patch_in_place_room_reset();
}