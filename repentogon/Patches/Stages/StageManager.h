#pragma once
#include "IsaacRepentance.h"
#include <unordered_map>
#include <array>
#include <tuple>
#include <filesystem>

namespace fs = std::filesystem;

struct StageDefinition {
	std::string binary;
	std::string greedBinary;
	std::string gfxRoot;
	std::string playerSpot;
	std::string bossSpot;
	std::string displayName;
	std::string suffix;
	std::string name;
	int musicId;
	int backdropId;
};

class StageManager
{
private:

	StageManager() {}
	std::unordered_map<std::string, size_t> GetStageBinaryFiles(const fs::path&, size_t&);
	const std::array<const char*, 32> vanillaStbs = {
		"00.special rooms.stb",
		"01.basement.stb",
		"02.cellar.stb",
		"03.burning basement.stb",
		"04.caves.stb",
		"05.catacombs.stb",
		"06.flooded caves.stb",
		"07.depths.stb",
		"08.necropolis.stb",
		"09.dank depths.stb",
		"10.womb.stb",
		"11.utero.stb",
		"12.scarred womb.stb",
		"13.blue womb.stb",
		"14.sheol.stb",
		"15.cathedral.stb",
		"16.dark room.stb",
		"17.chest.stb",
		"24.the shop.stb",
		"25.ultra greed.stb",
		"26.the void.stb",
		"26.the void_ex.stb",
		"27.downpour.stb",
		"28.dross.stb",
		"29.mines.stb",
		"30.ashpit.stb",
		"31.mausoleum.stb",
		"32.gehenna.stb",
		"33.corpse.stb",
		"34.mortis.stb",
		"35.home.stb",
		"36.backwards.stb"
	};


public:
	static StageManager& GetInstance() {
		static StageManager instance;
		return instance;
	}

	// New binaries are loaded into this unused RoomConfig_Stage id
	const unsigned int BUFFER_STAGEID = 23;

	//TODO: merge these two maps

	// Associates a .stb filepath to its respective RoomSet
	std::unordered_map<std::string, RoomSet> binaryMap;

	// Associates a .stb filepath to an id for later restoration
	std::unordered_map<std::string, size_t> filenameMap;

	// Associates a stage id to a struct of definitions for its RoomConfig_Stage
	std::unordered_map<size_t, StageDefinition> stageDefinitionMap;

	// Associates the config id of a stage to its current stage id
	int stageForConfigId[37];
	inline bool IsStageOverriden(int id) const{
		return stageForConfigId[id] != id;
	}
	RoomSet* LoadBinary(std::string& path);
	RoomSet* GetBinary(std::string& path, bool loadIfUncached);
	bool AppendContentBinary(RoomSet* roomSet, std::string& binary);
	bool IsBinaryLoaded(std::string& path);
	bool LoadStage(int configId, int newId);
	void ResetRoomWeights(RoomSet* set);
	void ResetAllRoomWeights();
	StageDefinition& StageManager::GetStageDefinition(int stageId);
	void BuildFilenameMap();
};