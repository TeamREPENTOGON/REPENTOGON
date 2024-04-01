#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomConfigStageGetBackdrop)
{
	RoomConfig_Stage* stage = lua::GetUserdata<RoomConfig_Stage*>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_backdrop);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBackdrop)
{
	RoomConfig_Stage* stage = lua::GetUserdata<RoomConfig_Stage*>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_backdrop = (int)lua_tointeger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetMusic)
{
	RoomConfig_Stage* stage = lua::GetUserdata<RoomConfig_Stage*>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_musicId);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetMusic)
{
	RoomConfig_Stage* stage = lua::GetUserdata<RoomConfig_Stage*>(L, 1, lua::metatables::RoomConfigStageMT);
	int musicId = (int)lua_tointeger(L, 2);
	int max;
	if (!g_Manager->_musicmanager.ValidateMusicID(musicId, max)) {
		return luaL_error(L, "Invalid music ID %d. Min = 0, Max = %d", musicId, max - 1);
	}
	stage->_musicId = musicId;

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetRoomSet)
{
	RoomConfig_Stage* stage = lua::GetUserdata<RoomConfig_Stage*>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	
	RoomSet** ud = (RoomSet**)lua_newuserdata(L, sizeof(RoomSet*));
	*ud = &stage->_rooms[mode];
	luaL_setmetatable(L, lua::metatables::RoomSetMT);

	return 1;
}

static void RegisterRoomConfigStage(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetBackdrop", Lua_RoomConfigStageGetBackdrop },
		{ "SetBackdrop", Lua_RoomConfigStageSetBackdrop },
		{ "GetMusic", Lua_RoomConfigStageGetMusic },
		{ "SetMusic", Lua_RoomConfigStageSetMusic },
		{ "GetRoomSet", Lua_RoomConfigStageGetRoomSet },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::RoomConfigStageMT, lua::metatables::RoomConfigStageMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfigStage(_state);
}