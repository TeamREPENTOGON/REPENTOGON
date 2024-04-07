#pragma once
#include "IsaacRepentance.h"
#include <unordered_map>

namespace StageHandler {
	extern const unsigned int BUFFER_STAGEID;
	extern std::unordered_map<std::string, RoomSet> binaryMap;
	extern bool stageOverwritten[37];
	RoomSet* LoadBinary(std::string* path);
	RoomSet* GetBinary(std::string* path);
	bool IsBinaryLoaded(std::string* path);
	bool SwapStage(int stageId, const char* stageName, bool restoring);
	void ResetRoomWeights(RoomSet* set);
};