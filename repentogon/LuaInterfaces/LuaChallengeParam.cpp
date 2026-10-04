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

LUA_FUNCTION(Lua_GameGetChallengeParams)
{
	Game* game = lua::GetLuabridgeUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	LuaChallengeParam::PushPtr(L, game->GetChallengeParams());
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterFunction(_state, lua::Metatables::GAME, "GetChallengeParams", Lua_GameGetChallengeParams);
}
