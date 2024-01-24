#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LootListEntryGetType) {
	LootListEntry* entry = lua::GetUserdata<LootListEntry*>(L, 1, lua::metatables::LootListEntryMT);
	lua_pushinteger(L, entry->_type);
	return 1;
}

static void RegisterLootListEntry(lua_State* L) {

	luaL_Reg functions[] = {
		{ "GetType", Lua_LootListEntryGetType },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LootListEntryMT, lua::metatables::LootListEntryMT, functions);
	//lua_register(L, lua::metatables::CapsuleMT, Lua_CapsuleConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterLootListEntry(_state);
}