#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageHandler.h"
#include "suffixes.h"

#include <unordered_map>
#include <sstream>

namespace StageHandler {
	const unsigned int BUFFER_STAGEID = 23;
	std::unordered_map<std::string, RoomSet> binaryMap;
	bool stageOverwritten[37] = { false };

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
		std::string comp = restoring ? xmlData["id"] : xmlData["basestage"];

		ZHL::Logger logger(true);
		if (comp.empty()) {
			logger.Log("[ERROR] StageConfig::SwapStage: no XML data for stage %s!\n", (stageName != nullptr ? stageName : std::to_string(stageId).c_str()));
		}

		if (!restoring && (comp != "" && !stageOverwritten[stoi(comp)])
			|| (comp != "" && stageOverwritten[stoi(comp)]))
		{
			RoomConfig* roomConfig = g_Game->GetRoomConfig();
			std::string binary = xmlData["root"] + xmlData["path"];
			std::string greedBinary = xmlData["greedroot"] + xmlData["path"];
			std::string gfxRoot = xmlData["bossgfxroot"];
			std::string playerSpot = gfxRoot + xmlData["playerspot"];
			std::string bossSpot = gfxRoot + xmlData["bossspot"];
			std::string displayName = xmlData["name"];
			std::string suffix = xmlData["suffix"];
			int musicId = stoi(xmlData["music"]);
			int backdropId = stoi(xmlData["backdrop"]);

			logger.Log("[INFO] StageHandler::SwapStage: id %d, path %s, greed path %s, gfx path %s, playerSpot %s, bossSpot %s, suffix %s, musicId %d, backDropId %d\n", stageId, binary.c_str(), greedBinary.c_str(), gfxRoot.c_str(), playerSpot.c_str(), suffix.c_str(), bossSpot.c_str(), musicId, backdropId);

			RoomConfig_Stage* stage = &roomConfig->_stages[stageId];
			std::unordered_map<std::string, RoomSet>::const_iterator itr;
			bool success = false;
			for (size_t i = 0; i < 2; i++)
			{
				std::string* path = i ? &greedBinary : &binary;
				// i was having trouble crashing here when trying to get the c_str of path, so fuck it. we're doing it this way
				logger.Log("[INFO] Processing RoomSet \"%s\" (mode %d)\n", path->c_str(), i);
				if (stage->_rooms[i]._loaded)
				{
					logger.Log("[INFO] already loaded\n");
					itr = StageHandler::binaryMap.find(stage->_rooms[i]._filepath);
					if (itr == binaryMap.end()) {
						// somehow hasn't been inserted yet, do so
						logger.Log("[INFO] inserting existing binary \"%s\"\n", stage->_rooms[i]._filepath.c_str());
						binaryMap.insert({ stage->_rooms[i]._filepath, stage->_rooms[i] });
					}
					else
					{
						// update with current copy
						logger.Log("[INFO] updating cache with copy in RoomConfig::Stage\n");
						itr._Ptr->_Myval.second = stage->_rooms[i];
					}
				}
				else
				{
					logger.Log("[INFO] \"%s\" not loaded yet, try loading...\n", stage->_rooms[i]._filepath.c_str());
					RoomSet* baseSet = StageHandler::LoadBinary(&stage->_rooms[i]._filepath);
					if (baseSet != nullptr)
						StageHandler::binaryMap.insert({ stage->_rooms[i]._filepath, *baseSet });
						
				}

				RoomSet* set = StageHandler::GetBinary(path);
				if (set != nullptr) {
					logger.Log("[INFO] replacing with existing RoomSet \"%s\"\n", set->_filepath.c_str());
					stage->_rooms[i] = *set;
					success = true;
				}
				else
				{
					logger.Log("[INFO] loading new binary \"%s\" to overwrite with\n", path->c_str());
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

				StageHandler::stageOverwritten[stageId] = !StageHandler::stageOverwritten[stageId];
				logger.Log("[INFO] StageHandler::SwapStage: finished sucessfully\n");
				return true;
			}
			else
			{
				logger.Log("[ERROR] StageHandler::SwapStage: could not replace the RoomSet of either mode!\n");
			}
		}
		else
		{
			logger.Log("[INFO] StageHandler::SwapStage: nothing to restore for id %s\n", comp.c_str());
		}
		return false;
	};

	void ResetRoomWeights(RoomSet* set) {
		for (unsigned int i = 0; i < set->_count; i++) {
			set->_configs[i].Weight = set->_configs[i].InitialWeight;
		}
	};
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
	if (stage->_suffix.empty()) {
		stage->_suffix = suffixes[id];
	}

	// cache the loaded binary
	if (res) {
		StageHandler::binaryMap.insert({ *path, *set });
	}

	return res;
}

HOOK_STATIC(LuaEngine, PostGameStart, (unsigned int state) -> void, __stdcall) {
	if (state == 0) {
		for (auto i = StageHandler::binaryMap.begin(); i != StageHandler::binaryMap.end(); i++) {
			StageHandler::ResetRoomWeights(&i->second);
		}
	}

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

	lua_pushboolean(L, StageHandler::SwapStage(stoi(xmlData["basestage"]), name, false));
	return 1;
}

LUA_FUNCTION(Lua_StageHandler_RestoreStage) {
	int id = (int)luaL_checkinteger(L, 1);

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

static void RegisterStageHandler(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "LoadStage", Lua_StageHandler_LoadStage);
	lua::TableAssoc(L, "RestoreStage", Lua_StageHandler_RestoreStage);
	lua::TableAssoc(L, "LoadBinary", Lua_StageHandler_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_StageHandler_GetBinary);
	lua_setglobal(L, "StageHandler");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterStageHandler(_state);
}