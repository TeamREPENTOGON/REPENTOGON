#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SetStageSeed) {
	Seeds* seeds = lua::GetLuabridgeUserdata<Seeds*>(L, 1, lua::Metatables::SEEDS, "Seeds");
	const int stage = (int)luaL_checkinteger(L, 2);
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 3);

	if (seed == 0) seed = 1;

	if (stage >= 0 && stage <= LevelStage::NUM_STAGES - 1) {
		seeds->_stageSeeds[stage] = seed;
	}
	else {
		luaL_argerror(L, 2, "Invalid LevelStage (must be between 0 and 13)");
	}
	
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "SetStageSeed", Lua_SetStageSeed },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::SEEDS, functions);
}