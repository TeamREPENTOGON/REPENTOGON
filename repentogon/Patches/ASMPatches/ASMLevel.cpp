#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMLevel.h"
#include "../../LuaInterfaces/Level.h"
#include "../../LuaInterfaces/Room/Room.h"
#include "../Stages/StageManager.h"

#include "../XMLData.h"

std::bitset<36> generateLevels;

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
		// to include Void portal rooms
		maxDifficulty = (maxDifficulty == 15 ? 20 : maxDifficulty);
		for (int id = 0; id < 36; ++id) {
			if (generateLevels.test(id)) {
				//ZHL::Log("Adding stage id %d\n", id + 1);
				std::vector<RoomConfig_Room*> stageRooms = _this->GetRooms(id + 1, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode);
				rooms->insert(rooms->begin(), stageRooms.begin(), stageRooms.end());
			}
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

HOOK_METHOD(Level, generate_dungeon, (RNG* rng) -> void)
{
	if (this->_stage == 12)
	{
		if (generateLevels.any())
		{
			RoomConfig* roomConfig = &g_Game->_roomConfig;
			if (roomConfig != nullptr) {
				uint8_t mode = g_Game->IsGreedMode();
				for (int id = 1; id < 37; ++id) {
					if (generateLevels.test(id - 1))
						roomConfig->ResetRoomWeights(id, mode);
				}
			}
			generateLevels.reset();
		}
		
		for (int id = 1; id < 37; ++id) {
			if (repentogonOptions.betterVoidGeneration) {
				if ((id > 17 && id < 26) || id == 34 || id == 35 || !IsFloorUnlocked(id))
					continue;
			}
			else
			{
				// mimic default generation (except for letting void rooms be added)
				if ((id > 17 && id != 26) || id == 13)
					continue;
			}
			generateLevels.set(id-1, true);
		}
	}

	super(rng);
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

static const int TRAPDOOR_DEAL_SUBTYPE = 666;
// Change subtype of trapdoor Devil and Angel room, after they are loaded in RoomConfig
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int Stage, unsigned int Mode) -> bool)
{
	bool ret = super(Stage, Mode);

	if (Stage == 0)
	{
		//Change Trapdoor Subtype
		for (int roomType = 14; roomType < 16; roomType++)
		{
			unsigned int doors = 0;
			RoomConfigRoomPtrVector rooms = g_Game->_roomConfig.GetRooms(0, roomType, 13, 100, 100, 0, 20, &doors, 0, Mode);
			for (RoomConfig_Room* p : rooms)
			{
				p->Subtype = TRAPDOOR_DEAL_SUBTYPE;
			}
		}
	}

	return ret;
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
	 * In this case, change the requested subtype.
	 */
	if (roomType == 14 || roomType == 15) {
		if (minVariant == maxVariant && maxVariant == 100) {
			ZHL::Log("[DEBUG] Game naturally selected trapdoor variant when generating "
				"deal, let it proceed\n");
			return config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
				minDifficulty, maxDifficulty, requiredDoors, TRAPDOOR_DEAL_SUBTYPE, mode);
		}

		/* Attempt to draw a random room up to 6 times, ignoring all bounds on variants. As soon as a room that
		 * is not a trapdoor variant appears, return it.
		 * If we cannot draw a non trapdoor room in that many attempts, let the game proceed as it would normally,
		 * mimicking vanilla behavior.
		 */
		RoomConfig_Room* result = config->GetRandomRoom(seed, false, stage, roomType, roomShape, 0, -1,
			minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
		if (result != nullptr)
		{
			for (int i = 0; result->Variant == 100 && i < 5; ++i)
			{
				ZHL::Log("[DEBUG] We drew a trapdoor variant when performing unbounded variant "
					"lookup. Try again (%d/5)\n", i + 1);
				result = config->GetRandomRoom(seed, false, stage, roomType, roomShape, 0, -1,
					minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
			}

			if (result->Variant == 100)
			{
				ZHL::Log("[WARN] Could not draw a non trapdoor variant, deferring to default parameters\n");
			}
			else {
				ZHL::Log("[DEBUG] Drew variant %u\n", result->Variant);
				if (reduceWeight)
					result->Weight = std::max(1e-7f, result->Weight * .1f);
				return result;
			}
		}
		else
		{
			ZHL::Log("[WARN] Drew nothing! (probably from MakeRedRoomDoor, returning to default behavior)\n");
			return result;
		}
	}
	return config->GetRandomRoom(seed, reduceWeight, stage, roomType, roomShape, minVariant, maxVariant,
		minDifficulty, maxDifficulty, requiredDoors, roomSubtype, mode);
}

void PatchDealRoomVariant(char * signature, char* message) {
	SigScan scanner(signature);
	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch %s deal variants\n", message);
		return;
	}

	void* patchAddr = scanner.GetAddress();

	printf("[REPENTOGON] Patching %s at %p\n", message, patchAddr);
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

void ASMPatchDealRoomVariants()
{
	PatchDealRoomVariant("E825040D008BCF894310E82BFCFFFF", "InitializeDevilAngelRoom");
	PatchDealRoomVariant("e8????????85c075??68????????6a00", "MakeRedRoomDoor call #1");
	PatchDealRoomVariant("e8????????85c00f85????????83fe01", "MakeRedRoomDoor call #2");
	PatchDealRoomVariant("e8????????85c00f85????????6aff", "MakeRedRoomDoor call #3");
}

// eliminates the checks that replaces the current OverrideData if it's not a miniboss RoomConfig_Room
void PatchOverrideDataHandling() {
	const char* signature[2] = { "74??83380075", "8945??85c974??833900" };
	SigScan scanner(signature[0]); // jz 0x0061bcb2
	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch OverrideData handling\n");
		return;
	}

	void* patchAddr = scanner.GetAddress();

	printf("[REPENTOGON] Patching Level::load_room at %p\n", patchAddr);
	ASMPatch patch1;
	patch1.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)patchAddr + 0x135)
		.AddRelativeJump((char*)patchAddr + 0x11);

	sASMPatcher.FlatPatch(patchAddr, &patch1);

	SigScan scanner2(signature[1]); // mov dword ptr [EBP + data],EAX
	if (!scanner2.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch Room::RespawnEnemies OverrideData handling\n");
		return;
	}

	patchAddr = (char*)scanner2.GetAddress() + 5;

	printf("[REPENTOGON] Patching Room::RespawnEnemies at %p\n", patchAddr);
	ASMPatch patch2;
	patch2.AddBytes("\x0F\x45\xC1") // cmovnz eax, ecx
		.AddRelativeJump((char*)patchAddr + 0xe);
	sASMPatcher.FlatPatch(patchAddr, &patch2);
}

void __stdcall AdjustLevelStageBackdrop(FXLayers* fxlayers) {
	StageManager& stageManager = StageManager::GetInstance();
	int stage = g_Game->GetStageID(false);
	int backdrop = fxlayers->_backdropType;

	//printf("stage %d, overriden %d, id %d, token %s\n", stage, stageManager.stageState[stage].overriden, stageManager.stageState[stage].id, stageManager.stageState[stage].token.empty() ? "EMPTY" : stageManager.stageState[stage].token.c_str());

	if (stageManager.stageState[stage].overriden) {
		fxlayers->_levelStage = stageManager.stageState[stage].id + 4; // to counter dumb math later on in xml parsing
		fxlayers->_stageType = 1;
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

// https://docs.google.com/spreadsheets/d/1Y9SUTWnsVTrc_0f1vSZzqK6zc-1qttDrCG5kpDLrTvA/
void PatchTryResizeEndroomIncorrectDoorSlotsForLongWalls() {
	SigScan scanner("898424????????898c24????????898424????????898c24????????898424????????898c24????????898424????????898c24????????898424");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching LevelGenerator::try_resize_endroom at %p\n", addr);

	// Swap EAX and ECX when populating the DoorSlot array indexes 2~11.
	for (int i = 0; i < 10; i++) {
		void* subAddr = (char*)addr + ((7 * i) + 1);
		ASMPatch patch;
		patch.AddBytes((i % 2 == 0) ? "\x8C" : "\x84");
		sASMPatcher.FlatPatch(subAddr, &patch);
	}
}
void PatchTryResizeEndroomMissingLongThinRoomDoorSlot() {
	SigScan scanner("8b94??????????b801000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching LevelGenerator::try_resize_endroom at %p\n", addr);

	// If we attempt to read from index 13 in the DoorSlot array (which seems to be reading outside the bounds of the array) read from index 12 instead.
	ASMPatch patch;
	patch.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::ESI)
		.AddBytes("\x83\xfa\x0d")  // cmp edx, 13
		.AddBytes("\x0f\x85\x01").AddZeroes(3)  // jne (skips the next command if edx != 13)
		.AddBytes("\x4a")  // dec edx (skipped if the jump happened)
		.AddBytes("\x8B\x94\x94\xE0").AddZeroes(3)  // mov edx, dword ptr [esp + edx*0x4 + 0xe0]
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchLevelGeneratorTryResizeEndroom() {
	PatchTryResizeEndroomIncorrectDoorSlotsForLongWalls();
	PatchTryResizeEndroomMissingLongThinRoomDoorSlot();
}
