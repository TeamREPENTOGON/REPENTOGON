// New functions missing in LuaJIT needed for Isaac to launch.
// Many of these are already implemented by compat53. For those, we just expose their implementation.

#include "lua.h"
#include "lauxlib.h"
#include "compat-5.3.h"

#undef lua_getglobal
LUA_API int lua_getglobal(lua_State* L, const char* name) {
    lua_getfield(L, LUA_GLOBALSINDEX, name);
    return lua_type(L, -1);
}

#undef lua_setglobal
LUA_API void lua_setglobal(lua_State* L, const char* name) {
    lua_setfield(L, LUA_GLOBALSINDEX, name);
}

#undef lua_absindex
LUA_API int lua_absindex(lua_State* L, int i) {
    return compat53_absindex(L, i);
}

#undef lua_rotate
LUA_API void lua_rotate(lua_State* L, int idx, int n) {
    compat53_rotate(L, idx, n);
}

#undef lua_rawgetp
LUA_API int lua_rawgetp(lua_State* L, int idx, const void* p) {
    return compat53_rawgetp(L, idx, p);
}

#undef lua_rawsetp
LUA_API void lua_rawsetp(lua_State* L, int idx, const void* p) {
    compat53_rawsetp(L, idx, p);;
}

#undef lua_len
LUA_API void lua_len(lua_State* L, int idx) {
    compat53_len(L, idx);
}

#undef luaL_len
LUA_API lua_Integer luaL_len(lua_State* L, int idx) {
    return compat53L_len(L, idx);
}

#undef luaL_checkstring
LUA_API const char* luaL_checkstring(lua_State* L, int arg) {
    return luaL_checklstring(L, arg, NULL);
}

#undef luaL_tolstring
LUA_API const char* luaL_tolstring(lua_State* L, int idx, size_t* len) {
    return compat53L_tolstring(L, idx, len);
}

#undef lua_compare
LUA_API int lua_compare(lua_State* L, int idx1, int idx2, int op) {
    return compat53_compare(L, idx1, idx2, op);
}

#undef luaL_requiref
LUA_API void luaL_requiref(lua_State* L, const char* modname, lua_CFunction openf, int glb) {
 
    // Luabridge gonna Luabridge, it needs package loaded already. Let's keep it happy and well fed :)
    lua_getglobal(L, "package");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setglobal(L, "package");
    }

    lua_getfield(L, -1, "loaded");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, -3, "loaded");
    }
    lua_remove(L, -2);

    compat53L_requiref_53(L, modname, openf, glb);
}

#undef lua_callk
LUA_API void lua_callk(lua_State* L, int nargs, int nresults, intptr_t ctx, void* k) {
    (void)ctx; (void)k;

    lua_call(L, nargs, nresults);
}

LUA_API int luaopen_coroutine(lua_State* L) {
    lua_getglobal(L, "coroutine");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
    }
    return 1;
}

LUA_API int luaopen_utf8(lua_State* L) {
    return luaopen_compat53_utf8(L);
}

#undef lua_pcallk
LUA_API int lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc,
    intptr_t ctx, void* k) {
    (void)ctx;
    (void)k;
    return lua_pcall(L, nargs, nresults, errfunc);
}