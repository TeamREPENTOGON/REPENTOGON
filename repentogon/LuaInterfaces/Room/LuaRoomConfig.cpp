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
	lua::RegisterVariableGetter(L, lua::Metatables::ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigGetMode);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigGetMode);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	FixRoomConfigModeGet(_state);
}