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

StatusEffectPatchInfo patches[16] = {
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
	{"e8????????84c074??8b8e????????85c974??0f1f84??000000008bf18b8e????????85c975??8b8e????????8bc1",
	"AddConfusion", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x27},
	{"e8????????84c074??8b8e????????85c974??0f1f40008bf18b8e????????85c975??85db",
	"AddFear", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},
	{"e8????????84c074??8b8e????????85c974??0f1f40008bf18b8e????????85c975??85d2",
	"AddFreeze", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},
	{"e8????????84c074??8b8e????????85c974??0f1f84??000000008bf18b8e????????85c975??8b8e????????85d2",
	"AddIce", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x27},
	{"8bcee8????????84c074??8b86????????85c074??90",
	"AddKnockback", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x22},
	{"e8????????84c074??8b8e????????85c974??0f1f44??008bf18b8e????????85c975??85d20f84????????8b86????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b92c0100003bc10f4cc8eb??f7da3bca0f4cca898e????????8d4c24??0f2805????????6a010f114424??6a000f2805",
	"AddMagnetized", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},
	{"e8????????84c074??8b8e????????85c974??0f1f44??008bf18b8e????????85c975??85ff",
	"AddMidasFreeze", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},

	// this crashed on me once, but now i can't replicate it
	{"e8????????84c074??8b86????????85c074??0f1f80000000008bf08b86????????85c075??85d20f84????????8bbe????????8bc78b9e????????83e00183c8000f85????????3986????????0f8f????????8b8e????????85d27e??8d04??b9780000003bc10f4cc8eb??f7da3bca0f4cca8bc7898e????????83e040",
	"AddPoison", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x26},

	{"8bf7e8????????84c074??8b8f????????85c974??8bf18b8e????????85c975??8b8e",
	"AddShrink", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x21},
	{"e8????????84c074??8b8e????????85c974??0f1f00",
	"AddSlowing", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x22},
	{"e8????????84c074??8b8e????????85c974??8bf1",
	"AddWeakness", 0, ASMPatch::Registers::ECX, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1f},
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
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS + ASMPatch::SavedRegisters::XMM1 - savedReg, true);
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

/* /////////////////////
// Thank you, God bless you, and God bless the United States of America.
*/ /////////////////////

bool avoidIceCrash = false;
void __stdcall AvoidIceCrashHack() {
	avoidIceCrash = true;
}

void ASMPatchIcedKnockbackPreNPC() {
	SigScan scanner("6a0168c80000008d85");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.AddInternalCall(AvoidIceCrashHack)
		.AddBytes("\x6a\x01\x68\xc8").AddZeroes(3)
		.AddRelativeJump((char*)addr + 7);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchIcedKnockbackPrePlayer() {
	SigScan scanner("68c8000000508d4424");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.AddInternalCall(AvoidIceCrashHack)
		.AddBytes("\x68\xc8").AddZeroes(3)
		.AddRelativeJump((char*)addr + 5);
	sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD(Entity_NPC, HandleCollision, (Entity* collider, bool low) -> bool) {
	bool res = super(collider, low);
	avoidIceCrash = false;
	return res;
}

HOOK_METHOD(Entity_Player, HandleCollision, (Entity* collider, bool low) -> bool) {
	bool res = super(collider, low);
	avoidIceCrash = false;
	return res;
}

HOOK_METHOD(Entity, AddKnockback, (EntityRef& ref, const Vector& pushDirection, int duration, bool takeImpactDamage) -> void) {
	if (!avoidIceCrash)
		super(ref, pushDirection, duration, takeImpactDamage);
	else
	{
		Entity* entity = this;
		if (!IgnoreEffectFromFriendly(&ref)) {
			if (GetStatusEffectTarget()) {
				Entity* parent = this->_parent;
				Entity* otherEnt = parent;
				while (otherEnt = parent, otherEnt != nullptr) {
					entity = otherEnt;
					parent = otherEnt->_parent;
				}
			}
			if (duration != 0 && ((entity->_flags & 1) == 0 || (entity->_flags & 0x2000000000000) != 0)) {
				entity->_knockbackCountdown = duration;
				entity->_knockbackDirection = pushDirection;
				if (10.0f < entity->_mass && ((this->_flags & 0x2000000000000) == 0)) {
					float logMass = 1.0f / std::log10(entity->_mass);
					entity->_knockbackDirection *= logMass;
					int countdown = (int)(entity->_knockbackCountdown * logMass);
					if (countdown < 1)
						countdown = 1;
					entity->_knockbackCountdown = countdown;
				}
				entity->_flags |= 0x800000000000;
				if (takeImpactDamage)
					entity->_flags |= 0x1800000000000;
				entity->CopyStatusEffects();
			}
		}
	}
}

/* /////////////////////
// Run patches
*/ /////////////////////

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
	ASMPatchIcedKnockbackPreNPC();
	ASMPatchIcedKnockbackPrePlayer();
}
