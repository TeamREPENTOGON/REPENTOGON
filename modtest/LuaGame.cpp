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

static void RegisterAchievementUnlocksDisallowed(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "AchievementUnlocksDisallowed");
	lua_pushcfunction(L, Lua_GameAchievementUnlocksDisallowed);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_GameGetPlanetariumsVisited(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	lua_pushinteger(L, game->GetPlanetariumsVisited());

	return 1;
}


static void RegisterGetPlanetariumsVisited(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetPlanetariumsVisited");
	lua_pushcfunction(L, Lua_GameGetPlanetariumsVisited);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterAchievementUnlocksDisallowed(state);
	RegisterGetPlanetariumsVisited(state);
}