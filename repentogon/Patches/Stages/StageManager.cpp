#include "IsaacRepentance.h"
#include "Log.h"

#include "../repentogon/Patches/XMLData.h"
#include "StageManager.h"
#include "suffixes.h"

#include <filesystem>
#include <unordered_map>
#include <sstream>

/**
 * Loads a stage binary from the specified path (relative to resources directory).
 *
 * @param path the file path of the stage binary to load
 *
 * @return a pointer to the loaded RoomSet, or nullptr if loading fails
 */
RoomSet* StageManager::LoadBinary(std::string& path) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::LoadBinary: Loading stage binary \"%s\"\n", path.c_str());

	RoomSet newSet;
	RoomConfig_Stage* buffer = roomConfig->_stages + BUFFER_STAGEID;
	newSet._filepath = path;
	buffer->_rooms[0] = newSet;

	// to look better in the log
	buffer->_displayName = "(binary) \"" + path + "\"";

	bool res = roomConfig->LoadStageBinary(BUFFER_STAGEID, 0);

	if (res) {
		RoomSet* ret = &(binaryMap.find(path)->second);
		logger.Log("[INFO] StageManager::LoadBinary: Loaded successfully at pointer %p\n", ret);
		return ret;
	}

	logger.Log("[WARNING] StageManager::LoadBinary: Failed to load binary \"%s\"!\n", path.c_str());
	return nullptr;
};

/**
 * Retrieves a RoomSet from the binary cache.
 *
 * @param path The file path of the binary to retrieve.
 * @param loadIfUncached If true, loads the binary from file if it is not in the cache.
 *
 * @return A pointer to the retrieved RoomSet, or nullptr if not found.
 */
RoomSet* StageManager::GetBinary(std::string& path, bool loadIfUncached) {
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::GetBinary: Attempting to retrieve binary \"%s\" from cache\n", path.c_str());

	std::unordered_map<std::string, RoomSet>::iterator itr = binaryMap.find(path);

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

/**
 * Checks if a binary is loaded in the cache.
 *
 * @param path the file path of the binary to check
 *
 * @return true if the binary is loaded, false otherwise
 */
bool StageManager::IsBinaryLoaded(std::string& path) {
	std::unordered_map<std::string, RoomSet>::const_iterator itr = binaryMap.find(path);
	return itr != binaryMap.end();
};

/**
 * Builds a StageDefinition object from the provided stageId.
 *
 * @param stageId The ID of the stage to build the definition for.
 *
 * @return A reference to the built StageDefinition object.
 */
static StageDefinition BuildStageDefinition(int stageId) {
	XMLAttributes xmlData = XMLStuff.StageData->GetNodeById(stageId);
	StageDefinition stageDefinition;

	std::string gfxRoot = xmlData["bossgfxroot"];
	stageDefinition.binary = xmlData["root"] + xmlData["path"];
	stageDefinition.greedBinary = xmlData["greedroot"] + xmlData["path"];
	stageDefinition.playerSpot = gfxRoot + xmlData["playerspot"];
	stageDefinition.bossSpot = gfxRoot + xmlData["bossspot"];
	stageDefinition.displayName = xmlData["displayname"].empty() ? xmlData["name"] : xmlData["displayname"];
	stageDefinition.suffix = xmlData["suffix"];
	stageDefinition.name = xmlData["name"];
	stageDefinition.musicId = toint(xmlData["music"]);
	stageDefinition.backdropId = toint(xmlData["backdrop"]);

	return stageDefinition;
}

/**
 * Retrieves a StageDefinition from the cache, building it if it does not exist.
 *
 * @param stageId the ID of the stage to retrieve the definition for
 *
 * @return a reference to the retrieved StageDefinition
 */
StageDefinition& StageManager::GetStageDefinition(int stageId) {
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::GetStageDefinition: Attempting to retrieve definition for id \"%d\" from cache\n", stageId);

	std::unordered_map<size_t, StageDefinition>::iterator itr = stageDefinitionMap.find(stageId);

	if (itr != stageDefinitionMap.end()) {
		StageDefinition ret = (itr->second);
		logger.Log("[INFO] StageManager::GetStageDefinition: Retrieved successfully from pointer %p\n", ret);
		return ret;
	}

	logger.Log("[INFO] StageManager::GetStageDefinition: Definition is not in cache, building\n");
	stageDefinitionMap.insert({ stageId, BuildStageDefinition(stageId) });
	return stageDefinitionMap.find(stageId)->second;
}

/**
 * Replaces the members of a RoomConfig_Stage object with the members of a StageDefinition.
 *
 * @param stage the RoomConfig_Stage object to replace the stage definition for
 * @param newDef the new StageDefinition to replace the existing one with
 *
 * @return none
 */
static void ReplaceStageDefinition(RoomConfig_Stage* stage, StageDefinition& newDef) {
	stage->_displayName = newDef.name;
	stage->_musicId = newDef.musicId;
	stage->_backdrop = newDef.backdropId;
	stage->_playerSpot = newDef.playerSpot;
	stage->_bossSpot = newDef.bossSpot;
	stage->_suffix = newDef.suffix;
}

/**
 * Loads a new stage into the current game configuration.
 *
 * This function replaces the RoomSet of the current stage with a new one,
 * either by loading a new binary or by reusing an existing one from the cache.
 * It also updates the RoomConfig_Stage object with the new stage definition.
 *
 * Setting the same id on both params will reset the RoomConfig_Stage to its default state.
 * 
 * @param configId The ID of the RoomConfig_Stage to be replaced.
 * @param newId The ID of the new stage to be loaded.
 *
 * @return True if the stage was successfully loaded or already loaded, false if no binary was able to be loaded.
 */
bool StageManager::LoadStage(int configId, int newId) {
	ZHL::Logger logger(true);
	logger.Log("[INFO] StageManager::LoadStage: Attempting to load stage id %d into config id %d\n", newId, configId);

	if (stageForConfigId[configId] == newId) {
		logger.Log("[INFO] StageManager::LoadStage: Stage id %d is already loaded into config id %d\n", newId, configId);
		return true;
	}

	RoomConfig* roomConfig = g_Game->GetRoomConfig();

	// This also builds the StageDefinition of the existing and new stages
	StageDefinition& oldDef = GetStageDefinition(configId);
	StageDefinition& newDef = GetStageDefinition(newId);

	logger.Log("[INFO] StageManager::LoadStage: [CURRENT] name %s, path %s, greed path %s, playerSpot %s, bossSpot %s, suffix %s, musicId %d, backDropId %d\n", oldDef.name.c_str(), oldDef.binary.c_str(), oldDef.greedBinary.c_str(), oldDef.playerSpot.c_str(), oldDef.bossSpot.c_str(), oldDef.suffix.c_str(), oldDef.musicId, oldDef.backdropId);
	logger.Log("[INFO] StageManager::LoadStage: [NEW] name %s, path %s, greed path %s, playerSpot %s, bossSpot %s, suffix %s, musicId %d, backDropId %d\n", newDef.name.c_str(), newDef.binary.c_str(), newDef.greedBinary.c_str(), newDef.playerSpot.c_str(), newDef.bossSpot.c_str(), newDef.suffix.c_str(), newDef.musicId, newDef.backdropId);

	RoomConfig_Stage& stage = roomConfig->_stages[configId];
	std::unordered_map<std::string, RoomSet>::const_iterator itr;
	bool success = false;
	for (size_t i = 0; i < 2; i++)
	{
		std::string& path = i ? newDef.greedBinary : newDef.binary;
		logger.Log("[INFO] Processing RoomSet \"%s\" (mode %d)\n", path.c_str(), i);
		if (stage._rooms[i]._loaded)
		{
			logger.Log("[INFO] already loaded\n");
			itr = binaryMap.find(stage._rooms[i]._filepath);
			if (itr == binaryMap.end()) {
				// somehow hasn't been inserted yet, do so
				logger.Log("[WARNING] existing RoomSet not cached yet, inserting \"%s\"\n", stage._rooms[i]._filepath.c_str());
				binaryMap.insert({ stage._rooms[i]._filepath, stage._rooms[i] });
			}
		}
		/*
		else
		{
			logger.Log("[INFO] \"%s\" not loaded yet, attempting...\n", stage._rooms[i]._filepath.c_str());
			RoomSet* baseSet = LoadBinary(&stage._rooms[i]._filepath);
			if (baseSet != nullptr)
				binaryMap.insert({ stage._rooms[i]._filepath, *baseSet });

		}
		*/

		RoomSet* set = GetBinary(path, false);
		if (set != nullptr) {
			logger.Log("[INFO] replacing with existing RoomSet \"%s\"\n", set->_filepath.c_str());
			stage._rooms[i] = *set;
			success = true;
		}
		else
		{
			logger.Log("[INFO] loading new binary \"%s\" for RoomSet replacement\n", path.c_str());
			RoomSet* newBinary = LoadBinary(path);
			if (newBinary != nullptr) {
				stage._rooms[i] = *newBinary;
				success = true;
			}
		}
	}

	if (success) {
		ReplaceStageDefinition(&stage, newDef);

		stageForConfigId[configId] = newId;
		logger.Log("[INFO] StageManager::LoadStage: successfully assigned stage id %d to config id %d\n", newId, configId);
		return true;
	}
	else
	{
		logger.Log("[ERROR] StageManager::LoadStage: could not replace the RoomSet of either mode!\n");
	}

	return false;
};

/**
 * Restores the specified stage to its default state.
 *
 * @param configId The stage to restore.
 *
 * @return True if the stage was restored successfully, false otherwise.
 */
bool StageManager::RestoreStage(int configId) {
	return LoadStage(configId, configId);
}

/**
 * Attempts to append the specified content binary to the specified RoomSet.
 *
 * @param roomSet The RoomSet to append the binary to.
 * @param binary The binary to append.
 *
 * @return True if the room count has increased after appending the binary, false otherwise.
 */
bool StageManager::AppendContentBinary(RoomSet* roomSet, std::string& binary) {
	RoomConfig* roomConfig = g_Game->GetRoomConfig();
	ModManager* modManager = g_Manager->GetModManager();
	roomConfig->_stages[BUFFER_STAGEID]._id = BUFFER_STAGEID;
	RoomSet* buffer = &roomConfig->_stages[BUFFER_STAGEID]._rooms[0];
	unsigned int roomCount = buffer->_count;
	buffer->_filepath = binary;
	// TODO: mark this binary as appended to the input RoomSet so subsequent attempts can be cancelled
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

namespace fs = std::filesystem;
/**
 * Checks if a given filename is present in a blacklist of vanilla STB files.
 *
 * @param name The filename to check.
 * @param blacklist An array of blacklisted vanilla STB filenames.
 *
 * @return True if the filename is in the blacklist, false otherwise.
 */
static bool CheckVanillaStb(const fs::path& name, const std::array<const char*, 32>& blacklist)
{
	for (const auto& blacklisted : blacklist) {
		ZHL::Log("Checking %s against %s\n", name.string().c_str(), blacklisted);
		if (name.string() == blacklisted) {
			return true;
		}
	}
	return false;
}

/**
 * Retrieves a map of stage binary files in the specified mod directory, excluding vanilla STB files.
 *
 * @param directory The directory to search for stage binary files.
 * @param idCounter A counter used to assign unique IDs to the found files.
 *
 * @return A map of file IDs to their corresponding relative file paths.
 */
std::unordered_map<std::string, size_t> StageManager::GetStageBinaryFiles(const fs::path& directory, size_t& idCounter) {
	std::unordered_map<std::string, size_t> matchingFiles;

	for (const auto& entry : fs::recursive_directory_iterator(directory)) {
		if (entry.is_regular_file() && entry.path().extension() == ".stb") {
			if (!CheckVanillaStb(entry.path().filename(), vanillaStbs))
				matchingFiles.insert({ fs::relative(entry.path(), directory.parent_path()).string(), idCounter++ });
		}
	}

	return matchingFiles;
}

/**
 * Builds a map of stage binary files from the resources directories of enabled mods.
 *
 * @param none
 *
 * @return none
 *
 * @throws none
 */
void StageManager::BuildFilenameMap() {
	vector_ModEntryPointer mods = g_Manager->GetModManager()->_mods;
	size_t idCounter = 0;

	filenameMap.clear();

	for (const auto& entry : mods) {
		if (entry->_enabled) {
			fs::path resourcesRoomsPath = (fs::path(entry->_resourcesDirectory) / "rooms");
			if (fs::exists(resourcesRoomsPath) && fs::is_directory(resourcesRoomsPath)) {
				std::unordered_map<std::string, size_t> files = GetStageBinaryFiles(resourcesRoomsPath, idCounter);
				filenameMap.merge(files);
			}
		}
	}
}

// Initialize StageManager
HOOK_METHOD(ModManager, LoadConfigs, () -> void) {
	super();
	StageManager& stageManager = StageManager::GetInstance();
	for (int i = 0; i < 37; i++) {
		stageManager.stageForConfigId[i] = i;
	}
	stageManager.BuildFilenameMap();
	for (const auto& entry : stageManager.filenameMap) {
		ZHL::Log("Found file %s with id %d\n", entry.first.c_str(), entry.second);
	}
}

// Handle RoomSet cacheing
HOOK_METHOD(RoomConfig, LoadStageBinary, (unsigned int id, unsigned int mode) -> bool) {
	RoomConfig_Stage& stage = this->_stages[id];
	RoomSet& set = stage._rooms[mode];
	std::string& path = set._filepath;
	StageManager& stageManager = StageManager::GetInstance();

	// if we already loaded this binary, use the cached version
	std::unordered_map<std::string, RoomSet>::const_iterator itr = stageManager.binaryMap.find(path);
	if (itr != stageManager.binaryMap.end()) {
		stringstream message;
		message << "[RoomConfig] stage " << id << ": " << stage._displayName << " (mode " << mode << ") already loaded from binary \"" << set._filepath << "\"\n";
		KAGE::LogMessage(0, message.str().c_str());
		set = itr->second;
		return true;
	}
	bool res = super(id, mode);

	// cache the loaded binary
	if (res) {
		stringstream message;
		message << "[RoomConfig] caching binary \"" << set._filepath << "\"\n";
		KAGE::LogMessage(0, message.str().c_str());
		stageManager.binaryMap.insert({ path, set });
		if (stage._suffix.empty()) {
			stage._suffix = suffixes[id];
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
	if (restored) {
		StageDefinition &def = stageManager.GetStageDefinition(this->_id);

		ReplaceStageDefinition(this, def);

		this->_rooms[0]._filepath = def.binary;
		this->_rooms[1]._filepath = def.greedBinary;

		stageManager.stageForConfigId[this->_id] = this->_id;

		// we already handled destroying the RoomSet above, don't do it again
		return;
	}

	super();
}

// Force use our custom FXLayers if in a custom stage. A patch in ASMLevel handles setting the backdrop before this gets called.
HOOK_STATIC(FXLayers, check_fxlayer_match, (int stage, int altStages, int compLevelStage, int compStageType) -> bool, __cdecl) {
	if (stage >= 41) {
		return stage == compLevelStage;
	}
	return super(stage, altStages, compLevelStage, compStageType);
}

// Reset room weights on starting a new game
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