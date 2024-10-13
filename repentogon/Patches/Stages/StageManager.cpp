#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageManager.h"
#include "suffixes.h"

#include <filesystem>
#include <unordered_map>
#include <sstream>

RoomSet* StageManager::LoadBinary(std::string* path) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::LoadBinary: Loading stage binary \"%s\"\n", path->c_str());

	RoomSet newSet;
	RoomConfig_Stage* buffer = roomConfig->_stages + BUFFER_STAGEID;
	newSet._filepath = *path;
	buffer->_rooms[0] = newSet;

	// to look better in the log
	buffer->_displayName = "(binary) \"" + *path + "\"";

	bool res = roomConfig->LoadStageBinary(BUFFER_STAGEID, 0);

	if (res) {
		RoomSet* ret = &(binaryMap.find(*path)->second);
		logger.Log("[INFO] StageManager::LoadBinary: Loaded successfully at pointer %p\n", ret);
		return ret;
	}

	logger.Log("[WARNING] StageManager::LoadBinary: Failed to load binary \"%s\"!\n", path->c_str());
	return nullptr;
};

RoomSet* StageManager::GetBinary(std::string* path, bool loadIfUncached) {
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::GetBinary: Attempting to retrieve binary \"%s\" from cache\n", path->c_str());

	std::unordered_map<std::string, RoomSet>::iterator itr = binaryMap.find(*path);

	if (itr != binaryMap.end()) {
		RoomSet* ret = &(itr->second);
		logger.Log("[INFO] StageManager::GetBinary: Retrieved successfully from pointer %p\n", ret);
		return ret;
	}

	logger.Log("[INFO] StageManager::GetBinary: Binary is not in cache, %s\n", loadIfUncached ? "loading" : "returning nullptr");

	if (loadIfUncached) {
		return LoadBinary(path);
	}
	return nullptr;
};

bool StageManager::IsBinaryLoaded(std::string* path) {
	std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(*path);
	return itr != binaryMap.end();
};

bool StageManager::SwapStage(int stageId, const char* stageName, bool restoring) {
	// i'm doing xmlData this way to avoid adding an XMLData.h include to StageManager.h (and thus all other cpp files that use it)
	XMLAttributes xmlData = (stageName != nullptr ? XMLStuff.StageData->GetNodeByName(stageName) : XMLStuff.StageData->GetNodeById(stageId));
	int comp = toint(restoring ? xmlData["id"] : xmlData["basestage"]);

	ZHL::Logger logger(true);
	/*
	if (comp == 0) {
		logger.Log("[ERROR] StageConfig::SwapStage: no XML data for stage %s!\n", stageName);
	}
	*/

	if ((!restoring && !stageState[comp].overriden) || (restoring && stageState[comp].overriden))
	{
		RoomConfig* roomConfig = g_Game->GetRoomConfig();
		std::string binary = xmlData["root"] + xmlData["path"];
		std::string greedBinary = xmlData["greedroot"] + xmlData["path"];
		std::string gfxRoot = xmlData["bossgfxroot"];
		std::string playerSpot = gfxRoot + xmlData["playerspot"];
		std::string bossSpot = gfxRoot + xmlData["bossspot"];
		std::string displayName = xmlData["displayname"].empty() ? xmlData["name"] : xmlData["displayname"];
		std::string suffix = xmlData["suffix"];
		std::string name = (restoring ? tokens[comp] : xmlData["name"]);
		int musicId = toint(xmlData["music"]);
		int backdropId = toint(xmlData["backdrop"]);

		logger.Log("[INFO] StageManager::SwapStage: name %s, id %d, path %s, greed path %s, gfx path %s, playerSpot %s, bossSpot %s, suffix %s, musicId %d, backDropId %d\n", name.c_str(), stageId, binary.c_str(), greedBinary.c_str(), gfxRoot.c_str(), playerSpot.c_str(), bossSpot.c_str(), suffix.c_str(), musicId, backdropId);

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
				itr = binaryMap.find(stage->_rooms[i]._filepath);
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
				RoomSet* baseSet = LoadBinary(&stage->_rooms[i]._filepath);
				if (baseSet != nullptr)
					binaryMap.insert({ stage->_rooms[i]._filepath, *baseSet });

			}
			*/

			RoomSet* set = GetBinary(path, false);
			if (set != nullptr) {
				logger.Log("[INFO] replacing with existing RoomSet \"%s\"\n", set->_filepath.c_str());
				stage->_rooms[i] = *set;
				success = true;
			}
			else
			{
				logger.Log("[INFO] loading new binary \"%s\" for RoomSet replacement\n", path->c_str());
				RoomSet* newBinary = LoadBinary(path);
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

			stageState[stageId].overriden = !stageState[stageId].overriden;
			stageState[stageId].id = toint(xmlData["id"]);
			stageState[stageId].token = name;
			logger.Log("[INFO] StageManager::SwapStage: successfully assigned %s to id %d\n", name.c_str(), stageId);
			return true;
		}
		else
		{
			logger.Log("[ERROR] StageManager::SwapStage: could not replace the RoomSet of either mode!\n");
		}
	}
	else
	{
		logger.Log("[INFO] StageManager::SwapStage: nothing to restore for id %d\n", comp);
	}
	return false;
};

// TODO: mark this binary as appended to the input RoomSet so subsequent attempts can be cancelled
bool StageManager::AppendBinary(RoomSet* roomSet, std::string* binary) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	ModManager* modManager = g_Manager->GetModManager();
	roomConfig->_stages[BUFFER_STAGEID]._id = BUFFER_STAGEID;
	RoomSet* buffer = &roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
	unsigned int roomCount = buffer->_count;
	buffer->_filepath = *binary;
	modManager->UpdateRooms(BUFFER_STAGEID, 0);
	buffer->_filepath = roomSet->_filepath;
	*roomSet = *buffer;
	return roomCount < buffer->_count;
}

void StageManager::ResetRoomWeights(RoomSet* set) {
	for (unsigned int i = 0; i < set->_count; i++) {
		set->_configs[i].Weight = set->_configs[i].InitialWeight;
	}
};

void StageManager::ResetAllRoomWeights() {
	for (auto i = binaryMap.begin(); i != binaryMap.end(); i++) {
		ResetRoomWeights(&i->second);
	}
}

int StageManager::GetStageIdForToken(std::string token) {
	for (unsigned int i = 0; i < 37; i++) {
		if (stageState[i].token == token)
			return i;
	}
	return -1;
}

std::string* StageManager::GetTokenForStageId(int id) {
	return &stageState[id].token;
}

namespace fs = std::filesystem;

static bool CheckBlacklisted(const fs::path& name, const std::array<const char*, 32>& blacklist)
{
	for (const auto& blacklisted : blacklist) {
		ZHL::Log("Checking %s against %s\n", name.string().c_str(), blacklisted);
		if (name.string() == blacklisted) {
			return true;
		}
	}
	return false;
}

std::unordered_map<std::string, size_t> StageManager::GetFilesWithExtension(const fs::path& directory, const std::wstring& extension, size_t& idCounter) {
	std::unordered_map<std::string, size_t> matchingFiles;

	for (const auto& entry : fs::recursive_directory_iterator(directory)) {
		if (entry.is_regular_file() && entry.path().extension() == extension) {
			if (!CheckBlacklisted(entry.path().filename(), vanillaStbs))
				matchingFiles.insert({ fs::relative(entry.path(), directory.parent_path()).string(), idCounter++ });
		}
	}

	return matchingFiles;
}

void StageManager::ParseModsDirectory() {
	vector_ModEntryPointer mods = g_Manager->GetModManager()->_mods;
	size_t idCounter = 0;

	for (const auto& entry : mods) {
		if (entry->_enabled) {
			fs::path resourcesRoomsPath = (fs::path(entry->_resourcesDirectory) / "rooms");
			if (fs::exists(resourcesRoomsPath) && fs::is_directory(resourcesRoomsPath)) {
				std::unordered_map<std::string, size_t> files = GetFilesWithExtension(resourcesRoomsPath, L".stb", idCounter);
				filenameMap.merge(files);
			}
		}
	}
}

HOOK_METHOD(ModManager, LoadConfigs, () -> void) {
	super();
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ParseModsDirectory();
	for (const auto& entry : stageManager.filenameMap) {
		ZHL::Log("Found file %s with id %d\n", entry.first.c_str(), entry.second);
	}
}

// Handle RoomSet cacheing
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int id, unsigned int mode) -> bool) {
	RoomConfig_Stage* stage = &this->_stages[id];
	RoomSet* set = &stage->_rooms[mode];
	std::string* path = &set->_filepath;
	StageManager& stageManager = StageManager::GetInstance();

	// if we already loaded this binary, use the cached version
	std::unordered_map<std::string, RoomSet>::const_iterator itr = stageManager.binaryMap.find(*path);
	if (itr != stageManager.binaryMap.end()) {
		stringstream message;
		message << "[RoomConfig] stage " << id << ": " << stage->_displayName << " (mode " << mode << ") already loaded from binary \"" << set->_filepath << "\"\n";
		KAGE::LogMessage(0, message.str().c_str());
		*set = itr->second;
		return true;
	}
	bool res = super(id, mode);

	// cache the loaded binary
	if (res) {
		stageManager.binaryMap.insert({ *path, *set });
		if (stage->_suffix.empty()) {
			stage->_suffix = suffixes[id];
		}
		if (stageManager.stageState[id].token.empty()) {
			stageManager.stageState[id].id = id;
			stageManager.stageState[id].token = tokens[id];
		}
	}

	return res;
}

HOOK_METHOD(RoomConfig_Stage, unload, () -> void) {
	StageManager& stageManager = StageManager::GetInstance();
	std::unordered_map<std::string, RoomSet>::const_iterator itr;
	bool restored = false;
	for (unsigned int i = 0; i < 2; i++) {
		itr = stageManager.binaryMap.find(this->_rooms[i]._filepath);
		if (itr != stageManager.binaryMap.end()) {
			stageManager.binaryMap.erase(itr);
			stringstream message;
			message << "removed binary \"" << this->_rooms[i]._filepath << "\" from StageManager cache.\n";
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
		int musicId = toint(xmlData["music"]);
		int backdropId = toint(xmlData["backdrop"]);

		this->_backdrop = backdropId;
		this->_displayName = displayName;
		this->_playerSpot = playerSpot;
		this->_bossSpot = bossSpot;
		this->_suffix = suffix;
		this->_musicId = musicId;

		super();

		this->_rooms[0]._filepath = binary;
		this->_rooms[1]._filepath = greedBinary;

		stageManager.stageState[this->_id].overriden = false;
		stageManager.stageState[this->_id].id = 0;
		stageManager.stageState[this->_id].token = tokens[this->_id];

		return;
	}

	super();
}

HOOK_STATIC(FXLayers, check_fxlayer_match, (int stage, int altStages, int compLevelStage, int compStageType) -> bool, __cdecl) {
	if (stage >= 41) {
		return stage == compLevelStage;
	}
	return super(stage, altStages, compLevelStage, compStageType);
}

HOOK_METHOD(Game, Start, (int playertype, int challenge, Seeds seeds, unsigned int difficulty) -> void) {
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ResetAllRoomWeights();
	super(playertype, challenge, seeds, difficulty);
}
HOOK_METHOD(Game, NetStart, (void* unk, int challenge, Seeds seed, unsigned int difficulty, GameState* state) -> void) {
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ResetAllRoomWeights();
	super(unk, challenge, seed, difficulty, state);
}
HOOK_METHOD(Game, StartDebug, (int levelStage, int stageType, int difficulty, std_string* filepath) -> void) {
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ResetAllRoomWeights();
	super(levelStage, stageType, difficulty, filepath);
}
HOOK_METHOD(Game, StartFromRerunState, (GameState* state) -> void) {
	StageManager& stageManager = StageManager::GetInstance();
	stageManager.ResetAllRoomWeights();
	super(state);
}

/////
// Start of lua binders
/////

LUA_FUNCTION(Lua_StageManager_LoadStage) {
	StageManager& stageManager = StageManager::GetInstance();
	const char* name = luaL_checkstring(L, 1);
	XMLAttributes xmlData = XMLStuff.StageData->GetNodeByName(name);
	if (xmlData["basestage"].empty())
		return luaL_error(L, "No basestage for stage name %s", name);

	lua_pushboolean(L, stageManager.SwapStage(toint(xmlData["basestage"]), name, false));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_RestoreStage) {
	StageManager& stageManager = StageManager::GetInstance();
	int id = (int)luaL_checkinteger(L, 1);
	if (id < 0 || id > 36) {
		return luaL_error(L, "id must be between 0 and 36, both inclusive (got %d)", id);
	}

	lua_pushboolean(L, stageManager.SwapStage(id, nullptr, true));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_LoadBinary) {
	StageManager& stageManager = StageManager::GetInstance();
	std::string path = luaL_checkstring(L, 1);

	RoomSet* set = stageManager.GetBinary(&path, true);

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

LUA_FUNCTION(Lua_StageManager_AppendBinary) {
	StageManager& stageManager = StageManager::GetInstance();
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = stageManager.GetBinary(&path, true);
		if (roomSet == nullptr) {
			return luaL_error(L, "No binary exists for path \"%s\"!", path.empty() ? "" : path.c_str());
		}
	}
	else
	{
		roomSet = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	}
	std::string filepath = luaL_checkstring(L, 2);

	lua_pushboolean(L, stageManager.AppendBinary(roomSet, &filepath));

	return 1;
}

LUA_FUNCTION(Lua_StageManager_IsStageOverriden) {
	StageManager& stageManager = StageManager::GetInstance();
	int id = (int)luaL_checkinteger(L, 1);

	lua_pushboolean(L, stageManager.stageState[id].overriden);
	return 1;
}

LUA_FUNCTION(Lua_StageManager_ResetRoomWeights) {
	StageManager& stageManager = StageManager::GetInstance();
	RoomSet* roomSet;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		std::string path = luaL_checkstring(L, 1);
		roomSet = stageManager.GetBinary(&path, false);
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

LUA_FUNCTION(Lua_StageManager_GetStageIdForToken) {
	StageManager& stageManager = StageManager::GetInstance();
	std::string token = luaL_checkstring(L, 1);

	lua_pushinteger(L, stageManager.GetStageIdForToken(token));
	return 1;
}

LUA_FUNCTION(Lua_StageManager_GetTokenForStageId) {
	StageManager& stageManager = StageManager::GetInstance();
	int id = (int)luaL_checkinteger(L, 1);
	if (id < 0 || id > 36) {
		return luaL_error(L, "id must be between 0 and 36, both inclusive (got %d)", id);
	}
	std::string* res = stageManager.GetTokenForStageId(id);
	if (res->empty())
		lua_pushnil(L);
	else
		lua_pushstring(L, res->c_str());

	return 1;
}

static void RegisterStageManager(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "LoadStage", Lua_StageManager_LoadStage);
	lua::TableAssoc(L, "RestoreStage", Lua_StageManager_RestoreStage);
	lua::TableAssoc(L, "LoadBinary", Lua_StageManager_LoadBinary);
	lua::TableAssoc(L, "GetBinary", Lua_StageManager_GetBinary);
	lua::TableAssoc(L, "AppendBinary", Lua_StageManager_AppendBinary);
	lua::TableAssoc(L, "ResetRoomWeights", Lua_StageManager_ResetRoomWeights);
	lua::TableAssoc(L, "ResetAllRoomWeights", Lua_StageManager_ResetAllRoomWeights);
	lua::TableAssoc(L, "IsStageOverriden", Lua_StageManager_IsStageOverriden);
	lua::TableAssoc(L, "GetStageIdForToken", Lua_StageManager_GetStageIdForToken);
	lua::TableAssoc(L, "GetTokenForStageId", Lua_StageManager_GetTokenForStageId);
	lua_setglobal(L, "StageManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterStageManager(_state);
}