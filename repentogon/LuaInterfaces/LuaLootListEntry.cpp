#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LootListEntryGetType) {
	LootListEntry* entry = *lua::GetRawUserdata<LootListEntry**>(L, 1, lua::metatables::LootListEntryMT);
	lua_pushinteger(L, entry->_type);
	return 1;
}

LUA_FUNCTION(Lua_LootListEntryGetVariant) {
	LootListEntry* entry = *lua::GetRawUserdata<LootListEntry**>(L, 1, lua::metatables::LootListEntryMT);
	lua_pushinteger(L, entry->_variant);
	return 1;
}

LUA_FUNCTION(Lua_LootListEntryGetSubType) {
	LootListEntry* entry = *lua::GetRawUserdata<LootListEntry**>(L, 1, lua::metatables::LootListEntryMT);
	lua_pushinteger(L, entry->_subType);
	return 1;
}

LUA_FUNCTION(Lua_LootListEntryGetSeed) {
	LootListEntry* entry = *lua::GetRawUserdata<LootListEntry**>(L, 1, lua::metatables::LootListEntryMT);
	lua_pushinteger(L, entry->_seed);
	return 1;
}

LUA_FUNCTION(Lua_LootListEntryGetRNG) {
	LootListEntry* entry = *lua::GetRawUserdata<LootListEntry**>(L, 1, lua::metatables::LootListEntryMT);
	RNG* rng = entry->_rng;

	if (rng == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, rng, lua::Metatables::RNG);
	}

	return 1;
}


static void RegisterLootListEntry(lua_State* L) {

	luaL_Reg functions[] = {
		{ "GetType", Lua_LootListEntryGetType },
		{ "GetVariant", Lua_LootListEntryGetVariant },
		{ "GetSubType", Lua_LootListEntryGetSubType },
		{ "GetSeed", Lua_LootListEntryGetSeed },
		{ "GetRNG", Lua_LootListEntryGetRNG },
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