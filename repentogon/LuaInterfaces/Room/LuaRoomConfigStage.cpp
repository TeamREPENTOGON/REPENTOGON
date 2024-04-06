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
	luaL_setmetatable(L, lua::metatables::RoomConfigSetMT);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetRoomSet)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 2, lua::metatables::RoomConfigSetMT);
	int mode = (int)luaL_optinteger(L, 3, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	stage->_rooms[mode] = *set;

	return 0;
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
	std::string* name = &stage->_displayName;
	lua_pushstring(L, name->empty() ? "" : name->c_str());
	
	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetDisplayName)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_displayName = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	std::string* name = &stage->_playerSpot;
	lua_pushstring(L, name->empty() ? "" : name->c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_playerSpot = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	std::string* name = &stage->_bossSpot;
	lua_pushstring(L, name->empty() ? "" : name->c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_bossSpot = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	std::string* name = &stage->_suffix;
	lua_pushstring(L, name->empty() ? "" : name->c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_suffix = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetXMLName)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}

	std::string* name = &stage->_rooms[mode]._filepath;
	lua_pushstring(L, name->empty() ? "" : name->c_str());
	
	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetXMLName)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, -1);
	if (mode > 1) {
		mode = -1;
	}
	std::string name = luaL_checkstring(L, 2);

	if (mode == -1 || mode == 0)
		stage->_rooms[0]._filepath = name;
	if (mode == -1 || mode == 1)
		stage->_rooms[1]._filepath = name;

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetRoomSetLoaded)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	lua_pushboolean(L, stage->_rooms[mode]._loaded);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageUnload)
{
	RoomConfig_Stage* stage = *lua::GetUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->unload();

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
		{ "SetRoomSet", Lua_RoomConfigStageSetRoomSet },
		{ "GetXMLName", Lua_RoomConfigStageGetXMLName },
		{ "SetXMLName", Lua_RoomConfigStageSetXMLName },
		{ "IsLoaded", Lua_RoomConfigStageGetRoomSetLoaded },
		{ "Unload", Lua_RoomConfigStageUnload },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::RoomConfigStageMT, lua::metatables::RoomConfigStageMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfigStage(_state);
}