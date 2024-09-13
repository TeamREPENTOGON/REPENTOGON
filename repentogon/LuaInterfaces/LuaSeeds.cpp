#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


LUA_FUNCTION(Lua_SeedsGetStageSeed_BoundFix) {
	Seeds* seeds = lua::GetUserdata<Seeds*>(L, 1, lua::Metatables::SEEDS, "Seeds");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 13)
		return luaL_error(L, "bad argument #2 to 'GetStageSeed' (Invalid LevelStage %d)", id);
	lua_pushinteger(L, seeds->_stageSeeds[id]);

	return 1;
}

LUA_FUNCTION(Lua_SeedsForgetStageSeed_BoundFix) {
	Seeds* seeds = lua::GetUserdata<Seeds*>(L, 1, lua::Metatables::SEEDS, "Seeds");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 13)
		return luaL_error(L, "bad argument #2 to 'ForgetStageSeed' (Invalid LevelStage %d)", id);
	seeds->ForgetStageSeed(id);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetStageSeed", Lua_SeedsGetStageSeed_BoundFix },
		{ "ForgetStageSeed", Lua_SeedsForgetStageSeed_BoundFix },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::SEEDS, functions);
}
