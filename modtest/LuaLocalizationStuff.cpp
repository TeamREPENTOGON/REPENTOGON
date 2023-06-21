#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
}

int Lua_IsaacGetString(lua_State* L)
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

int Lua_IsaacGetLocalizedString(lua_State* L) {
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
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetString");
	lua_pushcfunction(L, Lua_IsaacGetString);
	lua_rawset(L, -3);
	lua_pushstring(L, "GetLocalizedString");
	lua_pushcfunction(L, Lua_IsaacGetLocalizedString);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterIsaacGetString(state);
}