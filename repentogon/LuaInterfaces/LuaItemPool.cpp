#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_ItemPoolGetCardEx(lua_State* L)
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

	size_t length = (size_t) lua_rawlen(L, 2);

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

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ITEM_POOL;
	lua::RegisterFunction(state, mt, "GetCardEx", Lua_ItemPoolGetCardEx);
	lua::RegisterFunction(state, mt, "GetCollectibleFromList", Lua_ItemPoolGetCollectibleFromList);
}