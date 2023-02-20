#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ConsoleMT = "Console";

static int Lua_GetConsole(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1BB60;
	luaL_setmetatable(L, ConsoleMT);
	return 1;
}

int Lua_ConsolePrintError(lua_State* L)
{
	Console* console = *lua::GetUserdata<Console**>(L, 1, ConsoleMT);
	// std::string err = luaL_checkstring(L, 2);
	IsaacString str;
	const char* err = luaL_checkstring(L, 2);
	if (strlen(err) < 16) {
		strcpy(str.text, err);
	}
	else {
		*(char**)str.text = (char*)err;
	}
	str.unk = str.size = strlen(err);
	console->PrintError(str);
	return 1;
}

static void RegisterConsole(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetConsole");
	lua_pushcfunction(L, Lua_GetConsole);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, ConsoleMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "PrintError", Lua_ConsolePrintError },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterConsole(state);
}

