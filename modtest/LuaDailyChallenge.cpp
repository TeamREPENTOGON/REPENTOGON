#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* DailyChallengeMT = "DailyChallenge";

static int Lua_GetDailyChallenge(lua_State* L) {
	Manager* manager = g_Manager;
	DailyChallenge** ud = (DailyChallenge**)lua_newuserdata(L, sizeof(DailyChallenge*));
	*ud = manager->GetDailyChallenge();
	luaL_setmetatable(L, DailyChallengeMT);
	return 1;
}

static int Lua_DailyChallengeGetStageGoal(lua_State* L) {
	DailyChallenge* dailyChallenge = *lua::GetUserdata<DailyChallenge**>(L, 1, DailyChallengeMT);
	lua_pushinteger(L, dailyChallenge->GetStageGoal());
	return 1;
}

static int Lua_DailyChallengeIsAltPath(lua_State* L) {
	DailyChallenge* dailyChallenge = *lua::GetUserdata<DailyChallenge**>(L, 1, DailyChallengeMT);
	lua_pushboolean(L, dailyChallenge->IsAltPath());
	return 1;
}

static int Lua_DailyChallengeIsHardMode(lua_State* L) {
	DailyChallenge* dailyChallenge = *lua::GetUserdata<DailyChallenge**>(L, 1, DailyChallengeMT);
	lua_pushboolean(L, dailyChallenge->IsHardMode());
	return 1;
}

static int Lua_DailyChallengeIsMegaSatan(lua_State* L) {
	DailyChallenge* dailyChallenge = *lua::GetUserdata<DailyChallenge**>(L, 1, DailyChallengeMT);
	lua_pushboolean(L, dailyChallenge->IsMegaSatan());
	return 1;
}

static void RegisterDailyChallenge(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetDailyChallenge");
	lua_pushcfunction(L, Lua_GetDailyChallenge);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, DailyChallengeMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetStageGoal", Lua_DailyChallengeGetStageGoal },
		{ "IsAltPath", Lua_DailyChallengeIsAltPath },
		{ "IsHardMode", Lua_DailyChallengeIsHardMode },
		{ "IsMegaSatan", Lua_DailyChallengeIsMegaSatan },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterDailyChallenge(state);
}