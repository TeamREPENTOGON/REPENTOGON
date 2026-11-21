#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
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
    patch_in_place_room_reset();
}
