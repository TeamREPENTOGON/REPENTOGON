#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetDailyChallenge) {
	Manager* manager = g_Manager;
	DailyChallenge** ud = (DailyChallenge**)lua_newuserdata(L, sizeof(DailyChallenge*));
	*ud = manager->GetDailyChallenge();
	luaL_setmetatable(L, lua::metatables::DailyChallengeMT);
	return 1;
}

LUA_FUNCTION(Lua_DailyChallengeGetChallengeParams)
{
	DailyChallenge* dailyChallenge = g_Manager->GetDailyChallenge();
	ChallengeParam** ud = (ChallengeParam**)lua_newuserdata(L, sizeof(ChallengeParam*));
	*ud = dailyChallenge->GetChallengeParams();
	luaL_setmetatable(L, lua::metatables::ChallengeParamMT);
	return 1;
}

static void RegisterDailyChallenge(lua_State* L)
{
	//lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetDailyChallenge", Lua_GetDailyChallenge);
	lua_newtable(L);
	//luaL_Reg functions[] = {
		lua::TableAssoc(L, "GetChallengeParams", Lua_DailyChallengeGetChallengeParams);
		//{ NULL, NULL }
	//};
	//lua::RegisterNewClass(L, lua::metatables::DailyChallengeMT, lua::metatables::DailyChallengeMT, functions);
	lua_setglobal(L, "DailyChallenge");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterDailyChallenge(_state);
}