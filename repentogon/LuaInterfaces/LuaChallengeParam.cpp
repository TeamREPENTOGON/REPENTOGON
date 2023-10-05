#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ChallengeParamMT = "ChallengeParam";

static int Lua_GameGetChallengeParams(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ChallengeParam** ud = (ChallengeParam**)lua_newuserdata(L, sizeof(ChallengeParam*));
	*ud = game->GetChallengeParams();
	luaL_setmetatable(L, ChallengeParamMT);
	return 1;
}

static int Lua_ChallengeParamGetName(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static int Lua_ChallengeParamGetRoomSet(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	std::set<int> roomSet = (std::set<int>)challengeParam->_roomset;

	lua_newtable(L);
	int idx = 1;
	for (int room : roomSet) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, room);
		lua_settable(L, -3);
		idx++;
	}
	
 	//lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static int Lua_ChallengeParamGetCollectibleList(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	std::vector<int> collectibleSet = (std::vector<int>)challengeParam->_collectibleList;

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collectibleSet) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}

	//lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static int Lua_ChallengeParamGetTrinketList(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	std::vector<int> trinketSet = (std::vector<int>)challengeParam->_trinketList;

	lua_newtable(L);
	int idx = 1;
	for (int trinket : trinketSet) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, trinket);
		lua_settable(L, -3);
		idx++;
	}

	//lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static int Lua_ChallengeParamGetStartingPill(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_startingPill);

	return 1;
}

static int Lua_ChallengeParamGetStartingCard(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_startingCard);

	return 1;
}

static int Lua_ChallengeParamGetEndStage(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_endStage);

	return 1;
}

static int Lua_ChallengeParamGetPlayerType(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_playerType);

	return 1;
}

static int Lua_ChallengeParamGetAchievementList(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	std::vector<int> achievementList = (std::vector<int>)challengeParam->_achievementList;

	lua_newtable(L);
	int idx = 1;
	for (int achievement : achievementList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, achievement);
		lua_settable(L, -3);
		idx++;
	}

	//lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static int Lua_ChallengeParamGetSoulHearts(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_soulHearts);

	return 1;
}

static int Lua_ChallengeParamGetBlackHearts(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_blackHearts);

	return 1;
}

static int Lua_ChallengeParamGetHearts(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_hearts);

	return 1;
}

static int Lua_ChallengeParamGetMaxHearts(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_maxHearts);

	return 1;
}

static int Lua_ChallengeParamGetCoins(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_coins);

	return 1;
}

static int Lua_ChallengeParamGetAddDamage(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushnumber(L, challengeParam->_addDamage);

	return 1;
}

static int Lua_ChallengeParamCanShoot(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_canShoot);

	return 1;
}

static int Lua_ChallengeParamIsAltPath(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isAltPath);

	return 1;
}

static int Lua_ChallengeParamGetDifficulty(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_difficulty);

	return 1;
}

static int Lua_ChallengeParamIsMegaSatanRun(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isMegaSatan);

	return 1;
}

static int Lua_ChallengeParamGetMinFireRate(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushnumber(L, challengeParam->_minFireRate);

	return 1;
}

static int Lua_ChallengeParamIsMaxDamageEnabled(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_maxDamage);

	return 1;
}

static int Lua_ChallengeParamIsMinShotSpeedEnabled(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_minShotSpeed);

	return 1;
}

static int Lua_ChallengeParamIsBigRangeEnabled(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_bigRange);

	return 1;
}

static int Lua_ChallengeParamGetCurse(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_getCurse);

	return 1;
}

static int Lua_ChallengeParamGetCurseFilter(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_curseFilter);

	return 1;
}

static int Lua_ChallengeParamIsSecretPath(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isSecretPath);

	return 1;
}

static int Lua_ChallengeParamGetCollectibleTwinList(lua_State* L)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, ChallengeParamMT);
	std::vector<int> collectibleList = (std::vector<int>)challengeParam->_collectibleTwinList;

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collectibleList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}

	//lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

static void RegisterChallengeParam(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetChallengeParams");
	lua_pushcfunction(L, Lua_GameGetChallengeParams);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, ChallengeParamMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetName", Lua_ChallengeParamGetName },
		{ "GetRoomFilter", Lua_ChallengeParamGetRoomSet },
		{ "GetCollectibleList", Lua_ChallengeParamGetCollectibleList },
		{ "GetTrinketList", Lua_ChallengeParamGetTrinketList },
		{ "GetStartingPill", Lua_ChallengeParamGetStartingPill },
		{ "GetStartingCard", Lua_ChallengeParamGetStartingCard },
		{ "GetEndStage", Lua_ChallengeParamGetEndStage },
		{ "GetPlayerType", Lua_ChallengeParamGetPlayerType },
		{ "GetAchievementList", Lua_ChallengeParamGetAchievementList },
		{ "GetSoulHearts", Lua_ChallengeParamGetSoulHearts },
		{ "GetBlackHearts", Lua_ChallengeParamGetBlackHearts },
		{ "GetHearts", Lua_ChallengeParamGetHearts },
		{ "GetMaxHearts", Lua_ChallengeParamGetMaxHearts },
		{ "GetCoins", Lua_ChallengeParamGetCoins },
		{ "GetAddDamage", Lua_ChallengeParamGetAddDamage },
		{ "CanShoot", Lua_ChallengeParamCanShoot },
		{ "IsAltPath", Lua_ChallengeParamIsAltPath },
		{ "GetDifficulty", Lua_ChallengeParamGetDifficulty },
		{ "IsMegaSatanRun", Lua_ChallengeParamIsMegaSatanRun },
		{ "GetMinFireRate", Lua_ChallengeParamGetMinFireRate },
		{ "IsMaxDamageEnabled", Lua_ChallengeParamIsMaxDamageEnabled },
		{ "IsMinShotSpeedEnabled", Lua_ChallengeParamIsMinShotSpeedEnabled },
		{ "IsBigRangeEnabled", Lua_ChallengeParamIsBigRangeEnabled },
		{ "GetCurse", Lua_ChallengeParamGetCurse },
		{ "GetCurseFilter", Lua_ChallengeParamGetCurseFilter },
		{ "IsSecretPath", Lua_ChallengeParamIsSecretPath },
		{ "GetCollectibleTwinList", Lua_ChallengeParamGetCollectibleTwinList },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterChallengeParam(state);
}