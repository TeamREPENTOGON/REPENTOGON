#include <stdio.h>

#include <LuaJIT/src/lua.hpp>
#include "HookSystem.h"

lua_State* L;

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	printf("luajit test\n");

	L = luaL_newstate();
	luaL_openlibs(L);
	lua_getglobal(L, "print");
	lua_pushstring(L, "Hello from luajit!\n");
	lua_pcall(L, 1, 0, 0);
	lua_close(L);

	return 0;
}