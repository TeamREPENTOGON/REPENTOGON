#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

bool overrideMegaSatanEnding = false;


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

int Lua_GameGetDebugFlag(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flag = luaL_checkinteger(L, 2) - 1;
	lua_pushboolean(L, game->GetDebugFlag(flag));

	return 1;
}

int Lua_GameGetDebugFlags(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, *game->GetDebugFlags());

	return 1;
}


int Lua_GameToggleDebugFlag(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flag = luaL_checkinteger(L, 2) - 1;
	game->ToggleDebugFlag(flag);
	lua_pushboolean(L, game->GetDebugFlag(flag));

	return 1;
}

int Lua_GameAddDebugFlags(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	unsigned int flags = luaL_checkinteger(L, 2);
	*game->GetDebugFlags() |= flags;
	return 0;
}

int Lua_GetDimension(lua_State* L){
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, game->GetDimension());
	return 1;
}

int Lua_GameForceMegaSatanVoidPortal(lua_State* L) {
	overrideMegaSatanEnding = lua_toboolean(L, 2);
	return 0;
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
	lua::RegisterFunction(state, mt, "GetDebugFlag", Lua_GameGetDebugFlag); // this one's on the chopping block
	lua::RegisterFunction(state, mt, "GetDebugFlags", Lua_GameGetDebugFlags);
	lua::RegisterFunction(state, mt, "ToggleDebugFlag", Lua_GameToggleDebugFlag); // this too
	lua::RegisterFunction(state, mt, "AddDebugFlags", Lua_GameAddDebugFlags);
	lua::RegisterFunction(state, mt, "ForceMegaSatanVoidPortal", Lua_GameForceMegaSatanVoidPortal);

	lua::RegisterFunction(state, lua::Metatables::LEVEL, "GetDimension", Lua_GetDimension);

}