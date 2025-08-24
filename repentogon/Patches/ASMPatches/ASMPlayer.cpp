#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMPlayer.h"
#include "../XMLData.h"

#include "ASMDefinition.h"

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
	SigScan scanner("f30f1187????????8b87????????83f828");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* speedPtr = &PlayerStats::modCharacterSpeed;
	printf("[REPENTOGON] Patching EvaluateCache Speed at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\xF3\x0F\x58\x05").AddBytes(ByteBuffer().AddAny((char*)&speedPtr, 4)) // addss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddBytes(ByteBuffer().AddAny((char*)addr, 8)) // movss dword ptr [edi + 0x????], xmm0
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

void __stdcall RangeTrampoline(Entity_Player* player) {
	player->_tearrange += PlayerStats::modCharacterRange;
}
void ASMPatchRange() {
	SigScan scanner("83f80974??83f81e74");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching EvaluateCache Range at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS | ASMPatch::SavedRegisters::Registers::XMM_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI)
		.AddInternalCall(RangeTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x83\xF8\x09") // cmp eax, 0x9
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)addr + 0x30) // jz isaac-ng.XXXXXXXX
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
		.AddBytes("\xF3\x0F\x58\x06") // addss xmm0, dword ptr [esi]
		.AddBytes("\xF3\x0F\x11\x06") // movss dword ptr [esi], xmm0
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
		.AddBytes(ByteBuffer().AddAny((char*)addr, 8)) // addss xmm2, dword ptr [edi + 0x????]
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

int marsDoubleTapWindow = 10;
void __stdcall SetMarsDoubleTapWindow() {
	marsDoubleTapWindow = repentogonOptions.marsDoubleTapWindow;
}

void ASMPatchMarsDoubleTapWindow() {
	SigScan scanner("83bf????????0a7f"); // cmp dword ptr [EDI + XXXXXXXX],0xa
	scanner.Scan();
	void* addr = scanner.GetAddress();
	void* frameWindowPtr = &marsDoubleTapWindow;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)  // Store for later
		.AddInternalCall(SetMarsDoubleTapWindow)
		.AddBytes("\xA1").AddBytes(ByteBuffer().AddAny((char*)&frameWindowPtr, 4)) // mov eax, dword ptr ds:[XXXXXXXX]
		// Gets offset of mars framecount directly from the address for update resistance
		.AddBytes("\x39\x87").AddBytes(ByteBuffer().AddAny((char*)addr+0x2, 4)) // cmp dword ptr [EDI + 0xXXXXXXXX,EAX
		.RestoreRegisters(savedRegisters)
		// This code block is otherwise so simple that I don't expect this offset to ever change
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

// When AddActiveCharge is called with a negative value for the ActiveSlot, it iterates over each valid ActiveSlot and recursively calls AddActiveCharge for that slot.
// For some reason, when it does this, it passes the function's boolean params in the wrong order. From the user's perspective, this would cause the function to
// allow overcharge despite the bool for allowing overcharge being to false. This patch puts the params in the correct order.
//
// It is worth noting that there does not appear to be any place in the game's code where -1 is passed as the ActiveSlot while any of the booleans are also set to TRUE,
// so this mistake does not cause any vanilla bugs, and likewise fixing it should not have any effects to the basegame.
void ASMPatchAddActiveCharge() {
	SigScan scanner("ff75??8bcfff75??ff75??56");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	sASMPatcher.FlatPatch((char*)addr + 0x2, "\x18", 1);
	sASMPatcher.FlatPatch((char*)addr + 0x7, "\x14", 1);
	sASMPatcher.FlatPatch((char*)addr + 0xA, "\x10", 1);
}

unsigned int __stdcall GetPlayerHurtSound(Entity_Player* player) {
	int playerType = player->GetPlayerType();
	XMLAttributes playerData = XMLStuff.PlayerData->GetNodeById(playerType);

	const unsigned int defaultHurtSound = 55; // Default hurt grunt sound. Nicalis won't ever change it... right?

	if (playerData.count("hurtsound") == 0) {
		return defaultHurtSound; 
	}

	XMLAttributes soundData = XMLStuff.SoundData->GetNodeByName(playerData["hurtsound"]);

	if (soundData.count("id") == 0) {
		return defaultHurtSound;
	}
	else {
		return stoi(soundData["id"]);
	}
}

void ASMPatchPlayerHurtSound() {
	ASMPatch patch;
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Player_HurtSoundOverride);

	patch.Push(ASMPatch::Registers::EDI) // EntityPlayer*
		.AddInternalCall(GetPlayerHurtSound)
		.AddRelativeJump((char*)addr + 0x5); // Jump to next instruction (play sound)

	sASMPatcher.PatchAt(addr, &patch);
}