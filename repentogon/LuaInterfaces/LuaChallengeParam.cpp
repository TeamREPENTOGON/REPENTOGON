#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

MOD_EXPORT int L_ChallengeParam_GetRoomFilterSize(ChallengeParam* challengeParam) {
	return (int)challengeParam->_roomset.size();
}

MOD_EXPORT void L_ChallengeParam_GetRoomFilter(ChallengeParam* challengeParam, int* out) {
	for (int room : challengeParam->_roomset) {
		*out++ = room;
	}
}
