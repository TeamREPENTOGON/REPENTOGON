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

	// IsaacString str;
	const char* translateString = luaL_checkstring(L, 1);
	/* if (strlen(translateString) < 16) {
		strcpy(str.text, translateString);
	}
	else {
		*(char**)str.text = (char*)translateString;
	}
	str.unk = str.size = strlen(translateString); */

	const char* retStr = Isaac::GetString(stringTable, translateString);
	lua_pushstring(L, retStr);

	return 1;
}

static void RegisterIsaacGetString(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetString");
	lua_pushcfunction(L, Lua_IsaacGetString);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterIsaacGetString(state);
}