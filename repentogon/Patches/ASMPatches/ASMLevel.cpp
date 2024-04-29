#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMLevel.h"
#include "../../LuaInterfaces/Level.h"
#include "../../LuaInterfaces/Room/Room.h"
#include "../Stages/StageHandler.h"

#include "../XMLData.h"

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
bool __stdcall VoidGenerationOverride(RoomConfig* _this, std::vector<RoomConfig_Room*>* rooms, int type, int shape, int minVariant,
	int maxVariant, int minDifficulty, int maxDifficulty, unsigned int* doors, unsigned int subtype, int mode) {
	// we want to skip all this and let the game handle things if we're generating death certificate
	if (g_Game->GetDimension() != 2) {
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
			std::vector<RoomConfig_Room*> stageRooms = _this->GetRooms(i, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode);
			rooms->insert(rooms->begin(), stageRooms.begin(), stageRooms.end());
		}
		return true;
	}
	return false;
}

void ASMPatchVoidGeneration() {
	SigScan scanner("8b5d??be010000008b7d");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching void generation at %p\n", addr);

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::XMM0 | ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.PreserveRegisters(registers)
	.Push(ASMPatch::Registers::EBP, -0x38)
	.Push(ASMPatch::Registers::EBP, -0x68)
	.Push(ASMPatch::Registers::EBP, -0x40)
	.Push(0x14)
	.Push(ASMPatch::Registers::EBP, -0x70)
	.Push(ASMPatch::Registers::EBP, -0x74)
	.Push(ASMPatch::Registers::EBP, -0x78)
	.Push(ASMPatch::Registers::EBP, -0x7C)
	.Push(ASMPatch::Registers::EBP, -0x80)
	.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x64, ASMPatch::Registers::EBX)
	.Push(ASMPatch::Registers::EBX)
	.Push(ASMPatch::Registers::EBP, -0x30)
	.AddInternalCall(VoidGenerationOverride)
	.AddBytes("\x84\xC0") // test al, al
	.RestoreRegisters(registers)
	.AddBytes("\xc7\x45\x94\x14").AddZeroes(3)
	.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 103) // we handled the vector ourselves
	.AddBytes(ByteBuffer().AddAny((char*)addr, 0x8))  // Restore the commands we overwrote
	.AddRelativeJump((char*)addr + 0x8);

	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall SpawnSpecialQuestDoorValidStageTypeCheck() {
	// Always return true if we forced it via Room:TrySpawnSpecialQuestDoor
	bool ret = roomASM.ForceSpecialQuestDoor;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (!ret && levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
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

const char* sigs[2] = {
	"c74424??63000000c74424??00000000",
	"6affff7424??8bd0"
};

void PatchDealRoomVariant() {
	SigScan maxVariantScanner(sigs[0]); // maxVariant 0x63 (99)
	SigScan subTypePushScanner(sigs[1]); // PUSH -0x1, PUSH dword ptr[ESP + subtype]
	maxVariantScanner.Scan();
	subTypePushScanner.Scan();
	void* addrs[2] = { maxVariantScanner.GetAddress(), subTypePushScanner.GetAddress() };

	printf("[REPENTOGON] Patching InitializeDevilAngelRoom max variant at %p\n", (char*)addrs[0] + 8);
	ASMPatch patch1;
	patch1.AddBytes("\xFF\xFF\xFF\xFF"); // -1
	sASMPatcher.FlatPatch((char*)addrs[0] + 4, &patch1);

	printf("[REPENTOGON] Patching InitializeDevilAngelRoom subtype push %p\n", (char*)addrs[1] + 2);
	ASMPatch patch2;
	patch2.Push(-0x1) // mode
		.AddBytes("\x83\x7c\x24\x2C\x64") // cmp dword ptr ss:[EBP+minVariant],0x64 (100) 
		.AddBytes("\x75\x11") // jne 0x11 (DEFAULT)
		.AddBytes("\x83\x7c\x24\x30\x64") // cmp dword ptr ss:[EBP+maxVariant],0x64 (100)
		.AddBytes("\x75\x0a") // jne 0x0a (DEFAULT)
		.Push(0x29a) // 666
		.AddRelativeJump((char*)addrs[1] + 0x6)
		// DEFAULT
		.AddBytes("\xFF\x74\x24\x28") // push dword ptr ss:[EBP+subType] 
		.AddRelativeJump((char*)addrs[1] + 0x6);
	sASMPatcher.PatchAt((char*)addrs[1], &patch2);
}

void __stdcall AdjustLevelStageBackdrop(FXLayers* fxlayers) {
	int stage = g_Game->GetStageID(false);
	int backdrop = fxlayers->_backdropType;

	//printf("stage %d, overriden %d, id %d, token %s\n", stage, StageHandler::stageState[stage].overriden, StageHandler::stageState[stage].id, StageHandler::stageState[stage].token.empty() ? "EMPTY" : StageHandler::stageState[stage].token.c_str());

	if (StageHandler::stageState[stage].overriden) {
		fxlayers->_levelStage = StageHandler::stageState[stage].id;
		fxlayers->_stageType = 0;
	}
	if (XMLStuff.BackdropData->backdropState.first == backdrop) {
		fxlayers->_backdropType = XMLStuff.BackdropData->backdropState.second;
	}

	fxlayers->_averagePlayerPos = Vector(0, 0);
}

void ASMPatchFXLayersInit() {
	SigScan scanner("f30f1106f30f1005????????f30f1146??e8"); // this->_averagePlayerPos = g_VectorZero
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching FXLayers::Init at %p\n", addr);
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // FXLayers
		.AddInternalCall(AdjustLevelStageBackdrop)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xc);
	sASMPatcher.PatchAt(addr, &patch);
}