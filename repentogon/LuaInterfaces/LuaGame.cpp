#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GameAchievementUnlocksDisallowed)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->AchievementUnlocksDisallowed());

	return 1;
}

LUA_FUNCTION(Lua_GameIsPauseMenuOpen)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsPauseMenuOpen());

	return 1;
}

LUA_FUNCTION(Lua_GameGetPauseMenuState)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPauseMenu()->status);

	return 1;
}

LUA_FUNCTION(Lua_GameGetPlanetariumsVisited)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPlanetariumsVisited());

	return 1;
}

LUA_FUNCTION(Lua_GameIsHardMode)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsHardMode());

	return 1;
}

LUA_FUNCTION(Lua_GameGetLastDevilRoomStageFix)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastDevilRoomStage());

	return 1;
}

LUA_FUNCTION(Lua_GetLastLevelWithDamageFix)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithDamage());

	return 1;
}

LUA_FUNCTION(Lua_GetLastLevelWithoutHalfHpFix)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithoutHalfHp());

	return 1;
}

LUA_FUNCTION(Lua_GameGetDebugFlags)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, *game->GetDebugFlags());

	return 1;
}

LUA_FUNCTION(Lua_GameAddDebugFlags)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flags = (unsigned int)luaL_checkinteger(L, 2);
	*game->GetDebugFlags() |= flags;
	return 0;
}

LUA_FUNCTION(Lua_GameSpawnBombCrater) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	const float radius = (const float)luaL_optnumber(L, 3, 1.0f);
	Entity* crater = game->SpawnBombCrater(pos, radius);

	lua::luabridge::UserdataPtr::push(L, crater, lua::GetMetatableKey(lua::Metatables::ENTITY));

	return 1;
}

LUA_FUNCTION(Lua_GameDevolveEnemy) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Entity* ent = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	game->DevolveEnemy(ent);

	return 0;
}

LUA_FUNCTION(lua_GameStartStageTransition) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	bool sameStage = lua::luaL_checkboolean(L, 2);
	int transition = (int)luaL_checkinteger(L, 3);
	Entity_Player* player = nullptr;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		player = lua::GetUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY_PLAYER, "Player");
	}

	game->StartStageTransition(sameStage, transition, player);
	return 0;
}

LUA_FUNCTION(Lua_GameIsGreedBoss)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedBoss());

	return 1;
}

LUA_FUNCTION(Lua_GameIsGreedFinalBoss)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedFinalBoss());

	return 1;
}

LUA_FUNCTION(lua_GameIsErased) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity* entity = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

		bool wasErased = game->IsErased(entity->_type, entity->_variant, entity->_subtype);
		lua_pushboolean(L, wasErased);
	}
	else {
		int type = (int)luaL_checkinteger(L, 2);
		int variant = (int)luaL_optinteger(L, 3, -1);
		int subtype = (int)luaL_optinteger(L, 4, -1);

		bool wasErased = game->IsErased(type, variant, subtype);
		lua_pushboolean(L, wasErased);
	}

	return 1;
}

LUA_FUNCTION(Lua_GameGetCurrentColorModifier)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetCurrentColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameGetTargetColorModifier)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetTargetColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameGetLerpColorModifier)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* color = game->GetLerpColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_GameSetColorModifier)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	ColorModState* pColor = lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);
	bool lerp = lua::luaL_optboolean(L, 3, true);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	game->SetColorModifier(pColor, lerp, rate);

	return 0;
}

LUA_FUNCTION(Lua_GameIsRerun) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->_isRerun);

	return 1;
}

LUA_FUNCTION(Lua_GameGetPlayer) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int idx = (int)luaL_optinteger(L, 2, 0);

	if (!g_Game || g_Game->_playerManager._playerList.size() == 0) {
		lua_pushnil(L);
	}
	else
	{
		if (idx < 0)
			idx = 0;
		Entity_Player* player = game->GetPlayer(idx);
		if (!player) {
			lua_pushnil(L);
		}
		else
		{
			lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		}
	}

	return 1;
}

LUA_FUNCTION(Lua_ShowGenericLeaderboard) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_leaderboard.Show(1, &game->_scoreSheet, false);
	return 0;
}

// Reimplementation
// Fix for original MoveToRandomRoom function, that prevents crashes caused by a seed = 0
LUA_FUNCTION(Lua_MoveToRandomRoom) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	bool iAmErrorRoom = lua_toboolean(L, 2);
	int seed = (int)luaL_checkinteger(L, 3);
	if (seed == 0)
	{
		return luaL_error(L, "The given seed is not valid");
	}

	Entity_Player* player = nullptr;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		player = lua::GetUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY_PLAYER, "Player");
	}

	game->MoveToRandomRoom(iAmErrorRoom, seed, player);
	return 0;
}

LUA_FUNCTION(Lua_SetDonationModAngel) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_donationModAngel = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_SetDonationModGreed) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");

	game->_donationModGreed = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_SetBloom) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	const int time = (int)luaL_checkinteger(L, 2);
	const float strength = (float)luaL_checknumber(L, 3);
	game->SetBloom(time, strength);

	return 0;
}

LUA_FUNCTION(Lua_SetDizzyAmount)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	float targetAmount = (float)luaL_checknumber(L, 2);
	float intensity = (float)luaL_checknumber(L, 3);

	game->_dizzyTargetIntensity = targetAmount;
	game->_dizzyIntensity = intensity;

	return 0;
}

LUA_FUNCTION(Lua_GetDizzyAmount)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->_dizzyAmount);

	return 1;
}

LUA_FUNCTION(Lua_GameAddShopVisits) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int visitCount = (int)luaL_checkinteger(L, 2);
	game->_shopVisits += visitCount;

	if (game->_shopVisits >= 6 && !game->IsGreedMode()) {
		g_Manager->GetPersistentGameData()->TryUnlock(379); // Unlock schoolbag
	}
	return 0;
}

LUA_FUNCTION(Lua_GameGetShopVisits) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->_shopVisits);

	return 1;
}

LUA_FUNCTION(Lua_ClearErasedEnemies) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	game->_erasedEntities.clear();

	return 0;
}

LUA_FUNCTION(Lua_RecordPlayerCompletion) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	int event = (int)luaL_checkinteger(L, 2);
	if (event < 0 || event > 17) {
		return luaL_error(L, "Bad CompletionType %d (valid range is 0-17)", event);
	}
	g_Manager->RecordPlayerCompletion(event);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "ClearErasedEnemies", Lua_ClearErasedEnemies },
		{ "AddShopVisits", Lua_GameAddShopVisits },
		{ "GetShopVisits", Lua_GameGetShopVisits },
		{ "AchievementUnlocksDisallowed", Lua_GameAchievementUnlocksDisallowed},
		{ "IsPauseMenuOpen", Lua_GameIsPauseMenuOpen},
		{ "GetPauseMenuState", Lua_GameGetPauseMenuState},
		{ "GetPlanetariumsVisited", Lua_GameGetPlanetariumsVisited},
		{ "IsHardMode", Lua_GameIsHardMode},
		{ "GetLastDevilRoomStage", Lua_GameGetLastDevilRoomStageFix},
		{ "GetLastLevelWithDamage", Lua_GetLastLevelWithDamageFix},
		{ "GetLastLevelWithoutHalfHp", Lua_GetLastLevelWithoutHalfHpFix},
		{ "GetDebugFlags", Lua_GameGetDebugFlags},
		{ "AddDebugFlags", Lua_GameAddDebugFlags},
		{ "SpawnBombCrater", Lua_GameSpawnBombCrater},
		{ "DevolveEnemy", Lua_GameDevolveEnemy},
		{ "IsGreedBoss", Lua_GameIsGreedBoss},
		{ "IsGreedFinalBoss", Lua_GameIsGreedFinalBoss},
		{ "StartStageTransition", lua_GameStartStageTransition},
		{ "IsErased", lua_GameIsErased},
		{ "GetCurrentColorModifier", Lua_GameGetCurrentColorModifier},
		{ "GetTargetColorModifier", Lua_GameGetTargetColorModifier},
		{ "GetLerpColorModifier", Lua_GameGetLerpColorModifier},
		{ "SetColorModifier", Lua_GameSetColorModifier},
		{ "IsRerun", Lua_GameIsRerun},
		{ "GetPlayer", Lua_GameGetPlayer},
		{ "ShowGenericLeaderboard", Lua_ShowGenericLeaderboard},
		{ "MoveToRandomRoom", Lua_MoveToRandomRoom},
		{ "SetDonationModAngel", Lua_SetDonationModAngel},
		{ "SetDonationModGreed", Lua_SetDonationModGreed},
		{ "SetBloom", Lua_SetBloom},
		{ "GetDizzyAmount", Lua_GetDizzyAmount},
		{ "SetDizzyAmount", Lua_SetDizzyAmount},
		{ "RecordPlayerCompletion", Lua_RecordPlayerCompletion},
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GAME, functions);

}