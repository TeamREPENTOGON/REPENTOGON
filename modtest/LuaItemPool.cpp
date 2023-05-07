#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_ItemPoolGetCardEx(lua_State* L)
{
	ItemPool* itemPool = lua::GetUserdata<ItemPool*>(L, 1, lua::Metatables::ITEM_POOL, "ItemPool");
	int seed = luaL_checkinteger(L, 2);
	int specialChance = luaL_checkinteger(L, 3);
	int runeChance = luaL_checkinteger(L, 4);
	int suitChance = luaL_checkinteger(L, 5);
	bool allowNonCards = lua_toboolean(L, 6);
	lua_pushinteger(L, itemPool->GetCardEx(seed, specialChance, runeChance, suitChance, allowNonCards));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ITEM_POOL;
	lua::RegisterFunction(state, mt, "GetCardEx", Lua_ItemPoolGetCardEx);
}