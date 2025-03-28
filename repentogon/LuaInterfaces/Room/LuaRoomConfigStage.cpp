#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/VirtualRooms.h"

LUA_FUNCTION(Lua_RoomConfigStageGetBackdrop)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_backdrop);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBackdrop)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_backdrop = (int)lua_tointeger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetMusic)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_musicId);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetMusic)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
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
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	
	auto& roomSet = stage->_rooms[mode];
	if (!roomSet._loaded)
	{
		g_Game->GetRoomConfig()->LoadStageBinary(stage->_id, mode);
	}
	VirtualRoomManager::RoomSet** ud = (VirtualRoomManager::RoomSet**)lua_newuserdata(L, sizeof(VirtualRoomManager::RoomSet*));
	*ud = &VirtualRoomManager::GetRoomSet(stage->_id, mode);
	luaL_setmetatable(L, lua::metatables::RoomConfigSetMT);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageGetID)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushinteger(L, stage->_id);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageGetDisplayName)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_displayName.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetDisplayName)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_displayName = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_playerSpot.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetPlayerSpot)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_playerSpot = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_bossSpot.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetBossSpot)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_bossSpot = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	lua_pushstring(L, stage->_suffix.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetSuffix)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	stage->_suffix = luaL_checkstring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetXMLName)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	std::string* string = &stage->_rooms[0]._filepath;
	lua_pushstring(L, string->substr(6).c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageSetXMLName)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	std::string name = luaL_checkstring(L, 2);

	stage->_rooms[0]._filepath = "rooms/" + name;
	stage->_rooms[1]._filepath = "rooms/greed/" + name;

	return 0;
}

LUA_FUNCTION(Lua_RoomConfigStageGetRoomSetLoaded)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
	int mode = (int)luaL_optinteger(L, 2, 0);
	if (mode < 0 || mode > 1) {
		return luaL_error(L, "Invalid RoomSet mode %d", mode);
	}
	lua_pushboolean(L, stage->_rooms[mode]._loaded);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigStageUnload)
{
	RoomConfig_Stage* stage = *lua::GetRawUserdata<RoomConfig_Stage**>(L, 1, lua::metatables::RoomConfigStageMT);
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