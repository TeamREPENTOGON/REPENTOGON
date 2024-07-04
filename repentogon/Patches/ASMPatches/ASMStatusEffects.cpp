#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMStatusEffects.h"
#include "HookSystem.h"
#include "Log.h"

struct StatusEffectPatchInfo {
	char* signature;
	char* funcName;
	int sigOffset;
	ASMPatch::Registers entityReg;
	ASMPatch::Registers targetReg;
	ASMPatch::SavedRegisters::Registers saveReg;
	unsigned int jumpOffset;
};

StatusEffectPatchInfo patches[5] = {
	{"e8????????84c074??8b8e????????85c974??0f1f44??008bf18b8e????????85c975??85d20f84????????8b86????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b92c0100003bc10f4cc8eb??f7da3bca0f4cca898e????????8d4c24??0f2805????????6a010f114424??6a000f57c0",
	"AddBaited", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},
	{"e8????????84c074??8b8e????????85c974??90",
	"AddBleeding", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x20},
	{"e8????????84c074??8b8e????????85c974??660f1f84??00000000",
	"AddBrimstoneMark", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x28},
	{"e8????????84c074??8b86????????85c074??0f1f80000000008bf08b86????????85c075??85d20f84????????8bbe????????8bc78b9e????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b9780000003bc10f4cc8eb??f7da3bca0f4cca8bc7898e????????2500100000",
	"AddBurn", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x26},
	{"8bf7e8????????84c074??8b8f????????85c974??8bf18b8e????????85c975??8b96",
	"AddCharmed", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x21},
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
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS - savedReg, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(entityReg)
		.AddInternalCall(GetStatusEffectTargetTrampoline)
		// poor man's mov reg,reg
		.Push(ASMPatch::Registers::EAX)
		.Pop(targetReg)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchInlinedGetStatusEffectTarget()
{
	ZHL::Logger logger;
	for (StatusEffectPatchInfo i : patches) {
		SigScan scanner(i.signature);
		scanner.Scan();
		void* addr = (char*)scanner.GetAddress() + i.sigOffset;

		logger.Log("Patching inlined GetStatusEffectTarget in %s at %p\n", i.funcName, addr);
		ASMPatchInlinedGetStatusEffectTarget(addr, i.entityReg, i.targetReg, i.saveReg, i.jumpOffset);
	};
}
