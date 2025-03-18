#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem.h"

int NUM_SHIFT_INDEXES = 81;
int NUM_SHIFTS = 3;

inline uint32_t* GetShifts(int shift_idx) {
	return &s_Shifts + (shift_idx * NUM_SHIFTS);
}

inline int CeilIntegerDivision(int dividend, int divisor) //The same as performing ceil(dividend / divisor), but much faster
{
	return (dividend + (divisor - 1)) / divisor;
}

LUA_FUNCTION(Lua_RNGSetSeed) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);

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

LUA_FUNCTION(Lua_RNG_GetShiftIdx) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);

	for (int shift_idx = 0; shift_idx < NUM_SHIFT_INDEXES; shift_idx++) {
		uint32_t* shifts = GetShifts(shift_idx);

		if (rng->_shift1 == shifts[0] && rng->_shift2 == shifts[1] && rng->_shift3 == shifts[2]) {
			lua_pushinteger(L, shift_idx);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

LUA_FUNCTION(Lua_RNG_PhantomNext) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
	uint32_t original_seed = rng->_seed;

	lua_pushinteger(L, rng->Next());
	rng->_seed = original_seed;
	return 1;
}

inline uint32_t ReverseShrXor(uint32_t result, uint32_t shift) {
	uint32_t operand1 = 0, operand2 = 0;

	int loops = CeilIntegerDivision(32, shift);
	for (int i = 0; i < loops; i++) {
		operand1 = operand2 >> shift;
		operand2 = operand1 ^ result;
	}

	return operand2;
}

inline uint32_t ReverseShlXor(uint32_t result, uint32_t shift) {
	uint32_t operand1 = 0, operand2 = 0;

	int loops = CeilIntegerDivision(32, shift);
	for (int i = 0; i < loops; i++) {
		operand1 = operand2 << shift;
		operand2 = operand1 ^ result;
	}

	return operand2;
}

inline uint32_t DoPrevious(RNG* rng) {
	uint32_t tmp;

	tmp = ReverseShrXor(rng->_seed, rng->_shift3);
	tmp = ReverseShlXor(tmp, rng->_shift2);
	return ReverseShrXor(tmp, rng->_shift1);
}

LUA_FUNCTION(Lua_RNG_Previous) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);

	rng->_seed = DoPrevious(rng);
	lua_pushinteger(L, rng->_seed);
	return 1;
}

LUA_FUNCTION(Lua_RNG_PhantomPrevious) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);

	lua_pushinteger(L, DoPrevious(rng));
	return 1;
}

static void DoRandomInt(lua_State* L, RNG* rng, int& result, bool& negative) {
	int res = 0;
	int arg1 = (int)luaL_checkinteger(L, 2);

	if (lua_gettop(L) == 3) {
		int arg2 = (int)luaL_checkinteger(L, 3);

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
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
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
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
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
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
	RNG copy;
	memcpy(&copy, rng, sizeof(RNG));

	float result = copy.RandomFloat();
	lua_pushnumber(L, result);
	return 1;
}

void DoRandomVector(lua_State* L, RNG* rng, bool phantom) {
	Vector result = Isaac::RandomUnitVector(rng->_seed);
	if (!phantom) {
		rng->Next(); // we do it after calling bc the func increments the seed before use
	}
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = result;
	return;
}

LUA_FUNCTION(Lua_RNG_RandomVector) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
	DoRandomVector(L, rng, false);
	return 1;
}

LUA_FUNCTION(Lua_RNG_PhantomVector) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, lua::metatables::RngMT);
	DoRandomVector(L, rng, true);
	return 1;
}

LUA_FUNCTION(Lua_RNG_Constructor) {
	int seed = (int)luaL_optinteger(L, 2, 0xAA17414F);
	int shiftIdx = (int)luaL_optinteger(L, 3, 35);

	if (seed == 0) {
		return luaL_error(L, "Invalid seed 0 for RNG object\n");
	}

	if (shiftIdx < 0 || shiftIdx > 80) {
		return luaL_error(L, "Invalid shift index %d for RNG object\n", shiftIdx);
	}

	void* key = lua::GetMetatableKey(lua::Metatables::RNG);
	RNG* rng = lua::luabridge::UserdataValue<RNG>::place(L, key);
	rng->SetSeed(seed, shiftIdx);

	return 1;
}

static void OverrideRNGConstructor(lua_State* L) {
	lua::LuaStackProtector protector(L);

	int result = lua_getglobal(L, "RNG");
	if (result == LUA_TNIL) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] No global \"RNG\" in Lua environment\n");
		return;
	} 
	else if (result != LUA_TTABLE) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] Global \"RNG\" is not a table\n");
		return;
	}

	result = lua_getmetatable(L, -1);
	if (result == 0) {
		lua_pop(L, 1);
		ZHL::Log("[ERR] Global \"RNG\" has no metatable\n");
		return;
	}

	lua_pushcfunction(L, Lua_RNG_Constructor);
	lua_setfield(L, -2, "__call");
	lua_pop(L, 2);
	return;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "SetSeed", Lua_RNGSetSeed },
		{ "GetShiftIdx", Lua_RNG_GetShiftIdx},
		{ "PhantomNext", Lua_RNG_PhantomNext},
		{ "Previous", Lua_RNG_Previous},
		{ "PhantomPrevious", Lua_RNG_PhantomPrevious},
		{ "RandomInt", Lua_RNG_RandomInt },
		{ "PhantomInt", Lua_RNG_PhantomInt },
		{ "PhantomFloat", Lua_RNG_PhantomFloat },
		{ "RandomVector", Lua_RNG_RandomVector },
		{ "PhantomVector", Lua_RNG_PhantomVector },
		{ NULL, NULL }
	};

	OverrideRNGConstructor(_state);
	lua::RegisterFunctions(_state, lua::Metatables::RNG, functions);
}