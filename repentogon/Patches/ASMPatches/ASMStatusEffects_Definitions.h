#pragma once

struct StatusEffectPatchInfo {
	char* asmdef;
	char* comment;
	int sigOffset;
	ASMPatch::Registers entityReg; // pointer containing entity at the time of jmp
	ASMPatch::Registers targetReg; // register where entity pointer is expected to be after the patch
	ASMPatch::SavedRegisters::Registers saveReg; // its corresponding SavedRegisters::Register
	unsigned int jumpOffset;
	std::optional<bool> preserveXMM1; // for AddPoison
};

// These all overwrite the call to DoesEntityShareStatus, calls the non-inlined function,
// puts the resulting Entity back into targetRegister, and jumps back to immediately after the inlined function
// (this is usually test edx, edx with edx holding the calculated duration)
static const std::vector<StatusEffectPatchInfo> patches = {
	{&AsmDefinitions::InlinedGetStatusEffectTarget_Baited,
	"AddBaited", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x21},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Bleeding,
	"AddBleeding", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_BrimstoneMark,
	"AddBrimstoneMark", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x28},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Burn,
	"AddBurn", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x24},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Charmed,
	"AddCharmed", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Confusion,
	"AddConfusion", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Fear,
	"AddFear", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Freeze,
	"AddFreeze", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Ice,
	"AddIce", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x27},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Knockback,
	"AddKnockback", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x20},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Magnetized,
	"AddMagnetized", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_MidasFreeze,
	"AddMidasFreeze", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x23},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Poison,
	"AddPoison", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x26, true},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Shrink,
	"AddShrink", 0, ASMPatch::Registers::EDI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x26},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Slowing,
	"AddSlowing", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x20},

	{&AsmDefinitions::InlinedGetStatusEffectTarget_Weakness,
	"AddWeakness", 0, ASMPatch::Registers::ESI, ASMPatch::Registers::ESI, ASMPatch::SavedRegisters::Registers::ESI, 0x1F},
};