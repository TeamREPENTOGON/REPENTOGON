#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMPlayer.h"
#include "../XMLData.h"

thread_local CheckFamiliarStorage familiarsStorage;

void __stdcall CheckFamiliar_Internal(Entity_Familiar* familiar) {
	familiar->Update();

	if (familiarsStorage.inUse) {
		familiarsStorage.familiars.push_back(familiar);
	}

	return;
}

void ASMPatchCheckFamiliar() {
	SigScan scanner("8b06ff50??8b75"); // this is immediately before the call to Update()
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // Push the familiar spawned
		.AddInternalCall(CheckFamiliar_Internal)
		.RestoreRegisters(savedRegisters)
		// this should neatly fit in the 5 bytes used to call Update, and we handle calling it there, nothing to restore
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

//////////////////////////////////////////////
// !!!!! EVALUATEITEMS STATS HERE !!!!!
//////////////////////////////////////////////

void ASMPatchFireDelay() {
	SigScan scanner("83fa0974??83fa1e");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* fireDelayPtr = &PlayerStats::modCharacterFireDelay;
	printf("[REPENTOGON] Patching EvaluateCache FireDelay at %p\n", addr);
	ASMPatch patch;

	// Override Eden's calculation.
	// Move our stat into XMM0 and add to XMM2, reimplement the overwritten Eden check and jump.
	patch.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&fireDelayPtr, 4)) // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x58\xD0") // addss xmm2, xmm0
		.AddBytes("\x83\xFA\x09") // cmp edx, 0x9
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0xA) // jz isaac-ng.XXXXXXXX
		.AddRelativeJump((char*)addr + 0x5); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchSpeed() {
	SigScan scanner("f30f1187????????8b87????????83f829");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* speedPtr = &PlayerStats::modCharacterSpeed;
	printf("[REPENTOGON] Patching EvaluateCache Speed at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x58\x05").AddBytes(ByteBuffer().AddAny((char*)&speedPtr, 4)) // addss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x11\x87\xAC\x14").AddZeroes(2) // movss dword ptr [edi + 0x14ac], xmm0
		.AddRelativeJump((char*)addr + 0x8); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchDamage() {
	SigScan scanner("83f90974??83f91e74??f30f1080");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* damagePtr = &PlayerStats::modCharacterDamage;
	printf("[REPENTOGON] Patching EvaluateCache Damage at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&damagePtr, 4)) // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x58\x06") // addss xmm0, dword ptr [esi]
		.AddBytes("\xF3\x0F\x11\x06") // movss dword ptr [esi], xmm0
		.AddBytes("\x83\xF9\x09") // cmp this, 0x9
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x14) // jz isaac-ng.XXXXXXXX
		.AddRelativeJump((char*)addr + 0x5);  // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchRange() {
	SigScan scanner("83f80974??83f81e74");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* rangePtr = &PlayerStats::modCharacterRange;
	printf("[REPENTOGON] Patching EvaluateCache Range at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&rangePtr, 4)) // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x58\x87\xC8\x13").AddZeroes(2) // addss xmm0, dword ptr [edi + 0x13c8]
		.AddBytes("\xF3\x0F\x11\x87\xC8\x13").AddZeroes(2) // movss dword ptr [edi + 0x13c8], xmm0
		.AddBytes("\x83\xF8\x09") // cmp eax, 0x9
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x48) // jz isaac-ng.XXXXXXXX
		.AddRelativeJump((char*)addr + 0x5);  // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchShotSpeed() {
	SigScan scanner("83f90974??83f91e74??83f913");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* shotSpeedPtr = &PlayerStats::modCharacterShotSpeed;
	printf("[REPENTOGON] Patching EvaluateCache Shot Speed at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&shotSpeedPtr, 4)) // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x58\x07") // addss xmm0, dword ptr [edi]
		.AddBytes("\xF3\x0F\x11\x07") // movss dword ptr [edi], xmm0
		.AddBytes("\x83\xF9\x09") // cmp this, 0x9
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x30) // jz isaac-ng.XXXXXXXX
		.AddRelativeJump((char*)addr + 0x5);  // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchLuck() {
	SigScan scanner("f30f5897????????f30f58d1");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* luckPtr = &PlayerStats::modCharacterLuck;
	printf("[REPENTOGON] Patching EvaluateCache Luck at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x58\x15").AddBytes(ByteBuffer().AddAny((char*)&luckPtr, 4)) // addss xmm2, dword ptr ds:[0xXXXXXXXX]
		.AddBytes("\xF3\x0F\x58\x97\xA4\x14").AddZeroes(2) // addss xmm2, dword ptr [edi + 0x14a4]
		.AddRelativeJump((char*)addr + 0x8);  // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchPlayerStats() {
	ASMPatchFireDelay();
	ASMPatchSpeed();
	ASMPatchDamage();
	ASMPatchRange();
	ASMPatchShotSpeed();
	ASMPatchLuck();
}

//////////////////////////////////////////////
// !!!!! EVALUATEITEMS STATS DONE !!!!!
//////////////////////////////////////////////