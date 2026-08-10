#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "compat-5.3.h"
#ifdef __cplusplus
}
#endif

#ifndef LUA_LOADED_TABLE
#define LUA_LOADED_TABLE "_LOADED"
#endif

#ifndef LUA_PRELOAD_TABLE
#define LUA_PRELOAD_TABLE "_PRELOAD"
#endif

#ifndef lua_newuserdatauv
#define lua_newuserdatauv(L, size, nuvalue) lua_newuserdata(L, size)
#endif

#ifndef l_likely
#define l_likely(x)   (x)
#endif

#ifndef l_unlikely
#define l_unlikely(x) (x)
#endif

#ifndef luaL_typeerror
#define luaL_typeerror(L, arg, tname) \
    luaL_argerror(L, arg, lua_pushfstring(L, "%s expected, got %s", \
        tname, luaL_typename(L, arg)))
#endif

#ifndef lua_KContext
typedef intptr_t lua_KContext;
#endif
#ifndef lua_KFunction
typedef int (*lua_KFunction)(lua_State *L, int status, lua_KContext ctx);
#endif
