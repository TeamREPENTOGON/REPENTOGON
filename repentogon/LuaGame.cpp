#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_GameAchievementUnlocksDisallowed(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->AchievementUnlocksDisallowed());

	return 1;
}

int Lua_GameIsPauseMenuOpen(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsPauseMenuOpen());

	return 1;
}

int Lua_GameGetPauseMenuState(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPauseMenu()->status);

	return 1;
}

int Lua_GameGetPlanetariumsVisited(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPlanetariumsVisited());

	return 1;
}

int Lua_GameIsHardMode(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsHardMode());

	return 1;
}

int Lua_GameGetLastDevilRoomStageFix(lua_State* L) 
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastDevilRoomStage());

	return 1;
}

int Lua_GetLastLevelWithDamageFix(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithDamage());

	return 1;
}

int Lua_GetLastLevelWithoutHalfHpFix(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetLastLevelWithoutHalfHp());

	return 1;
}

int Lua_GameGetDebugFlags(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, *game->GetDebugFlags());

	return 1;
}

int Lua_GameAddDebugFlags(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flags = (unsigned int)luaL_checkinteger(L, 2);
	*game->GetDebugFlags() |= flags;
	return 0;
}

int Lua_GetDimension(lua_State* L){
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, game->GetDimension());
	return 1;
}

int Lua_GameSpawnBombCrater(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Entity* crater = game->SpawnBombCrater(pos);
	lua::luabridge::UserdataPtr::push(L, crater, lua::GetMetatableKey(lua::Metatables::ENTITY));

	return 1;
}

int Lua_GameDevolveEnemy(lua_State* L) {
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

int Lua_GameIsGreedBoss(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedBoss());

	return 1;
}

int Lua_GameIsGreedFinalBoss(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushboolean(L, game->IsGreedFinalBoss());

	return 1;
}

LUA_FUNCTION(lua_GameIsErased) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity* entity = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

		if (game->IsErased(entity->_type, entity->_variant, entity->_subtype)) {
			lua_pushinteger(L, 1);
		}
		else {
			lua_pushinteger(L, 0);
		}
	}
	else {
		int type = luaL_checkinteger(L, 2);
		int variant = luaL_optinteger(L, 3, -1);
		int subtype = luaL_optinteger(L, 4, -1);

		if (game->IsErased(type, variant, subtype)) {
			lua_pushinteger(L, 1);
		}
		else {
			lua_pushinteger(L, 0);
		}
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::GAME;
	lua::RegisterFunction(state, mt, "AchievementUnlocksDisallowed", Lua_GameAchievementUnlocksDisallowed);
	lua::RegisterFunction(state, mt, "IsPauseMenuOpen", Lua_GameIsPauseMenuOpen);
	lua::RegisterFunction(state, mt, "GetPauseMenuState", Lua_GameGetPauseMenuState);
	lua::RegisterFunction(state, mt, "GetPlanetariumsVisited", Lua_GameGetPlanetariumsVisited);
	lua::RegisterFunction(state, mt, "IsHardMode", Lua_GameIsHardMode);
	lua::RegisterFunction(state, mt, "GetLastDevilRoomStage", Lua_GameGetLastDevilRoomStageFix);
	lua::RegisterFunction(state, mt, "GetLastLevelWithDamage", Lua_GetLastLevelWithDamageFix);
	lua::RegisterFunction(state, mt, "GetLastLevelWithoutHalfHp", Lua_GetLastLevelWithoutHalfHpFix);
	lua::RegisterFunction(state, mt, "GetDebugFlags", Lua_GameGetDebugFlags);
	lua::RegisterFunction(state, mt, "AddDebugFlags", Lua_GameAddDebugFlags);
	//lua::RegisterFunction(state, mt, "SpawnBombCrater", Lua_GameSpawnBombCrater);
	lua::RegisterFunction(state, mt, "DevolveEnemy", Lua_GameDevolveEnemy);
	lua::RegisterFunction(state, mt, "IsGreedBoss", Lua_GameIsGreedBoss);
	lua::RegisterFunction(state, mt, "IsGreedFinalBoss", Lua_GameIsGreedFinalBoss);

	lua::RegisterFunction(state, lua::Metatables::LEVEL, "GetDimension", Lua_GetDimension);
	lua::RegisterFunction(state, mt, "StartStageTransition", lua_GameStartStageTransition);
	lua::RegisterFunction(state, mt, "IsErased", lua_GameIsErased);

}