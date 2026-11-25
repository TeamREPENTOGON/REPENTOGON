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

void ASMVanillaFeatures()
{
	vf_cod_worm_hiding();
}
