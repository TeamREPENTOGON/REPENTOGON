#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PickupGetLootList) {
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 1, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	bool unk = lua::luaL_optboolean(L, 2, false);
	LootList ret = pickup->GetLootList(unk);
	LootList** ud = (LootList**)lua_newuserdata(L, sizeof(LootList*));

	*ud = &ret;
	luaL_setmetatable(L, lua::metatables::LootListMT);
	return 1;
}

LUA_FUNCTION(Lua_LootListGetEntries)
{
	LootList* lootList = *lua::GetUserdata<LootList**>(L, 1, lua::metatables::LootListMT);
	std::deque<LootListEntry>& entries = lootList->_entries;

	
	lua_newtable(L);
	int idx = 1;
	for (LootListEntry& item : entries) {
		LootListEntry** ud = (LootListEntry**)lua_newuserdata(L, sizeof(LootListEntry*));
		*ud = &item;
		luaL_setmetatable(L, lua::metatables::LootListEntryMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_LootListFirstEntry) {
	LootList* lootList = *lua::GetUserdata<LootList**>(L, 1, lua::metatables::LootListMT);
	lua_pushinteger(L, lootList->_entries.size());
	return 1;
}

/*LUA_FUNCTION(Lua_LootList__gc) {
	LootList* lootlist = lua::GetUserdata<LootList*>(L, 1, lua::metatables::LootListMT);
	lootlist->~LootList();

	return 0;
}
*/

static void RegisterLootList(lua_State* L) {

	lua::RegisterFunction(L, lua::Metatables::ENTITY_PICKUP, "GetLootList", Lua_PickupGetLootList);

	luaL_Reg functions[] = {
		{ "GetEntries", Lua_LootListGetEntries },
		{ "FirstEntry", Lua_LootListFirstEntry },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LootListMT, lua::metatables::LootListMT, functions);
	//lua_register(L, lua::metatables::CapsuleMT, Lua_CapsuleConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLootList(_state);
}