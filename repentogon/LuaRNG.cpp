#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RngMT = "RNG";

int Lua_RNGSetSeed(lua_State* L) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, RngMT);

	unsigned int seed = (unsigned int)luaL_checkinteger(L, 2);
	unsigned int shiftidx = (unsigned int)luaL_optinteger(L, 3, 35);

	if (seed <= 0) {
		luaL_argerror(L, 2, "RNG seed must be an integer above 0");
	}

	if (shiftidx < 0 || shiftidx > 80) {
		luaL_argerror(L, 3, "RNG shift index must be between 0 and 80 (inclusive)");
	}

	rng->SetSeed(seed, shiftidx);

	return 0;
}

LUA_FUNCTION(Lua_RNG_PhantomInt) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, RngMT);
	int max = luaL_checkinteger(L, 2);
	if (max < 0) {
		return luaL_error(L, "Invalid max parameter for PhantomInt: %d\n", max);
	}

	RNG copy;
	memcpy(&copy, rng, sizeof(RNG));
	unsigned int result = copy.RandomInt(max);

	lua_pushinteger(L, result);
	return 1;
}

LUA_FUNCTION(Lua_RNG_PhantomFloat) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, RngMT);
	RNG copy;
	memcpy(&copy, rng, sizeof(RNG));

	float result = copy.RandomFloat();
	lua_pushnumber(L, result);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::RegisterFunction(state, lua::Metatables::RNG, "SetSeed", Lua_RNGSetSeed);
	lua::RegisterFunction(state, lua::Metatables::RNG, "PhantomInt", Lua_RNG_PhantomInt);
	lua::RegisterFunction(state, lua::Metatables::RNG, "PhantomFloat", Lua_RNG_PhantomFloat);
}