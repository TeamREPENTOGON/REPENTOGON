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

/* This function overrides the call to GetRandomRoom in InitDevilAngelRoom.
 * InitDevilAngelRoom is thiscall. stdcall will mirror the stack cleaning
 * convention, and, as we don't need to preserve ecx under thiscall, nothing
 * more is required.
 */
static RoomConfig_Room* __stdcall OverrideGetRandomRoom(RoomConfig* config, unsigned int seed, bool reduceWeight, 
	int stage, int roomType, int roomShape, unsigned int minVariant, int maxVariant, int minDifficulty, 
	int maxDifficulty, unsigned int* requiredDoors, unsigned int roomSubtype, int mode);

RoomConfig_Room* __stdcall OverrideGetRandomRoom(RoomConfig* config, unsigned int seed, bool reduceWeight, 
	int stage, int roomType, int roomShape, unsigned int minVariant, int maxVariant, int minDifficulty,
	int maxDifficulty, unsigned int* requiredDoors, unsigned int roomSubtype, int mode) {
	/* Hardcoded variant: 100 is used for trapdoor variants after Mom's Heart / It Lives. 
	 * Variant bounds are both set to 100 only when the game draws these specific variants. 
	 * In this case, do nothing, let the game proceeds as it would.
	 */
	if (minVariant == maxVariant && maxVariant == 100) {
		ZHL::Log("[DEBUG] Game naturally selected trapdoor variant when generating "
			"deal, let it proceed\n");
		return config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
	}

	/* Attempt to draw a random room up to 6 times, ignoring all bounds on variants. As soon as a room that 
	 * is not a trapdoor variant appears, return it.
	 * If we cannot draw a non trapdoor room in that many attempts, let the game proceed as it would normally,
	 * mimicking vanilla behavior.
	 */
	RoomConfig_Room* result = config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, 0, -1,
		minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
	for (int i = 0; result->Variant == 100 && i < 5; ++i) {
		ZHL::Log("[DEBUG] We drew a trapdoor variant when performing unbounded variant "
			"lookup. Try again (%d/5)\n", i + 1);
		result = config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, 0, -1,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
	}

	if (result->Variant == 100) {
		ZHL::Log("[WARN] Could not draw a non trapdoor variant, deferring to default parameters\n");
		return config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
	}
	else {
		ZHL::Log("[DEBUG] Drew variant %u\n", result->Variant);
		return result;
	}
}

void PatchDealRoomVariant() {
	const char* signature = "E825040D008BCF894310E82BFCFFFF"; // call GetRandomRoom
	SigScan scanner(signature);
	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch deal room variants\n");
		return;
	}

	void* patchAddr = scanner.GetAddress();

	printf("[REPENTOGON] Patching InitializeDevilAngelRoom at %p\n", patchAddr);
	ASMPatch patch;
	// ASMPatch::SavedRegisters registers(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	// patch.PreserveRegisters(registers);
	/* Registers need not be saved, as we are performing the equivalent of a 
	 * function call. Push ecx so the override function can access the current
	 * RoomConfig object.
	 */
	patch.Push(ASMPatch::Registers::ECX);
	patch.AddInternalCall(&OverrideGetRandomRoom);
	patch.AddRelativeJump((char*)patchAddr + 5);
	// patch.RestoreRegisters(registers);

	sASMPatcher.PatchAt(patchAddr, &patch);
}