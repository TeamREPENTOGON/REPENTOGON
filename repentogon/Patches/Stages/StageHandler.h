#pragma once
#include "IsaacRepentance.h"
#include <unordered_map>
#include <tuple>

struct StageState {
	bool overriden = false;
	unsigned short id = 0;
	std::string token;
};

namespace StageHandler {
	extern const unsigned int BUFFER_STAGEID;
	extern std::unordered_map<std::string, RoomSet> binaryMap;
	extern StageState stageState[37];
	RoomSet* LoadBinary(std::string* path);
	RoomSet* GetBinary(std::string* path, bool loadIfUncached);
	bool IsBinaryLoaded(std::string* path);
	bool SwapStage(int stageId, const char* stageName, bool restoring);
	void ResetRoomWeights(RoomSet* set);
	void ResetAllRoomWeights();
	int GetStageIdForToken(std::string token);
	std::string* GetTokenForStageId(int stageId);
};