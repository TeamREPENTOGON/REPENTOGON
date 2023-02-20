#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


static constexpr const char* RoomDescriptorDoors = "RoomDescriptorDoors";
static constexpr const char* RoomDescriptorDoorsConst = "RoomDescriptorDoorsConst";

static RoomDescriptor* GetLeftRoom(RoomDescriptor* source) {
	return nullptr;
}
static void RoomDescriptorGetAllowedDoors(lua_State* L, RoomDescriptor* descriptor) {
	lua_pushinteger(L, descriptor->AllowedDoors);
}

static int Lua_RoomDescriptorGetAllowedDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

static int Lua_RoomDescriptorGetAllowedDoorsConst(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetAllowedDoors(L, descriptor);
	return 1;
}

static int Lua_RoomDescriptorSetAllowedDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	int doors = luaL_checkinteger(L, 2);
	descriptor->AllowedDoors = doors;
	return 0;
}

static int Lua_RoomDescriptorSetDoor(lua_State* L) {
	return 0;
}

static void RoomDescriptorGetDoors(lua_State* L, RoomDescriptor* descriptor, bool allowSet) {
	RoomDescriptor** ptr = (RoomDescriptor**)lua_newuserdata(L, sizeof(RoomDescriptor*));
	*ptr = descriptor;
	if (allowSet) {
		luaL_setmetatable(L, RoomDescriptorDoors);
	}
	else {
		luaL_setmetatable(L, RoomDescriptorDoorsConst);
	}
}

static int Lua_RoomDescriptorGetDoors(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::ROOM_DESCRIPTOR, "RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, true);
	return 1;
}

static int Lua_RoomDescriptorGetDoorsConst(lua_State* L) {
	RoomDescriptor* descriptor = lua::GetUserdata<RoomDescriptor*>(L, 1, lua::Metatables::CONST_ROOM_DESCRIPTOR, "const RoomDescriptor");
	RoomDescriptorGetDoors(L, descriptor, false);
	return 1;
}

static void _RoomDescriptorDoorsGet(lua_State* L, RoomDescriptor* descriptor) {
	int slot = luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		luaL_error(L, "Invalid door slot %d\n", slot);
		return;
	}

	lua_pushinteger(L, descriptor->Doors[slot]);
}

static int Lua_RoomDescriptorDoorsGet(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoors);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

static int Lua_RoomDescriptorDoorsGetConst(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoorsConst);
	_RoomDescriptorDoorsGet(L, *ptr);
	return 1;
}

static int Lua_RoomDescriptorDoorsSet(lua_State* L) {
	RoomDescriptor** ptr = lua::GetUserdata<RoomDescriptor**>(L, 1, RoomDescriptorDoors);
	RoomDescriptor* descriptor = *ptr;

	int slot = luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "Invalid door slot %d\n", slot);
	}

	int value = luaL_checkinteger(L, 3);
	if (value < -1 || value > 255) {
		return luaL_error(L, "Invalid door mask %d\n", value);
	}

	descriptor->Doors[slot] = value;
	return 0;
}

static void FixRoomDescriptorProperties(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	int type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoors);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoors);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::CONST_ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propget");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propget is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetAllowedDoorsConst);
	lua_rawset(L, -3);

	lua_pushstring(L, "Doors");
	lua_pushcfunction(L, Lua_RoomDescriptorGetDoorsConst);
	lua_rawset(L, -3);

	lua_pop(L, 2);

	lua::PushMetatable(L, lua::Metatables::ROOM_DESCRIPTOR);
	lua_pushstring(L, "__propset");
	type = lua_rawget(L, -2);

	if (type != LUA_TTABLE) {
		lua_pop(L, 2);
		printf("__propset is not a table\n");
		return;
	}

	lua_pushstring(L, "AllowedDoors");
	lua_pushcfunction(L, Lua_RoomDescriptorSetAllowedDoors);

	lua_rawset(L, -3);
	lua_pop(L, 2);

	luaL_newmetatable(L, RoomDescriptorDoors);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGet);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsSet);
	lua_rawset(L, -5);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomDescriptorDoorsConst);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, Lua_RoomDescriptorDoorsGetConst);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	FixRoomDescriptorProperties(state);
}