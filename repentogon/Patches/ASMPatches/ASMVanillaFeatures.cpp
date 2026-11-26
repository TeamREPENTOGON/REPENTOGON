#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../../REPENTOGONOptions.h"

#include "ASMVanillaFeatures.h"

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
}
