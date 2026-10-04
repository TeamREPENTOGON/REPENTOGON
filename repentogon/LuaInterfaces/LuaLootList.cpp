#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "HookSystem.h"
#include "CustomCallbacks.h"

MOD_EXPORT void L_LootList_Init(LootList* list) {
	new (list) LootList();
}

MOD_EXPORT void L_LootList_Destroy(LootList* list) {
	list->~LootList();
	memset(list, 0, sizeof(LootList));
}

// Note for any future lookers, adding a function to erase entries from the list could potentially be unsafe,
// due to potentially invalidating existing LootListEntry pointers, such as if iterating over GetEntries.
// Adding the capability to modify LootListEntry in-place may be better.

MOD_EXPORT unsigned int L_LootList_GetSize(LootList* list) {
	return list->size();
}

MOD_EXPORT LootListEntry* L_LootList_GetEntry(LootList* list, unsigned int index) {
	return &(*list)[index];
}

MOD_EXPORT void L_LootList_PushEntry(LootList* list, unsigned int type, unsigned int variant, unsigned int subType, unsigned int seed, RNG* rng) {
	list->push_back({ type, variant, subType, seed, rng });
}