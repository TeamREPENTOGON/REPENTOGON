#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LevelPlaceRoom) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Game");
	LevelGenerator_Room* room = lua::GetRawUserdata<LevelGenerator_Room*>(L, 2, lua::metatables::LevelGeneratorEntryMT);
	RoomConfig_Room* config = lua::GetUserdata<RoomConfig_Room*>(L, 3, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	uint32_t seed = (uint32_t)luaL_checkinteger(L, 4);

	bool result = game->PlaceRoom(room, config, seed, 0);
	lua_pushboolean(L, result);

	if (result) {
		// Fix doors for other rooms
		uint32_t roomIdx = room->_gridLineIdx * 13 + room->_gridColIdx;
		RoomDescriptor* descriptor = game->GetRoomByIdx(roomIdx, -1);

		switch (descriptor->Data->Shape) {
		case ROOMSHAPE_1x1:
			break;
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomPlacerSetColIdx) {
	LevelGenerator_Room* room = lua::GetRawUserdata<LevelGenerator_Room*>(L, 1, lua::metatables::LevelGeneratorEntryMT);
	room->_gridColIdx = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomPlacerSetLineIdx) {
	LevelGenerator_Room* room = lua::GetRawUserdata<LevelGenerator_Room*>(L, 1, lua::metatables::LevelGeneratorEntryMT);
	room->_gridLineIdx = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_RoomPlacerSetAllowedDoors) {
	LevelGenerator_Room* room = lua::GetRawUserdata<LevelGenerator_Room*>(L, 1, lua::metatables::LevelGeneratorEntryMT);
	room->_doors = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_LevelGeneratorEntry) {
	LevelGenerator_Room* ud = (LevelGenerator_Room*)lua_newuserdata(L, sizeof(LevelGenerator_Room));
	luaL_setmetatable(L, lua::metatables::LevelGeneratorEntryMT);
	return 1;
}

void RegisterRoomPlacer(lua_State* L) {
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "LevelGeneratorEntry", Lua_LevelGeneratorEntry);
	lua::RegisterFunction(L, lua::Metatables::LEVEL, "PlaceRoom", Lua_LevelPlaceRoom);

	luaL_Reg functions[] = {
		{ "SetColIdx", Lua_RoomPlacerSetColIdx },
		{ "SetLineIdx", Lua_RoomPlacerSetLineIdx },
		{ "SetAllowedDoors", Lua_RoomPlacerSetAllowedDoors },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LevelGeneratorEntryMT, lua::metatables::LevelGeneratorEntryMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomPlacer(_state);
}