#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomConfigStageGetBackdrop)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_backdrop);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBackdrop)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_backdrop = (int)lua_tointeger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetMusic)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_musicId);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetMusic)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
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
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	
	RoomSet** ud = (RoomSet**)lua_newuserdata(L, sizeof(RoomSet*));
	*ud = &stage->_rooms[mode];
	luaL_setmetatable(L, lua::metatables::RoomSetMT);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageGetID)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_id);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageGetDisplayName)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_displayName.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetDisplayName)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_displayName = lua_tostring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_playerSpot.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_playerSpot = lua_tostring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_bossSpot.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_bossSpot = lua_tostring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_suffix.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_suffix = lua_tostring(L, 2);

	return 0;
}

static void RegisterRoomConfigStage(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetBackdrop", Lua_RoomConfigStageGetBackdrop },
		{ "SetBackdrop", Lua_RoomConfigStageSetBackdrop },
		{ "GetMusic", Lua_RoomConfigStageGetMusic },
		{ "SetMusic", Lua_RoomConfigStageSetMusic },
		{ "GetID", Lua_RoomConfigStageGetID },
		{ "GetDisplayName", Lua_RoomConfigStageGetDisplayName },
		{ "SetDisplayName", Lua_RoomConfigStageSetDisplayName },
		{ "GetPlayerSpot", Lua_RoomConfigStageGetPlayerSpot },
		{ "SetPlayerSpot", Lua_RoomConfigStageSetPlayerSpot },
		{ "GetBossSpot", Lua_RoomConfigStageGetBossSpot },
		{ "SetBossSpot", Lua_RoomConfigStageSetBossSpot },
		{ "GetSuffix", Lua_RoomConfigStageGetSuffix },
		{ "SetSuffix", Lua_RoomConfigStageSetSuffix },
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