#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RoomPlacerMT = "RoomPlacer";

static int Lua_LevelPlaceRoom(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Game");
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 2, RoomPlacerMT);
	RoomConfig* config = lua::GetUserdata<RoomConfig*>(L, 3, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	uint32_t seed = (uint32_t) luaL_checkinteger(L, 4);

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

static int Lua_RoomPlacerSetColIdx(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_gridColIdx = (unsigned int) luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_RoomPlacerSetLineIdx(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_gridLineIdx = (unsigned int) luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_RoomPlacerSetAllowedDoors(lua_State* L) {
	LevelGenerator_Room* room = lua::GetUserdata<LevelGenerator_Room*>(L, 1, RoomPlacerMT);
	room->_doors = (unsigned int) luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_LevelGeneratorEntry(lua_State* L) {
	LevelGenerator_Room* ud = (LevelGenerator_Room*)lua_newuserdata(L, sizeof(LevelGenerator_Room));
	luaL_setmetatable(L, RoomPlacerMT);
	return 1;
}

void RegisterRoomPlacer(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::LEVEL);
	lua_pushstring(L, "PlaceRoom");
	lua_pushcfunction(L, Lua_LevelPlaceRoom);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomPlacerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "SetColIdx", Lua_RoomPlacerSetColIdx },
		{ "SetLineIdx", Lua_RoomPlacerSetLineIdx },
		{ "SetAllowedDoors", Lua_RoomPlacerSetAllowedDoors },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "LevelGeneratorEntry");
	lua_pushcfunction(L, Lua_LevelGeneratorEntry);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterRoomPlacer(state);
}