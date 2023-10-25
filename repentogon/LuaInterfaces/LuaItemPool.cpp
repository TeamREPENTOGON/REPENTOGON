#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ItemPoolGetCardEx)
{
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int seed = (int)luaL_checkinteger(L, 2);
	int specialChance = (int)luaL_checkinteger(L, 3);
	int runeChance = (int)luaL_checkinteger(L, 4);
	int suitChance = (int)luaL_checkinteger(L, 5);
	bool allowNonCards = lua_toboolean(L, 6);
	lua_pushinteger(L, itemPool->GetCardEx(seed, specialChance, runeChance, suitChance, allowNonCards));
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectibleFromList) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	if (!lua_istable(L, 2))
	{
		luaL_error(L, "Expected a table as second argument");
		return 0;
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

LUA_FUNCTION(Lua_ItemPoolHasCollectible) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int collectibleID = (int)luaL_checkinteger(L, 2);

	std::vector<bool>& removedCollectibles = *itemPool->GetRemovedCollectibles();
	std::vector<ItemConfig_Item*>& collectList = *g_Manager->GetItemConfig()->GetCollectibles();

	lua_pushboolean(L, (collectibleID < collectList.size()) && (!removedCollectibles[collectibleID]));
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
	int idx = 1;
	for (auto collectible : removedCollectibles) {
		lua_pushnumber(L, idx);
		lua_pushboolean(L, collectible);
		lua_settable(L, -3);
		idx++;
	}
	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetCollectiblesFromPool) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int itemPoolType = (int)luaL_checkinteger(L, 2);

	if (itemPoolType >= 0 && itemPoolType <= 30) {
		std::vector<PoolItem>& poolItem = itemPool->_pools[itemPoolType]._poolList;

		lua_newtable(L);
		for (const auto& item : poolItem) {
			//lua_pushinteger(L, idx);
			lua_newtable(L);


			lua_pushstring(L, "itemID");
			lua_pushinteger(L, item._itemID);
			lua_settable(L, -3);

			lua_pushstring(L, "initialWeight");
			lua_pushnumber(L, item._initialWeight);
			lua_settable(L, -3);

			lua_pushstring(L, "weight");
			lua_pushnumber(L, item._weight);
			lua_settable(L, -3);

			lua_pushstring(L, "decreaseBy");
			lua_pushnumber(L, item._decreaseBy);
			lua_settable(L, -3);

			lua_pushstring(L, "removeOn");
			lua_pushnumber(L, item._removeOn);
			lua_settable(L, -3);

			lua_pushinteger(L, item._itemID);
			lua_insert(L, -2);
			lua_settable(L, -3);

		}
	}
	else {
		luaL_error(L, "Invalid ItemPoolType");
	}

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolHasTrinket) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	const unsigned int trinketID = (int)luaL_checkinteger(L, 2);

	std::vector<ItemConfig_Item*>& trinketList = *g_Manager->GetItemConfig()->GetTrinkets();
	std::vector<TrinketPoolItem>& poolTrinketItems = itemPool->_trinketPoolItems;

	lua_pushinteger(L, poolTrinketItems[trinketID]._ID);

	/*bool found = false;
	for (const auto& trinketItem : poolTrinketItems) {
		if (trinketItem._ID == trinketID) {
			found = true;
			lua_pushboolean(L, (trinketItem._inPool));
			break;
		}
	}

	if (!found || trinketID >= trinketList.size()) {
		lua_pushboolean(L, false);
	}
	*/

	return 1;
}

LUA_FUNCTION(Lua_ItemPoolGetNumAvailableTrinkets) {
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	lua_pushinteger(L, itemPool->_numAvailableTrinkets);

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetCardEx", Lua_ItemPoolGetCardEx },
		{ "GetCollectibleFromList", Lua_ItemPoolGetCollectibleFromList },
		{ "HasCollectible", Lua_ItemPoolHasCollectible },
		{ "GetRemovedCollectibles", Lua_ItemPoolGetRemovedCollectibles },
		{ "GetCollectiblesFromPool", Lua_ItemPoolGetCollectiblesFromPool },
		{ "HasTrinket", Lua_ItemPoolHasTrinket },
		{ "GetNumAvailableTrinkets", Lua_ItemPoolGetNumAvailableTrinkets },

		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ITEM_POOL, functions);
}