#include <string>
#include <cstring>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Log.h"
#include "HookSystem.h"

MOD_EXPORT void L_Seeds_RemoveBlockingSeedEffects(Seeds* seeds, unsigned int effect) {
	seeds->RemoveBlockingSeedEffects(effect);
}

MOD_EXPORT void L_Seeds_AddSeedEffect(Seeds* seeds, unsigned int seed) {
	seeds->AddSeedEffect(seed);
}

MOD_EXPORT bool L_Seeds_CanAddSeedEffect(Seeds* seeds, unsigned int seed) {
	return seeds->CanAddSeedEffect(seed);
}

MOD_EXPORT void L_Seeds_ClearSeedEffects(Seeds* seeds) {
	seeds->ClearSeedEffects();
}

MOD_EXPORT void L_Seeds_ClearStartSeed(Seeds* seeds) {
	seeds->_gameStartSeed = 0;
}

MOD_EXPORT int L_Seeds_CountUnlockedSeedEffects() {
	return Seeds::CountUnlockedSeedEffects();
}

MOD_EXPORT void L_Seeds_ForgetStageSeed(Seeds* seeds, unsigned int stage) {
	seeds->ForgetStageSeed(stage);
}

MOD_EXPORT void L_Seeds_Seed2String(unsigned int seed, char* buf) {
    std::string res = Seeds::Seed2String(seed);
	std::memcpy(buf, res.data(), 9);
}

MOD_EXPORT bool L_Seeds_HasSeedEffect(Seeds* seeds, int value) {
	return seeds->HasSeedEffect(value);
}

MOD_EXPORT void L_Seeds_InitSeedInfo() {
	Seeds::InitSeedInfo();
}

MOD_EXPORT bool L_Seeds_IsSeedComboBanned(unsigned int effect1, unsigned int effect2) {
	return Seeds::IsSeedComboBanned(effect1, effect2);
}

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

MOD_EXPORT void L_Seeds_SetStageSeed(Seeds* seeds, int stage, unsigned int seed) {
	if (seed == 0) seed = 1;

	if (stage >= 0 && stage <= LevelStage::NUM_STAGES - 1) {
		seeds->_stageSeeds[stage] = seed;
	}
}

MOD_EXPORT void L_Seeds_RemoveSeedEffect(Seeds* seeds, int effect) {
	seeds->RemoveSeedEffect(effect);
}

MOD_EXPORT void L_Seeds_Reset(Seeds* seeds) {
	seeds->Reset();
}

MOD_EXPORT int L_Seeds_String2Seed(char* seed) {
	std::string seedString(seed);
	return Seeds::String2Seed(&seedString);
}

MOD_EXPORT void L_Seeds_SetStartSeed(Seeds* seeds, unsigned int seed) {
	seeds->set_start_seed(seed);
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