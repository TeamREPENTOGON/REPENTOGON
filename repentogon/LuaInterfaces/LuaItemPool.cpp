#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

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

static inline bool Lua_NotPassedOrNil(lua_State* L, int narg) {
	return (lua_gettop(L) < narg || lua_isnil(L, narg));
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

LUA_FUNCTION(Lua_ItemPoolGetCardEx)
{
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 2);
	int specialChance = (int)luaL_checkinteger(L, 3);
	int runeChance = (int)luaL_checkinteger(L, 4);
	int suitChance = (int)luaL_checkinteger(L, 5);
	bool allowNonCards = lua::luaL_checkboolean(L, 6);

	EnsureValidSeed(seed);
	lua_pushinteger(L, itemPool->GetCardEx(seed, specialChance, runeChance, suitChance, allowNonCards));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectibleEx) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	bool decrease = lua::luaL_optboolean(L, 3, false);
	uint32_t seed = (unsigned int)luaL_optinteger(L, 4, Isaac::genrand_int32());
	int defaultItem = (int)luaL_optinteger(L, 5, COLLECTIBLE_NULL);
	uint32_t flags = (unsigned int)luaL_optinteger(L, 6, 0);

	EnsureValidSeed(seed);

	if (poolType == POOL_NULL) {
		lua_pushinteger(L, COLLECTIBLE_NULL);
		return 1;
	}

	if (!ItemPoolManager::GetItemPool(poolType)) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	flags = flags << 1;
	if (!decrease) {
		flags |= 1;
	}

	lua_pushinteger(L, itemPool->GetCollectible(poolType, seed, flags, defaultItem));
	return 1;
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

LUA_FUNCTION(Lua_ItemPoolGetRandomPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	RNG* rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	bool advancedSearch = lua::luaL_optboolean(L, 3, false);

	EnsureValidSeed(rng->_seed);

	if (!advancedSearch) {
		lua_pushinteger(L, itemPool->get_chaos_pool(rng));
		return 1;
	}

	if (!(Lua_NotPassedOrNil(L, 4) || lua_istable(L, 4))) {
		return luaL_argerror(L, 4, "Invalid Filter");
	}

	bool isWhitelist = lua::luaL_optboolean(L, 5, false);

	std::unordered_map<int, bool> filter;

	if (lua_istable(L, 4)) {
		size_t length = (size_t)lua_rawlen(L, 4);

		for (size_t i = 0; i < length; i++)
		{
			lua_rawgeti(L, 4, i + 1);
			filter[(int)luaL_checkinteger(L, -1)] = true;
			lua_pop(L, 1);
		}
	}

	lua_pushinteger(L, GetChaosPoolEx(itemPool, rng, filter, isWhitelist));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolPickCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	bool decrease = lua::luaL_optboolean(L, 3, false);

	RNG* rng = nullptr;
	if (!Lua_NotPassedOrNil(L, 4)) {
		rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, "RNG");
	}
	uint32_t flags = (unsigned int)luaL_optinteger(L, 5, 0);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

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

	PoolItem* poolItem = itemPool->pick_collectible(targetWeight, poolData, flags);

	if (poolItem == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	lua_newtable(L);

	lua_pushstring(L, "itemID");
	lua_pushinteger(L, poolItem->_itemID);
	lua_rawset(L, -3);

	lua_pushstring(L, "initialWeight");
	lua_pushnumber(L, poolItem->_initialWeight);
	lua_rawset(L, -3);

	lua_pushstring(L, "weight");
	lua_pushnumber(L, poolItem->_weight);
	lua_rawset(L, -3);

	lua_pushstring(L, "decreaseBy");
	lua_pushnumber(L, poolItem->_decreaseBy);
	lua_rawset(L, -3);

	lua_pushstring(L, "removeOn");
	lua_pushnumber(L, poolItem->_removeOn);
	lua_rawset(L, -3);

	lua_pushstring(L, "isUnlocked");
	lua_pushboolean(L, poolItem->_isUnlocked);
	lua_rawset(L, -3);

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectibleFromList) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	if (!lua_istable(L, 2))
	{
		return luaL_error(L, "Expected a table as second argument");
	}

	size_t length = (size_t)lua_rawlen(L, 2);

	// if the table is empty, we should pass default item
	if (length == 0)
	{
		unsigned int defaultItem = (unsigned int)luaL_optinteger(L, 4, 25); //COLLECTIBLE_BREAKFAST
		lua_pushinteger(L, defaultItem);
		return 1;
	}

	int* list = new int[length];

	for (size_t i = 0; i < length; i++)
	{
		lua_rawgeti(L, 2, i + 1);
		list[i] = (int)luaL_checkinteger(L, -1);
		lua_pop(L, 1);
	}

	unsigned int seed = (unsigned int)luaL_optinteger(L, 3, Random());
	unsigned int defaultItem = (unsigned int)luaL_optinteger(L, 4, 25); //COLLECTIBLE_BREAKFAST
	bool addToBlacklist = lua::luaL_optboolean(L, 5, true);
	bool excludeLockedItems = lua::luaL_optboolean(L, 6, false);

	EnsureValidSeed(seed);
	lua_pushinteger(L, itemPool->GetCollectibleFromList(list, length, seed, defaultItem, addToBlacklist, excludeLockedItems));

	// delete the array
	delete[] list;

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryBibleMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	ItemPool_Item* poolData = pool->GetPoolData();

	ItemConfig_Item* bibleConfig = g_Manager->GetItemConfig()->GetCollectible(COLLECTIBLE_BIBLE);
	if (bibleConfig == nullptr) {
		lua_pushboolean(L, false);
		return 1;
	}

	if (isCollectibleRemoved(itemPool, COLLECTIBLE_BIBLE)
	|| isCollectibleBlacklisted(itemPool, COLLECTIBLE_BIBLE)
	|| !bibleConfig->IsAvailableEx(-1)) {
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, rng->RandomInt((uint32_t)poolData->_totalWeight) < poolData->_bibleUpgrade);
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryMagicSkinMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	uint32_t seed = (uint32_t)luaL_optinteger(L, 2, Isaac::genrand_int32());
	
	EnsureValidSeed(seed);
	lua_pushboolean(L, itemPool->TryReplaceWithMagicSkin(seed));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryRosaryMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	ItemPool_Item* poolData = pool->GetPoolData();

	uint32_t trinketMultiplier = g_Game->_playerManager.GetTrinketMultiplier(TRINKET_ROSARY_BEAD);
	if (trinketMultiplier <= 1) {
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, rng->RandomInt((uint32_t)poolData->_totalWeight) < trinketMultiplier);
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolHasCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int collectibleID = (int)luaL_checkinteger(L, 2);

	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	std::vector<ItemConfig_Item*>& collectList = *g_Manager->GetItemConfig()->GetCollectibles();

	lua_pushboolean(L, (collectibleID >= 0 && (unsigned int)collectibleID < collectList.size()) && (!removedCollectibles[collectibleID]));
	/*const int itemPoolType = luaL_optinteger(L, 3, -1);

	if (itemPoolType < POOL_NULL || itemPoolType > POOL_ROTTEN_BEGGAR) {
		luaL_error(L, "Invalid ItemPoolType");
	}

	bool result = false;
	if (itemPoolType == -1) {
		for (int i = 0; i < 33; i++) {
			std::vector<PoolItem> poolList = itemPool->_pools[i]._poolList;
			for (auto& item : poolList) {
				result = item._itemID == collectibleID && item._weight > .0f ? true : result;
				if (result) break;
			}
			if (result) break;
		}
	}
	else {
		std::vector<PoolItem> poolList = itemPool->_pools[itemPoolType]._poolList;
		for (auto& item : poolList) {
			result = item._itemID == collectibleID && item._weight > .0f ? true : result;
			if (result) break;
		}
	}
	lua_pushboolean(L, result);
	*/

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetRemovedCollectibles) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();

	lua_newtable(L);
	for (size_t i = 1; i < removedCollectibles.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushboolean(L, removedCollectibles[i]);
		lua_rawset(L, -3);
	}
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetRoomBlacklistedCollectibles) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	std::vector<bool>& blacklistedCollectibles = *itemPool->GetRoomBlacklistedCollectibles();

	lua_newtable(L);
	for (size_t i = 1; i < blacklistedCollectibles.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushboolean(L, blacklistedCollectibles[i]);
		lua_rawset(L, -3);
	}
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectiblesFromPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int itemPoolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(itemPoolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	std::vector<PoolItem>& poolItem = pool->GetPoolData()->_poolList;

	lua_newtable(L);

	for (size_t i = 0; i < poolItem.size(); i++) {
		const auto& item = poolItem[i];
		//lua_pushinteger(L, idx);
		lua_newtable(L);

		lua_pushstring(L, "itemID");
		lua_pushinteger(L, item._itemID);
		lua_rawset(L, -3);

		lua_pushstring(L, "initialWeight");
		lua_pushnumber(L, item._initialWeight);
		lua_rawset(L, -3);

		lua_pushstring(L, "weight");
		lua_pushnumber(L, item._weight);
		lua_rawset(L, -3);

		lua_pushstring(L, "decreaseBy");
		lua_pushnumber(L, item._decreaseBy);
		lua_rawset(L, -3);

		lua_pushstring(L, "removeOn");
		lua_pushnumber(L, item._removeOn);
		lua_rawset(L, -3);

		lua_pushstring(L, "isUnlocked");
		lua_pushboolean(L, item._isUnlocked);
		lua_rawset(L, -3);

		lua_pushinteger(L, i + 1); // using one-based indexing since it's what's used by LUA.
		lua_insert(L, -2);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolSetLastPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	itemPool->_lastPool = poolType;

	return 0;
}

LUA_FUNCTION(Lua_ItemPoolHasTrinket) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const unsigned int trinketID = (int)luaL_checkinteger(L, 2);

	std::vector<ItemConfig_Item*>& trinketList = *g_Manager->GetItemConfig()->GetTrinkets();
	std::vector<TrinketPoolItem>& poolTrinketItems = itemPool->_trinketPoolItems;

	if (trinketID >= trinketList.size()) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, (poolTrinketItems[trinketID]._inPool));
	}

	return 1;
}

// this is a rewrite of an existing function with error checking so invalid ids don't crash
LUA_FUNCTION(Lua_ItemPoolCanSpawnCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int id = (int)luaL_checkinteger(L, 2);
	bool unkFlag = lua::luaL_checkboolean(L, 3);

	ItemConfig_Item* item = g_Manager->GetItemConfig()->GetCollectible(id);
	if (item == nullptr) {
		return luaL_argerror(L, 2, "Invalid collectible ID");
	}

	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	std::vector<bool>& blacklistedCollectibles = *itemPool->GetRoomBlacklistedCollectibles();

	lua_pushboolean(L,
		!removedCollectibles[id] && !blacklistedCollectibles[id]
		&& item->IsAvailableEx((unkFlag ^ 1) * 2 - 3)
		&& !(g_Game->GetPlayerManager()->AnyoneHasTrinket(TRINKET_NO) && item->type == 3));

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetNumAvailableTrinkets) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	lua_pushinteger(L, itemPool->_numAvailableTrinkets);

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolUnidentifyPill) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int pillColor = ((int)luaL_checkinteger(L, 2)) & 0x7ff;
	if (pillColor >= 0 && pillColor <= 14) {
		itemPool->_idendifiedPillEffects[pillColor] = false;
	}
	
	return 0;
}

LUA_FUNCTION(Lua_ItemPoolGetPillColor) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int pillEffect = (int)luaL_checkinteger(L, 2);

	for (int i = 0; i < 15; i++) {
		if (itemPool->_pillEffects[i] == pillEffect) {
			lua_pushinteger(L, i);
			return 1;
		}
	}

	lua_pushinteger(L, -1);
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolAddBibleUpgrade) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int add = (int)luaL_checkinteger(L, 2);
	const int poolType = (int)luaL_checkinteger(L, 3);

	if ((uint32_t)poolType > ItemPoolManager::GetNumItemPools()) {
		return luaL_argerror(L, 3, "Invalid ItemPoolType");
	}

	itemPool->AddBibleUpgrade(add, poolType);

	return 0;
}

LUA_FUNCTION(Lua_ItemPoolGetBibleUpgrades) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int poolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	lua_pushinteger(L, pool->GetPoolData()->_bibleUpgrade);

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolResetCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const int collectible = (int)luaL_checkinteger(L, 2);

	if (collectible < COLLECTIBLE_NULL || collectible >= (int)g_Manager->GetItemConfig()->GetCollectibles()->size()) {
		return luaL_argerror(L, 2, "Invalid Collectible");
	}

	itemPool->ResetCollectible(collectible);

	return 0;
}

LUA_FUNCTION(Lua_ItemPoolGetNumItemPools) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");

	lua_pushinteger(L, ItemPoolManager::GetNumItemPools());

	return 1;
}

static std::vector<ItemPoolManager::PoolItemDesc> create_lua_pool_items(lua_State* L, int index, ItemPoolManager::Warnings& warnings)
{
	std::vector<ItemPoolManager::PoolItemDesc> poolItems;
	size_t length = (size_t)lua_rawlen(L, index);

	if (length > 0)
	{
		// Treat table as a table of virtual items;

		ItemPoolManager::Warnings conversionWarnings;
		for (size_t i = 1; i <= length; i++)
		{
			lua_rawgeti(L, index, i);

			poolItems.emplace_back(L, -1, conversionWarnings);

			if (!conversionWarnings.empty())
			{
				warnings.emplace_back("Something went wrong when building PoolItem " + std::to_string(i) + ":\n");
				warnings.insert(warnings.end(), conversionWarnings.begin(), conversionWarnings.end());
				conversionWarnings.clear();
			}

			lua_pop(L, 1);
		}
	}
	else
	{
		ItemPoolManager::Warnings conversionWarnings;
		poolItems.emplace_back(L, index, conversionWarnings);

		if (!conversionWarnings.empty())
		{
			warnings.emplace_back("Something went wrong when building PoolItem :\n");
			warnings.insert(warnings.end(), conversionWarnings.begin(), conversionWarnings.end());
			conversionWarnings.clear();
		}
	}

	return poolItems;
}

LUA_FUNCTION(Lua_ItemPoolAddCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	if (!lua_istable(L, 3)) {
		return luaL_argerror(L, 3, "Expected table");
	}

	ItemPoolManager::Warnings warnings;
	auto poolItems = create_lua_pool_items(L, 3, warnings);

	for (const auto& poolItem : poolItems)
	{
		pool->AddVirtualItem(poolItem);
	}

	if (!warnings.empty())
	{
		print_warnings(warnings);
	}

	return 0;
}

LUA_FUNCTION(Lua_ItemPoolAddTemporaryCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	if (!lua_istable(L, 3)) {
		return luaL_argerror(L, 3, "Expected table");
	}

	ItemPoolManager::Warnings warnings;
	auto poolItems = create_lua_pool_items(L, 3, warnings);
	ItemPoolManager::Error error;

	for (const auto& poolItem : poolItems)
	{
		pool->AddTemporaryItem(poolItem, error);
		if (error)
		{
			return luaL_error(L, error->c_str());
		}
	}

	if (!warnings.empty())
	{
		print_warnings(warnings);
	}

	return 0;
}

LUA_FUNCTION(Lua_ItemPoolRemoveTemporaryCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);

	auto* pool = ItemPoolManager::GetItemPool(poolType);

	if (!pool) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	if (!lua_istable(L, 3)) {
		return luaL_argerror(L, 3, "Expected table");
	}

	ItemPoolManager::Warnings warnings;
	auto poolItems = create_lua_pool_items(L, 3, warnings);
	ItemPoolManager::Error error;

	for (const auto& poolItem : poolItems)
	{
		pool->RemoveTemporaryItem(poolItem, error);
		if (error)
		{
			return luaL_error(L, error->c_str());
		}
	}

	if (!warnings.empty())
	{
		print_warnings(warnings);
	}

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetCollectible", Lua_ItemPoolGetCollectibleEx },
		{ "AddBibleUpgrade", Lua_ItemPoolAddBibleUpgrade },

		{ "GetCardEx", Lua_ItemPoolGetCardEx },
		{ "GetRandomPool", Lua_ItemPoolGetRandomPool },
		{ "PickCollectible", Lua_ItemPoolPickCollectible },
		{ "GetCollectibleFromList", Lua_ItemPoolGetCollectibleFromList },
		//{ "TryBibleMorph", Lua_ItemPoolTryBibleMorph },
		//{ "TryMagicSkinMorph", Lua_ItemPoolTryMagicSkinMorph },
		//{ "TryRosaryMorph", Lua_ItemPoolTryRosaryMorph },
		{ "HasCollectible", Lua_ItemPoolHasCollectible },
		{ "GetRemovedCollectibles", Lua_ItemPoolGetRemovedCollectibles },
		{ "GetRoomBlacklistedCollectibles", Lua_ItemPoolGetRoomBlacklistedCollectibles },
		{ "GetCollectiblesFromPool", Lua_ItemPoolGetCollectiblesFromPool },
		{ "SetLastPool", Lua_ItemPoolSetLastPool },
		{ "CanSpawnCollectible", Lua_ItemPoolCanSpawnCollectible },
		{ "HasTrinket", Lua_ItemPoolHasTrinket },
		{ "GetNumAvailableTrinkets", Lua_ItemPoolGetNumAvailableTrinkets },
		{ "UnidentifyPill", Lua_ItemPoolUnidentifyPill },
		{ "GetPillColor", Lua_ItemPoolGetPillColor },
		{ "GetBibleUpgrades", Lua_ItemPoolGetBibleUpgrades },
		{ "ResetCollectible", Lua_ItemPoolResetCollectible },
		{ "GetNumItemPools", Lua_ItemPoolGetNumItemPools },
		{ "AddCollectible", Lua_ItemPoolAddCollectible },
		{ "AddTemporaryCollectible", Lua_ItemPoolAddTemporaryCollectible },
		{ "RemoveTemporaryCollectible", Lua_ItemPoolRemoveTemporaryCollectible },

		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ITEM_POOL, functions);
}
