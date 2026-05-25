#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_TearParams_GetMassMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	lua_pushnumber(L, params->_massMultiplier);
	return 1;
}
LUA_FUNCTION(Lua_TearParams_SetMassMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	float value = (float)luaL_checknumber(L, 2);
	if (value <= 0) {
		return luaL_argerror(L, 2, "MassMultiplier must be positive!");
	}
	params->_massMultiplier = value;
	return 0;
}

LUA_FUNCTION(Lua_TearParams_GetKnockbackMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	lua_pushnumber(L, params->_knockbackMultiplier);
	return 1;
}
LUA_FUNCTION(Lua_TearParams_SetKnockbackMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	float value = (float)luaL_checknumber(L, 2);
	if (value <= 0) {
		return luaL_argerror(L, 2, "KnockbackMultiplier must be positive!");
	}
	params->_knockbackMultiplier = value;
	return 0;
}

LUA_FUNCTION(Lua_TearParams_GetSpeedMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	lua_pushnumber(L, params->_speedMultiplier);
	return 1;
}
LUA_FUNCTION(Lua_TearParams_SetSpeedMultiplier) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	params->_speedMultiplier = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_TearParams_GetTearDisplacement) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	lua_pushinteger(L, params->_tearDisplacement);
	return 1;
}
LUA_FUNCTION(Lua_TearParams_SetTearDisplacement) {
	TearParams* params = lua::GetLuabridgeUserdata<TearParams*>(L, 1, lua::Metatables::TEAR_PARAMS, "TearParams");
	int value = (int)luaL_checkinteger(L, 2);
	if (value < -1 || value > 1) {
		// 0 is technically "valid" in that the game can sometimes allow it via GetTearHitParams.
		// I dunno what the implication of having it be zero is though, it doesn't seem entirely intentional.
		// Not treating 0 as an error, but also not going to mention it in the error message.
		return luaL_argerror(L, 2, "TearDisplacement may only be set to -1 or 1");
	}
	params->_tearDisplacement = value;
	return 0;
}

inline void RegisterTearParamsVariable(lua_State* L, const char* name, lua_CFunction getFunc, lua_CFunction setFunc) {
	lua::RegisterVariable(L, lua::Metatables::TEAR_PARAMS, name, getFunc, setFunc);
	lua::RegisterVariableGetter(L, lua::Metatables::CONST_TEAR_PARAMS, name, getFunc);
}
HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	RegisterTearParamsVariable(_state, "MassMultiplier", Lua_TearParams_GetMassMultiplier, Lua_TearParams_SetMassMultiplier);
	RegisterTearParamsVariable(_state, "KnockbackMultiplier", Lua_TearParams_GetKnockbackMultiplier, Lua_TearParams_SetKnockbackMultiplier);
	RegisterTearParamsVariable(_state, "SpeedMultiplier", Lua_TearParams_GetSpeedMultiplier, Lua_TearParams_SetSpeedMultiplier);
	RegisterTearParamsVariable(_state, "TearDisplacement", Lua_TearParams_GetTearDisplacement, Lua_TearParams_SetTearDisplacement);
}
