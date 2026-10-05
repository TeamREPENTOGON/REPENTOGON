#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"
#include "../Patches/ItemPoolManager.h"

static inline void print_console_warning(const std::string& warning)
{
	g_Game->GetConsole()->Print(warning, 0xFFFCCA03, 0x96u);
}

static inline void print_warnings(ItemPoolManager::Warnings warnings)
{
	for (const auto& warning : warnings)
	{
		print_console_warning(warning);
	}
}

static inline void EnsureValidSeed(uint32_t& seed) {
	seed = seed == 0 ? 1 : seed;
}

static inline bool isCollectibleRemoved(ItemPool* itemPool, uint32_t collectibleID) {
	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	return removedCollectibles[collectibleID];
}

static inline bool isCollectibleBlacklisted(ItemPool* itemPool, uint32_t collectibleID) {
	std::vector<bool>& blacklistedCollectibles = *itemPool->GetRoomBlacklistedCollectibles();
	return blacklistedCollectibles[collectibleID];
}

inline int GetChaosPoolEx(ItemPool* itemPool, RNG* rng, std::unordered_map<int, bool> filter, bool isWhitelist) {
	WeightedOutcomePicker picker;

	for (auto& pool : ItemPoolManager::GetItemPools()) {
		if (isWhitelist != (filter.find(pool->GetId()) != filter.end())) {
			continue;
		}

		ItemPool_Item* poolData = pool->GetPoolData();

		const uint32_t scaleFactor = 100;
		WeightedOutcomePicker_Outcome outcome{ pool->GetId(), (uint32_t)(poolData->_totalWeight * scaleFactor)};
		picker.AddOutcomeWeight(outcome, false);
	}
	
	rng->Next();

	if (picker.GetOutcomes()->empty())
	{
		return POOL_NULL;
	}

	RNG pickerRNG;
	pickerRNG.SetSeed(rng->_seed, 35);

	return picker.PickOutcome(pickerRNG);
}

static std::string s_poolItemError;

static ItemPoolManager::PoolItemDesc MakeDesc(int itemId, float weight, float decreaseBy, float removeOn) {
	ItemPoolManager::PoolItemDesc desc;
	desc.itemId = itemId;
	desc.weight = weight;
	desc.decreaseBy = decreaseBy;
	desc.removeOn = removeOn;
	return desc;
}

MOD_EXPORT bool L_ItemPool_IsPoolValid(int poolType) {
	return ItemPoolManager::GetItemPool(poolType) != nullptr;
}

MOD_EXPORT int L_ItemPool_GetNumItemPools() {
	return (int)ItemPoolManager::GetNumItemPools();
}

MOD_EXPORT bool L_ItemPool_GetCollectible(ItemPool* itemPool, int poolType, bool decrease, uint32_t seed, int defaultItem, uint32_t flags, int* result) {
	EnsureValidSeed(seed);

	if (poolType == POOL_NULL) {
		*result = COLLECTIBLE_NULL;
		return true;
	}

	if (!ItemPoolManager::GetItemPool(poolType)) {
		return false;
	}

	flags = flags << 1;
	if (!decrease) {
		flags |= 1;
	}

	*result = itemPool->GetCollectible(poolType, seed, flags, defaultItem);
	return true;
}

MOD_EXPORT int L_ItemPool_GetTrinket(ItemPool* itemPool, bool dontAdvanceRNG) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return TRINKET_NULL;
	}
	return itemPool->GetTrinket(dontAdvanceRNG);
}

MOD_EXPORT int L_ItemPool_GetCardEx(ItemPool* itemPool, unsigned int seed, int specialChance, int runeChance, int suitChance, bool allowNonCards) {
	EnsureValidSeed(seed);
	return itemPool->GetCardEx(seed, specialChance, runeChance, suitChance, allowNonCards);
}

MOD_EXPORT void L_ItemPool_AddRoomBlacklist(unsigned int item) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return;
	}
	g_Game->_itemPool.AddRoomBlacklist(item);
}

MOD_EXPORT int L_ItemPool_GetRandomPool(ItemPool* itemPool, RNG* rng, bool advancedSearch, const int* filterList, int filterCount, bool isWhitelist) {
	EnsureValidSeed(rng->_seed);

	if (!advancedSearch) {
		return itemPool->get_chaos_pool(rng);
	}

	std::unordered_map<int, bool> filter;
	for (int i = 0; i < filterCount; i++) {
		filter[filterList[i]] = true;
	}

	return GetChaosPoolEx(itemPool, rng, filter, isWhitelist);
}

MOD_EXPORT PoolItem* L_ItemPool_PickCollectible(ItemPool* itemPool, int poolType, bool decrease, RNG* rng, uint32_t flags) {
	auto* pool = ItemPoolManager::GetItemPool(poolType);
	ItemPool_Item* poolData = pool->GetPoolData();

	float targetWeight = 0;
	if (rng == nullptr) {
		RNG tempRNG;
		uint32_t seed = Isaac::genrand_int32();
		EnsureValidSeed(seed);
		tempRNG.SetSeed(seed, 4);
		targetWeight = tempRNG.RandomFloat() * poolData->_totalWeight;
	}
	else {
		EnsureValidSeed(rng->_seed);
		targetWeight = rng->RandomFloat() * poolData->_totalWeight;
	}

	flags = flags << 1;
	if (!decrease) {
		flags |= 1;
	}

	return itemPool->pick_collectible(targetWeight, poolData, flags);
}

MOD_EXPORT int L_ItemPool_GetCollectibleFromList(ItemPool* itemPool, const int* list, int length, unsigned int seed, unsigned int defaultItem, bool addToBlacklist, bool excludeActiveItems) {
	EnsureValidSeed(seed);
	return itemPool->GetCollectibleFromList(list, length, seed, defaultItem, addToBlacklist, excludeActiveItems);
}

MOD_EXPORT bool L_ItemPool_HasCollectible(ItemPool* itemPool, int collectibleID) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return false;
	}

	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	std::vector<ItemConfig_Item*>& collectList = *g_Manager->GetItemConfig()->GetCollectibles();

	return (collectibleID >= 0 && (unsigned int)collectibleID < collectList.size()) && (!removedCollectibles[collectibleID]);
}

MOD_EXPORT int L_ItemPool_GetCollectibleBits(ItemPool* itemPool, bool roomBlacklist, bool* out) {
	std::vector<bool>& bits = roomBlacklist ? *itemPool->GetRoomBlacklistedCollectibles() : *itemPool->GetRemovedCollectibles();
	if (out) {
		for (size_t i = 0; i < bits.size(); i++) {
			out[i] = bits[i];
		}
	}
	return (int)bits.size();
}

MOD_EXPORT std::vector<PoolItem>* L_ItemPool_GetPoolList(int poolType) {
	auto* pool = ItemPoolManager::GetItemPool(poolType);
	if (!pool) {
		return nullptr;
	}
	return &pool->GetPoolData()->_poolList;
}

MOD_EXPORT bool L_ItemPool_HasTrinket(ItemPool* itemPool, unsigned int trinketID) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return false;
	}

	std::vector<ItemConfig_Item*>& trinketList = *g_Manager->GetItemConfig()->GetTrinkets();
	if (trinketID >= trinketList.size()) {
		return false;
	}
	return itemPool->_trinketPoolItems[trinketID]._inPool;
}

MOD_EXPORT int L_ItemPool_CanSpawnCollectible(ItemPool* itemPool, int id, bool unkFlag) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return 0;
	}

	ItemConfig_Item* item = g_Manager->GetItemConfig()->GetCollectible(id);
	if (item == nullptr) {
		return -1;
	}

	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	std::vector<bool>& blacklistedCollectibles = *itemPool->GetRoomBlacklistedCollectibles();

	return !removedCollectibles[id] && !blacklistedCollectibles[id]
		&& item->IsAvailableEx((unkFlag ^ 1) * 2 - 3)
		&& !(g_Game->GetPlayerManager()->AnyoneHasTrinket(TRINKET_NO) && item->type == 3);
}

MOD_EXPORT void L_ItemPool_UnidentifyPill(ItemPool* itemPool, int pillColor) {
	pillColor &= PILL_COLOR_MASK;
	if (pillColor >= 0 && pillColor < NUM_PILLS) {
		itemPool->_idendifiedPillEffects[pillColor] = false;
	}
}

MOD_EXPORT int L_ItemPool_GetPillColor(ItemPool* itemPool, int pillEffect) {
	for (int i = 0; i < 15; i++) {
		if (itemPool->_pillEffects[i] == pillEffect) {
			return i;
		}
	}
	return -1;
}

MOD_EXPORT bool L_ItemPool_AddBibleUpgrade(ItemPool* itemPool, int add, int poolType) {
	if ((uint32_t)poolType > ItemPoolManager::GetNumItemPools()) {
		return false;
	}

	itemPool->AddBibleUpgrade(add, poolType);
	return true;
}

MOD_EXPORT bool L_ItemPool_GetBibleUpgrades(int poolType, int* out) {
	auto* pool = ItemPoolManager::GetItemPool(poolType);
	if (!pool) {
		return false;
	}

	*out = pool->GetPoolData()->_bibleUpgrade;
	return true;
}

MOD_EXPORT bool L_ItemPool_ResetCollectible(ItemPool* itemPool, int collectible) {
	if (!ItemPoolManager::IsItemPoolInitialized()) {
		return true;
	}

	if (collectible < COLLECTIBLE_NULL || collectible >= (int)g_Manager->GetItemConfig()->GetCollectibles()->size()) {
		return false;
	}

	itemPool->ResetCollectible(collectible);
	return true;
}

MOD_EXPORT int L_ItemPool_GetCollectibleByName(const char* name) {
	std::string nameString = name;
	int itemId = LuaEngine::Isaac_GetItemIdByName(&nameString);
	return itemId > -1 ? itemId : COLLECTIBLE_NULL;
}

MOD_EXPORT void L_ItemPool_PrintWarning(const char* warning) {
	print_console_warning(warning);
}

MOD_EXPORT void L_ItemPool_AddVirtualItem(int poolType, int itemId, float weight, float decreaseBy, float removeOn) {
	ItemPoolManager::GetItemPool(poolType)->AddVirtualItem(MakeDesc(itemId, weight, decreaseBy, removeOn));
}

MOD_EXPORT const char* L_ItemPool_AddTemporaryItem(int poolType, int itemId, float weight, float decreaseBy, float removeOn) {
	ItemPoolManager::Error error;
	ItemPoolManager::GetItemPool(poolType)->AddTemporaryItem(MakeDesc(itemId, weight, decreaseBy, removeOn), error);
	if (!error) {
		return nullptr;
	}
	s_poolItemError = *error;
	return s_poolItemError.c_str();
}

MOD_EXPORT const char* L_ItemPool_RemoveTemporaryItem(int poolType, int itemId, float weight, float decreaseBy, float removeOn) {
	ItemPoolManager::Error error;
	ItemPoolManager::GetItemPool(poolType)->RemoveTemporaryItem(MakeDesc(itemId, weight, decreaseBy, removeOn), error);
	if (!error) {
		return nullptr;
	}
	s_poolItemError = *error;
	return s_poolItemError.c_str();
}

MOD_EXPORT bool L_ItemPool_RemoveCollectible(ItemPool* itemPool, int collectible, bool param2, bool param3) {
	return itemPool->RemoveCollectible(collectible, param2, param3);
}

MOD_EXPORT bool L_ItemPool_RemoveTrinket(ItemPool* itemPool, int trinket) {
	return itemPool->RemoveTrinket(trinket);
}

MOD_EXPORT void L_ItemPool_ResetTrinkets(ItemPool* itemPool) {
	itemPool->ResetTrinkets();
}

MOD_EXPORT int L_ItemPool_GetCard(ItemPool* itemPool, unsigned int seed, bool includePlayingCards, bool includeRunes, bool onlyRunes) {
	return itemPool->GetCard(seed, includePlayingCards, includeRunes, onlyRunes);
}

MOD_EXPORT int L_ItemPool_GetPill(ItemPool* itemPool, unsigned int seed) {
	return itemPool->GetPill(seed);
}

MOD_EXPORT void L_ItemPool_ResetRoomBlacklist(ItemPool* itemPool) {
	itemPool->ResetRoomBlacklist();
}

MOD_EXPORT void L_ItemPool_IdentifyPill(ItemPool* itemPool, unsigned int pillColor) {
	itemPool->IdentifyPill(pillColor);
}

MOD_EXPORT bool L_ItemPool_IsPillIdentified(ItemPool* itemPool, unsigned int pillColor) {
	return itemPool->IsPillIdentified(pillColor);
}

MOD_EXPORT int L_ItemPool_ForceAddPillEffect(ItemPool* itemPool, int pillEffect) {
	return itemPool->ForceAddPillEffect(pillEffect);
}

MOD_EXPORT int L_ItemPool_GetPoolForRoom(ItemPool* itemPool, unsigned int roomType, unsigned int seed) {
	return itemPool->GetPoolForRoom(roomType, seed);
}

LUA_FUNCTION(Lua_ItemPool_GetPillEffect) {
	ItemPool* itemPool = LuaItemPool::Get(L, 1);
	unsigned int pillColor = (unsigned int)luaL_checkinteger(L, 2);
	Entity_Player* player = LuaEntityPlayer::GetOpt(L, 3);

	lua_pushinteger(L, itemPool->GetPillEffect(pillColor, player));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_ItemPool_GetPillEffect", Lua_ItemPool_GetPillEffect);

	super();
}
