#include <stdio.h>

#include <LuaJIT/src/lua.hpp>

lua_State* L;

namespace LuaJITState {
	void Init() {
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_getglobal(L, "print");
		lua_pushstring(L, "Hello from Luajit!\n");
		lua_pcall(L, 1, 0, 0);

		luaL_dofile(L, "resources/scripts/enums.lua");
		luaL_dofile(L, "resources/scripts/enums_ex.lua");
		luaL_dofile(L, "resources/scripts/ljtest.lua");
	}
}