#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_IsaacGetString)
{
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();

	const char* category = luaL_checkstring(L, 1);
	const char* translateString = luaL_checkstring(L, 2);
	if (*translateString == '#') {
		++translateString;
	}
	uint32_t unk;

	const char* retStr = stringTable->GetString(category, stringTable->language, translateString, &unk);
	lua_pushstring(L, retStr);

	return 1;
}

LUA_FUNCTION(Lua_IsaacGetLocalizedString) {
	Manager* manager = g_Manager;
	StringTable* stringTable = manager->GetStringTable();

	const char* category = luaL_checkstring(L, 1);
	const char* translateString = luaL_checkstring(L, 2);
	if (*translateString == '#') {
		++translateString;
	}
	uint32_t language = (uint32_t)luaL_checkinteger(L, 3);
	uint32_t unk;

	const char* retStr = stringTable->GetString(category, language, translateString, &unk);
	lua_pushstring(L, retStr);

	return 1;
}

static void RegisterIsaacGetString(lua_State* L) {
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetString", Lua_IsaacGetString);
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetLocalizedString", Lua_IsaacGetLocalizedString);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterIsaacGetString(_state);
}