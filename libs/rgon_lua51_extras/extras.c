// Defining some functions that Isaac requires to launch.
// These are missing in LuaJIT by default (or are macros)

#include "lua.h"
#include "lauxlib.h"

#undef lua_getglobal
LUA_API int lua_getglobal(lua_State* L, const char* name) {
    lua_getfield(L, LUA_GLOBALSINDEX, name);
    return lua_type(L, -1);
}

#undef lua_setglobal
LUA_API void lua_setglobal(lua_State* L, const char* name) {
    lua_setfield(L, LUA_GLOBALSINDEX, name);
}

LUA_API int lua_absindex(lua_State* L, int i) {
    return (i > 0 || i <= LUA_REGISTRYINDEX) ? i : lua_gettop(L) + i + 1;
}

LUA_API void lua_rotate(lua_State* L, int idx, int n) {
    int top = lua_gettop(L);
    int abs_idx = lua_absindex(L, idx);
    int count = top - abs_idx + 1;
    if (count <= 1 || n == 0) return;
    n = ((n % count) + count) % count;
    int i;
    for (i = 0; i < n; i++) {
        lua_pushvalue(L, top);
        lua_remove(L, top);
        lua_insert(L, abs_idx);
    }
}

LUA_API int lua_rawgetp(lua_State* L, int idx, const void* p) {
    int abs_idx = lua_absindex(L, idx);
    lua_pushlightuserdata(L, (void*)p);
    lua_rawget(L, abs_idx);
    return lua_type(L, -1);
}

LUA_API void lua_rawsetp(lua_State* L, int idx, const void* p) {
    int abs_idx = lua_absindex(L, idx);
    lua_pushlightuserdata(L, (void*)p);
    lua_insert(L, -2);
    lua_rawset(L, abs_idx);
}


#undef lua_len
LUA_API void lua_len(lua_State* L, int idx) {
    lua_pushnumber(L, (lua_Number)lua_objlen(L, idx));
}

#undef luaL_len
LUA_API lua_Integer luaL_len(lua_State* L, int idx) {
    return (lua_Integer)lua_objlen(L, idx);
}

#undef luaL_checkstring
LUA_API const char* luaL_checkstring(lua_State* L, int arg) {
    return luaL_checklstring(L, arg, NULL);
}

LUA_API const char* luaL_tolstring(lua_State* L, int idx, size_t* len) {
    if (luaL_callmeta(L, idx, "__tostring")) {
        if (!lua_isstring(L, -1)) {
            luaL_error(L, "'__tostring' must return a string");
        }
    }
    else {
        switch (lua_type(L, idx)) {
        case LUA_TNUMBER:
            lua_pushstring(L, lua_tostring(L, idx));
            break;
        case LUA_TSTRING:
            lua_pushvalue(L, idx);
            break;
        case LUA_TBOOLEAN:
            lua_pushstring(L, lua_toboolean(L, idx) ? "true" : "false");
            break;
        case LUA_TNIL:
            lua_pushstring(L, "nil");
            break;
        default:
            lua_pushfstring(L, "%s: %p", luaL_typename(L, idx), lua_topointer(L, idx));
            break;
        }
    }
    return lua_tolstring(L, -1, len);
}

LUA_API int lua_compare(lua_State* L, int idx1, int idx2, int op) {
    switch (op) {
    case 0: return lua_equal(L, idx1, idx2);     /* LUA_OPEQ */
    case 1: return lua_lessthan(L, idx1, idx2);  /* LUA_OPLT */
    case 2: return lua_lessthan(L, idx1, idx2) || lua_equal(L, idx1, idx2); /* LUA_OPLE */
    default: return 0;
    }
}

LUA_API void luaL_requiref(lua_State* L, const char* modname, lua_CFunction openf, int glb) {
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

    lua_getfield(L, -1, modname);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_pushcfunction(L, openf);
        lua_pushstring(L, modname);
        lua_call(L, 1, 1);

        lua_pushvalue(L, -1);
        lua_setfield(L, -3, modname);
    }

    lua_remove(L, -2);

    if (glb) {
        lua_pushvalue(L, -1);
        lua_setglobal(L, modname);
    }
}


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
    lua_newtable(L);
    lua_pushstring(L, "[\0-\x7F\xC2-\xFD][\x80-\xBF]*");
    lua_setfield(L, -2, "charpattern");
    return 1;
}

LUA_API int lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc, intptr_t ctx, void* k) {
    (void)ctx;
    (void)k;
    return lua_pcall(L, nargs, nresults, errfunc);
}

// I do not apologize, except to Mike Pall.
LUA_API int rgon_luaL_loadfilex(lua_State *L, const char *filename, const char *mode) {
    (void)mode;

    lua_getglobal(L, "_RGON_PREPROCESS");
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, filename);
        if (lua_pcall(L, 1, 1, 0) == 0 && lua_isstring(L, -1)) {
            size_t len;
            const char *src = lua_tolstring(L, -1, &len);
            int r = luaL_loadbuffer(L, src, len, filename);
            lua_remove(L, -2);
            return r;
        }
        lua_pop(L, 1);
    } else {
        lua_pop(L, 1);
    }

    return luaL_loadfile(L, filename);
}