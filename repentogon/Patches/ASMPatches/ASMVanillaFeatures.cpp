#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../../REPENTOGONOptions.h"

#include "ASMVanillaFeatures.h"
#include "../VanillaFeatures.h"

// These ASM patches allow for reverting Repentance+ changes to 1.7.9b behavior.
static bool __fastcall asm_cod_worm_should_dig_out(Entity_NPC* npc)
{
	if (repentogonOptions.vfCodWormHiding) {
		return npc->_stateFrame <= 0;
	}

	return Isaac::genrand_int32() % 20 == 0 && npc->_stateFrame <= 0;
}

// 1.9.7.11 removes Cod Worm's 1 in 20 roll once the hide timer had run out, so they pop out faster.
static void vf_cod_worm_hiding()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_cod_worm_IdleDigOutCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_cod_worm dig-out check at %p\n", addr);

	// cmp dword ptr [edi + 0x410], 0 ; jle digOut
	char* digOut = addr + 0x9 + *(int8_t*)(addr + 0x8);

	ASMPatch patch;
	patch.AddBytes("\x8B\xCF") // mov ecx, edi // npc
		.AddInternalCall(asm_cod_worm_should_dig_out)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addr + 0x9) // mov eax, dword ptr [edi + 0x168]
		.AddRelativeJump(digOut); // mov dword ptr [edi + 0xb64], 6
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_kineti_can_grab(bool onScreen)
{
	return onScreen || !repentogonOptions.vfKinetiOnScreen;
}

// 1.9.7.11 makes it so Kinetis can only grab rocks if they are on screen.
static void vf_kineti_on_screen()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_polty_KinetiOnScreenCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_polty Kineti on-screen check at %p\n", addr);

	// test al, al ; jz skip
	char* skip = addr + 0x8 + *(int32_t*)(addr + 0x4);

	ASMPatch patch;
	patch.AddBytes("\x50") // push eax // IsPosVisible result
		.AddInternalCall(asm_kineti_can_grab)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, skip)
		.AddRelativeJump(addr + 0x8); // push 0 ; push "TeleStart"
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_blood_cultist_can_attack(bool onScreen)
{
	return onScreen || !repentogonOptions.vfBloodCultistOnScreen;
}

// 1.9.7.11 makes it so Blood Cultists can only use their blood attack if they are on screen.
static void vf_blood_cultist_on_screen()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_cultist_BloodCultistOnScreenCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_cultist Blood Cultist on-screen check at %p\n", addr);

	// test al, al ; mov eax, dword ptr [ebp - 0xec] ; jz skipAttack
	ASMPatch patch;
	patch.AddBytes("\x50") // push eax // IsPosVisible result
		.AddInternalCall(asm_blood_cultist_can_attack)
		.AddBytes("\x84\xC0") // test al, al
		.AddBytes(ByteBuffer().AddAny(addr + 0x2, 0x6)) // mov eax, dword ptr [ebp - 0xec] // player target
		.AddRelativeJump(addr + 0x8); // jz skipAttack
	sASMPatcher.PatchAt(addr, &patch);
}

static void __stdcall asm_mega_fatty_red_puke(Entity_NPC* npc, Vector* pos, ProjectileParams* params)
{
	pos->x = npc->_pos.x;
	pos->y = npc->_pos.y;

	if (repentogonOptions.vfMegaFattyRedPukeHeight) {
		pos->y -= 8.0f;
		return;
	}

	params->HeightModifier = -50.0f;
	params->FallingSpeedModifier = -2.0f;
}

// 1.9.7.11 lowers red champion Mega Fatty's puke (height -50 -> -40, falling speed -2 -> 1.5, fired from 8 units higher up).
static void vf_mega_fatty_red_puke_height()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityNPC_ai_mega_fatty_RedChampionPukePosition);
	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_mega_fatty red champion puke at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\xFF\x74\x24\x0C") // push dword ptr [esp + 0xc] // params
		.AddBytes("\x50") // push eax // pos
		.AddBytes("\x57") // push edi // npc
		.AddInternalCall(asm_mega_fatty_red_puke)
		.AddBytes("\x8B\xCF") // mov ecx, edi
		.AddRelativeJump(addr + 0x30); // call Entity_NPC::FireProjectiles
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_amnesia_horse_pill_unidentifies()
{
	return repentogonOptions.vfAmnesiaHorsePillUnidentify;
}

// 1.9.7.11 makes the Amnesia horse pill unidentify every pill (unless the player has PHD, False PHD, Virgo or Lucky Foot).
static void vf_amnesia_horse_pill_unidentify()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UsePill_AmnesiaHorsePillCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::UsePill Amnesia horse pill check at %p\n", addr);

	// cmp byte ptr [ebp - 0x144], 0 ; jz skipUnidentify
	char* skip = addr + 0xD + *(int32_t*)(addr + 0x9);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp byte ptr [ebp - 0x144], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, skip)
		.AddInternalCall(asm_amnesia_horse_pill_unidentifies)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, skip)
		.AddRelativeJump(addr + 0xD); // push 0 ; push COLLECTIBLE_FALSE_PHD
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_sharp_plug_bleeding_hearts_half_charge()
{
	return repentogonOptions.vfSharpPlugBleedingHearts;
}

// 1.9.7.11 makes Sharp Plug only give a charge for every other soul heart / bleeding red heart Tainted Magdalene loses.
static void vf_sharp_plug_bleeding_hearts()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_control_active_item_SharpPlugTaintedMagdaleneCheck);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::control_active_item Sharp Plug Tainted Magdalene check at %p\n", addr);

	// cmp dword ptr [edi + 0x13bc], PLAYER_MAGDALENE_B ; jnz addCharge
	char* addCharge = addr + 0x9 + *(int8_t*)(addr + 0x8);

	ASMPatch patch;
	patch.AddInternalCall(asm_sharp_plug_bleeding_hearts_half_charge)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addCharge)
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp dword ptr [edi + 0x13bc], PLAYER_MAGDALENE_B
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, addCharge)
		.AddRelativeJump(addr + 0x9); // mov eax, dword ptr [ebp - 0x80]
	sASMPatcher.PatchAt(addr, &patch);
}

static int __stdcall asm_wheel_of_fortune_machine(RNG* rng)
{
	if (repentogonOptions.vfWheelOfFortuneCraneGame) {
		return 0;
	}

	return rng->RandomInt(3) == 0 ? 3 : 1; // fortune telling machine : slot machine
}

// 1.9.7.11 changes the Wheel of Fortune machine roll from 1/3 fortune telling machine to 1% crane game (if unlocked), 25% fortune telling machine.
static void vf_wheel_of_fortune_crane_game()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseCard_WheelOfFortuneMachineRoll);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::UseCard Wheel of Fortune machine roll at %p\n", addr);

	// mov edx, 100 ; mov dword ptr [ebp - 0x2bc], 1 ; xor ecx, ecx ; call RandomInt ; ... cmp eax, 0x19 ; jg spawn
	char* spawn = addr + 0x46 + *(int8_t*)(addr + 0x45);

	ASMPatch patch;
	patch.AddBytes("\x56") // push esi // rng
		.AddInternalCall(asm_wheel_of_fortune_machine)
		.AddBytes("\x85\xC0") // test eax, eax
		.AddBytes(ByteBuffer().AddAny(addr, 0xF)) // mov edx, 100 ; mov dword ptr [ebp - 0x2bc], 1
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addr + 0xF) // xor ecx, ecx ; call RandomInt
		.AddBytes("\x89\x85").AddBytes(ByteBuffer().AddAny(addr + 0x7, 0x4)) // mov dword ptr [ebp - 0x2bc], eax // variant
		.AddRelativeJump(spawn); // mov ecx, esi ; call RNG::Next
	sASMPatcher.PatchAt(addr, &patch);
}

static int __stdcall asm_punching_bag_collision_class()
{
	return repentogonOptions.vfPunchingBagBlockShots ? 4 : 3; // ENTCOLL_ALL : ENTCOLL_ENEMIES
}

// 1.9.7.11 undoes a Repentance change that prevented Punching Bag from blocking enemy shots.
static void vf_punching_bag_block_shots()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_ai_punchingbag_EntityCollisionClass);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::ai_punchingbag entity collision class at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr + 0xA, 0x8)) // movss dword ptr [edi + 0x36c], xmm0
		.AddInternalCall(asm_punching_bag_collision_class)
		.AddBytes("\x89\x87").AddBytes(ByteBuffer().AddAny(addr + 0x2, 0x4)) // mov dword ptr [edi + 0x188], eax
		.AddBytes("\x80\x7D\xB8").AddZeroes(1) // cmp byte ptr [ebp - 0x48], 0
		.AddRelativeJump(addr + 0x12); // jnz skipFree
	sASMPatcher.PatchAt(addr, &patch);
}

enum AngelKeyPieceCheck : int {
	ANGEL_KEY_PIECE_REPPLUS_CHECK = 0,
	ANGEL_KEY_PIECE_AWARD = 1,
	ANGEL_KEY_PIECE_NO_AWARD = 2,
};

static int __stdcall asm_angel_key_piece_check(Entity* boss)
{
	if (repentogonOptions.vfMeatCleaverAngelKeyPiece) {
		return ANGEL_KEY_PIECE_REPPLUS_CHECK;
	}

	return boss->GetSpawnerEntity() ? ANGEL_KEY_PIECE_NO_AWARD : ANGEL_KEY_PIECE_AWARD;
}

static bool __stdcall asm_angel_key_piece_scan_floor()
{
	return repentogonOptions.vfMeatCleaverAngelKeyPiece;
}

// 1.9.7.11 makes it so using Meat Cleaver on an Angel will now award only one Key Piece.
static void vf_meat_cleaver_angel_key_piece()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_TriggerBossDeath_AngelKeyPieceSameTypeCheck);
	ZHL::Log("[REPENTOGON] Patching Room::TriggerBossDeath angel key piece check at %p\n", addr);

	char* award = addr + 0xE + *(int8_t*)(addr + 0xD);
	char* noAward = addr + 0x27 + *(int32_t*)(addr + 0x23);

	ASMPatch patch;
	patch.AddBytes("\x52") // push edx // room
		.AddBytes("\x56") // push esi // boss
		.AddInternalCall(asm_angel_key_piece_check)
		.AddBytes("\x5A") // pop edx
		.AddBytes("\x83\xF8\x01") // cmp eax, ANGEL_KEY_PIECE_AWARD
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, award)
		.AddBytes("\x83\xF8\x02") // cmp eax, ANGEL_KEY_PIECE_NO_AWARD
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, noAward)
		.AddBytes(ByteBuffer().AddAny(addr, 0x8)) // mov ecx, dword ptr [edx + 0x7300] ; xor edi, edi
		.AddRelativeJump(addr + 0x8); // mov eax, dword ptr [ecx]
	sASMPatcher.PatchAt(addr, &patch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Room_TriggerBossDeath_AngelKeyPieceFloorScan);
	ZHL::Log("[REPENTOGON] Patching Room::TriggerBossDeath angel key piece floor scan at %p\n", addr);

	char* noPickups = addr + 0xA + *(int8_t*)(addr + 0x9);

	ASMPatch floorPatch;
	floorPatch.AddBytes("\x50\x51\x52") // push eax ; push ecx ; push edx
		.AddInternalCall(asm_angel_key_piece_scan_floor)
		.AddBytes("\x84\xC0") // test al, al
		.AddBytes("\x5A\x59\x58") // pop edx ; pop ecx ; pop eax
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noPickups)
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // mov edi, dword ptr [eax + 0x1264]
		.AddRelativeJump(addr + 0x6); // test edi, edi
	sASMPatcher.PatchAt(addr, &floorPatch);
}

struct DeliriousBoss {
	unsigned int type;
	unsigned int variant;
	unsigned int count;
};

// 1.7.9b Delirious boss pool
static const DeliriousBoss deliriousBosses[] = {
	{ 20, 0, 1 }, { 19, 0, 3 }, { 28, 0, 3 }, { 36, 0, 1 }, { 43, 0, 1 }, { 62, 1, 1 }, { 63, 0, 1 }, { 64, 0, 1 },
	{ 65, 0, 1 }, { 66, 0, 1 }, { 67, 0, 1 }, { 68, 0, 1 }, { 69, 0, 1 }, { 74, 0, 1 }, { 79, 0, 1 }, { 71, 0, 1 },
	{ 43, 1, 1 }, { 79, 1, 1 }, { 28, 1, 3 }, { 82, 0, 1 }, { 81, 0, 1 }, { 19, 1, 3 }, { 28, 2, 3 }, { 99, 0, 1 },
	{ 67, 1, 1 }, { 68, 1, 1 }, { 69, 1, 1 }, { 79, 2, 1 }, { 71, 1, 1 }, { 100, 0, 1 }, { 97, 0, 1 }, { 100, 1, 1 },
	{ 62, 0, 1 }, { 65, 1, 1 }, { 101, 0, 1 }, { 101, 1, 1 }, { 260, 0, 1 }, { 261, 0, 1 }, { 262, 0, 1 }, { 263, 0, 1 },
	{ 264, 0, 1 }, { 265, 0, 1 }, { 266, 0, 1 }, { 267, 0, 1 }, { 268, 0, 1 }, { 269, 0, 1 }, { 270, 0, 1 }, { 274, 0, 1 },
	{ 237, 1, 1 }, { 401, 0, 1 }, { 402, 0, 1 }, { 403, 0, 1 }, { 404, 0, 1 }, { 405, 0, 1 }, { 407, 0, 1 }, { 261, 1, 1 },
	{ 237, 2, 1 }, { 62, 2, 1 }, { 409, 0, 1 }, { 410, 0, 1 }, { 411, 0, 1 },
};

static const int deliriousWisps[] = { 0x1000e, 0x1000f, 0x10010, 0x10011 };

static void ReplaceDeliriumGraphics(Entity* entity)
{
	Entity_NPC::DeliriumReplaceGraphics(&entity->_sprite, entity->_type, entity->_variant, true);
}

static Entity_NPC* ToNPC(Entity* entity)
{
	return entity ? entity->ToNPC() : nullptr;
}

// Reimplementation of Delirious 1.7.9b. Boss health doesn't decay and they aren't persistent
static bool __stdcall asm_delirious_use(Entity_Player* player, bool addWisp)
{
	if (repentogonOptions.vfDeliriousBossRework) {
		return false;
	}

	RNG* rng = player->GetCollectibleRNG(COLLECTIBLE_DELIRIOUS);
	const DeliriousBoss& boss = deliriousBosses[rng->RandomInt(sizeof(deliriousBosses) / sizeof(deliriousBosses[0]))];

	std::vector<Entity*> spawned;
	for (unsigned int i = 0; i < boss.count; ++i) {
		const unsigned int seed = rng->Next();
		Vector buffer;
		Vector* position = g_Game->_room->FindFreePickupSpawnPosition(&buffer, player->GetPosition(), 40.0f, false, false, false);
		Entity* entity = g_Game->Spawn(boss.type, boss.variant, *position, Vector(0.0f, 0.0f), player, 0, seed, 0);

		EntityRef ref(player);
		entity->AddCharmed(ref, -1, false, false);
		((Entity_NPC*)entity)->SetCanShutDoors(false);
		Entity_NPC::DeliriumReplaceGraphics(&entity->_sprite, boss.type, boss.variant, true);
		spawned.push_back(entity);
	}

	for (Entity* entity : spawned) {
		entity->Update();
		entity->AddEntityFlags((unsigned int)FLAG_DONT_COUNT_BOSS_HP, 0);

		Entity* first = entity;
		while (first->GetParent()) {
			first = first->GetParent();
		}

		Entity_NPC* segment = ToNPC(first);
		Entity_NPC* previous = nullptr;
		while (segment) {
			if ((Entity*)segment == entity) {
				segment = ToNPC(segment->GetChild());
				if (segment == previous) {
					break;
				}
				continue;
			}

			ReplaceDeliriumGraphics(segment);
			previous = segment;
			segment = ToNPC(segment->GetChild());
		}
	}

	if (g_Game->_hallucinationTimer <= 0) {
		g_Game->ShowHallucination(25, 63);
	}

	if (player->HasCollectible(COLLECTIBLE_BOOK_OF_VIRTUES, false) && addWisp) {
		player->AddWisp((CollectibleType)deliriousWisps[rng->RandomInt(4)], player->GetPosition(), false, false);
	}

	return true;
}

// 1.9.7.11 gives Delirious bosses fixed health, a 1 damage per hit cap and health decay (1.7.9.4 also made them persistent, replaced on reuse, with a new pool).
static void vf_delirious_boss_rework()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_DeliriousUse);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem Delirious at %p\n", addr);

	char* caseEnd = addr + 0x34 + *(int32_t*)(addr + 0x30);

	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny(addr + 0x19, 0x6)) // push dword ptr [ebp - 0x5f4] // Book of Virtues wisp flag
		.AddBytes("\x56") // push esi // player
		.AddInternalCall(asm_delirious_use)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, caseEnd)
		.AddBytes(ByteBuffer().AddAny(addr, 0xB)) // push 0x1fe ; lea ecx, [esi + 0x1760]
		.AddRelativeJump(addr + 0xB); // call GetCollectibleRNG
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_mars_dash_rework()
{
	return repentogonOptions.vfMarsDashRework;
}

// Reimplementation of the 1.7.9b Mars.
static bool __stdcall asm_mars_dash_start(Entity_Player* player)
{
	if (repentogonOptions.vfMarsDashRework) {
		return false;
	}

	static const Vector directions[] = { Vector(-1.0f, 0.0f), Vector(1.0f, 0.0f), Vector(0.0f, -1.0f), Vector(0.0f, 1.0f) };
	Vector direction = directions[player->_marsMoveDirection];
	if (Room::IsMirrorWorld()) {
		direction.x = -direction.x;
	}

	player->_temporaryeffects.AddCollectibleEffect(COLLECTIBLE_MARS, false, 1);
	player->_marsCooldown = 90;
	player->_velocity.x += direction.x * 4.0f * player->_speedMultiplier;
	player->_velocity.y += direction.y * 4.0f * player->_speedMultiplier;
	player->_hitList.clear();
	Manager::PlaySound(119, 1.0f, 2, false, 1.0f);
	player->RemoveCostume(g_Manager->GetItemConfig()->GetNullItem(58));

	return true;
}

// 1.9.7.11 reworked Mars. It no longer passes through enemies, and every impact spawns a ring of fire.
// It also grants 30 i-frames.
static void vf_mars_dash_rework()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_control_movement_MarsDashStart);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::control_movement Mars dash start at %p\n", addr);

	char* noDash = addr + *(int8_t*)(addr - 0x1);

	ASMPatch startPatch;
	startPatch.AddBytes("\x57") // push edi // player
		.AddInternalCall(asm_mars_dash_start)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, noDash)
		.AddBytes(ByteBuffer().AddAny(addr, 0x9)) // push 1 ; push 0 ; push COLLECTIBLE_MARS
		.AddRelativeJump(addr + 0x9); // lea ecx, [edi + 0x1500]
	sASMPatcher.PatchAt(addr, &startPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_update_effects_MarsDeferredDashStart);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::update_effects Mars deferred dash start at %p\n", addr);

	char* dashFrame = addr + 0x12 + *(int32_t*)(addr + 0xE);

	ASMPatch deferredPatch;
	deferredPatch.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, dashFrame)
		.AddBytes(ByteBuffer().AddAny(addr, 0xC)) // mov eax, [edi + 0x1e74] ; cmp eax, [edi + 0x1e70]
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, dashFrame)
		.AddRelativeJump(addr + 0x12); // mov eax, [edi + 0x1e78]
	sASMPatcher.PatchAt(addr, &deferredPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_update_effects_MarsDashEnd);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::update_effects Mars dash end at %p\n", addr);

	char* clearDirection = addr + 0x8 + *(int8_t*)(addr + 0x7);
	char* cooldownTick = clearDirection + 0xA; // after mov dword ptr [edi + 0x1e78], -1

	ASMPatch endPatch;
	endPatch.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, cooldownTick)
		.AddBytes("\x80\xBF").AddBytes(ByteBuffer().AddAny(addr + 0x2, 0x4)).AddZeroes(1) // cmp byte ptr [edi + 0x1e7c], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, clearDirection)
		.AddRelativeJump(addr + 0x8); // push 0x10
	sASMPatcher.PatchAt(addr, &endPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_update_effects_MarsWallImpact);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::update_effects Mars wall impact at %p\n", addr);

	ASMPatch wallPatch;
	wallPatch.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addr + 0x11) // push -1 ; push COLLECTIBLE_PONY
		.AddBytes(ByteBuffer().AddAny(addr, 0xC)) // mov ecx, edi ; mov dword ptr [edi + 0x1e78], -1
		.AddRelativeJump(addr + 0xC); // call MarsImpact
	sASMPatcher.PatchAt(addr, &wallPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_update_effects_MarsDashBlink);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::update_effects Mars dash blink at %p\n", addr);

	char* addBlink = addr + 0x8 + *(int8_t*)(addr + 0x7);

	ASMPatch blinkPatch;
	blinkPatch.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, addBlink)
		.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // cmp esi, COLLECTIBLE_MARS
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, addBlink)
		.AddRelativeJump(addr + 0x8); // cmp byte ptr [edi + 0x1e7c], 0
	sASMPatcher.PatchAt(addr, &blinkPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_handle_collision_MarsEnemyImpact);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::handle_collision Mars enemy impact at %p\n", addr);

	char* noEnemyImpact = addr + 0x18 + *(int32_t*)(addr + 0x14);

	ASMPatch enemyPatch;
	enemyPatch.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noEnemyImpact)
		.AddBytes(ByteBuffer().AddAny(addr, 0xB)) // push COLLECTIBLE_MARS ; lea ecx, [esi + 0x1500]
		.AddRelativeJump(addr + 0xB); // call HasCollectibleEffect
	sASMPatcher.PatchAt(addr, &enemyPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_Update_MarsMegaSatanImpact);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::Update Mars Mega Satan barrier impact at %p\n", addr);

	char* noBarrierImpact = addr + 0x18 + *(int8_t*)(addr + 0x17);

	ASMPatch barrierPatch;
	barrierPatch.AddBytes(ByteBuffer().AddAny(addr + 0x7, 0xA)) // mov dword ptr [edi + 0x340], 320.0
		.AddInternalCall(asm_mars_dash_rework)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noBarrierImpact)
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // push COLLECTIBLE_MARS ; mov ecx, esi
		.AddRelativeJump(addr + 0x11); // call HasCollectibleEffect
	sASMPatcher.PatchAt(addr, &barrierPatch);
}

static void __stdcall asm_jar_of_wisps_initial_var_data(int* varData)
{
	const int minimum = repentogonOptions.vfJarOfWispsRework ? 2 : 1;
	if (*varData < minimum) {
		*varData = minimum;
	}
}

static ItemConfig_Item* __stdcall asm_jar_of_wisps_wisp_item(Entity_Player* player)
{
	if (repentogonOptions.vfJarOfWispsRework) {
		return nullptr;
	}

	ItemConfig* config = g_Manager->GetItemConfig();
	RNG* rng = player->GetCollectibleRNG(COLLECTIBLE_JAR_OF_WISPS);
	const int id = rng->RandomInt(config->GetCollectibles()->size() - 1) + 1;

	ItemConfig_Item* item = config->GetCollectible(id);
	if (!item || !ItemConfig::IsValidCollectible(id) || item->type != 3) { // ITEM_ACTIVE
		item = config->GetCollectible(COLLECTIBLE_BOOK_OF_VIRTUES);
	}

	return item;
}

// 1.9.7.11 makes the first Jar of Wisps use grant two wisps,
// and rolls a special wisp 67% of the time.
static void vf_jar_of_wisps_rework()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_AddCollectible_JarOfWispsInitialVarData);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::AddCollectible Jar of Wisps initial varData at %p\n", addr);

	char* caseEnd = addr + 0xE + *(int32_t*)(addr + 0xA);

	ASMPatch varDataPatch;
	varDataPatch.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddBytes("\x8D\x8C\x30").AddBytes(ByteBuffer().AddAny(addr + 0x3, 0x4)) // lea ecx, [eax + esi + 0x1590] // varData
		.Push(ASMPatch::Registers::ECX) // varData
		.AddInternalCall(asm_jar_of_wisps_initial_var_data)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.Pop(ASMPatch::Registers::EAX)
		.AddRelativeJump(caseEnd);
	sASMPatcher.PatchAt(addr, &varDataPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_JarOfWispsWispItem);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem Jar of Wisps wisp item at %p\n", addr);

	ASMPatch wispPatch;
	wispPatch.Push(ASMPatch::Registers::EAX) // player + 0x1760
		.Push(ASMPatch::Registers::ESI) // player
		.AddInternalCall(asm_jar_of_wisps_wisp_item)
		.AddBytes("\x85\xC0") // test eax, eax
		.Pop(ASMPatch::Registers::ECX) // player + 0x1760
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, addr + 0xAA)
		.AddBytes("\x8B\xC1") // mov eax, ecx
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // push COLLECTIBLE_JAR_OF_WISPS ; mov ecx, eax
		.AddRelativeJump(addr + 0x7); // call GetCollectibleRNG
	sASMPatcher.PatchAt(addr, &wispPatch);
}

static bool __stdcall asm_glitched_crown_pause_cycle()
{
	return repentogonOptions.vfGlitchedCrownPauseCycle;
}

// 1.9.7.11 keeps Glitched Crown items cycling while the game is paused.
static void vf_glitched_crown_pause_cycle()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PauseScreen_Update_GlitchedCrownCycle);
	ZHL::Log("[REPENTOGON] Patching PauseScreen::Update Glitched Crown cycle at %p\n", addr);

	// push 1 ; lea ecx, [edx + 0x1baa8] ; push ecx ; push COLLECTIBLE_GLITCHED_CROWN ; call FirstCollectibleOwner ; test eax, eax ; jz skip
	char* skip = addr + 0x17 + *(int8_t*)(addr + 0x16);

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EDX) // game
		.AddInternalCall(asm_glitched_crown_pause_cycle)
		.Pop(ASMPatch::Registers::EDX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, skip)
		.AddBytes(ByteBuffer().AddAny(addr, 0x8)) // push 1 ; lea ecx, [edx + 0x1baa8] // player manager
		.AddRelativeJump(addr + 0x8); // push ecx
	sASMPatcher.PatchAt(addr, &patch);
}

static bool __stdcall asm_abyss_rework()
{
	return VanillaAbyssRework();
}

static bool __stdcall asm_abyss_locust_hit(Entity_Familiar* familiar, Entity* collider)
{
	return VanillaAbyssLocustHit(familiar, collider);
}

static bool __stdcall asm_abyss_use(Entity_Player* player)
{
	return VanillaAbyssUse(player);
}

// 1.9.7.4 reworked Abyss. Many more items have unique locusts, and they're sized by quality.
static void vf_abyss_rework()
{
	char* addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_HandleCollision_AbyssLocustHit);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::HandleCollision Abyss locust hit at %p\n", addr);

	// cmp dword ptr [edi + 0x2c], 0xe7 ; jnz notLocust
	char* notLocust = addr + 0xD + *(int32_t*)(addr + 0x9);

	ASMPatch hitPatch;
	hitPatch.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp dword ptr [edi + 0x2c], FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, notLocust)
		.Push(ASMPatch::Registers::ESI) // collider
		.Push(ASMPatch::Registers::EDI) // familiar
		.AddInternalCall(asm_abyss_locust_hit)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, notLocust)
		.AddRelativeJump(addr + 0xD); // cmp dword ptr [edi + 0xd88], -1
	sASMPatcher.PatchAt(addr, &hitPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_HandleCollision_AbyssLocustSkatole);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::HandleCollision Abyss locust Skatole check at %p\n", addr);

	// cmp eax, 0xe7 ; jz noCollision
	char* noCollision = addr + 0xB + *(int32_t*)(addr + 0x7);

	ASMPatch skatolePatch;
	skatolePatch.AddBytes(ByteBuffer().AddAny(addr, 0x5)) // cmp eax, FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, addr + 0xB)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(asm_abyss_rework)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, noCollision)
		.AddRelativeJump(addr + 0xB); // mov eax, [ebp - 0xb4]
	sASMPatcher.PatchAt(addr, &skatolePatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_HandleCollision_AbyssLocustProjectile);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::HandleCollision Abyss locust projectile block at %p\n", addr);

	// cmp dword ptr [edi + 0x2c], 0xe7 ; jnz killProjectile
	char* killProjectile = addr + 0xD + *(int32_t*)(addr + 0x9);

	ASMPatch projectilePatch;
	projectilePatch.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp dword ptr [edi + 0x2c], FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, killProjectile)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(asm_abyss_rework)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, killProjectile)
		.AddRelativeJump(addr + 0xD); // push TEAR_SHIELDED
	sASMPatcher.PatchAt(addr, &projectilePatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_Init_AbyssLocustHungry);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::Init Abyss locust hungry sheet at %p\n", addr);

	char* notHungry = addr + 0xD + *(int32_t*)(addr + 0x9);

	ASMPatch hungryPatch;
	hungryPatch.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.Push(ASMPatch::Registers::EAX) // locust config
		.AddInternalCall(asm_abyss_rework)
		.AddBytes("\x84\xC0") // test al, al
		.Pop(ASMPatch::Registers::EAX)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, notHungry)
		.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // test dword ptr [eax + 0x18], 0x40000
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, notHungry)
		.AddRelativeJump(addr + 0xD); // mov eax, [g_Manager]
	sASMPatcher.PatchAt(addr, &hungryPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_Update_AbyssLocustQualityScale);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::Update Abyss locust quality scale at %p\n", addr);

	// cmp eax, 0xe7 ; jnz setScale // xmm1 = xmm2 = 1.0 here
	char* setScale = addr + 0x7 + *(int8_t*)(addr + 0x6);

	ASMPatch scalePatch;
	scalePatch.AddBytes(ByteBuffer().AddAny(addr, 0x5)) // cmp eax, FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, setScale)
		.AddBytes("\x83\xEC\x08") // sub esp, 8
		.AddBytes("\xF3\x0F\x11\x0C\x24") // movss [esp], xmm1
		.AddBytes("\xF3\x0F\x11\x54\x24\x04") // movss [esp + 4], xmm2
		.AddInternalCall(asm_abyss_rework)
		.AddBytes("\xF3\x0F\x10\x0C\x24") // movss xmm1, [esp]
		.AddBytes("\xF3\x0F\x10\x54\x24\x04") // movss xmm2, [esp + 4]
		.AddBytes("\x8D\x64\x24\x08") // lea esp, [esp + 8]
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, setScale)
		.AddRelativeJump(addr + 0x7); // push 0
	sASMPatcher.PatchAt(addr, &scalePatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPickup_HandleCollision_AbyssLocustHunger);
	ZHL::Log("[REPENTOGON] Patching Entity_Pickup::HandleCollision Abyss locust hunger at %p\n", addr);

	// cmp dword ptr [ecx + 0x2c], 0xe7 ; jnz noFamiliarPickup
	char* noFamiliarPickup = addr + 0xD + *(int32_t*)(addr + 0x9);

	ASMPatch hungerPatch;
	hungerPatch.AddBytes(ByteBuffer().AddAny(addr, 0x7)) // cmp dword ptr [ecx + 0x2c], FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, noFamiliarPickup)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(asm_abyss_rework)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, noFamiliarPickup)
		.AddRelativeJump(addr + 0xD); // call
	sASMPatcher.PatchAt(addr, &hungerPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityFamiliar_GetMultiplier_AbyssLocustHiveMind);
	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::GetMultiplier Abyss locust Hive Mind at %p\n", addr);

	// cmp edi, 0xe7 ; jz boosted
	char* boosted = addr + 0x8 + *(int8_t*)(addr + 0x7);

	ASMPatch hiveMindPatch;
	hiveMindPatch.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // cmp edi, FAMILIAR_ABYSS_LOCUST
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, addr + 0x8)
		.Push(ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(asm_abyss_rework)
		.Pop(ASMPatch::Registers::EDX)
		.Pop(ASMPatch::Registers::ECX)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, boosted)
		.AddRelativeJump(addr + 0x8); // movss xmm0, 1.0
	sASMPatcher.PatchAt(addr, &hiveMindPatch);

	addr = (char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlayer_UseActiveItem_AbyssUse);
	ZHL::Log("[REPENTOGON] Patching Entity_Player::UseActiveItem Abyss use at %p\n", addr);

	// mov edi, [ebp - 0x5c4] ; cmp dword ptr [edi + 0x2c], 0 ; jnz notMainPlayer
	char* notMainPlayer = addr + 0x10 + *(int32_t*)(addr + 0xC);
	char* caseEnd = notMainPlayer - 0x6; // mov esi, [ebp - 0x5d8]
	if (*(uint16_t*)caseEnd != 0xB58B) {
		ZHL::Log("[REPENTOGON] Unexpected Abyss case end at %p, skipping the Abyss use patch\n", caseEnd);
		return;
	}

	ASMPatch usePatch;
	usePatch.AddBytes(ByteBuffer().AddAny(addr, 0x6)) // mov edi, [ebp - 0x5c4] // player
		.Push(ASMPatch::Registers::EDI) // player
		.AddInternalCall(asm_abyss_use)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, caseEnd)
		.AddBytes(ByteBuffer().AddAny(addr + 0x6, 0x4)) // cmp dword ptr [edi + 0x2c], 0
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, notMainPlayer)
		.AddRelativeJump(addr + 0x10); // mov ecx, [edi + 0x17b4]
	sASMPatcher.PatchAt(addr, &usePatch);
}

void ASMVanillaFeatures()
{
	// 1.9.7.11
	vf_cod_worm_hiding();
	vf_kineti_on_screen();
	vf_blood_cultist_on_screen();
	vf_mega_fatty_red_puke_height();
	vf_amnesia_horse_pill_unidentify();
	vf_sharp_plug_bleeding_hearts();
	vf_wheel_of_fortune_crane_game();
	vf_punching_bag_block_shots();
	vf_meat_cleaver_angel_key_piece();
	vf_delirious_boss_rework();
	vf_mars_dash_rework();
	vf_jar_of_wisps_rework();
	vf_glitched_crown_pause_cycle();
	vf_abyss_rework();
}
