#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include <unordered_map>

constexpr unsigned int BUFFER_STAGEID = 23;

std::unordered_map<std::string, RoomSet> roomSetMap;

LUA_FUNCTION(Lua_RoomConfig_GetRoomByStageTypeAndVariant) {
	int n = lua_gettop(L);
	if (n != 4) {
		return luaL_error(L, "Expected four parameters, got %d\n", n);
	}

	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	int stage = -1;
	RoomSet* set;

	if (lua_type(L, 1) == LUA_TUSERDATA) {
		set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	else
	{
		stage = (int)luaL_checkinteger(L, 1);
		if (stage < 0 || stage > 36) {
			return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
		}
	}

	int type = (int)luaL_checkinteger(L, 2);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Type must be between 1 and 29 (both inclusive), got %d\n", type);
	}

	int variant = (int)luaL_checkinteger(L, 3);
	int difficulty = (int)luaL_checkinteger(L, 4);
	if (difficulty < -2 || difficulty > 1) {
		difficulty = -1;
	}

	RoomConfig_Room* config;
	if (stage == -1) {
		// swap in
		//RoomSet oldSet = roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
		roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = *set;

		// get room
		config = roomConfig->GetRoomByStageTypeAndVariant(BUFFER_STAGEID, type, variant, difficulty);

		// swap out
		*set = roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
		//roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = oldSet;
	}
	else
	{
		config = roomConfig->GetRoomByStageTypeAndVariant(stage, type, variant, difficulty);
	}

	if (config == NULL) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, config, lua::Metatables::CONST_ROOM_CONFIG_ROOM);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfig_GetRandomRoom) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	int seed = (int)luaL_checkinteger(L, 1);
	bool reduceWeight = lua::luaL_checkboolean(L, 2);
	int stage = -1;
	RoomSet* set;

	if (lua_type(L, 3) == LUA_TUSERDATA) {
		set = *lua::GetUserdata<RoomSet**>(L, 3, lua::metatables::RoomConfigSetMT);
	}
	else
	{
		stage = (int)luaL_checkinteger(L, 3);
		if (stage < 0 || (stage > 17 && stage < 27) || stage > 36) {
			return luaL_error(L, "Invalid stage %d\n", stage);
		}
	}

	int type = (int)luaL_checkinteger(L, 4);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Invalid type %d\n", type);
	}

	int shape = (int)luaL_optinteger(L, 5, 13); //NUM_ROOMSHAPES
	if (shape < 1 || shape > 13) {
		return luaL_error(L, "Invalid shape %d\n", shape);
	}

	int minVariant = (int)luaL_optinteger(L, 6, 0);
	if (minVariant < 0) {
		minVariant = 0;
	}

	int maxVariant = (int)luaL_optinteger(L, 7, -1);
	if (maxVariant < minVariant && maxVariant >= 0) {
		return luaL_error(L, "maxVariant is lower than minVariant (min = %d, max = %d)\n", minVariant, maxVariant);
	}
	else if (maxVariant < 0) {
		maxVariant = -1;
	}

	int minDifficulty = (int)luaL_optinteger(L, 8, 0);
	if (minDifficulty < 0) {
		minDifficulty = 0;
	}

	int maxDifficulty = (int)luaL_optinteger(L, 9, 10);
	if (maxDifficulty < minDifficulty) {
		return luaL_error(L, "maxDifficulty is lower than minDifficulty (min = %d, max = %d)\n", minDifficulty, maxDifficulty);
	}

	int doors = (int)luaL_optinteger(L, 10, 0);
	if (doors < 0) {
		return luaL_error(L, "Invalid door mask %d\n", doors);
	}

	int subtype = (int)luaL_optinteger(L, 11, -1);
	if (subtype < -1) {
		return luaL_error(L, "Invalid subtype %d\n", subtype);
	}
	int mode = (int)luaL_optinteger(L, 12, -1);
	if (mode < -1 || mode > 1) {
		return luaL_error(L, "Invalid mode %d\n", mode);
	}

	RoomConfig_Room* config;
	if (stage == -1) {
		// swap in
		//RoomSet oldSet = roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
		roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = *set;

		// get room
		config = roomConfig->GetRandomRoom(seed, reduceWeight, BUFFER_STAGEID, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, (unsigned int*)&doors, subtype, mode);

		// swap out
		*set = roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
		//roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = oldSet;
	}
	else
	{
		config = roomConfig->GetRandomRoom(seed, reduceWeight, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, (unsigned int*)&doors, subtype, mode);
	}

	if (config == NULL) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, config, lua::Metatables::CONST_ROOM_CONFIG_ROOM);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfig_GetStage) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	int stage = (int)luaL_checkinteger(L, 1);

	if (stage < 0 || stage > 36) {
		return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
	}

	RoomConfig_Stage* configStage = &roomConfig->_stages[stage];
	//printf("%p, %p, %d\n", roomConfig, configStage, configStage->_musicId);

	RoomConfig_Stage** ud = (RoomConfig_Stage**)lua_newuserdata(L, sizeof(RoomConfig_Stage*));
	*ud = configStage;
	luaL_setmetatable(L, lua::metatables::RoomConfigStageMT);
	return 1;
}

LUA_FUNCTION(Lua_RoomConfig_LoadBinary) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	const char* path = luaL_checkstring(L, 1);
	const char* id = luaL_checkstring(L, 2);

	// store this, it's going to be overwritten in a moment
	//RoomSet oldSet = roomConfig->_stages[BUFFER_STAGEID]._rooms[0];

	RoomSet newSet;
	newSet._filepath = path;
	roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = newSet;

	std::pair<std::unordered_map<std::string, RoomSet>::iterator, bool> itr;

	// switcharoo
	roomConfig->LoadStageBinary(BUFFER_STAGEID, 0);
	itr = roomSetMap.insert({ std::string(id), roomConfig->_stages[BUFFER_STAGEID]._rooms[0] });

	// restore old set
	//roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = oldSet;

	if (!itr.second) {
		lua_pushnil(L);
	}
	else
	{
		RoomSet** ud = (RoomSet**)lua_newuserdata(L, sizeof(RoomSet*));
		*ud = &itr.first._Ptr->_Myval.second;
		luaL_setmetatable(L, lua::metatables::RoomConfigSetMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfig_GetBinary) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	const char* id = luaL_checkstring(L, 1);
	std::unordered_map<std::string, RoomSet>::const_iterator itr = roomSetMap.find(std::string(id));

	if (itr == roomSetMap.end()) {
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

static void RegisterRoomConfig(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetRoomConfig", Lua_GameGetRoomConfig);
	lua_newtable(L);
	lua::TableAssoc(L, "GetRoomByStageTypeAndVariant", Lua_RoomConfig_GetRoomByStageTypeAndVariant);
	lua::TableAssoc(L, "GetRandomRoom", Lua_RoomConfig_GetRandomRoom);
	lua::TableAssoc(L, "GetStage", Lua_RoomConfig_GetStage);
	lua::TableAssoc(L, "LoadBinary", Lua_RoomConfig_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_RoomConfig_GetBinary);
	lua_setglobal(L, "RoomConfig");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfig(_state);
}