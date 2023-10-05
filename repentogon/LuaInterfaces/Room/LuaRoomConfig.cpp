#include <array>

#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomConfigGetMode)
{
	RoomConfig* roomConfig = lua::GetUserdata<RoomConfig*>(L, 1, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfigRoom");
	lua_pushinteger(L, roomConfig->Mode);

	return 1;
}

static void FixRoomConfigModeGet(lua_State* L) {
	lua::LuaStackProtector protector(L);

	std::array<lua::Metatables, 2> metatables = {
		lua::Metatables::ROOM_CONFIG_ROOM,
		lua::Metatables::CONST_ROOM_CONFIG_ROOM
	};

	for (lua::Metatables metatable : metatables) {

		lua::PushMetatable(L, metatable);
		lua_pushstring(L, "__propget");
		lua_rawget(L, -2);

		lua_pushstring(L, "Mode");
		lua_pushcfunction(L, Lua_RoomConfigGetMode);
		lua_rawset(L, -3);


		lua_pop(L, 2);
	}
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	FixRoomConfigModeGet(_state);
}