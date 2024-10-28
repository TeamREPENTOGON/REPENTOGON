#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageManager.h"

LUA_FUNCTION(Lua_StageManager_LoadStage) {
	StageManager& stageManager = StageManager::GetInstance();
	const char* name = luaL_checkstring(L, 1);
	XMLAttributes xmlData = XMLStuff.StageData->GetNodeByName(name);
	if (xmlData["basestage"].empty())
		return luaL_error(L, "No basestage for stage name %s", name);

	lua_pushboolean(L, stageManager.LoadStage(toint(xmlData["basestage"]), toint(xmlData["id"])));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_RestoreStage) {
	StageManager& stageManager = StageManager::GetInstance();
	int id = (int)luaL_checkinteger(L, 1);
	if (id < 0 || id > 36) {
		return luaL_error(L, "id must be between 0 and 36, both inclusive (got %d)", id);
	}

	lua_pushboolean(L, stageManager.RestoreStage(id));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_LoadBinary) {
	StageManager& stageManager = StageManager::GetInstance();
	std::string path = luaL_checkstring(L, 1);

	RoomSet* set = stageManager.GetBinary(path, true);

	if (set == nullptr) {
		lua_pushnil(L);
	}
	else
	{
		RoomSet** ud = (RoomSet**)lua_newuserdata(L, sizeof(RoomSet*));
		*ud = set;
		luaL_setmetatable(L, lua::metatables::RoomConfigSetMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_StageManager_GetBinary) {
	StageManager& stageManager = StageManager::GetInstance();
	const char* id = luaL_checkstring(L, 1);

	std::unordered_map<std::string, RoomSet>::const_iterator itr = stageManager.binaryMap.find(id);

	if (itr == stageManager.binaryMap.end()) {
		lua_pushnil(L);
	}
	else
	{
		const RoomSet** ud = (const RoomSet**)lua_newuserdata(L, sizeof(RoomSet*));
		*ud = &itr->second;
		luaL_setmetatable(L, lua::metatables::RoomConfigSetMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_StageManager_AppendContentBinary) {
	StageManager& stageManager = StageManager::GetInstance();
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = stageManager.GetBinary(path, true);
		if (roomSet == nullptr) {
			return luaL_error(L, "No binary exists for path \"%s\"!", path.empty() ? "" : path.c_str());
		}
	}
	else
	{
		roomSet = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	std::string filepath = luaL_checkstring(L, 2);

	lua_pushboolean(L, stageManager.AppendContentBinary(roomSet, filepath));

	return 1;
}

LUA_FUNCTION(Lua_StageManager_IsStageOverriden) {
	StageManager& stageManager = StageManager::GetInstance();
	int id = (int)luaL_checkinteger(L, 1);

	lua_pushboolean(L, stageManager.IsStageOverriden(id));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_ResetRoomWeights) {
	StageManager& stageManager = StageManager::GetInstance();
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = stageManager.GetBinary(path, false);
		if (roomSet == nullptr) {
			return luaL_error(L, "No binary exists for path \"%s\"!", path.empty() ? "" : path.c_str());
		}
	}
	else
	{
		roomSet = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	stageManager.ResetRoomWeights(roomSet);

	return 0;
}

LUA_FUNCTION(Lua_StageManager_ResetAllRoomWeights) {
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ResetAllRoomWeights();
	return 0;
}

static void RegisterStageManager(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "LoadStage", Lua_StageManager_LoadStage);
	lua::TableAssoc(L, "RestoreStage", Lua_StageManager_RestoreStage);
	lua::TableAssoc(L, "LoadBinary", Lua_StageManager_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_StageManager_GetBinary);
	lua::TableAssoc(L, "AppendContentBinary", Lua_StageManager_AppendContentBinary);
	lua::TableAssoc(L, "ResetRoomWeights", Lua_StageManager_ResetRoomWeights);
	lua::TableAssoc(L, "ResetAllRoomWeights", Lua_StageManager_ResetAllRoomWeights);
	lua::TableAssoc(L, "IsStageOverriden", Lua_StageManager_IsStageOverriden);
	lua_setglobal(L, "StageManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterStageManager(_state);
}