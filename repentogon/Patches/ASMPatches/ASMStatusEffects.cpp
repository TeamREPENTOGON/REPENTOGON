#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMStatusEffects.h"
#include "HookSystem.h"
#include "Log.h"

#include "ASMStatusEffects_Definitions.h"

/* /////////////////////
// shared GetStatusEffectTarget trampoline
*/ /////////////////////

Entity* __stdcall GetStatusEffectTargetTrampoline(Entity* target) {
	return target->GetStatusEffectTarget();
}

/* /////////////////////
// Generic inline patch
*/ /////////////////////

void ASMPatchInlinedGetStatusEffectTarget(void* addr, ASMPatch::Registers entityReg, ASMPatch::Registers targetReg, ASMPatch::SavedRegisters::Registers savedReg, unsigned int jumpOffset, std::optional<bool> preserveXMM1) {
	// XMM1 is saved because AddPoison has damage stored in it at the time of jmp, and the call ends up corrupting its value
	ASMPatch::SavedRegisters savedRegisters((ASMPatch::SavedRegisters::Registers::GP_REGISTERS |
		((preserveXMM1.has_value() && preserveXMM1.value()) ? ASMPatch::SavedRegisters::XMM1 : 0x0)) & ~savedReg, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(entityReg)
		.AddInternalCall(GetStatusEffectTargetTrampoline)
		.CopyRegister(targetReg, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}


/* /////////////////////
// Run patches
*/ /////////////////////

void PatchInlinedGetStatusEffectTarget()
{
	ZHL::Logger logger;
	for (const StatusEffectPatchInfo& i : patches) {
		SigScan scanner(i.signature);
		if (!scanner.Scan()) {
			logger.Log("GetStatusEffectTarget patch for %s failed, signature not found!\n", i.comment);
			continue;
		}
		void* addr = (char*)scanner.GetAddress() + i.sigOffset;

		logger.Log("Patching inlined GetStatusEffectTarget in %s at %p\n", i.comment, addr);
		ASMPatchInlinedGetStatusEffectTarget(addr, i.entityReg, i.targetReg, i.saveReg, i.jumpOffset, i.preserveXMM1);
	};
}

void ASMPatchAddWeakness() {
	SigScan scanner("576afd"); // this is the first push of args for ComputeStausEffectDuration
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EDI)  // Push entity
		.Push(ASMPatch::Registers::EBP, 0xc) // Push duration
		.AddBytes("\x8b\xce") // MOV ECX, ESI
		// this fits exactly in the 5 bytes uses to push arguments
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}