#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"
#include <unordered_map>

#include "../repentogon/Patches/XMLData.h"

constexpr unsigned int BUFFER_STAGEID = 23;

std::unordered_map<std::string, RoomSet> binaryMap;
bool stageOverwritten[37] = { false };

RoomSet* LoadBinary(RoomConfig* roomConfig, std::string* path) {
	ZHL::Logger logger(true);
	logger.Log("Loading binary %s\n", path->c_str());
	RoomSet newSet;
	newSet._filepath = *path;
	roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = newSet;

	// switcharoo
	bool res = roomConfig->LoadStageBinary(BUFFER_STAGEID, 0);

	if (res) {
		return &binaryMap.find(*path)._Ptr->_Myval.second;
	}

	return nullptr;
}

RoomSet* GetBinary(std::string* path) {
	ZHL::Logger logger(true);
	logger.Log("Getting binary for path %s\n", path->c_str());

	std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(*path);

	if (itr != binaryMap.end()) {
		return &itr._Ptr->_Myval.second;
	}

	return nullptr;
}

bool IsBinaryLoaded(std::string* path) {
	std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(*path);
	return itr != binaryMap.end();
}

// automatically inserts a copy of the new RoomSet into the map
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int stage, unsigned int mode) -> bool) {
	bool res = super(stage, mode);

	if (res) {
		RoomSet* newSet = &this->_stages[stage]._rooms[mode];
		binaryMap.insert({ newSet->_filepath, *newSet });
	}

	return res;
}

void SwapStage(RoomConfig* roomConfig, XMLAttributes xmlData, int stageId) {
	std::string binary = xmlData["root"] + xmlData["path"];
	std::string greedBinary = xmlData["greedroot"] + xmlData["path"];
	std::string gfxRoot = xmlData["bossgfxroot"];
	std::string playerSpot = gfxRoot + xmlData["playerspot"];
	std::string bossSpot = gfxRoot + xmlData["playerspot"];
	int musicId = stoi(xmlData["music"]);
	int backdropId = stoi(xmlData["backdrop"]);

	ZHL::Logger logger(true);
	logger.Log("\nLOADING STAGE: id %d, path %s, greed path %s, gfx path %s, playerSpot %s, bossSpot %s, musicId %d, backDropId %d\n", stageId, binary.c_str(), greedBinary.c_str(), gfxRoot.c_str(), playerSpot.c_str(), bossSpot.c_str(), musicId, backdropId);

	RoomConfig_Stage* stage = &roomConfig->_stages[stageId];
	std::unordered_map<std::string, RoomSet>::const_iterator itr;
	for (size_t i = 0; i < 2; i++)
	{
		// :goblinPls:P:goblinPls:A:goblinPls:I:goblinPls:N:goblinPls:
		std::string path;
		if (i)
			path = greedBinary;
		else
			path = binary;

		logger.Log("RoomSet %d\n");
		if (stage->_rooms[i]._loaded)
		{
			logger.Log("already loaded\n");
			itr = binaryMap.find(stage->_rooms[i]._filepath);
			if (itr == binaryMap.end()) {
				// somehow hasn't been inserted yet, do so
				logger.Log("inserting filepath %s\n", stage->_rooms[i]._filepath.c_str());
				binaryMap.insert({ stage->_rooms[i]._filepath, stage->_rooms[i] });
			}
			else
			{
				// update with current copy
				logger.Log("updating with existing copy\n");
				itr._Ptr->_Myval.second = stage->_rooms[i];
			}
		}
		else
		{
			logger.Log("not loaded yet\n");
			RoomSet* baseSet = LoadBinary(roomConfig, &stage->_rooms[i]._filepath);
			if (baseSet != nullptr)
				binaryMap.insert({ stage->_rooms[i]._filepath, *baseSet });
			else
				logger.Log("LoadBinary failed\n");
		}

		RoomSet* set = GetBinary(&path);
		if (set != nullptr) {
			logger.Log("replacing with existing RoomSet, %s\n", set->_filepath.c_str());
			stage->_rooms[i] = *set;
		}
		else
		{
			logger.Log("loading new binary to overwrite\n");
			RoomSet* newBinary = LoadBinary(roomConfig, &path);
			if (newBinary != nullptr) {
				logger.Log("loading successful, %s\n", newBinary->_filepath.c_str());
				stage->_rooms[i] = *newBinary;
			}
			else
			{
				logger.Log("LoadBinary failed\n");
			}
		}
	}

	stage->_backdrop = backdropId;
	stage->_displayName = xmlData["name"];
	stage->_playerSpot = playerSpot;
	stage->_bossSpot = bossSpot;
	stage->_suffix = xmlData["suffix"];
	stage->_musicId = musicId;

	stageOverwritten[stageId] = !stageOverwritten[stageId];
	logger.Log("we're done here\n\n");
}

LUA_FUNCTION(Lua_RoomConfig_LoadStage) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	const char* name = luaL_checkstring(L, 1);

	XMLAttributes xmlData = XMLStuff.StageData->GetNodeByName(std::string(name));

	if (xmlData["basestage"] != "" && !stageOverwritten[stoi(xmlData["basestage"])]) {
		SwapStage(roomConfig, xmlData, stoi(xmlData["basestage"]));
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfig_RestoreStage) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	int id = (int)luaL_checkinteger(L, 1);

	XMLAttributes xmlData = XMLStuff.StageData->GetNodeById(id);

	if (xmlData["id"] != "" && stageOverwritten[stoi(xmlData["id"])]) {
		SwapStage(roomConfig, xmlData, stoi(xmlData["id"]));
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
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
	std::string path = luaL_checkstring(L, 1);

	RoomSet* set = LoadBinary(roomConfig, &path);

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

LUA_FUNCTION(Lua_RoomConfig_GetBinary) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	const char* id = luaL_checkstring(L, 1);
	int mode = (int)luaL_optinteger(L, 2, 0);

	if (mode < -1 || mode > 1) {
		return luaL_error(L, "Invalid mode %d\n", mode);
	}

	std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(id);

	if (itr == binaryMap.end()) {
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

void ResetRoomWeights(RoomSet* set) {
	for (unsigned int i = 0; i < set->_count; i++) {
		set->_configs[i].Weight = set->_configs[i].InitialWeight;
	}
}

HOOK_STATIC(LuaEngine, PostGameStart, (unsigned int state) -> void, __stdcall) {
	if (state == 0) {
		for (auto i = binaryMap.begin(); i != binaryMap.end(); i++) {
			ResetRoomWeights(&i->second);
		}
	}

	super(state);
}

// nonbinary stuff
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

static void RegisterRoomConfig(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetRoomConfig", Lua_GameGetRoomConfig);
	lua_newtable(L);
	lua::TableAssoc(L, "GetRoomByStageTypeAndVariant", Lua_RoomConfig_GetRoomByStageTypeAndVariant);
	lua::TableAssoc(L, "GetRandomRoom", Lua_RoomConfig_GetRandomRoom);
	lua::TableAssoc(L, "GetStage", Lua_RoomConfig_GetStage);
	lua::TableAssoc(L, "LoadStage", Lua_RoomConfig_LoadStage);
	lua::TableAssoc(L, "RestoreStage", Lua_RoomConfig_RestoreStage);
	lua::TableAssoc(L, "LoadBinary", Lua_RoomConfig_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_RoomConfig_GetBinary);
	lua_setglobal(L, "RoomConfig");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfig(_state);
}