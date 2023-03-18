#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_RNGSetSeed(lua_State* L) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, "RNG");

	unsigned int seed = luaL_checkinteger(L, 2);
	unsigned int shiftidx = 35;

	if(lua_isinteger(L, 3))
		shiftidx = lua_tointeger(L, 3);

	if (seed <= 0) {
		luaL_argerror(L, 2, "RNG seed must be an integer above 0");
	}

	if (shiftidx < 0 || shiftidx > 80) {
		luaL_argerror(L, 3, "RNG shift index must be between 0 and 80 (inclusive)");
	}

	rng->SetSeed(seed, shiftidx);

	return 0;
}

static void RegisterFixedRNGSetSeed(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::RNG);
	lua_pushstring(L, "SetSeed");
	lua_pushcfunction(L, Lua_RNGSetSeed);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterFixedRNGSetSeed(state);
}