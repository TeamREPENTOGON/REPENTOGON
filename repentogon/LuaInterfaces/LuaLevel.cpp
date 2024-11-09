#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "Level.h"
#include "LuaEntitySaveState.h"
#include "Room/RoomPlacement.h"

LevelASM levelASM;

LUA_FUNCTION(Lua_LevelCanSpawnDoorOutline) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	int roomIDX = (int)luaL_checkinteger(L, 2);
	unsigned int doorSlot = (unsigned int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, level->CanSpawnDoorOutline(roomIDX, doorSlot));
	return 1;
}

LUA_FUNCTION(Lua_LevelHasAbandonedMineshaft)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasAbandonedMineshaft());

	return 1;
}

LUA_FUNCTION(Lua_LevelHasMirrorDimension)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasMirrorDimension());

	return 1;
}

LUA_FUNCTION(Lua_LevelHasPhotoDoor)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasPhotoDoor());

	return 1;
}

static std::string CustomStageName;

LUA_FUNCTION(lua_LevelSetName) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	const char* name = luaL_checkstring(L, 2);
	CustomStageName = name;
	return 0;
}

LUA_FUNCTION(lua_LevelSetGreedWavesClearedWithoutRedHeartDamage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	level->_greedwavesclearedwithoutredheartdamage = (uint32_t)luaL_checkinteger(L, 2);
	return 0;
}
LUA_FUNCTION(lua_LevelGetGreedWavesClearedWithoutRedHeartDamage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->_greedwavesclearedwithoutredheartdamage);
	return 1;
}

LUA_FUNCTION(lua_LevelIsStageAvailable) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	int levelStage = (int)luaL_checkinteger(L, 2);
	int stageType = (int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, level->IsStageAvailable(levelStage, stageType));
	return 1;
}

LUA_FUNCTION(Lua_GetDimension) {
	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->GetDimension());
	return 1;
}

LUA_FUNCTION(Lua_GetForceSpecialQuest) {
	lua_pushinteger(L, (int)levelASM.ForceSpecialQuest);
	return 1;
}

LUA_FUNCTION(Lua_SetForceSpecialQuest) {
	levelASM.ForceSpecialQuest = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetMyosotisPickups) {
	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	Lua_EntitiesSaveStateVector* ud = lua::place<Lua_EntitiesSaveStateVector>(L, lua::metatables::EntitiesSaveStateVectorMT);
	ud->data = (&level->_myosotisPickups);
	return 1;
}

LUA_FUNCTION(Lua_LevelIsAltPath) {
	bool ret = false;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
	}
	lua_pushboolean(L, ret);
	return 1;
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
		if (stage = g_Game->_stage, 5 < stage - 1 || (g_Game->_stateFlags & 0x10000) == 0 && level->IsAltPath()) {
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

LUALIB_API int LuaCheckDimension(lua_State* L, int arg) {
	lua_Integer dimension = luaL_optinteger(L, arg, -1);
	if (dimension < -1 || dimension > 2) {
		return luaL_argerror(L, arg, "Invalid Dimension");
	}
	return (int)dimension;
}

// Generates a determinstic seed for room placement based on the shape of the room and where we are trying to place it.
// This is very similar to what is done when the game creates red rooms.
uint32_t GetRoomPlacementSeed(const int roomShape, const int x, const int y) {
	const uint32_t seed = (g_Game->_dungeonPlacementSeed + y * 929) * (x + roomShape);
	RNG rng;
	rng.SetSeed(seed, 35);
	return std::max(rng.Next(), 1u);
}

// Validates room placement seed passed from lua. If 0 or nil is passed, generate a seed.
LUALIB_API uint32_t LuaCheckRoomPlacementSeed(lua_State* L, int arg, const int roomShape, const int x, const int y) {
	lua_Integer inputSeed = luaL_optinteger(L, arg, 0);
	if (inputSeed < 0) {
		luaL_argerror(L, arg, "Invalid Seed");
	}
	else if (inputSeed == 0) {
		return GetRoomPlacementSeed(roomShape, x, y);
	}
	return (uint32_t)inputSeed;
}

LUA_FUNCTION(Lua_LevelCanPlaceRoom) {
	int stackIdx = 2;

	int roomShape = 0;
	int doorMask = 0;

	if (lua_type(L, stackIdx) == LUA_TUSERDATA) {
		RoomConfig_Room* roomConfig = lua::GetUserdata<RoomConfig_Room*>(L, stackIdx++, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
		if (roomConfig) {
			roomShape = roomConfig->Shape;
			doorMask = roomConfig->Doors;
		}
	}
	else {
		roomShape = (int)luaL_checkinteger(L, stackIdx++);
		doorMask = (int)luaL_optinteger(L, stackIdx++, -1);
	}

	const int gridIndex = (int)luaL_checkinteger(L, stackIdx++);
	if (gridIndex < 0 || gridIndex > 168) {
		lua_pushboolean(L, false);
		return 1;
	}
	const XY coords = RoomIndexToCoords(gridIndex);
	const int dimension = LuaCheckDimension(L, stackIdx++);
	const bool allowMultipleDoors = lua::luaL_optboolean(L, stackIdx++, true);
	const bool allowSpecialNeighbors = lua::luaL_optboolean(L, stackIdx++, false);
	const bool allowNoNeighbors = lua::luaL_optboolean(L, stackIdx++, false);
	const bool result = CanPlaceRoom(roomShape, doorMask, coords.x, coords.y, dimension, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors);
	lua_pushboolean(L, result);
	return 1;
}

LUA_FUNCTION(Lua_LevelTryPlaceRoom) {
	RoomConfig_Room* roomConfig = lua::GetUserdata<RoomConfig_Room*>(L, 2, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	const int gridIndex = (int)luaL_checkinteger(L, 3);
	if (gridIndex < 0 || gridIndex > 168) {
		lua_pushboolean(L, false);
		return 1;
	}
	const XY coords = RoomIndexToCoords(gridIndex);
	const int dimension = LuaCheckDimension(L, 4);
	const uint32_t seed = LuaCheckRoomPlacementSeed(L, 5, roomConfig->Shape, coords.x, coords.y);
	const bool allowMultipleDoors = lua::luaL_optboolean(L, 6, true);
	const bool allowSpecialNeighbors = lua::luaL_optboolean(L, 7, false);
	const bool allowNoNeighbors = lua::luaL_optboolean(L, 8, false);
	RoomDescriptor* newRoom = TryPlaceRoom(roomConfig, coords.x, coords.y, dimension, seed, allowMultipleDoors, allowSpecialNeighbors, allowNoNeighbors);
	if (newRoom) {
		lua::luabridge::UserdataPtr::push(L, newRoom, lua::GetMetatableKey(lua::Metatables::ROOM_DESCRIPTOR));
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_LevelCanPlaceRoomAtDoor) {
	int stackIdx = 2;

	int roomShape = 0;
	int doorMask = 0;

	if (lua_type(L, stackIdx) == LUA_TUSERDATA) {
		RoomConfig_Room* roomConfig = lua::GetUserdata<RoomConfig_Room*>(L, stackIdx++, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
		if (roomConfig) {
			roomShape = roomConfig->Shape;
			doorMask = roomConfig->Doors;
		}
	}
	else {
		roomShape = (int)luaL_checkinteger(L, stackIdx++);
		doorMask = (int)luaL_optinteger(L, stackIdx++, -1);
	}

	RoomDescriptor* roomDescToConnect = lua::GetUserdata<RoomDescriptor*>(L, stackIdx++, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	const int doorSlot = (int)luaL_checkinteger(L, stackIdx++);
	const bool allowMultipleDoors = lua::luaL_optboolean(L, stackIdx++, true);
	const bool allowSpecialNeighbors = lua::luaL_optboolean(L, stackIdx++, false);
	const bool result = CanPlaceRoomAtDoor(roomShape, doorMask, roomDescToConnect, doorSlot, allowMultipleDoors, allowSpecialNeighbors);
	lua_pushboolean(L, result);
	return 1;
}

LUA_FUNCTION(Lua_LevelTryPlaceRoomAtDoor) {
	RoomConfig_Room* roomConfigToPlace = lua::GetUserdata<RoomConfig_Room*>(L, 2, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	RoomDescriptor* roomDescToConnect = lua::GetUserdata<RoomDescriptor*>(L, 3, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	if (!roomDescToConnect || !roomDescToConnect->Data) {
		lua_pushboolean(L, false);
		return 1;
	}
	const int doorSlot = (int)luaL_checkinteger(L, 4);

	// Find the target coordinates of this door, as we might use it to generate a seed.
	const DoorSourceTarget doorsourceTarget = GetDoorSourceTarget(roomDescToConnect->GridIndex, roomDescToConnect->Data->Shape, doorSlot, false);
	if (!doorsourceTarget.IsValid()) {
		lua_pushboolean(L, false);
		return 1;
	}
	const uint32_t seed = LuaCheckRoomPlacementSeed(L, 5, roomConfigToPlace->Shape, doorsourceTarget.target.x, doorsourceTarget.target.y);

	const bool allowMultipleDoors = lua::luaL_optboolean(L, 6, true);
	const bool allowSpecialNeighbors = lua::luaL_optboolean(L, 7, false);
	RoomDescriptor* newRoom = TryPlaceRoomAtDoor(roomConfigToPlace, roomDescToConnect, doorSlot, seed, allowMultipleDoors, allowSpecialNeighbors);
	if (newRoom) {
		lua::luabridge::UserdataPtr::push(L, newRoom, lua::GetMetatableKey(lua::Metatables::ROOM_DESCRIPTOR));
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_LevelFindValidRoomPlacementLocations) {
	int stackIdx = 2;

	int roomShape = 0;
	int doorMask = 0;

	if (lua_type(L, stackIdx) == LUA_TUSERDATA) {
		RoomConfig_Room* roomConfig = lua::GetUserdata<RoomConfig_Room*>(L, stackIdx++, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
		if (roomConfig) {
			roomShape = roomConfig->Shape;
			doorMask = roomConfig->Doors;
		}
	} else {
		roomShape = (int)luaL_checkinteger(L, stackIdx++);
		doorMask = (int)luaL_optinteger(L, stackIdx++, -1);
	}

	const int dimension = LuaCheckDimension(L, stackIdx++);
	const bool allowMultipleDoors = lua::luaL_optboolean(L, stackIdx++, true);
	const bool allowSpecialNeighbors = lua::luaL_optboolean(L, stackIdx++, false);
	const std::set<int> validLocations = FindValidRoomPlacementLocations(roomShape, doorMask, dimension, allowMultipleDoors, allowSpecialNeighbors);
	
	lua_newtable(L);
	int i = 0;
	for (const int gridIndex : validLocations) {
		lua_pushinteger(L, gridIndex);
		lua_rawseti(L, -2, i + 1);
		i++;
	}

	return 1;
}

LUA_FUNCTION(Lua_LevelGetNeighboringRooms) {
	const int gridIndex = (int)luaL_checkinteger(L, 2);
	const int roomShape = (int)luaL_checkinteger(L, 3);
	const int dimension = LuaCheckDimension(L, 4);
	
	const std::map<int, RoomDescriptor*> neighbors = GetNeighboringRooms(gridIndex, roomShape, dimension);

	lua_newtable(L);
	for (const auto& [doorSlot, neighborDesc] : neighbors) {
		lua::luabridge::UserdataPtr::push(L, neighborDesc, lua::GetMetatableKey(lua::Metatables::ROOM_DESCRIPTOR));
		lua_rawseti(L, -2, doorSlot);
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "CanSpawnDoorOutline", Lua_LevelCanSpawnDoorOutline },
		{ "HasAbandonedMineshaft", Lua_LevelHasAbandonedMineshaft },
		{ "HasMirrorDimension", Lua_LevelHasMirrorDimension },
		{ "HasPhotoDoor", Lua_LevelHasPhotoDoor },
		{ "SetName", lua_LevelSetName },
		{ "IsStageAvailable", lua_LevelIsStageAvailable },
		{ "GetDimension", Lua_GetDimension},
		{ "GetForceSpecialQuest", Lua_GetForceSpecialQuest },
		{ "SetForceSpecialQuest", Lua_SetForceSpecialQuest },
		{ "GetMyosotisPickups", Lua_GetMyosotisPickups },

		{ "SetGreedWavesClearedWithoutRedHeartDamage", lua_LevelSetGreedWavesClearedWithoutRedHeartDamage },
		{ "GetGreedWavesClearedWithoutRedHeartDamage", lua_LevelGetGreedWavesClearedWithoutRedHeartDamage },

		{ "CanPlaceRoom", Lua_LevelCanPlaceRoom },
		{ "TryPlaceRoom", Lua_LevelTryPlaceRoom },
		{ "CanPlaceRoomAtDoor", Lua_LevelCanPlaceRoomAtDoor },
		{ "TryPlaceRoomAtDoor", Lua_LevelTryPlaceRoomAtDoor },
		{ "FindValidRoomPlacementLocations", Lua_LevelFindValidRoomPlacementLocations },
		{ "GetNeighboringRooms", Lua_LevelGetNeighboringRooms },

		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::LEVEL, functions);
}