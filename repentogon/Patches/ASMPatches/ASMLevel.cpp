#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMLevel.h"
#include "../../LuaInterfaces/Level.h"
#include "../../LuaInterfaces/Room/Room.h"

// Prevents Curse of the Lost in Blue Womb in addition to Curse of Darkness (Vanilla)
// changes evaluated bitmask from 0xfe to 0xfa
void ASMPatchBlueWombCurse() {
	SigScan scanner("8367??fe6a00");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Level::Init() Curse evaluation for Blue Womb at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\x67\x0C\xFA");
	sASMPatcher.FlatPatch(addr, &patch);
}

bool IsFloorUnlocked(unsigned int stageId) {
	int achievement = stageidToAchievement[stageId];
	if (achievement != -1) {
		PersistentGameData* pgd = &g_Manager->_persistentGameData;
		return pgd->Unlocked(achievement);
	}
	return true;
}
// The void now draws from all floors
void __stdcall VoidGenerationOverride(RoomConfigHolder* _this, std::vector<RoomConfig*>* rooms, int type, int shape, int minVariant,
	int maxVariant, int minDifficulty, int maxDifficulty, unsigned int* doors, unsigned int subtype, int mode) {
	for (int i = 1; i < 37; ++i) {

		if (!repentogonOptions.betterVoidGeneration && (i > 17 && i != 26) && i != 13)
			continue;
		else if (repentogonOptions.betterVoidGeneration && ((i > 17 && i < 26) || i == 34 || i == 35 || !IsFloorUnlocked(i)))
			continue;

		// Configure the subtype here because backwards uses a 
		// specific set of subtypes. I'll need the RNG object to 
		// make this seed dependant.
		/* if (i == 36) {

		} */
		std::vector<RoomConfig*> stageRooms = _this->GetRooms(i, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode);
		rooms->insert(rooms->begin(), stageRooms.begin(), stageRooms.end());
	}
}

void ASMPatchVoidGeneration() {
	SigScan scanner("83f82374758b5dd4be010000008b7dd0");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching void generation at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::XMM0 | ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers);
	patch.Push(ASMPatch::Registers::EBP, -0x38);
	patch.Push(ASMPatch::Registers::EBP, -0x68);
	patch.Push(ASMPatch::Registers::EBP, -0x40);
	patch.Push(0x14);
	patch.Push(ASMPatch::Registers::EBP, -0x70);
	patch.Push(ASMPatch::Registers::EBP, -0x74);
	patch.Push(ASMPatch::Registers::EBP, -0x78);
	patch.Push(ASMPatch::Registers::EBP, -0x7C);
	patch.Push(ASMPatch::Registers::EBP, -0x80);
	// patch.Push(ASMPatch::Registers::EBP, -0x64);
	patch.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x64, ASMPatch::Registers::EBX);
	patch.Push(ASMPatch::Registers::EBX);
	patch.Push(ASMPatch::Registers::EBP, -0x30);
	patch.AddInternalCall(VoidGenerationOverride);
	patch.RestoreRegisters(registers);
	// ASMPatch::ByteBuffer mov;
	// mov.AddString("c7459414").AddZeroes(3);
	patch.AddBytes("\xc7\x45\x94\x14").AddZeroes(3);
	// patch.AddBytes(mov);
	patch.AddRelativeJump((char*)addr + 108);

	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall SpawnSpecialQuestDoorValidStageTypeCheck() {
	// Always return true if we forced it via Room:TrySpawnSpecialQuestDoor
	bool ret = roomASM.ForceSpecialQuestDoor;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (!ret && levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_level._stageType == 4 || g_Game->_level._stageType == 5);
	}
	roomASM.ForceSpecialQuestDoor = false;
	return ret;
}

void ASMPatchTrySpawnSpecialQuestDoor() {
	SigScan scanner("83f90474??83f9050f85????????83fb02");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching TrySpawnSpecialQuestDoor at %p\n", addr);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(SpawnSpecialQuestDoorValidStageTypeCheck)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0xe)
		.AddRelativeJump((char*)addr + 0xb7);
	sASMPatcher.PatchAt(addr, &patch);
}


/* // Bring this back after merge
bool __stdcall SpecialQuestValidStageTypeCheck() {
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		return levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
	}
	return false;
}

void ASMPatchGenericIsAltPath(void* addr, int condJumpOffset, int jumpOffset) {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(SpecialQuestValidStageTypeCheck)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + condJumpOffset)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}
*/

void PatchSpecialQuest() {
	ASMPatchTrySpawnSpecialQuestDoor();
}