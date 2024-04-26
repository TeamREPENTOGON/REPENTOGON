#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageHandler.h"
#include "suffixes.h"

#include <unordered_map>
#include <sstream>

extern int toint(const string& str);
extern float tofloat(const string& str);
extern bool tobool(const string& str);

namespace StageHandler {
	const unsigned int BUFFER_STAGEID = 23;
	std::unordered_map<std::string, RoomSet> binaryMap;
	std::pair<bool, std::string> stageState[37];

	RoomSet* LoadBinary(std::string* path) {
		RoomConfig* roomConfig = g_Game->GetRoomConfig();
		ZHL::Logger logger(true);
		logger.Log("[INFO] StageHandler::LoadBinary: Loading stage binary \"%s\"\n", path->c_str());

		RoomSet newSet;
		RoomConfig_Stage* buffer = &roomConfig->_stages[BUFFER_STAGEID];
		newSet._filepath = *path;
		buffer->_rooms[0] = newSet;
		roomConfig->_stages[BUFFER_STAGEID]._rooms[0] = newSet;

		// to look better in the log
		buffer->_displayName = "(binary) \"" + *path + "\"";

		bool res = roomConfig->LoadStageBinary(BUFFER_STAGEID, 0);

		if (res) {
			RoomSet* ret = &binaryMap.find(*path)._Ptr->_Myval.second;
			logger.Log("[INFO] StageHandler::LoadBinary: Loaded successfully at pointer %p\n", ret);
			return ret;
		}

		logger.Log("[WARNING] StageHandler::LoadBinary: Failed to load binary \"%s\"!\n", path->c_str());
		return nullptr;
	};

	RoomSet* GetBinary(std::string* path) {
		ZHL::Logger logger(true);
		logger.Log("[INFO] StageHandler::GetBinary: Attempting to retrieve binary \"%s\" from cache\n", path->c_str());

		std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(*path);

		if (itr != binaryMap.end()) {
			RoomSet* ret = &itr._Ptr->_Myval.second;
			logger.Log("[INFO] StageHandler::GetBinary: Retrieved successfully from pointer %p\n", ret);
			return ret;
		}

		logger.Log("[INFO] StageHandler::GetBinary: Binary is not in cache\n", path->c_str());
		return nullptr;
	};

	bool IsBinaryLoaded(std::string* path) {
		std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(*path);
		return itr != binaryMap.end();
	};

	bool SwapStage(int stageId, const char* stageName, bool restoring) {
		// i'm doing xmlData this way to avoid adding an XMLData.h include to StageHandler.h (and thus all other cpp files that use it)
		XMLAttributes xmlData = (stageName != nullptr ? XMLStuff.StageData->GetNodeByName(stageName) : XMLStuff.StageData->GetNodeById(stageId));
		int comp = toint(restoring ? xmlData["id"] : xmlData["basestage"]);

		ZHL::Logger logger(true);
		/*
		if (comp == 0) {
			logger.Log("[ERROR] StageConfig::SwapStage: no XML data for stage %s!\n", stageName);
		}
		*/

		if ((!restoring && !stageState[comp].first) || (restoring && stageState[comp].first))
		{
			RoomConfig* roomConfig = g_Game->GetRoomConfig();
			std::string binary = xmlData["root"] + xmlData["path"];
			std::string greedBinary = xmlData["greedroot"] + xmlData["path"];
			std::string gfxRoot = xmlData["bossgfxroot"];
			std::string playerSpot = gfxRoot + xmlData["playerspot"];
			std::string bossSpot = gfxRoot + xmlData["bossspot"];
			std::string displayName = xmlData["displayname"].empty() ? xmlData["name"]: xmlData["displayname"];
			std::string suffix = xmlData["suffix"];
			std::string name = (restoring ? tokens[comp] : xmlData["name"]);
			int musicId = stoi(xmlData["music"]);
			int backdropId = stoi(xmlData["backdrop"]);

			logger.Log("[INFO] StageHandler::SwapStage: name %s, id %d, path %s, greed path %s, gfx path %s, playerSpot %s, bossSpot %s, suffix %s, musicId %d, backDropId %d\n", name.c_str(), stageId, binary.c_str(), greedBinary.c_str(), gfxRoot.c_str(), playerSpot.c_str(), bossSpot.c_str(), suffix.c_str(), musicId, backdropId);

			RoomConfig_Stage* stage = &roomConfig->_stages[stageId];
			std::unordered_map<std::string, RoomSet>::const_iterator itr;
			bool success = false;
			for (size_t i = 0; i < 2; i++)
			{
				std::string* path = i ? &greedBinary : &binary;
				logger.Log("[INFO] Processing RoomSet \"%s\" (mode %d)\n", path->c_str(), i);
				if (stage->_rooms[i]._loaded)
				{
					logger.Log("[INFO] already loaded\n");
					itr = StageHandler::binaryMap.find(stage->_rooms[i]._filepath);
					if (itr == binaryMap.end()) {
						// somehow hasn't been inserted yet, do so
						logger.Log("[WARNING] existing RoomSet not cached yet, inserting \"%s\"\n", stage->_rooms[i]._filepath.c_str());
						binaryMap.insert({ stage->_rooms[i]._filepath, stage->_rooms[i] });
					}
				}
				/*
				else
				{
					logger.Log("[INFO] \"%s\" not loaded yet, attempting...\n", stage->_rooms[i]._filepath.c_str());
					RoomSet* baseSet = StageHandler::LoadBinary(&stage->_rooms[i]._filepath);
					if (baseSet != nullptr)
						StageHandler::binaryMap.insert({ stage->_rooms[i]._filepath, *baseSet });
						
				}
				*/

				RoomSet* set = StageHandler::GetBinary(path);
				if (set != nullptr) {
					logger.Log("[INFO] replacing with existing RoomSet \"%s\"\n", set->_filepath.c_str());
					stage->_rooms[i] = *set;
					success = true;
				}
				else
				{
					logger.Log("[INFO] loading new binary \"%s\" for RoomSet replacement\n", path->c_str());
					RoomSet* newBinary = StageHandler::LoadBinary(path);
					if (newBinary != nullptr) {
						stage->_rooms[i] = *newBinary;
						success = true;
					}
				}
			}

			if (success) {
				stage->_backdrop = backdropId;
				stage->_displayName = displayName;
				stage->_playerSpot = playerSpot;
				stage->_bossSpot = bossSpot;
				stage->_suffix = suffix;
				stage->_musicId = musicId;

				stageState[stageId].first = !stageState[stageId].first;
				stageState[stageId].second = name;
				logger.Log("[INFO] StageHandler::SwapStage: successfully assigned %s to id %d\n", name.c_str(), stageId);
				return true;
			}
			else
			{
				logger.Log("[ERROR] StageHandler::SwapStage: could not replace the RoomSet of either mode!\n");
			}
		}
		else
		{
			logger.Log("[INFO] StageHandler::SwapStage: nothing to restore for id %d\n", comp);
		}
		return false;
	};

	bool AppendBinary(RoomSet* roomSet, std::string* binary) {
		RoomConfig* roomConfig = g_Game->GetRoomConfig();
		ModManager* modManager = g_Manager->GetModManager();
		roomConfig->_stages[BUFFER_STAGEID]._id = BUFFER_STAGEID;
		RoomSet* buffer = &roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
		int roomCount = buffer->_count;
		buffer->_filepath = *binary;
		modManager->UpdateRooms(BUFFER_STAGEID, 0);
		buffer->_filepath = roomSet->_filepath;
		*roomSet = *buffer;
		return roomCount < buffer->_count;
	}

	void ResetRoomWeights(RoomSet* set) {
		for (unsigned int i = 0; i < set->_count; i++) {
			set->_configs[i].Weight = set->_configs[i].InitialWeight;
		}
	};

	void ResetAllRoomWeights() {
		for (auto i = binaryMap.begin(); i != binaryMap.end(); i++) {
			ResetRoomWeights(&i->second);
		}
	}

	int GetStageIdForToken(std::string token) {
		for (unsigned int i = 0; i < 37; i++) {
			if (stageState[i].second == token)
				return i;
		}
		return -1;
	}

	std::string* GetTokenForStageId(int id) {
		return &stageState[id].second;
	}
};

// Handle RoomSet cacheing
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int id, unsigned int mode) -> bool) {
	RoomConfig_Stage* stage = &this->_stages[id];
	RoomSet* set = &stage->_rooms[mode];
	std::string* path = &set->_filepath;

	// if we already loaded this binary, use the cached version
	std::unordered_map<std::string, RoomSet>::const_iterator itr = StageHandler::binaryMap.find(*path);
	if (itr != StageHandler::binaryMap.end()) {
		stringstream message;
		message << "[RoomConfig] stage " << id << ": " << stage->_displayName << " (mode " << mode << ") already loaded from binary \"" << set->_filepath << "\"\n";
		KAGE::LogMessage(0, message.str().c_str());
		*set = itr._Ptr->_Myval.second;
		return true;
	}
	bool res = super(id, mode);

	// cache the loaded binary
	if (res) {
		StageHandler::binaryMap.insert({ *path, *set });
		if (stage->_suffix.empty()) {
			stage->_suffix = suffixes[id];
		}
		if (StageHandler::stageState[id].second.empty()) {
			StageHandler::stageState[id].second = tokens[id];
		}
	}

	return res;
}

HOOK_METHOD(RoomConfig_Stage, unload, () -> void) {
	std::unordered_map<std::string, RoomSet>::const_iterator itr;
	bool restored = false;
	for (unsigned int i = 0; i < 2; i++) {
		itr = StageHandler::binaryMap.find(this->_rooms[i]._filepath);
		if (itr != StageHandler::binaryMap.end()) {
			StageHandler::binaryMap.erase(itr);
			stringstream message;
			message << "removed binary \"" << this->_rooms[i]._filepath << "\" from StageHandler cache.\n";
			KAGE::LogMessage(0, message.str().c_str());
			restored = true;
		}
	}
	if (restored && tokens[this->_id] != "") {
		XMLAttributes xmlData = XMLStuff.StageData->GetNodeById(this->_id);
		std::string binary = xmlData["root"] + xmlData["path"];
		std::string greedBinary = xmlData["greedroot"] + xmlData["path"];
		std::string gfxRoot = xmlData["bossgfxroot"];
		std::string playerSpot = gfxRoot + xmlData["playerspot"];
		std::string bossSpot = gfxRoot + xmlData["bossspot"];
		std::string displayName = xmlData["name"];
		std::string suffix = xmlData["suffix"];
		int musicId = stoi(xmlData["music"]);
		int backdropId = stoi(xmlData["backdrop"]);

		this->_backdrop = backdropId;
		this->_displayName = displayName;
		this->_playerSpot = playerSpot;
		this->_bossSpot = bossSpot;
		this->_suffix = suffix;
		this->_musicId = musicId;

		super();

		this->_rooms[0]._filepath = binary;
		this->_rooms[1]._filepath = greedBinary;

		StageHandler::stageState[this->_id].first = false;
		StageHandler::stageState[this->_id].second = tokens[this->_id];

		return;
	}

	super();
}


HOOK_METHOD(FXLayers, Init, (char* fileName, int levelStage, int stageType) -> void) {
	super(fileName, levelStage, stageType);

	int backdropType = this->_backdropType;
	if (XMLStuff.BackdropData->backdropState[backdropType].first) {
		XMLAttributes backdropData = XMLStuff.FxParamData->GetNodeById(XMLStuff.BackdropData->backdropState[backdropType].second);
		XMLAttributes paramData = XMLStuff.FxParamData->GetNodeByName(backdropData["name"]);
		int id = toint(paramData["id"]);

		if (id > 0) {
			FXParams* params = &this->_fxParams;
			params->shadowAlpha = tofloat(paramData["shadowalpha"]);
			params->useWaterV2 = tobool(paramData["waterv2"]);

			if (XMLStuff.FxParamData->childs.find(id) != XMLStuff.FxParamData->childs.end()) {
				XMLChilds childs = XMLStuff.FxParamData->childs[id];
				if (childs.find("roomcolor") != childs.end()) {
					ColorModState* mod = params->GetColorModifier();
					for each (XMLAttributes roomcolor in childs["roomcolor"])
					{
						if (roomcolor.find("r") != paramData.end()) {
							mod->r = tofloat(roomcolor["r"]);
							mod->g = tofloat(roomcolor["g"]);
							mod->b = tofloat(roomcolor["b"]);
							mod->a = tofloat(roomcolor["a"]);
							mod->brightness = tofloat(roomcolor["brightness"]);
							mod->contrast = tofloat(roomcolor["contrast"]);
						}
					}
				}
				if (childs.find("watercolor") != childs.end()) {
					for each (XMLAttributes watercolor in childs["watercolor"])
					{
						if (watercolor.find("r") != paramData.end()) {
							params->waterColor._red = tofloat(watercolor["r"]);
							params->waterColor._green = tofloat(watercolor["g"]);
							params->waterColor._blue = tofloat(watercolor["b"]);
							params->waterColor._alpha = tofloat(watercolor["a"]);
						}
					}
				}
				/*
				if (childs.find("watercolormultiplier") != childs.end()) {
					for each (XMLAttributes watercolormultiplier in childs["watercolormultiplier"])
					{
						if (watercolormultiplier.find("r") != paramData.end()) {
							params->waterColorMultiplier._red = tofloat(watercolormultiplier["r"]);
							params->waterColorMultiplier._green = tofloat(watercolormultiplier["g"]);
							params->waterColorMultiplier._blue = tofloat(watercolormultiplier["b"]);
							params->waterColorMultiplier._alpha = tofloat(watercolormultiplier["a"]);
						}
					}
				}
				*/
				if (childs.find("shadowcolor") != childs.end()) {
					for each (XMLAttributes shadowcolor in childs["shadowcolor"])
					{
						if (shadowcolor.find("r") != paramData.end()) {
							params->shadowColor._red = tofloat(shadowcolor["r"]);
							params->shadowColor._green = tofloat(shadowcolor["g"]);
							params->shadowColor._blue = tofloat(shadowcolor["b"]);
							params->shadowColor._alpha = tofloat(shadowcolor["a"]);
						}
					}
				}
				if (childs.find("lightcolor") != childs.end()) {
					for each (XMLAttributes lightcolor in childs["lightcolor"])
					{
						if (lightcolor.find("r") != paramData.end()) {
							params->lightColor._red = tofloat(lightcolor["r"]);
							params->lightColor._green = tofloat(lightcolor["g"]);
							params->lightColor._blue = tofloat(lightcolor["b"]);
							params->lightColor._alpha = tofloat(lightcolor["a"]);
						}
					}
				}
				if (childs.find("watereffectcolor") != childs.end()) {
					for each (XMLAttributes watereffectcolor in childs["watereffectcolor"])
					{
						if (watereffectcolor.find("r") != paramData.end()) {
							params->waterEffectColor._tint[0] = tofloat(watereffectcolor["r"]);
							params->waterEffectColor._tint[1] = tofloat(watereffectcolor["g"]);
							params->waterEffectColor._tint[2] = tofloat(watereffectcolor["b"]);
							params->waterEffectColor._tint[3] = tofloat(watereffectcolor["a"]);
						}
					}
				}
			}
		}
	}
}

HOOK_METHOD(Game, Start, (int playertype, int challenge, Seeds seeds, unsigned int difficulty) -> void) {
	StageHandler::ResetAllRoomWeights();
	super(playertype, challenge, seeds, difficulty);
}
HOOK_METHOD(Game, NetStart, (void* unk, int challenge, Seeds seed, unsigned int difficulty, GameState* state) -> void) {
	StageHandler::ResetAllRoomWeights();
	super(unk, challenge, seed, difficulty, state);
}
HOOK_METHOD(Game, StartDebug, (int levelStage, int stageType, int difficulty, std_string* filepath) -> void) {
	StageHandler::ResetAllRoomWeights();
	super(levelStage, stageType, difficulty, filepath);
}
HOOK_METHOD(Game, StartFromRerunState, (GameState* state) -> void) {
	StageHandler::ResetAllRoomWeights();
	super(state);
}

/////
// Start of lua binders
/////

LUA_FUNCTION(Lua_StageHandler_LoadStage) {
	const char* name = luaL_checkstring(L, 1);
	XMLAttributes xmlData = XMLStuff.StageData->GetNodeByName(name);
	if (xmlData["basestage"].empty())
		return luaL_error(L, "No basestage for stage name %s", name);

	lua_pushboolean(L, StageHandler::SwapStage(toint(xmlData["basestage"]), name, false));
	return 1;
}

LUA_FUNCTION(Lua_StageHandler_RestoreStage) {
	int id = (int)luaL_checkinteger(L, 1);
	if (id < 0 || id > 36) {
		return luaL_error(L, "id must be between 0 and 36, both inclusive (got %d)", id);
	}

	lua_pushboolean(L, StageHandler::SwapStage(id, nullptr, true));
	return 1;
}

LUA_FUNCTION(Lua_StageHandler_LoadBinary) {
	std::string path = luaL_checkstring(L, 1);

	RoomSet* set = StageHandler::LoadBinary(&path);

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

LUA_FUNCTION(Lua_StageHandler_GetBinary) {
	const char* id = luaL_checkstring(L, 1);

	std::unordered_map<std::string, RoomSet>::const_iterator itr = StageHandler::binaryMap.find(id);

	if (itr == StageHandler::binaryMap.end()) {
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

LUA_FUNCTION(Lua_StageHandler_AppendBinary) {
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = StageHandler::LoadBinary(&path);
		if (roomSet == nullptr) {
			return luaL_error(L, "No binary exists for path \"%s\"!", path.empty() ? "" : path.c_str());
		}
	}
	else
	{
		roomSet = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	std::string filepath = luaL_checkstring(L, 2);

	lua_pushboolean(L, StageHandler::AppendBinary(roomSet, &filepath));

	return 1;
}

LUA_FUNCTION(Lua_StageHandler_IsStageOverriden) {
	int id = (int)luaL_checkinteger(L, 1);

	lua_pushboolean(L, StageHandler::stageState[id].first);
	return 1;
}

LUA_FUNCTION(Lua_StageHandler_ResetRoomWeights) {
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = StageHandler::LoadBinary(&path);
		if (roomSet == nullptr) {
			return luaL_error(L, "No binary exists for path \"%s\"!", path.empty() ? "" : path.c_str());
		}
	}
	else
	{
		roomSet = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	StageHandler::ResetRoomWeights(roomSet);

	return 0;
}

LUA_FUNCTION(Lua_StageHandler_ResetAllRoomWeights) {
	StageHandler::ResetAllRoomWeights();
	return 0;
}

LUA_FUNCTION(Lua_StageHandler_GetStageIdForToken) {
	std::string token = luaL_checkstring(L, 1);

	lua_pushinteger(L, StageHandler::GetStageIdForToken(token));
	return 1;
}

LUA_FUNCTION(Lua_StageHandler_GetTokenForStageId) {
	int id = (int)luaL_checkinteger(L, 1);
	if (id < 0 || id > 36) {
		return luaL_error(L, "id must be between 0 and 36, both inclusive (got %d)", id);
	}
	std::string* res = StageHandler::GetTokenForStageId(id);
	if (res->empty())
		lua_pushnil(L);
	else
		lua_pushstring(L, res->c_str());

	return 1;
}

static void RegisterStageHandler(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "LoadStage", Lua_StageHandler_LoadStage);
	lua::TableAssoc(L, "RestoreStage", Lua_StageHandler_RestoreStage);
	lua::TableAssoc(L, "LoadBinary", Lua_StageHandler_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_StageHandler_GetBinary);
	lua::TableAssoc(L, "AppendBinary", Lua_StageHandler_AppendBinary);
	lua::TableAssoc(L, "ResetRoomWeights", Lua_StageHandler_ResetRoomWeights);
	lua::TableAssoc(L, "ResetAllRoomWeights", Lua_StageHandler_ResetAllRoomWeights);
	lua::TableAssoc(L, "IsStageOverriden", Lua_StageHandler_IsStageOverriden);
	lua::TableAssoc(L, "GetStageIdForToken", Lua_StageHandler_GetStageIdForToken);
	lua::TableAssoc(L, "GetTokenForStageId", Lua_StageHandler_GetTokenForStageId);
	lua_setglobal(L, "StageHandler");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterStageHandler(_state);
}