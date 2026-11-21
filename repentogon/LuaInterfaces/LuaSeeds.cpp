#include "IsaacRepentance.h"
#include "LuaCore.h"

static std::string s_seedString;

MOD_EXPORT void L_Seeds_AddSeedEffect(Seeds* seeds, int seedEffect) {
	seeds->AddSeedEffect(seedEffect);
}

MOD_EXPORT bool L_Seeds_CanAddSeedEffect(Seeds* seeds, int seedEffect) {
	return seeds->CanAddSeedEffect(seedEffect);
}

MOD_EXPORT void L_Seeds_ClearSeedEffects(Seeds* seeds) {
	seeds->ClearSeedEffects();
}

MOD_EXPORT void L_Seeds_ForgetStageSeed(Seeds* seeds, int stage) {
	seeds->ForgetStageSeed(stage);
}

MOD_EXPORT unsigned int L_Seeds_GetNextSeed(Seeds* seeds) {
	return seeds->GetNextSeed();
}

MOD_EXPORT bool L_Seeds_HasSeedEffect(Seeds* seeds, int seedEffect) {
	return seeds->HasSeedEffect(seedEffect);
}

MOD_EXPORT bool L_Seeds_IsSeedComboBanned(Seeds* seeds, int seedEffect1, int seedEffect2) {
	return seeds->IsSeedComboBanned(seedEffect1, seedEffect2);
}

MOD_EXPORT void L_Seeds_RemoveBlockingSeedEffects(Seeds* seeds, int seedEffect) {
	seeds->RemoveBlockingSeedEffects(seedEffect);
}

MOD_EXPORT void L_Seeds_RemoveSeedEffect(Seeds* seeds, int seedEffect) {
	Seeds::RemoveSeedEffect(seeds->GetSeedEffects(), &seedEffect);
}

MOD_EXPORT void L_Seeds_Reset(Seeds* seeds) {
	seeds->Reset();
}

MOD_EXPORT void L_Seeds_Restart(Seeds* seeds, int challenge) {
	seeds->Restart(challenge);
}

MOD_EXPORT void L_Seeds_SetStartSeed(Seeds* seeds, const char* seed) {
	std::string str = seed;
	seeds->set_start_seed(Seeds::String2Seed(&str));
}

MOD_EXPORT const char* L_Seeds_Seed2String(unsigned int seed) {
	s_seedString = Seeds::Seed2String(seed);
	return s_seedString.c_str();
}

MOD_EXPORT unsigned int L_Seeds_String2Seed(const char* seed) {
	std::string str = seed;
	return Seeds::String2Seed(&str);
}

// This function is cursed and takes an std::string by value.
MOD_EXPORT int L_Seeds_GetSeedEffect(const char* seed) {
	StdStringStorage storage;
	new (&storage) std::string(seed);
	return Seeds::GetSeedEffect(storage);
}

MOD_EXPORT int L_Seeds_CountUnlockedSeedEffects() {
	return Seeds::CountUnlockedSeedEffects();
}

MOD_EXPORT void L_Seeds_InitSeedInfo() {
	Seeds::InitSeedInfo();
}
