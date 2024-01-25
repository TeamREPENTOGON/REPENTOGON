#include <stdio.h>

#include <LuaJIT/src/lua.hpp>


#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "ffi.h"

lua_State* L;

HOOK_METHOD(Level, GetName, () -> std::string) {
    luaL_dofile(L, "test.lua");

    lua_getglobal(L, "getName");
    lua_pcall(L, 0, 1, 0);

    const char* str = luaL_checkstring(L, -1);
    printf("str: %s\n", str);

    return std::string(str);
}

extern "C" {
    __declspec(dllexport) Entity* L_Spawn(int type, int variant, const Vector* pos, const Vector* vel, Entity* spawner, unsigned int subtype, unsigned int seed, unsigned int unk) {
        return g_Game->Spawn(type, variant, *pos, *vel, spawner, subtype, seed, unk);
    }
}

//HOOK_METHOD(Game, Update, () -> void) {
//    luaL_dofile(L, "test.lua");
//
//    lua_getglobal(L, "getName");
//    lua_pcall(L, 0, 1, 0);
//
//    const char* str = luaL_checkstring(L, -1);
//    printf("str: %s\n", str);
//}

extern "C" __declspec(dllexport) int ModInit(int argc, char** argv) {
	printf("luajit test\n");

    ZHL::Init();

	L = luaL_newstate();
	luaL_openlibs(L);
    lua_getglobal(L, "print");
    lua_pushstring(L, "Hello from Luajit!\n");
    lua_pcall(L, 1, 0, 0);

	return 0;
}