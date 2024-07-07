#include "IsaacRepentance.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../LuaInit.h"
#include "HookSystem.h"
#include "ASMPatches.h"

void RunEvaluateCustomCacheCallback(Entity_Player* player, const std::string& customcache) {
	const int callbackid = 1111;
	if (CallbackState.test(callbackid - 1000)) {
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaCaller(L).push(callbackid)
			.push(customcache.c_str())
			.push(player, lua::Metatables::ENTITY_PLAYER)
			.push(customcache.c_str())
			.call(1);
	}
}

void TriggerNullCustomCache(Entity_Player* player, const int id) {
	for (const std::string& customcache : XMLStuff.NullItemData->GetCustomCache(id)) {
		RunEvaluateCustomCacheCallback(player, customcache);
	}
}

void TriggerCollectibleCustomCache(Entity_Player* player, const int id) {
	for (const std::string& customcache : XMLStuff.ItemData->GetCustomCache(id)) {
		RunEvaluateCustomCacheCallback(player, customcache);
	}
}

void TriggerTrinketCustomCache(Entity_Player* player, const int id) {
	for (const std::string& customcache : XMLStuff.TrinketData->GetCustomCache(id)) {
		RunEvaluateCustomCacheCallback(player, customcache);
	}
}

void TriggerItemCustomCache(Entity_Player* player, ItemConfig_Item* item) {
	if (item->type == 0) {  // Null
		TriggerNullCustomCache(player, item->id);
	}
	else if (item->type == 2) {  // Trinket
		TriggerTrinketCustomCache(player, item->id);
	}
	else {  // Collectible (Passive/Active/Familiar)
		TriggerCollectibleCustomCache(player, item->id);
	}
}

// Re-evaluate ALL custom caches if EvaluateItems is run for CacheFlag.CACHE_ALL.
HOOK_METHOD_PRIORITY(Entity_Player, EvaluateItems, -1, () -> void) {
	const int cacheFlagAll = (1 << 16) - 1;  // CacheFlag.CACHE_ALL
	const bool evaluateCustomCaches = (this->_cacheFlags & cacheFlagAll) == cacheFlagAll;
	super();
	if (evaluateCustomCaches) {
		for (const std::string& customcache : XMLStuff.AllCustomCaches) {
			RunEvaluateCustomCacheCallback(this, customcache);
		}
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, AddCollectible, -1, (int type, int charge, bool firsttime, int slot, int vardata) -> void) {
	super(type, charge, firsttime, slot, vardata);
	TriggerCollectibleCustomCache(this, type);
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerCollectibleRemoved, -1, (unsigned int collectibleType) -> void) {
	super(collectibleType);
	TriggerCollectibleCustomCache(this, collectibleType);
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketAdded, -1, (unsigned int trinketType, bool firstTime) -> void) {
	super(trinketType, firstTime);
	TriggerTrinketCustomCache(this, trinketType);
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerTrinketRemoved, -1, (unsigned int trinketType) -> void) {
	super(trinketType);
	TriggerTrinketCustomCache(this, trinketType);
}

HOOK_METHOD_PRIORITY(TemporaryEffects, AddEffect, -1, (TemporaryEffect* effect, bool addCostume, int count) -> void) {
	super(effect, addCostume, count);
	if (!this->_disabled && count != 0 && this->_player) {
		TriggerItemCustomCache(this->_player, effect->_item);  // This does not happen immediately for vanilla caches.
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, TriggerEffectRemoved, -1, (ItemConfig_Item* item) -> void) {
	super(item);
	TriggerItemCustomCache(this, item);  // This does not happen immediately for vanilla caches.
}

// ASM patch into RecomputeWispCollectibles?
// For removing I think that calls TriggerCollectibleRemoved, but what about added wisps?

// Trigger callback from AddInnateCollectible etc
