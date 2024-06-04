#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMEntityNPC.h"

void ASMPatchSpawnSelectedBaby() {
	SigScan scanner("8b4d??83c4086aff"); // this is the first push of args for ComputeStausEffectDuration
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.MoveFromMemory(ASMPatch::Registers::EBP, -0x38, ASMPatch::Registers::ECX)
		.AddBytes("\x83\xc4\x08")
		.Push(ASMPatch::Registers::EBX, 0x8) //death awaits
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}