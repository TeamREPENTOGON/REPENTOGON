#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "HookSystem.h"

#include "Level.h"
#include "Room/RoomPlacement.h"

LevelASM levelASM;

static std::string CustomStageName;
static std::string returnedString;

static const char* ReturnString(const std::string& string) {
	returnedString = string;
	return returnedString.c_str();
}

MOD_EXPORT void L_Level_Update(Level* level) {
	level->Update();
}

MOD_EXPORT void L_Level_SetStage(Level* level, int stage, int stageType) {
	level->SetStage(stage, stageType);
}

MOD_EXPORT void L_Level_SetNextStage(Level* level) {
	level->SetNextStage();
}

MOD_EXPORT const char* L_Level_GetName(Level* level) {
	return ReturnString(level->GetName());
}

MOD_EXPORT const char* L_Level_GetCurseName(Level* level) {
	return ReturnString(level->GetCurseName());
}

MOD_EXPORT bool L_Level_CanStageHaveCurseOfLabyrinth(Level* level, int stage) {
	return level->CanStageHaveCurseOfLabyrinth(stage);
}

MOD_EXPORT void L_Level_ShowName(Level* level, bool sticky) {
	level->ShowName(sticky);
}

MOD_EXPORT bool L_Level_GetStateFlag(Level* level, unsigned int flag) {
	return ((Game*)level)->GetLevelStateFlag(flag);
}

MOD_EXPORT void L_Level_SetStateFlag(Level* level, unsigned int flag, bool value) {
	level->SetStateFlag(flag, value);
}

MOD_EXPORT int L_Level_GetRandomRoomIndex(Level* level, bool iAmErrorRoom, unsigned int seed) {
	return level->GetRandomRoomIndex(iAmErrorRoom, seed);
}

MOD_EXPORT int L_Level_GetNonCompleteRoomIndex(Level* level) {
	return level->GetNonCompleteRoomIndex();
}

MOD_EXPORT RoomDescriptor* L_Level_GetRoomByIdx(Level* level, int idx, int dimension) {
	return level->GetRoomByIdx(idx, dimension);
}

MOD_EXPORT RoomDescriptor* L_Level_GetCurrentRoomDesc(Level* level) {
	return ((Game*)level)->GetCurrentRoomDesc();
}

MOD_EXPORT RoomDescriptor* L_Level_GetLastRoomDesc(Level* level) {
	Game* game = (Game*)level;
	return game->GetRoomByIdx(game->_lastRoomIdx, game->_lastRoomDimensionIdx);
}

MOD_EXPORT int L_Level_QueryRoomTypeIndex(Level* level, int roomType, bool visited, RNG* rng, bool ignoreGroup) {
	return level->QueryRoomTypeIndex(roomType, visited, rng, ignoreGroup);
}

MOD_EXPORT bool L_Level_CanOpenChallengeRoom(Level* level, int roomIndex) {
	return level->CanOpenChallengeRoom(roomIndex);
}

MOD_EXPORT void L_Level_GetEnterPosition(Level* level, Vector* out) {
	level->GetEnterPosition(out);
}

MOD_EXPORT void L_Level_ChangeRoom(Level* level, int roomIndex, int dimension) {
	level->ChangeRoom(roomIndex, dimension);
}

MOD_EXPORT bool L_Level_ForceHorsemanBoss(Level* level, int seed) {
	return level->ForceHorsemanBoss(seed);
}

MOD_EXPORT int L_Level_GetAbsoluteStage(Level* level) {
	return level->GetAbsoluteStage();
}

MOD_EXPORT int L_Level_GetCurses(Level* level) {
	return level->GetCurses();
}

MOD_EXPORT void L_Level_UpdateVisibility(Level* level) {
	((Game*)level)->UpdateVisibility();
}

MOD_EXPORT void L_Level_ApplyMapEffect(Level* level) {
	level->ApplyMapEffect();
}

MOD_EXPORT void L_Level_ApplyBlueMapEffect(Level* level) {
	level->ApplyBlueMapEffect();
}

MOD_EXPORT void L_Level_ApplyCompassEffect(Level* level, bool persistent) {
	level->ApplyCompassEffect(persistent);
}

MOD_EXPORT void L_Level_RemoveCompassEffect(Level* level) {
	level->RemoveCompassEffect();
}

MOD_EXPORT void L_Level_ShowMap(Level* level) {
	level->ShowMap();
}

MOD_EXPORT void L_Level_AddCurse(Level* level, int curse, bool showName) {
	level->AddCurse(curse, showName);
}

MOD_EXPORT void L_Level_RemoveCurses(Level* level, int curses) {
	level->RemoveCurses(curses);
}

MOD_EXPORT bool L_Level_CanSpawnDevilRoom(Level* level) {
	return level->CanSpawnDevilRoom();
}

MOD_EXPORT void L_Level_InitializeDevilAngelRoom(Level* level, bool forceAngel, bool forceDevil) {
	level->InitializeDevilAngelRoom(forceAngel, forceDevil);
}

MOD_EXPORT void L_Level_UncoverHiddenDoor(Level* level, int roomIndex, int doorSlot) {
	level->UncoverHiddenDoor(roomIndex, doorSlot);
}

MOD_EXPORT bool L_Level_IsNextStageAvailable(Level* level) {
	return level->IsNextStageAvailable();
}

MOD_EXPORT float L_Level_GetPlanetariumChance(Level* level) {
	return ((Game*)level)->GetPlanetariumChance();
}

MOD_EXPORT bool L_Level_MakeRedRoomDoor(Level* level, int roomIndex, int doorSlot) {
	return level->MakeRedRoomDoor(roomIndex, doorSlot);
}

MOD_EXPORT bool L_Level_IsAscent(Level* level) {
	return level->IsAscent();
}

MOD_EXPORT bool L_Level_IsPreAscent(Level* level) {
	return level->IsPreAscent();
}

MOD_EXPORT void L_Level_SetRedHeartDamage(Level* level) {
	level->SetRedHeartDamage();
}

MOD_EXPORT bool L_Level_CanSpawnDoorOutline(Level* level, int roomIDX, unsigned int doorSlot) {
	return level->CanSpawnDoorOutline(roomIDX, doorSlot);
}

MOD_EXPORT bool L_Level_HasAbandonedMineshaft(Level* level) {
	return level->HasAbandonedMineshaft();
}

MOD_EXPORT bool L_Level_HasMirrorDimension(Level* level) {
	return level->HasMirrorDimension();
}

MOD_EXPORT bool L_Level_HasPhotoDoor(Level* level) {
	return level->HasPhotoDoor();
}

MOD_EXPORT void L_Level_SetName(const char* name) {
	CustomStageName = name;
}

MOD_EXPORT bool L_Level_IsStageAvailable(int levelStage, int stageType) {
	return Level::IsStageAvailable(levelStage, stageType);
}

MOD_EXPORT int L_Level_GetForceSpecialQuest() {
	return levelASM.ForceSpecialQuest;
}

MOD_EXPORT void L_Level_SetForceSpecialQuest(int quest) {
	levelASM.ForceSpecialQuest = quest;
}

MOD_EXPORT bool L_Level_PlaceRoom(Level* level, LevelGenerator_Room* room, RoomConfig_Room* config, unsigned int seed) {
	return ((Game*)level)->PlaceRoom(room, config, seed, 0);
}

/*
HOOK_METHOD(Level, IsAltPath, () -> bool) {
	bool ret = false;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
	}
	return ret;
}
*/

bool CheckQuest(Level* level, const int levelStage, const int expected, const int quest) {
	unsigned int stage;
	if (g_Game->_difficulty < 2 && levelASM.ForceSpecialQuest > -1) {
		if (stage = g_Game->_stage, 5 < stage - 1 || (g_Game->_levelStateFlags & 0x10000) == 0 && level->IsAltPath()) {
			return (levelASM.ForceSpecialQuest == quest || levelStage == expected || (levelStage == expected - 1 && (g_Game->_curses & 2) != 0));
		}
	}
	return false;
}

HOOK_METHOD(Level, HasMirrorDimension, () -> bool) {
	return CheckQuest(this, g_Game->_stage, 2, 1);
}

HOOK_METHOD(Level, HasAbandonedMineshaft, () -> bool) {
	return CheckQuest(this, g_Game->_stage, 4, 2);
}

/* HOOK_METHOD(Level, GetName, () -> std::string) {
	std::string name = super();
	if (!CustomStageName.empty()) {
		name = CustomStageName;
	}

	return name;
} */

HOOK_GLOBAL(GetLevelName, (std_string* result, uint32_t levelStage, uint32_t stageType, uint32_t curseMask, uint32_t unk1, uint32_t unk2, bool unk3) -> void, __cdecl) {
	super(result, levelStage, stageType, curseMask, unk1, unk2, unk3);
	if (!CustomStageName.empty()) {
		*result = CustomStageName;
	}
}

// Generates a determinstic seed for room placement based on the shape of the room and where we are trying to place it.
// This is very similar to what is done when the game creates red rooms.
static uint32_t GetRoomPlacementSeed(const int roomShape, const int x, const int y) {
	const uint32_t seed = (g_Game->_dungeonPlacementSeed + y * 929) * (x + roomShape);
	RNG rng;
	rng.SetSeed(seed, 35);
	return std::max(rng.Next(), 1u);
}

// If 0 or nil is passed, generate a seed.
static uint32_t ResolvePlacementSeed(uint32_t seed, const int roomShape, const int x, const int y) {
	return seed == 0 ? GetRoomPlacementSeed(roomShape, x, y) : seed;
}

MOD_EXPORT bool L_Level_CanPlaceRoom(int roomShape, int doorMask, int gridIndex, int dimension, bool allowMultipleDoors, bool allowSpecialNeighbors, bool allowNoNeighbors) {
	const XY coords = RoomIndexToCoords(gridIndex);
	return CanPlaceRoom(roomShape, doorMask, coords.x, coords.y, dimension, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors);
}

MOD_EXPORT RoomDescriptor* L_Level_TryPlaceRoom(RoomConfig_Room* roomConfig, int gridIndex, int dimension, unsigned int seed, bool allowMultipleDoors, bool allowSpecialNeighbors, bool allowNoNeighbors) {
	const XY coords = RoomIndexToCoords(gridIndex);
	seed = ResolvePlacementSeed(seed, roomConfig->Shape, coords.x, coords.y);
	return TryPlaceRoom(roomConfig, coords.x, coords.y, dimension, seed, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors);
}

MOD_EXPORT bool L_Level_CanPlaceRoomAtDoor(int roomShape, int doorMask, RoomDescriptor* roomDescToConnect, int doorSlot, bool allowMultipleDoors, bool allowSpecialNeighbors) {
	return CanPlaceRoomAtDoor(roomShape, doorMask, roomDescToConnect, doorSlot, allowMultipleDoors, allowSpecialNeighbors);
}

MOD_EXPORT RoomDescriptor* L_Level_TryPlaceRoomAtDoor(RoomConfig_Room* roomConfigToPlace, RoomDescriptor* roomDescToConnect, int doorSlot, unsigned int seed, bool allowMultipleDoors, bool allowSpecialNeighbors, bool* attempted) {
	*attempted = false;
	if (!roomDescToConnect || !roomDescToConnect->Data) {
		return nullptr;
	}

	// Find the target coordinates of this door, as we might use it to generate a seed.
	const DoorSourceTarget doorsourceTarget = GetDoorSourceTarget(roomDescToConnect->GridIndex, roomDescToConnect->Data->Shape, doorSlot, false);
	if (!doorsourceTarget.IsValid()) {
		return nullptr;
	}

	*attempted = true;
	seed = ResolvePlacementSeed(seed, roomConfigToPlace->Shape, doorsourceTarget.target.x, doorsourceTarget.target.y);
	return TryPlaceRoomAtDoor(roomConfigToPlace, roomDescToConnect, doorSlot, seed, allowMultipleDoors, allowSpecialNeighbors);
}

MOD_EXPORT int L_Level_FindValidRoomPlacementLocations(int roomShape, int doorMask, int dimension, bool allowMultipleDoors, bool allowSpecialNeighbors, int* out) {
	const std::set<int> validLocations = FindValidRoomPlacementLocations(roomShape, doorMask, dimension, allowMultipleDoors, allowSpecialNeighbors);
	if (out) {
		int i = 0;
		for (const int gridIndex : validLocations) {
			out[i++] = gridIndex;
		}
	}
	return (int)validLocations.size();
}

MOD_EXPORT int L_Level_GetNeighboringRooms(int gridIndex, int roomShape, int dimension, int* doorSlots, RoomDescriptor** rooms) {
	const std::map<int, RoomDescriptor*> neighbors = GetNeighboringRooms(gridIndex, roomShape, dimension);
	if (doorSlots && rooms) {
		int i = 0;
		for (const auto& [doorSlot, neighborDesc] : neighbors) {
			doorSlots[i] = doorSlot;
			rooms[i] = neighborDesc;
			i++;
		}
	}
	return (int)neighbors.size();
}