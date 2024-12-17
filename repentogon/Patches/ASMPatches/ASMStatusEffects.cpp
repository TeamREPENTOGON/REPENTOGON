#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMStatusEffects.h"
#include "HookSystem.h"
#include "Log.h"

struct StatusEffectPatchInfo {
	char* signature;
	char* funcName;
	int sigOffset;
	ASMPatch::Registers entityReg; // pointer containing entity at the time of jmp
	ASMPatch::Registers targetReg; // register where entity pointer is expected to be after the patch
	ASMPatch::SavedRegisters::Registers saveReg; // its corresponding SavedRegisters::Register
	unsigned int jumpOffset;
};

// These all overwrite the call to (bool)GetStatusEffectTarget, calls the non-inlined function,
// puts the resulting Entity back into targetRegister, and jumps back to immediately after the inlined function
const StatusEffectPatchInfo patches[16] = {
	{"e8????????84c074??8b8f????????85c974??6690",
	"AddBaited", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},
	{"e8????????84c074??8b8f????????85c974??8bf18b8e????????85c975??85d20f84????????8b86????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b978000000",
	"AddBleeding", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
	{"e8????????84c074??8b8e????????85c974??660f1f84??00000000",
	"AddBrimstoneMark", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x28},
	{"e8????????84c074??8b86????????85c074??0f1f44??00",
	"AddBurn", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},
	{"e8????????84c074??8b8f????????85c974??0f1f4000",
	"AddCharmed", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},
	{"e8????????84c074??8b8e????????85c974??8bf18b8e????????85c975??8b8e",
	"AddConfusion", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
	{"e8????????84c074??8b8e????????85c974??8bf18b8e????????85c975??85ff",
	"AddFear", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
	{"e8????????84c074??8b8e????????85c974??0f1f40008bf18b8e????????85c975??85d2",
	"AddFreeze", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},
	{"e8????????84c074??8b8e????????85c974??0f1f84??00000000",
	"AddIce", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x27},
	{"e8????????84c074??8b86????????85c074??90",
	"AddKnockback", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x20},
	{"e8????????84c074??8b8f????????85c974??8bf18b8e????????85c975??85d20f84????????8b86????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b92c010000",
	"AddMagnetized", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
	{"e8????????84c074??8b8e????????85c974??0f1f40008bf18b8e????????85c975??85ff",
	"AddMidasFreeze", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},

	// this crashed on me once, but now i can't replicate it
	{"e8????????84c074??8b86????????85c074??0f1f80000000008bf0",
	"AddPoison", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x26},

	{"e8????????84c074??8b8f????????85c974??8bf18b8e????????85c975??8b8e",
	"AddShrink", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
	{"e8????????84c074??8b8e????????85c974??90",
	"AddSlowing", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x20},
	{"e8????????84c074??8b8e????????85c974??8bf18b8e????????85c975??85d2",
	"AddWeakness", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
};

/* /////////////////////
// shared GetStatusEffectTarget trampoline
*/ /////////////////////

Entity* __stdcall GetStatusEffectTargetTrampoline(Entity* target) {
	return target->GetStatusEffectTarget();
}

/* /////////////////////
// Generic inline patch
*/ /////////////////////

void ASMPatchInlinedGetStatusEffectTarget(void* addr, ASMPatch::Registers entityReg, ASMPatch::Registers targetReg, ASMPatch::SavedRegisters::Registers savedReg, unsigned int jumpOffset) {
	// XMM1 is saved because AddPoison has damage stored in it at the time of jmp, and the call ends up corrupting its value
	ASMPatch::SavedRegisters savedRegisters((ASMPatch::SavedRegisters::Registers::GP_REGISTERS | ASMPatch::SavedRegisters::XMM1) & ~savedReg, true);
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
		scanner.Scan();
		void* addr = (char*)scanner.GetAddress() + i.sigOffset;

		logger.Log("Patching inlined GetStatusEffectTarget in %s at %p\n", i.funcName, addr);
		ASMPatchInlinedGetStatusEffectTarget(addr, i.entityReg, i.targetReg, i.saveReg, i.jumpOffset);
	};
}