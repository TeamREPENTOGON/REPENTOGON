#pragma once
#include "IsaacRepentance.h"
#include <unordered_map>
#include <array>
#include <tuple>
#include <filesystem>

namespace fs = std::filesystem;

struct StageState {
	bool overriden = false;
	unsigned short id = 0;
	std::string token;
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

	const unsigned int BUFFER_STAGEID = 23;
	std::unordered_map<std::string, RoomSet> binaryMap;
	std::unordered_map<std::string, size_t> filenameMap;
	StageState stageState[37];
	RoomSet* LoadBinary(std::string* path);
	RoomSet* GetBinary(std::string* path, bool loadIfUncached);
	bool AppendBinary(RoomSet* roomSet, std::string& binary);
	bool IsBinaryLoaded(std::string* path);
	bool SwapStage(int stageId, const char* stageName, bool restoring);
	void ResetRoomWeights(RoomSet* set);
	void ResetAllRoomWeights();
	int GetStageIdForToken(std::string& token);
	std::string* GetTokenForStageId(int stageId);
	void BuildFilenameMap();
};