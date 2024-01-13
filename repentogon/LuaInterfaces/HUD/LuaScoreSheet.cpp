#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetScoreSheet) {
	Game* game = g_Game;
	ScoreSheet** ud = (ScoreSheet**)lua_newuserdata(L, sizeof(ScoreSheet*));
	*ud = game->GetScoreSheet();
	luaL_setmetatable(L, lua::metatables::ScoreSheetMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_ScoreSheetCalculate)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	scoreSheet->Calculate();

	return 0;
}

LUA_FUNCTION(Lua_ScoreSheetGetStageBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_stageBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetSchwagBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_schwagBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetBlueBabyBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_blueBabyBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetLambBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_lambBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetMegaSatanBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_megaSatanBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRushBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_rushBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetExplorationBonus)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_explorationBonus);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetDamagePenalty)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_damagePenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetTimePenalty)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_timePenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetItemPenalty)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_itemPenalty);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetTotalScore)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_totalScore);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTimeLevel)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_runTimeLevel);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTimeLevelType)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_runTimeLevelType);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunTime)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_runTime);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetGetRunEnding)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	lua_pushinteger(L, scoreSheet->_runEnding);

	return 1;
}

LUA_FUNCTION(Lua_ScoreSheetSetRunEnding)
{
	ScoreSheet* scoreSheet = g_Game->GetScoreSheet();
	scoreSheet->_runEnding = (int)luaL_checkinteger(L, 1);

	return 0;
}

static void RegisterScoreSheet(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetScoreSheet", Lua_GetScoreSheet);
	lua_newtable(L);
	//luaL_Reg functions[] = {
		lua::TableAssoc(L, "Calculate", Lua_ScoreSheetCalculate );
		lua::TableAssoc(L, "GetStageBonus", Lua_ScoreSheetGetStageBonus );
		lua::TableAssoc(L, "GetSchwagBonus", Lua_ScoreSheetGetSchwagBonus );
		lua::TableAssoc(L, "GetBlueBabyBonus", Lua_ScoreSheetGetBlueBabyBonus );
		lua::TableAssoc(L, "GetLambBonus", Lua_ScoreSheetGetLambBonus );
		lua::TableAssoc(L, "GetMegaSatanBonus", Lua_ScoreSheetGetMegaSatanBonus );
		lua::TableAssoc(L, "GetRushBonus", Lua_ScoreSheetGetRushBonus );
		lua::TableAssoc(L, "GetExplorationBonus", Lua_ScoreSheetGetExplorationBonus );
		lua::TableAssoc(L, "GetDamagePenalty", Lua_ScoreSheetGetDamagePenalty );
		lua::TableAssoc(L, "GetTimePenalty", Lua_ScoreSheetGetTimePenalty );
		lua::TableAssoc(L, "GetItemPenalty", Lua_ScoreSheetGetItemPenalty );
		lua::TableAssoc(L, "GetTotalScore", Lua_ScoreSheetGetTotalScore );
		lua::TableAssoc(L, "GetRunTimeLevel", Lua_ScoreSheetGetRunTimeLevel );
		lua::TableAssoc(L, "GetRunTimeLevelType", Lua_ScoreSheetGetRunTimeLevelType );
		lua::TableAssoc(L, "GetRunTime", Lua_ScoreSheetGetRunTime );
		lua::TableAssoc(L, "GetRunEnding", Lua_ScoreSheetGetRunEnding );
		lua::TableAssoc(L, "SetRunEnding", Lua_ScoreSheetSetRunEnding );
		//{ NULL, NULL }
	//};
	//lua::RegisterNewClass(L, lua::metatables::ScoreSheetMT, lua::metatables::ScoreSheetMT, functions);
	lua_setglobal(L, "ScoreSheet");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterScoreSheet(_state);
}