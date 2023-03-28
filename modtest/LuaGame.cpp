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

static void RegisterAchievementUnlocksDisallowed(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "AchievementUnlocksDisallowed");
	lua_pushcfunction(L, Lua_GameAchievementUnlocksDisallowed);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetPlanetariumsVisited(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetPlanetariumsVisited");
	lua_pushcfunction(L, Lua_GameGetPlanetariumsVisited);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterIsHardMode(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "IsHardMode");
	lua_pushcfunction(L, Lua_GameIsHardMode);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetLastDevilRoomStageFix(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetLastDevilRoomStage");
	lua_pushcfunction(L, Lua_GameGetLastDevilRoomStageFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetLastLevelWithDamageFix(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetLastLevelWithDamage");
	lua_pushcfunction(L, Lua_GetLastLevelWithDamageFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetLastLevelWithoutHalfHpFix(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetLastLevelWithoutHalfHp");
	lua_pushcfunction(L, Lua_GetLastLevelWithoutHalfHpFix);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetDebugFlag(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetDebugFlag");
	lua_pushcfunction(L, Lua_GameGetDebugFlag);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterGetDebugFlags(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetDebugFlags");
	lua_pushcfunction(L, Lua_GameGetDebugFlags);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterToggleDebugFlag(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "ToggleDebugFlag");
	lua_pushcfunction(L, Lua_GameToggleDebugFlag);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static void RegisterAddDebugFlags(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "AddDebugFlags");
	lua_pushcfunction(L, Lua_GameAddDebugFlags);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_GetDimension(lua_State* L){
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, game->GetDimension());
	return 1;
}

static void RegisterGetDimension(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::LEVEL);
	lua_pushstring(L, "GetDimension");
	lua_pushcfunction(L, Lua_GetDimension);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::GAME;
	lua::RegisterFunction(state, mt, "AchievementUnlocksDisallowed", Lua_GameAchievementUnlocksDisallowed);
	lua::RegisterFunction(state, mt, "GetPlanetariumsVisited", Lua_GameGetPlanetariumsVisited);
	lua::RegisterFunction(state, mt, "IsHardMode", Lua_GameIsHardMode);
	lua::RegisterFunction(state, mt, "GetLastDevilRoomStage", Lua_GameGetLastDevilRoomStageFix);
	lua::RegisterFunction(state, mt, "GetLastLevelWithDamage", Lua_GetLastLevelWithDamageFix);
	lua::RegisterFunction(state, mt, "GetLastLevelWithoutHalfHp", Lua_GetLastLevelWithoutHalfHpFix);
	lua::RegisterFunction(state, mt, "GetDebugFlag", Lua_GameGetDebugFlag); // this one's on the chopping block
	lua::RegisterFunction(state, mt, "GetDebugFlags", Lua_GameGetDebugFlags);
	lua::RegisterFunction(state, mt, "ToggleDebugFlag", Lua_GameToggleDebugFlag); // this too
	lua::RegisterFunction(state, mt, "AddDebugFlags", Lua_GameAddDebugFlags);

	lua::RegisterFunction(state, lua::Metatables::LEVEL, "GetDimension", Lua_GetDimension);

}