#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ScoreSheetMT = "ScoreSheet";

static int Lua_GetScoreSheet(lua_State* L) {
	Game* game = g_Game;
	ScoreSheet** ud = (ScoreSheet**)lua_newuserdata(L, sizeof(ScoreSheet*));
	*ud = game->GetScoreSheet();
	luaL_setmetatable(L, ScoreSheetMT);
	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetCalculate)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	scoreSheet->Calculate();

	return 0;
}

LUA_FUNCTION(Lua_ScoreSheetGetStageBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_stageBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetSchwagBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_schwagBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetBlueBabyBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_blueBabyBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetLambBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_lambBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetMegaSatanBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_megaSatanBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRushBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_rushBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetExplorationBonus)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_explorationBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetDamagePenalty)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_damagePenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetTimePenalty)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_timePenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetItemPenalty)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_itemPenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetTotalScore)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_totalScore);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTimeLevel)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_runTimeLevel);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTimeLevelType)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_runTimeLevelType);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTime)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_runTime);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunEnding)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	lua_pushinteger(L, scoreSheet->_runEnding);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetSetRunEnding)
{
	ScoreSheet* scoreSheet = *lua::GetUserdata<ScoreSheet**>(L, 1, ScoreSheetMT);
	scoreSheet->_runEnding = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterScoreSheet(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetScoreSheet");
	lua_pushcfunction(L, Lua_GetScoreSheet);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, ScoreSheetMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "Calculate", Lua_ScoreSheetCalculate },
		{ "GetStageBonus", Lua_ScoreSheetGetStageBonus },
		{ "GetSchwagBonus", Lua_ScoreSheetGetSchwagBonus },
		{ "GetBlueBabyBonus", Lua_ScoreSheetGetBlueBabyBonus },
		{ "GetLambBonus", Lua_ScoreSheetGetLambBonus },
		{ "GetMegaSatanBonus", Lua_ScoreSheetGetMegaSatanBonus },
		{ "GetRushBonus", Lua_ScoreSheetGetRushBonus },
		{ "GetExplorationBonus", Lua_ScoreSheetGetExplorationBonus },
		{ "GetDamagePenalty", Lua_ScoreSheetGetDamagePenalty },
		{ "GetTimePenalty", Lua_ScoreSheetGetTimePenalty },
		{ "GetItemPenalty", Lua_ScoreSheetGetItemPenalty },
		{ "GetTotalScore", Lua_ScoreSheetGetTotalScore },
		{ "GetRunTimeLevel", Lua_ScoreSheetGetRunTimeLevel },
		{ "GetRunTimeLevelType", Lua_ScoreSheetGetRunTimeLevelType },
		{ "GetRunTime", Lua_ScoreSheetGetRunTime },
		{ "GetRunEnding", Lua_ScoreSheetGetRunEnding },
		{ "SetRunEnding", Lua_ScoreSheetSetRunEnding },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterScoreSheet(_state);
}