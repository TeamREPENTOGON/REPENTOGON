#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

inline bool Lua_NotPassedOrNil(lua_State* L, int narg) {
	return (lua_gettop(L) < narg || lua_isnil(L, narg));
}

inline bool isCollectibleRemoved(ItemPool* itemPool, uint32_t collectibleID) {
	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	return removedCollectibles[collectibleID];
}

inline bool isCollectibleBlacklisted(ItemPool* itemPool, uint32_t collectibleID) {
	std::vector<bool>& blacklistedCollectibles = *itemPool->GetRoomBlacklistedCollectibles();
	return blacklistedCollectibles[collectibleID];
}

LUA_FUNCTION(Lua_ItemPoolGetCardEx)
{
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int seed = (int)luaL_checkinteger(L, 2);
	int specialChance = (int)luaL_checkinteger(L, 3);
	int runeChance = (int)luaL_checkinteger(L, 4);
	int suitChance = (int)luaL_checkinteger(L, 5);
	bool allowNonCards = lua::luaL_checkboolean(L, 6);
	lua_pushinteger(L, itemPool->GetCardEx(seed, specialChance, runeChance, suitChance, allowNonCards));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectibleEx) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	bool decrease = lua::luaL_optboolean(L, 3, false);
	uint32_t seed = (unsigned int)luaL_optinteger(L, 4, Isaac::genrand_int32());
	if (seed == 0) {
		seed = 1;
	}
	int defaultItem = (int)luaL_optinteger(L, 5, COLLECTIBLE_NULL);
	uint32_t flags = (unsigned int)luaL_optinteger(L, 6, 0);

	if (poolType <= POOL_NULL ||  poolType >= NUM_ITEMPOOLS) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	flags = flags << 1;
	if (!decrease) {
		flags |= 1;
	}

	lua_pushinteger(L, itemPool->GetCollectible(poolType, seed, flags, defaultItem));
	return 1;
}

inline int GetChaosPoolEx(ItemPool* itemPool, RNG* rng, std::unordered_map<int, bool> blacklist) {
	WeightedOutcomePicker picker;

	for (int poolType = POOL_TREASURE; poolType < NUM_ITEMPOOLS; poolType++) {
		if (blacklist.find(poolType) != blacklist.end()) {
			continue;
		}

		ItemPool_Item* pool = itemPool->_pools + poolType;

		const uint32_t scaleFactor = 100;
		WeightedOutcomePicker_Outcome outcome{poolType, (uint32_t)(pool->_totalWeight * scaleFactor)};
		picker.AddOutcomeWeight(outcome, false);
	}
	
	rng->Next();

	RNG pickerRNG;
	pickerRNG.SetSeed(rng->_seed, 35);

	return picker.PickOutcome(pickerRNG);
}

LUA_FUNCTION(Lua_ItemPoolGetRandomPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	RNG* rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	bool advancedSearch = lua::luaL_optboolean(L, 3, false);

	if (!advancedSearch) {
		lua_pushinteger(L, itemPool->get_chaos_pool(rng));
		return 1;
	}

	if (!(Lua_NotPassedOrNil(L, 4) || lua_istable(L, 4))) {
		return luaL_argerror(L, 4, "Invalid Blacklist");
	}

	std::unordered_map<int, bool> blacklist;

	if (lua_istable(L, 4)) {
		size_t length = (size_t)lua_rawlen(L, 4);

		for (size_t i = 0; i < length; i++)
		{
			lua_rawgeti(L, 4, i + 1);
			blacklist[(int)luaL_checkinteger(L, -1)] = true;
			lua_pop(L, 1);
		}
	}

	lua_pushinteger(L, GetChaosPoolEx(itemPool, rng, blacklist));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolPickCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	bool decrease = lua::luaL_optboolean(L, 3, false);
	RNG* rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, "RNG");
	uint32_t flags = (unsigned int)luaL_optinteger(L, 5, 0);

	if (poolType <= POOL_NULL ||  poolType >= NUM_ITEMPOOLS) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	ItemPool_Item* pool = itemPool->_pools + poolType;
	float targetWeight = rng->RandomFloat() * pool->_totalWeight;

	flags = flags << 1;
	if (!decrease) {
		flags |= 1;
	}

	PoolItem* poolItem = itemPool->pick_collectible(targetWeight, pool, flags);

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
	lua_pushinteger(L, itemPool->GetCollectibleFromList(list, length, seed, defaultItem, addToBlacklist, excludeLockedItems));

	// delete the array
	delete[] list;

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryBibleMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");

	if (poolType <= POOL_NULL || poolType >= NUM_ITEMPOOLS) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	ItemPool_Item* pool = itemPool->_pools + poolType;

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

	lua_pushboolean(L, rng->RandomInt(pool->_totalWeight) < pool->_bibleUpgrade);
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryMagicSkinMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	uint32_t seed = (unsigned int)luaL_optinteger(L, 2, Isaac::genrand_int32());
	if (seed == 0)
	{
		seed = 1;
	}
	
	lua_pushboolean(L, itemPool->TryReplaceWithMagicSkin(seed));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolTryRosaryMorph) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");

	if (poolType <= POOL_NULL || poolType >= NUM_ITEMPOOLS) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	ItemPool_Item* pool = itemPool->_pools + poolType;

	uint32_t trinketMultiplier = g_Game->_playerManager.GetTrinketMultiplier(TRINKET_ROSARY_BEAD);
	if (trinketMultiplier <= 1) {
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, rng->RandomInt(pool->_totalWeight) < trinketMultiplier);
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

	if (itemPoolType >= POOL_TREASURE && itemPoolType < NUM_ITEMPOOLS) {
		std::vector<PoolItem>& poolItem = itemPool->_pools[itemPoolType]._poolList;

		lua_newtable(L);

		for (const auto& item : poolItem) {
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

			lua_pushinteger(L, item._itemID);
			lua_insert(L, -2);
			lua_rawset(L, -3);

		}
	}
	else {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolSetLastPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int poolType = (int)luaL_checkinteger(L, 2);

	if (poolType <= POOL_NULL || poolType >= NUM_ITEMPOOLS) {
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
		&& !(g_Game->GetPlayerManager()->FirstTrinketOwner(TRINKET_NO, 0x0, true) != 0x0 && item->type == 3));

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

LUA_FUNCTION(Lua_ItemPoolGetBibleUpgrades) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const unsigned int pool = (int)luaL_checkinteger(L, 2);

	if (pool <= POOL_NULL || pool >= NUM_ITEMPOOLS) {
		return luaL_argerror(L, 2, "Invalid ItemPoolType");
	}

	lua_pushinteger(L, itemPool->_pools[pool]._bibleUpgrade);

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetCollectible", Lua_ItemPoolGetCollectibleEx },

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

		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ITEM_POOL, functions);
}
