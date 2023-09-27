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

static void DoRandomInt(lua_State* L, RNG* rng, int &result, bool &negative) {
	int res = 0;
	int arg1 = luaL_checkinteger(L, 2);

	if (lua_gettop(L) == 3) {
		int arg2 = luaL_checkinteger(L, 3);

		if (arg1 > arg2) {
			luaL_argerror(L, 1, "interval is empty");
		}
		else
		{
			unsigned int interval = arg2 - arg1;
			res = rng->RandomInt(interval + 1); // +1 to make it inclusive like math.random
			res += arg1;
		}
	}
	else {
		res = rng->RandomInt(arg1);
		negative = false;
	}

	result = res;
}

LUA_FUNCTION(Lua_RNG_RandomInt) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, RngMT);
	bool negative = true;
	int result = 0;
	DoRandomInt(L, rng, result, negative);
	if (!negative) {
		lua_pushinteger(L, (unsigned int)result);
	}
	else {
		lua_pushinteger(L, result);
	}
	return 1;
}

LUA_FUNCTION(Lua_RNG_PhantomInt) {
	RNG* rng = lua::GetUserdata<RNG*>(L, 1, lua::Metatables::RNG, RngMT);
	RNG copy;
	memcpy(&copy, rng, sizeof(RNG));
	bool negative = true;
	int result = 0;
	DoRandomInt(L, &copy, result, negative);
	if (!negative) {
		lua_pushinteger(L, (unsigned int)result);
	}
	else {
		lua_pushinteger(L, result);
	}
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
	lua::RegisterFunction(state, lua::Metatables::RNG, "RandomInt", Lua_RNG_RandomInt);
	lua::RegisterFunction(state, lua::Metatables::RNG, "PhantomInt", Lua_RNG_PhantomInt);
	lua::RegisterFunction(state, lua::Metatables::RNG, "PhantomFloat", Lua_RNG_PhantomFloat);
}