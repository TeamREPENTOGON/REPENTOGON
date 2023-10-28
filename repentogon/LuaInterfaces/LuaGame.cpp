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
	Entity* crater = game->SpawnBombCrater(pos);
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
	bool sameStage = lua_toboolean(L, 2);
	int transition = (int)luaL_checkinteger(L, 3);
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 4, lua::Metatables::ENTITY_PLAYER, "Player");

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
	bool lerp = true;
	if lua_isboolean(L, 3)
		lerp = lua_toboolean(L, 3);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	game->SetColorModifier(pColor, lerp, rate);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
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
		//{ "SpawnBombCrater", Lua_GameSpawnBombCrater},
		{ "DevolveEnemy", Lua_GameDevolveEnemy},
		{ "IsGreedBoss", Lua_GameIsGreedBoss},
		{ "IsGreedFinalBoss", Lua_GameIsGreedFinalBoss},
		{ "StartStageTransition", lua_GameStartStageTransition},
		{ "IsErased", lua_GameIsErased},
		{ "GetCurrentColorModifier", Lua_GameGetCurrentColorModifier},
		{ "GetTargetColorModifier", Lua_GameGetTargetColorModifier},
		{ "GetLerpColorModifier", Lua_GameGetLerpColorModifier},
		{ "SetColorModifier", Lua_GameSetColorModifier},
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GAME, functions);

}