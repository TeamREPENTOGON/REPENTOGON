#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomConfigRoomGetMode)
{
	RoomConfig_Room* roomConfig = lua::GetLuabridgeUserdata<RoomConfig_Room*>(L, 1, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfigRoom");
	lua_pushinteger(L, roomConfig->Mode);

	return 1;
}

static void FixRoomConfigRoomModeGet(lua_State* L) {
	lua::RegisterVariableGetter(L, lua::Metatables::ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigRoomGetMode);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "Mode", Lua_RoomConfigRoomGetMode);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	FixRoomConfigRoomModeGet(_state);
}