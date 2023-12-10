#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GameGetChallengeParams)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ChallengeParam** ud = (ChallengeParam**)lua_newuserdata(L, sizeof(ChallengeParam*));
	*ud = game->GetChallengeParams();
	luaL_setmetatable(L, lua::metatables::ChallengeParamMT);
	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetName)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetRoomSet)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
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

LUA_FUNCTION(Lua_ChallengeParamGetCollectibleList)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
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

LUA_FUNCTION(Lua_ChallengeParamGetTrinketList)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
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

LUA_FUNCTION(Lua_ChallengeParamGetStartingPill)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_startingPill);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetStartingCard)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_startingCard);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetEndStage)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_endStage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetPlayerType)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_playerType);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetAchievementList)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
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

LUA_FUNCTION(Lua_ChallengeParamGetSoulHearts)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_soulHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetBlackHearts)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_blackHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetHearts)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_hearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetMaxHearts)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_maxHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCoins)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_coins);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetAddDamage)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushnumber(L, challengeParam->_addDamage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamCanShoot)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_canShoot);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsAltPath)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isAltPath);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetDifficulty)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_difficulty);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMegaSatanRun)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isMegaSatan);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetMinFireRate)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushnumber(L, challengeParam->_minFireRate);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMaxDamageEnabled)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_maxDamage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMinShotSpeedEnabled)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_minShotSpeed);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsBigRangeEnabled)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_bigRange);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCurse)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_getCurse);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCurseFilter)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushinteger(L, challengeParam->_curseFilter);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsSecretPath)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
	lua_pushboolean(L, challengeParam->_isSecretPath);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCollectibleTwinList)
{
	ChallengeParam* challengeParam = *lua::GetUserdata<ChallengeParam**>(L, 1, lua::metatables::ChallengeParamMT);
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
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetChallengeParams", Lua_GameGetChallengeParams);

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
	lua::RegisterNewClass(L, lua::metatables::ChallengeParamMT, lua::metatables::ChallengeParamMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterChallengeParam(_state);
}