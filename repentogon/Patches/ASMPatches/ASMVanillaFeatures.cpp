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

void ASMVanillaFeatures()
{
	// 1.9.7.11
	vf_cod_worm_hiding();
	vf_kineti_on_screen();
	vf_blood_cultist_on_screen();
	vf_mega_fatty_red_puke_height();
	vf_amnesia_horse_pill_unidentify();
}
