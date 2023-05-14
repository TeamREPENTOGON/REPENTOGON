#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int ambushWaves = 3;
static constexpr const char* AmbushMT = "Ambush";

static int Lua_GetAmbush(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Ambush** ud = (Ambush**)lua_newuserdata(L, sizeof(Ambush*));
	*ud = game->GetAmbush();
	luaL_setmetatable(L, AmbushMT);
	return 1;
}

int Lua_AmbushStartChallenge(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	ambush->StartChallenge();
	return 0;
}

int Lua_AmbushSpawnBossrushWave(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	ambush->SpawnBossrushWave();
	return 0;
}

int Lua_AmbushSpawnWave(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	ambush->SpawnWave();
	return 0;
}

int Lua_GetMaxBossrushWaves(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	lua_pushinteger(L, *ambush->GetMaxBossrushWaves());
	return 1;
}

int Lua_SetMaxBossrushWaves(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	int amount = luaL_checkinteger(L, 2);
	if (amount > 25) amount = 25;

	*ambush->GetMaxBossrushWaves() = amount;

	return 0;
}

int Lua_GetMaxChallengeWaves(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	lua_pushinteger(L, ambushWaves);
	return 1;
}

int Lua_SetMaxChallengeWaves(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	int amount = luaL_checkinteger(L, 2);

	ambushWaves = amount;

	return 0;
}

int Lua_GetCurrentWave(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	lua_pushinteger(L, *ambush->GetCurrentWave());

	return 1;
}

static void RegisterAmbush(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetAmbush");
	lua_pushcfunction(L, Lua_GetAmbush);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, AmbushMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "StartChallenge", Lua_AmbushStartChallenge },
		{ "SpawnBossrushWave", Lua_AmbushSpawnBossrushWave },
		{ "SpawnWave", Lua_AmbushSpawnWave },
		//{ "GetNumBossesPerWave", Lua_GetNumBossesPerWave },
		//{ "SetNumBossesPerWave", Lua_SetNumBossesPerWave },
		{ "GetMaxBossrushWaves", Lua_GetMaxBossrushWaves },
		{ "SetMaxBossrushWaves", Lua_SetMaxBossrushWaves },
		{ "GetMaxChallengeWaves", Lua_GetMaxChallengeWaves },
		{ "SetMaxChallengeWaves", Lua_SetMaxChallengeWaves },
		{ "GetCurrentWave", Lua_GetCurrentWave },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterAmbush(state);
}