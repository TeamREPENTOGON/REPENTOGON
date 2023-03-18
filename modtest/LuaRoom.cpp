#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Room_GetRailType(lua_State* L) {

}

static int Room_SetRailType(lua_State* L) {

}

static int Room_Test(lua_State* L) {
	lua_pushinteger(L, 12);
	return 1;
}

static void RegisterRailFunctions(lua_State* L) {
	// Get metatable of Room object
	lua::PushMetatable(L, lua::Metatables::ROOM);
	lua_pushstring(L, "Test");
	lua_pushcfunction(L, Room_Test);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_RemoveGridEntityImmediate(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	int GridIndex = luaL_checkinteger(L, 2);
	int PathTrail = luaL_checkinteger(L, 3);
	bool KeepDecoration = lua_toboolean(L, 4);
	room->RemoveGridEntityImmediate(GridIndex, PathTrail, KeepDecoration);
	return 0;
}

static void RegisterRemoveGridEntityImmediate(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM);
	lua_pushstring(L, "RemoveGridEntityImmediate");
	lua_pushcfunction(L, Lua_RemoveGridEntityImmediate);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_RoomCanSpawnObstacleAtPosition(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	int GridIndex = luaL_checkinteger(L, 2);
	bool Force = lua_toboolean(L, 3);
	lua_pushboolean(L, room->CanSpawnObstacleAtPosition(GridIndex, Force));
	return 1;
}

static void RegisterRoomCanSpawnObstacleAtPosition(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM);
	lua_pushstring(L, "CanSpawnObstacleAtPosition");
	lua_pushcfunction(L, Lua_RoomCanSpawnObstacleAtPosition);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterRailFunctions(state);
	RegisterRemoveGridEntityImmediate(state);
	RegisterRoomCanSpawnObstacleAtPosition(state);
}