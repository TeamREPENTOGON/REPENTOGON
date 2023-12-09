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
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushstring(L, challengeParam->_name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetRoomSet)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
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
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
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
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
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
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_startingPill);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetStartingCard)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_startingCard);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetEndStage)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_endStage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetPlayerType)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_playerType);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetAchievementList)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
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
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_soulHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetBlackHearts)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_blackHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetHearts)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_hearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetMaxHearts)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_maxHearts);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCoins)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_coins);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetAddDamage)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushnumber(L, challengeParam->_addDamage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamCanShoot)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_canShoot);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsAltPath)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_isAltPath);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetDifficulty)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_difficulty);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMegaSatanRun)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_isMegaSatan);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetMinFireRate)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushnumber(L, challengeParam->_minFireRate);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMaxDamageEnabled)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_maxDamage);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsMinShotSpeedEnabled)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_minShotSpeed);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsBigRangeEnabled)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_bigRange);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCurse)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_getCurse);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCurseFilter)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushinteger(L, challengeParam->_curseFilter);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamIsSecretPath)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
	lua_pushboolean(L, challengeParam->_isSecretPath);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeParamGetCollectibleTwinList)
{
	ChallengeParam* challengeParam = g_Game->GetChallengeParams();
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
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetChallengeParams", Lua_GameGetChallengeParams);
	lua_newtable(L);
		lua::TableAssoc(L, "GetName", Lua_ChallengeParamGetName );
		lua::TableAssoc(L, "GetRoomFilter", Lua_ChallengeParamGetRoomSet );
		lua::TableAssoc(L, "GetCollectibleList", Lua_ChallengeParamGetCollectibleList );
		lua::TableAssoc(L, "GetTrinketList", Lua_ChallengeParamGetTrinketList );
		lua::TableAssoc(L, "GetStartingPill", Lua_ChallengeParamGetStartingPill );
		lua::TableAssoc(L, "GetStartingCard", Lua_ChallengeParamGetStartingCard );
		lua::TableAssoc(L, "GetEndStage", Lua_ChallengeParamGetEndStage );
		lua::TableAssoc(L, "GetPlayerType", Lua_ChallengeParamGetPlayerType );
		lua::TableAssoc(L, "GetAchievementList", Lua_ChallengeParamGetAchievementList );
		lua::TableAssoc(L, "GetSoulHearts", Lua_ChallengeParamGetSoulHearts );
		lua::TableAssoc(L, "GetBlackHearts", Lua_ChallengeParamGetBlackHearts );
		lua::TableAssoc(L, "GetHearts", Lua_ChallengeParamGetHearts );
		lua::TableAssoc(L, "GetMaxHearts", Lua_ChallengeParamGetMaxHearts );
		lua::TableAssoc(L, "GetCoins", Lua_ChallengeParamGetCoins );
		lua::TableAssoc(L, "GetAddDamage", Lua_ChallengeParamGetAddDamage );
		lua::TableAssoc(L, "CanShoot", Lua_ChallengeParamCanShoot );
		lua::TableAssoc(L, "IsAltPath", Lua_ChallengeParamIsAltPath );
		lua::TableAssoc(L, "GetDifficulty", Lua_ChallengeParamGetDifficulty );
		lua::TableAssoc(L, "IsMegaSatanRun", Lua_ChallengeParamIsMegaSatanRun );
		lua::TableAssoc(L, "GetMinFireRate", Lua_ChallengeParamGetMinFireRate );
		lua::TableAssoc(L, "IsMaxDamageEnabled", Lua_ChallengeParamIsMaxDamageEnabled );
		lua::TableAssoc(L, "IsMinShotSpeedEnabled", Lua_ChallengeParamIsMinShotSpeedEnabled );
		lua::TableAssoc(L, "IsBigRangeEnabled", Lua_ChallengeParamIsBigRangeEnabled );
		lua::TableAssoc(L, "GetCurse", Lua_ChallengeParamGetCurse );
		lua::TableAssoc(L, "GetCurseFilter", Lua_ChallengeParamGetCurseFilter );
		lua::TableAssoc(L, "IsSecretPath", Lua_ChallengeParamIsSecretPath );
		lua::TableAssoc(L, "GetCollectibleTwinList", Lua_ChallengeParamGetCollectibleTwinList );
		
		lua_setglobal(L, "ChallengeParam");
	//lua::RegisterNewClass(L, lua::metatables::ChallengeParamMT, lua::metatables::ChallengeParamMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterChallengeParam(_state);
}