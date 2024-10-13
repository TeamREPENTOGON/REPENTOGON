#pragma once
#include "IsaacRepentance.h"
#include <unordered_map>
#include <tuple>

struct StageState {
	bool overriden = false;
	unsigned short id = 0;
	std::string token;
};

class StageManager
{
private:
	StageManager() {}

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
	bool AppendBinary(RoomSet* roomSet, std::string* binary);
	bool IsBinaryLoaded(std::string* path);
	bool SwapStage(int stageId, const char* stageName, bool restoring);
	void ResetRoomWeights(RoomSet* set);
	void ResetAllRoomWeights();
	int GetStageIdForToken(std::string token);
	std::string* GetTokenForStageId(int stageId);
	void ParseModsDirectory();
};