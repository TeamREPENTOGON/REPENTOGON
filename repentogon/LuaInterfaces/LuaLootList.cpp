#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LootListGetEntries)
{
	LootList* lootList = *lua::GetUserdata<LootList**>(L, 1, lua::metatables::LootListMT);
	std::deque<LootListEntry>& entries = lootList->_entries;

	lua_newtable(L);
	
	for (size_t i = 0; i < entries.size(); i++) {
		const LootListEntry& entry = entries[i];
		LootListEntry* ud = (LootListEntry*)lua_newuserdata(L, sizeof(LootListEntry));
		*ud = entry;  // Copy the entry into the userdata
		luaL_setmetatable(L, lua::metatables::LootListEntryMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_LootList__gc) {
	LootList* lootlist = lua::GetUserdata<LootList*>(L, 1, lua::metatables::LootListMT);
	lootlist->~LootList();

	return 0;
}

static void RegisterLootList(lua_State* L) {

	luaL_Reg functions[] = {
		{ "GetEntries", Lua_LootListGetEntries },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LootListMT, lua::metatables::LootListMT, functions, Lua_LootList__gc);
	//lua_register(L, lua::metatables::CapsuleMT, Lua_CapsuleConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLootList(_state);
}