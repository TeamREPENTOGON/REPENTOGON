#define COMPAT53_LIB

#include "LuaJIT/src/lua.h"
#include "LuaJIT/src/lauxlib.h"
#include "LuaJIT/src/lualib.h"
#include "LuaJIT/src/luajit.h"
#include "lua53compat/lua53compat.h"

// Most code here taken from https://github.com/lunarmodules/lua-compat-5.3/, licensed under MIT.

#ifndef LUA_OPEQ
#  define LUA_OPEQ 0
#endif
#ifndef LUA_OPLT
#  define LUA_OPLT 1
#endif
#ifndef LUA_OPLE
#  define LUA_OPLE 2
#endif

#define lua_getfield(L, i, k) \
  (lua_getfield((L), (i), (k)), lua_type((L), -1))

COMPAT53_API int lua_rawgetp(lua_State* L, int i, const void* p) {
    int abs_i = lua_absindex(L, i);
    lua_pushlightuserdata(L, (void*)p);
    lua_rawget(L, abs_i);
    return lua_type(L, -1);
}

COMPAT53_API void lua_rawsetp(lua_State* L, int i, const void* p) {
    int abs_i = lua_absindex(L, i);
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_pushlightuserdata(L, (void*)p);
    lua_insert(L, -2);
    lua_rawset(L, abs_i);
}

static void compat53_call_lua(lua_State* L, char const code[], size_t len,
    int nargs, int nret) {
    lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)code);
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        if (luaL_loadbuffer(L, code, len, "=none"))
            lua_error(L);
        lua_pushvalue(L, -1);
        lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)code);
    }
    lua_insert(L, -nargs - 1);
    lua_call(L, nargs, nret);
}

static const char compat53_compare_code[] =
"local a,b=...\n"
"return a<=b\n";

COMPAT53_API int lua_compare(lua_State* L, int idx1, int idx2, int op) {
    int result = 0;
    switch (op) {
    case LUA_OPEQ:
        return lua_equal(L, idx1, idx2);
    case LUA_OPLT:
        return lua_lessthan(L, idx1, idx2);
    case LUA_OPLE:
        luaL_checkstack(L, 5, "not enough stack slots");
        idx1 = lua_absindex(L, idx1);
        idx2 = lua_absindex(L, idx2);
        lua_pushvalue(L, idx1);
        lua_pushvalue(L, idx2);
        compat53_call_lua(L, compat53_compare_code,
            sizeof(compat53_compare_code) - 1, 2, 1);
        result = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return result;
    default:
        luaL_error(L, "invalid 'op' argument for lua_compare");
    }
    return 0;
}

COMPAT53_API void lua_seti(lua_State* L, int index, lua_Integer i) {
    luaL_checkstack(L, 1, "not enough stack slots available");
    index = lua_absindex(L, index);
    lua_pushinteger(L, i);
    lua_insert(L, -2);
    lua_settable(L, index);
}

COMPAT53_API void luaL_requiref(lua_State* L, const char* modname,
    lua_CFunction openf, int glb) {
    luaL_checkstack(L, 3, "not enough stack slots available");
    luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
    if (lua_getfield(L, -1, modname) == LUA_TNIL) {
        lua_pop(L, 1);
        lua_pushcfunction(L, openf);
        lua_pushstring(L, modname);
        lua_call(L, 1, 1);
        lua_pushvalue(L, -1);
        lua_setfield(L, -3, modname);
    }
    if (glb) {
        lua_pushvalue(L, -1);
        lua_setglobal(L, modname);
    }
    lua_replace(L, -2);
}

COMPAT53_API int lua_geti(lua_State* L, int index, lua_Integer i) {
    index = lua_absindex(L, index);
    lua_pushinteger(L, i);
    lua_gettable(L, index);
    return lua_type(L, -1);
}

static void compat53_reverse(lua_State* L, int a, int b) {
    for (; a < b; ++a, --b) {
        lua_pushvalue(L, a);
        lua_pushvalue(L, b);
        lua_replace(L, a);
        lua_replace(L, b);
    }
}

COMPAT53_API void lua_rotate(lua_State* L, int idx, int n) {
    int n_elems = 0;
    idx = lua_absindex(L, idx);
    n_elems = lua_gettop(L) - idx + 1;
    if (n < 0)
        n += n_elems;
    if (n > 0 && n < n_elems) {
        luaL_checkstack(L, 2, "not enough stack slots available");
        n = n_elems - n;
        compat53_reverse(L, idx, idx + n - 1);
        compat53_reverse(L, idx + n, idx + n_elems - 1);
        compat53_reverse(L, idx, idx + n_elems - 1);
    }
}

// Wrappers (these are macros but need to be explicitly exported)
#undef lua_getglobal
#undef lua_setglobal
#undef lua_callk
#undef lua_pcallk

COMPAT53_API int lua_getglobal(lua_State* L, const char* name) {
    return lua_getfield(L, -10002, name);
};

COMPAT53_API void lua_setglobal(lua_State* L, const char* name) {
    lua_setfield(L, LUA_GLOBALSINDEX, name);
}

COMPAT53_API void lua_callk(lua_State* L, int nargs, int nresults, int ctx, int k) {
    lua_call(L, nargs, nresults);
}

COMPAT53_API void lua_pcallk(lua_State* L, int nargs, int nresults, int msgh, int ctx, int k) {
    lua_pcall(L, nargs, nresults, msgh);
}

// Stubs (these don't seem to be used by the game and don't exist in 5.1, but are still imported)
COMPAT53_API int luaopen_coroutine(lua_State* L) {
    return 0;
}

COMPAT53_API int luaopen_utf8(lua_State* L) {
    return 0;
}