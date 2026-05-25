#include "HookSystem.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMLevel.h"
#include "../../LuaInterfaces/Level.h"
#include "../../LuaInterfaces/Room/Room.h"

std::bitset<36> generateLevels;

// Prevents Curse of the Lost in Blue Womb in addition to Curse of Darkness (Vanilla)
// changes evaluated bitmask from 0xfe to 0xfa
void ASMPatchBlueWombCurse() {
	SigScan scanner("8367??fe6a00");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Log("[REPENTOGON] Patching Level::Init() Curse evaluation for Blue Womb at %p\n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\x67\x0C\xFA");
	sASMPatcher.FlatPatch(addr, &patch);
}

bool IsFloorUnlocked(unsigned int stageId) {
	if ((stageId >= STB_UNUSED1 && stageId <= STB_ULTRA_GREED) || stageId >= STB_BACKWARDS) {
		return false;
	}
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
		for (int id = STB_BASEMENT; id < NUM_STB; ++id) {
			if (id == STB_THE_VOID || generateLevels.test(id - 1)) {
				//ZHL::Log("Adding stage id %d\n", id);
				int getRoomsSubtype = subtype;
				if (id == STB_THE_VOID && subtype == 0) {
					// Allow any subtype to be pulled from `26.The Void_ex.xml` 
					getRoomsSubtype = -1;
				}
				std::vector<RoomConfig_Room*> stageRooms = _this->GetRooms(id, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, getRoomsSubtype, mode);
				if (id == STB_THE_VOID && subtype == 0) {
					// Respect `generateLevels` for void_ex room subtypes.
					for (RoomConfig_Room* room : stageRooms) {
						if (room->Subtype == 0 || room->Subtype == STB_THE_VOID || (room->Subtype > 0 && room->Subtype < NUM_STB && generateLevels.test(room->Subtype - 1))) {
							rooms->push_back(room);
						}
					}
				} else {
					rooms->insert(rooms->begin(), stageRooms.begin(), stageRooms.end());
				}
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

	ZHL::Log("[REPENTOGON] Patching void generation at %p\n", addr);

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
				for (int id = STB_BASEMENT; id < NUM_STB; ++id) {
					if (generateLevels.test(id - 1))
						roomConfig->ResetRoomWeights(id, mode);
				}
			}
			generateLevels.reset();
		}
		
		for (int id = STB_BASEMENT; id < NUM_STB; ++id) {
			if (repentogonOptions.betterVoidGeneration) {
				if ((id >= STB_UNUSED1 && id <= STB_ULTRA_GREED) || id == STB_MORTIS || id == STB_HOME || !IsFloorUnlocked(id))
					continue;
			}
			else
			{
				// mimic default generation (except for letting void rooms be added)
				if ((id > STB_CHEST && id != STB_THE_VOID) || id == STB_BLUE_WOMB)
					continue;
			}
			generateLevels.set(id-1, true);
		}
	}

	super(rng);
}

bool IsVoidExRoom() {
	Room* room = g_Game->_room;
	if (room->_descriptor && room->_descriptor->Data) {
		RoomConfig_Room* roomConfig = room->_descriptor->Data;
		return roomConfig->StageId == STB_THE_VOID;
	}
	return false;
}
int GetVoidExRoomStageId() {
	if (IsVoidExRoom()) {
		RoomDescriptor* roomDesc = g_Game->_room->_descriptor;
		RoomConfig_Room* roomConfig = roomDesc->Data;
		if ((roomConfig->Subtype >= STB_BASEMENT && roomConfig->Subtype <= STB_CHEST) || (roomConfig->Subtype >= STB_DOWNPOUR && roomConfig->Subtype <= STB_HOME)) {
			return roomConfig->Subtype;
		} else {
			RNG rng;
			rng.SetSeed(roomDesc->DecorationSeed, 7);
			if (repentogonOptions.betterVoidGeneration && rng.RandomInt(3) == 0) {
				const int id = STB_DOWNPOUR + rng.RandomInt(1 + STB_CORPSE - STB_DOWNPOUR);
				if (IsFloorUnlocked(id)) {
					return id;
				}
			}
			return rng.RandomInt(STB_CHEST) + 1;
		}
	}
	return STB_THE_VOID;
}

// Interpret the subtype of void_ex rooms as an StbType, and load the backdrop/fx appropriate for that stage.
int __stdcall LoadBackdropGraphicsVoidExTrampoline(const int original) {
	if (IsVoidExRoom()) {
		return GetVoidExRoomStageId();
	}
	return original;
}
void PatchLoadBackdropGraphicsVoidEx(void* addr, const bool ecx) {
	ZHL::Log("[REPENTOGON] Patching Room::LoadBackdropGraphics for void_ex @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~(ecx ? ASMPatch::SavedRegisters::Registers::ECX : ASMPatch::SavedRegisters::Registers::EAX), true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.Push(ecx ? ASMPatch::Registers::ECX : ASMPatch::Registers::EAX)
		.AddInternalCall(LoadBackdropGraphicsVoidExTrampoline);
	if (ecx) {
		patch.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX);
	}
	patch.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}
int __stdcall RoomInitVoidExTrampoline(const bool unused) {
	if (IsVoidExRoom()) {
		return GetVoidExRoomStageId();
	}
	return g_Game->GetStageID(unused);
}
void PatchRoomInitVoidEx() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::RoomInit_VoidEx);

	ZHL::Log("[REPENTOGON] Patching Room::Init for void_ex @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::ECX)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(RoomInitVoidExTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}
int __stdcall PortalsVoidExTrampoline1(const int levelStage, bool* isNonBaseStageType) {
	if (g_Game->_stage == STAGE7) {
		int stage = g_Game->_room->GetRoomConfigStage();
		if (stage == STB_MINES || stage == STB_ASHPIT) {
			return STAGE2_1;
		} else if (stage == STB_MAUSOLEUM || stage == STB_GEHENNA) {
			return STAGE3_1;
		} else if (stage == STB_MORTIS || stage == STB_CORPSE) {
			return STAGE4_1;
		} else if (stage == STB_CHEST || stage == STB_DARK_ROOM) {
			*isNonBaseStageType = (stage == STB_CHEST);
			return STAGE5;
		} else if (stage == STB_BLUE_WOMB) {
			return STAGE4_1;
		} else if (stage >= STB_HOME) {
			*isNonBaseStageType = false;
			return STAGE1_1;
		}
	} else if (levelStage == STAGE4_3) {
		return STAGE4_1;
	} else if (levelStage == STAGE6) {
		return STAGE5;
	} else if (levelStage >= STAGE8) {
		*isNonBaseStageType = false;
		return STAGE1_1;
	}
	return levelStage;
}
void PatchPortalsVoidEx1() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Portals_VoidEx1);

	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_portal for void_ex @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EDX, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore overwritten bytes
		.CopyRegister(ASMPatch::Registers::ESI, ASMPatch::Registers::ESP)
		.PreserveRegisters(savedRegisters)
		.AddBytes("\x8D\x46\x4F")
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(PortalsVoidExTrampoline1)
		.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}
int __stdcall PortalsVoidExTrampoline2(const int stageType) {
	if (g_Game->_stage == STAGE7) {
		int stage = g_Game->_room->GetRoomConfigStage();
		if (stage == STB_DOWNPOUR) {
			return STAGETYPE_REPENTANCE;
		} else if (stage == STB_DROSS) {
			return STAGETYPE_REPENTANCE_B;
		}
	}
	return stageType;
}
void PatchPortalsVoidEx2() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Portals_VoidEx2);

	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_portal for void_ex @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0xA))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(PortalsVoidExTrampoline2)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}
void PatchPortalUnaliveSelf() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PortalUnaliveSelf);
	ZHL::Log("[REPENTOGON] Patching Entity_NPC::ai_portal self destruction @ %p\n", addr);
	sASMPatcher.FlatPatch((char*)addr, "\x90\x90\x90", 3);
}
void ASMPatchesForVoidExSubtype() {
	PatchLoadBackdropGraphicsVoidEx(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LoadBackdropGraphics_VoidEx1), true);
	PatchLoadBackdropGraphicsVoidEx(sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LoadBackdropGraphics_VoidEx2), false);
	PatchRoomInitVoidEx();
	PatchPortalsVoidEx1();
	PatchPortalsVoidEx2();
	PatchPortalUnaliveSelf();
}

// This function is used to determine Portal (enemy) spawns and certain floor-specific enemy replacements.
HOOK_METHOD(Room, GetRoomConfigStage, () -> int) {
	if (IsVoidExRoom()) {
		return GetVoidExRoomStageId();
	}
	return super();
}

// Used for a few things including hasFloorAlt skins.
HOOK_STATIC(Level, GetStageID, () -> int, _stdcall) {
	if (IsVoidExRoom()) {
		return GetVoidExRoomStageId();
	}
	return super();
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
	ZHL::Log("[REPENTOGON] Patching TrySpawnSpecialQuestDoor at %p\n", addr);
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
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int Stage, unsigned int Mode) -> void)
{
	super(Stage, Mode);

	if (Stage != 0)
	{
		return;
	}

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

	ZHL::Log("[REPENTOGON] Patching %s at %p\n", message, patchAddr);
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
	PatchDealRoomVariant("e8????????8bcf8943", "InitializeDevilAngelRoom");
	PatchDealRoomVariant("e8????????85c075??68????????6a00", "MakeRedRoomDoor call #1");
	PatchDealRoomVariant("e8????????85c00f85????????83fe01", "MakeRedRoomDoor call #2");
	PatchDealRoomVariant("e8????????85c00f85????????6aff", "MakeRedRoomDoor call #3");
}

// eliminates the checks that replaces the current OverrideData if it's not a miniboss RoomConfig_Room
void PatchOverrideDataHandling() {
	const char* signature[2] = { "74??83380075??8378??06", "8945??85c974??833900" };
	SigScan scanner(signature[0]); // jz 0x0061bcb2
	if (!scanner.Scan()) {
		ZHL::Log("[ERROR] Unable to find signature to patch OverrideData handling\n");
		return;
	}

	void* patchAddr = scanner.GetAddress();

	ZHL::Log("[REPENTOGON] Patching Level::load_room at %p\n", patchAddr);
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

	ZHL::Log("[REPENTOGON] Patching Room::RespawnEnemies at %p\n", patchAddr);
	ASMPatch patch2;
	patch2.AddBytes("\x0F\x45\xC1") // cmovnz eax, ecx
		.AddRelativeJump((char*)patchAddr + 0xe);
	sASMPatcher.FlatPatch(patchAddr, &patch2);
}

// See https://docs.google.com/spreadsheets/d/1Y9SUTWnsVTrc_0f1vSZzqK6zc-1qttDrCG5kpDLrTvA/
int __stdcall TryResizeEndroomDoorSlotFix(int* doorSlotsToCheck, int idx) {
	if (doorSlotsToCheck[2] != doorSlotsToCheck[0]) {
		// Need to fix the values for large rooms, which are populated wrong.
		for (int i = 1; i <= 5; i++) {
			std::swap(doorSlotsToCheck[i * 2], doorSlotsToCheck[i * 2 + 1]);
		}
	}
	// The game may attempt to read outside the bounds of the array with an index of 13.
	if (idx > 12) {
		idx = 12;
	}
	return doorSlotsToCheck[idx];
}
void PatchLevelGeneratorTryResizeEndroom() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LevelGenerator_TryResizeEndroom_Fix);
	const int espOffset = *(int*)((char*)addr + 0x3);

	ZHL::Log("[REPENTOGON] Patching LevelGenerator::try_resize_endroom at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EDX, true);
	ASMPatch patch;
	patch.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::ESP)
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)
		.LoadEffectiveAddress(ASMPatch::Registers::EDX, espOffset, ASMPatch::Registers::EDX)
		.Push(ASMPatch::Registers::EDX)
		.AddInternalCall(TryResizeEndroomDoorSlotFix)
		.CopyRegister(ASMPatch::Registers::EDX, ASMPatch::Registers::EAX)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x7);;
	sASMPatcher.PatchAt(addr, &patch);
}
