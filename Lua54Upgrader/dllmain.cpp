#include <lua.hpp>
#include <Windows.h>

typedef int(*luaL_argerror_t)(lua_State* L, int arg, const char* extramsg);
static luaL_argerror_t g_luaL_argerror = NULL;
typedef lua_Integer(*luaL_checkinteger_t)(lua_State* L, int arg);
static luaL_checkinteger_t g_luaL_checkinteger = NULL;
typedef const char* (*luaL_checklstring_t)(lua_State* L, int arg, size_t* len);
static luaL_checklstring_t g_luaL_checklstring = NULL;
typedef lua_Number(*luaL_checknumber_t)(lua_State* L, int arg);
static luaL_checknumber_t g_luaL_checknumber = NULL;
typedef int(*luaL_error_t)(lua_State* L, const char* fmt, ...);
static luaL_error_t g_luaL_error = NULL;
typedef int(*luaL_loadbufferx_t)(lua_State* L, const char* buff, size_t size, const char* name, const char* mode);
static luaL_loadbufferx_t g_luaL_loadbufferx = NULL;
typedef int(*luaL_loadfilex_t)(lua_State* L, const char* filename, const char* mode);
static luaL_loadfilex_t g_luaL_loadfilex = NULL;
typedef lua_State* (*luaL_newstate_t)();
static luaL_newstate_t g_luaL_newstate = NULL;
typedef void(*luaL_openlibs_t)(lua_State* L);
static luaL_openlibs_t g_luaL_openlibs = NULL;
typedef int(*luaL_ref_t)(lua_State* L, int t);
static luaL_ref_t g_luaL_ref = NULL;
typedef void(*luaL_requiref_t)(lua_State* L, const char* modname, lua_CFunction openf, int glb);
static luaL_requiref_t g_luaL_requiref = NULL;
typedef const char* (*luaL_tolstring_t)(lua_State* L, int idx, size_t* len);
static luaL_tolstring_t g_luaL_tolstring = NULL;
typedef void(*luaL_unref_t)(lua_State* L, int t, int ref);
static luaL_unref_t g_luaL_unref = NULL;
typedef int(*lua_absindex_t)(lua_State* L, int idx);
static lua_absindex_t g_lua_absindex = NULL;
typedef lua_CFunction(*lua_atpanic_t)(lua_State* L, lua_CFunction panicf);
static lua_atpanic_t g_lua_atpanic = NULL;
typedef void(*lua_callk_t)(lua_State* L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k);
static lua_callk_t g_lua_callk = NULL;
typedef void(*lua_close_t)(lua_State* L);
static lua_close_t g_lua_close = NULL;
typedef int(*lua_compare_t)(lua_State* L, int index1, int index2, int op);
static lua_compare_t g_lua_compare = NULL;
typedef void(*lua_copy_t)(lua_State* L, int fromidx, int toidx);
static lua_copy_t g_lua_copy = NULL;
typedef void(*lua_createtable_t)(lua_State* L, int narr, int nrec);
static lua_createtable_t g_lua_createtable = NULL;
typedef int(*lua_gc_t)(lua_State* L, int what, ...);
static lua_gc_t g_lua_gc = NULL;
typedef int(*lua_getfield_t)(lua_State* L, int index, const char* k);
static lua_getfield_t g_lua_getfield = NULL;
typedef int(*lua_getglobal_t)(lua_State* L, const char* name);
static lua_getglobal_t g_lua_getglobal = NULL;
typedef int(*lua_getinfo_t)(lua_State* L, const char* what, lua_Debug* ar);
static lua_getinfo_t g_lua_getinfo = NULL;
typedef int(*lua_getmetatable_t)(lua_State* L, int index);
static lua_getmetatable_t g_lua_getmetatable = NULL;
typedef int(*lua_getstack_t)(lua_State* L, int level, lua_Debug* ar);
static lua_getstack_t g_lua_getstack = NULL;
typedef int(*lua_gettable_t)(lua_State* L, int index);
static lua_gettable_t g_lua_gettable = NULL;
typedef int(*lua_gettop_t)(lua_State* L);
static lua_gettop_t g_lua_gettop = NULL;
typedef int(*lua_iscfunction_t)(lua_State* L, int index);
static lua_iscfunction_t g_lua_iscfunction = NULL;
typedef int(*lua_isstring_t)(lua_State* L, int index);
static lua_isstring_t g_lua_isstring = NULL;
typedef int(*lua_isuserdata_t)(lua_State* L, int index);
static lua_isuserdata_t g_lua_isuserdata = NULL;
typedef void(*lua_len_t)(lua_State* L, int index);
static lua_len_t g_lua_len = NULL;
typedef lua_State* (*lua_newstate_t)(lua_Alloc f, void* ud);
static lua_newstate_t g_lua_newstate = NULL;
typedef void* (*lua_newuserdatauv_t)(lua_State* L, size_t size, int n);
static lua_newuserdatauv_t g_lua_newuserdatauv = NULL;
typedef int(*lua_pcallk_t)(lua_State* L, int nargs, int nresults, int msgh, lua_KContext ctx, lua_KFunction k);
static lua_pcallk_t g_lua_pcallk = NULL;
typedef void(*lua_pushboolean_t)(lua_State* L, int b);
static lua_pushboolean_t g_lua_pushboolean = NULL;
typedef void(*lua_pushcclosure_t)(lua_State* L, lua_CFunction fn, int n);
static lua_pushcclosure_t g_lua_pushcclosure = NULL;
typedef const char* (*lua_pushfstring_t)(lua_State* L, const char* fmt, ... );
static lua_pushfstring_t g_lua_pushfstring = NULL;
typedef void(*lua_pushinteger_t)(lua_State* L, lua_Integer n);
static lua_pushinteger_t g_lua_pushinteger = NULL;
typedef const char* (*lua_pushlstring_t)(lua_State* L, const char* s, size_t len);
static lua_pushlstring_t g_lua_pushlstring = NULL;
typedef void(*lua_pushnil_t)(lua_State* L);
static lua_pushnil_t g_lua_pushnil = NULL;
typedef void(*lua_pushnumber_t)(lua_State* L, lua_Number n);
static lua_pushnumber_t g_lua_pushnumber = NULL;
typedef const char* (*lua_pushstring_t)(lua_State* L, const char* s);
static lua_pushstring_t g_lua_pushstring = NULL;
typedef void(*lua_pushvalue_t)(lua_State* L, int index);
static lua_pushvalue_t g_lua_pushvalue = NULL;
typedef int(*lua_rawequal_t)(lua_State* L, int index1, int index2);
static lua_rawequal_t g_lua_rawequal = NULL;
typedef int(*lua_rawget_t)(lua_State* L, int index);
static lua_rawget_t g_lua_rawget = NULL;
typedef int(*lua_rawgeti_t)(lua_State* L, int index, lua_Integer n);
static lua_rawgeti_t g_lua_rawgeti = NULL;
typedef int(*lua_rawgetp_t)(lua_State* L, int index, const void* p);
static lua_rawgetp_t g_lua_rawgetp = NULL;
typedef void(*lua_rawset_t)(lua_State* L, int index);
static lua_rawset_t g_lua_rawset = NULL;
typedef void(*lua_rawseti_t)(lua_State* L, int index, lua_Integer i);
static lua_rawseti_t g_lua_rawseti = NULL;
typedef void(*lua_rawsetp_t)(lua_State* L, int index, const void* p);
static lua_rawsetp_t g_lua_rawsetp = NULL;
typedef void(*lua_rotate_t)(lua_State* L, int idx, int n);
static lua_rotate_t g_lua_rotate = NULL;
typedef void(*lua_setfield_t)(lua_State* L, int index, const char* k);
static lua_setfield_t g_lua_setfield = NULL;
typedef void(*lua_setglobal_t)(lua_State* L, const char* name);
static lua_setglobal_t g_lua_setglobal = NULL;
typedef int(*lua_setmetatable_t)(lua_State* L, int idx);
static lua_setmetatable_t g_lua_setmetatable = NULL;
typedef void(*lua_settop_t)(lua_State* L, int idx);
static lua_settop_t g_lua_settop = NULL;
typedef int(*lua_toboolean_t)(lua_State* L, int index);
static lua_toboolean_t g_lua_toboolean = NULL;
typedef const char* (*lua_tolstring_t)(lua_State* L, int index, size_t* len);
static lua_tolstring_t g_lua_tolstring = NULL;
typedef void* (*lua_touserdata_t)(lua_State* L, int index);
static lua_touserdata_t g_lua_touserdata = NULL;
typedef int(*lua_type_t)(lua_State* L, int index);
static lua_type_t g_lua_type = NULL;
typedef const char* (*lua_typename_t)(lua_State* L, int tp);
static lua_typename_t g_lua_typename = NULL;
typedef int(*luaopen_base_t)(lua_State* L);
static luaopen_base_t g_luaopen_base = NULL;
typedef int(*luaopen_coroutine_t)(lua_State* L);
static luaopen_coroutine_t g_luaopen_coroutine = NULL;
typedef int(*luaopen_debug_t)(lua_State* L);
static luaopen_debug_t g_luaopen_debug = NULL;
typedef int(*luaopen_math_t)(lua_State* L);
static luaopen_math_t g_luaopen_math = NULL;
typedef int(*luaopen_string_t)(lua_State* L);
static luaopen_string_t g_luaopen_string = NULL;
typedef int(*luaopen_table_t)(lua_State* L);
static luaopen_table_t g_luaopen_table = NULL;
typedef int(*luaopen_utf8_t)(lua_State* L);
static luaopen_utf8_t g_luaopen_utf8 = NULL;

static HMODULE g_Lua54 = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        // DLL is being loaded
        g_Lua54 = LoadLibrary("Lua5.4.4r.dll");
        if (g_Lua54 == NULL) {
            return FALSE;
        }

        g_luaL_argerror = (luaL_argerror_t)GetProcAddress(g_Lua54, "luaL_argerror");
        if (g_luaL_argerror == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_argerror\n");
            return FALSE;
        }
        g_luaL_checkinteger = (luaL_checkinteger_t)GetProcAddress(g_Lua54, "luaL_checkinteger");
        if (g_luaL_checkinteger == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_checkinteger\n");
            return FALSE;
        }
        g_luaL_checklstring = (luaL_checklstring_t)GetProcAddress(g_Lua54, "luaL_checklstring");
        if (g_luaL_checklstring == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_checklstring\n");
            return FALSE;
        }
        g_luaL_checknumber = (luaL_checknumber_t)GetProcAddress(g_Lua54, "luaL_checknumber");
        if (g_luaL_checknumber == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_checknumber\n");
            return FALSE;
        }
        g_luaL_error = (luaL_error_t)GetProcAddress(g_Lua54, "luaL_error");
        if (g_luaL_error == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_error\n");
            return FALSE;
        }
        g_luaL_loadbufferx = (luaL_loadbufferx_t)GetProcAddress(g_Lua54, "luaL_loadbufferx");
        if (g_luaL_loadbufferx == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_loadbufferx\n");
            return FALSE;
        }
        g_luaL_loadfilex = (luaL_loadfilex_t)GetProcAddress(g_Lua54, "luaL_loadfilex");
        if (g_luaL_loadfilex == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_loadfilex\n");
            return FALSE;
        }
        g_luaL_newstate = (luaL_newstate_t)GetProcAddress(g_Lua54, "luaL_newstate");
        if (g_luaL_newstate == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_newstate\n");
            return FALSE;
        }
        g_luaL_openlibs = (luaL_openlibs_t)GetProcAddress(g_Lua54, "luaL_openlibs");
        if (g_luaL_openlibs == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_openlibs\n");
            return FALSE;
        }
        g_luaL_ref = (luaL_ref_t)GetProcAddress(g_Lua54, "luaL_ref");
        if (g_luaL_ref == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_ref\n");
            return FALSE;
        }
        g_luaL_requiref = (luaL_requiref_t)GetProcAddress(g_Lua54, "luaL_requiref");
        if (g_luaL_requiref == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_requiref\n");
            return FALSE;
        }
        g_luaL_tolstring = (luaL_tolstring_t)GetProcAddress(g_Lua54, "luaL_tolstring");
        if (g_luaL_tolstring == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_tolstring\n");
            return FALSE;
        }
        g_luaL_unref = (luaL_unref_t)GetProcAddress(g_Lua54, "luaL_unref");
        if (g_luaL_unref == NULL) {
            printf("Failed to find address for Lua 5.4 function luaL_unref\n");
            return FALSE;
        }
        g_lua_absindex = (lua_absindex_t)GetProcAddress(g_Lua54, "lua_absindex");
        if (g_lua_absindex == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_absindex\n");
            return FALSE;
        }
        g_lua_atpanic = (lua_atpanic_t)GetProcAddress(g_Lua54, "lua_atpanic");
        if (g_lua_atpanic == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_atpanic\n");
            return FALSE;
        }
        g_lua_callk = (lua_callk_t)GetProcAddress(g_Lua54, "lua_callk");
        if (g_lua_callk == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_callk\n");
            return FALSE;
        }
        g_lua_close = (lua_close_t)GetProcAddress(g_Lua54, "lua_close");
        if (g_lua_close == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_close\n");
            return FALSE;
        }
        g_lua_compare = (lua_compare_t)GetProcAddress(g_Lua54, "lua_compare");
        if (g_lua_compare == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_compare\n");
            return FALSE;
        }
        g_lua_copy = (lua_copy_t)GetProcAddress(g_Lua54, "lua_copy");
        if (g_lua_copy == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_copy\n");
            return FALSE;
        }
        g_lua_createtable = (lua_createtable_t)GetProcAddress(g_Lua54, "lua_createtable");
        if (g_lua_createtable == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_createtable\n");
            return FALSE;
        }
        g_lua_gc = (lua_gc_t)GetProcAddress(g_Lua54, "lua_gc");
        if (g_lua_gc == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_gc\n");
            return FALSE;
        }
        g_lua_getfield = (lua_getfield_t)GetProcAddress(g_Lua54, "lua_getfield");
        if (g_lua_getfield == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_getfield\n");
            return FALSE;
        }
        g_lua_getglobal = (lua_getglobal_t)GetProcAddress(g_Lua54, "lua_getglobal");
        if (g_lua_getglobal == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_getglobal\n");
            return FALSE;
        }
        g_lua_getinfo = (lua_getinfo_t)GetProcAddress(g_Lua54, "lua_getinfo");
        if (g_lua_getinfo == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_getinfo\n");
            return FALSE;
        }
        g_lua_getmetatable = (lua_getmetatable_t)GetProcAddress(g_Lua54, "lua_getmetatable");
        if (g_lua_getmetatable == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_getmetatable\n");
            return FALSE;
        }
        g_lua_getstack = (lua_getstack_t)GetProcAddress(g_Lua54, "lua_getstack");
        if (g_lua_getstack == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_getstack\n");
            return FALSE;
        }
        g_lua_gettable = (lua_gettable_t)GetProcAddress(g_Lua54, "lua_gettable");
        if (g_lua_gettable == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_gettable\n");
            return FALSE;
        }
        g_lua_gettop = (lua_gettop_t)GetProcAddress(g_Lua54, "lua_gettop");
        if (g_lua_gettop == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_gettop\n");
            return FALSE;
        }
        g_lua_iscfunction = (lua_iscfunction_t)GetProcAddress(g_Lua54, "lua_iscfunction");
        if (g_lua_iscfunction == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_iscfunction\n");
            return FALSE;
        }
        g_lua_isstring = (lua_isstring_t)GetProcAddress(g_Lua54, "lua_isstring");
        if (g_lua_isstring == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_isstring\n");
            return FALSE;
        }
        g_lua_isuserdata = (lua_isuserdata_t)GetProcAddress(g_Lua54, "lua_isuserdata");
        if (g_lua_isuserdata == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_isuserdata\n");
            return FALSE;
        }
        g_lua_len = (lua_len_t)GetProcAddress(g_Lua54, "lua_len");
        if (g_lua_len == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_len\n");
            return FALSE;
        }
        g_lua_newstate = (lua_newstate_t)GetProcAddress(g_Lua54, "lua_newstate");
        if (g_lua_newstate == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_newstate\n");
            return FALSE;
        }
        g_lua_newuserdatauv = (lua_newuserdatauv_t)GetProcAddress(g_Lua54, "lua_newuserdatauv");
        if (g_lua_newuserdatauv == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_newuserdatauv\n");
            return FALSE;
        }
        g_lua_pcallk = (lua_pcallk_t)GetProcAddress(g_Lua54, "lua_pcallk");
        if (g_lua_pcallk == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pcallk\n");
            return FALSE;
        }
        g_lua_pushboolean = (lua_pushboolean_t)GetProcAddress(g_Lua54, "lua_pushboolean");
        if (g_lua_pushboolean == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushboolean\n");
            return FALSE;
        }
        g_lua_pushcclosure = (lua_pushcclosure_t)GetProcAddress(g_Lua54, "lua_pushcclosure");
        if (g_lua_pushcclosure == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushcclosure\n");
            return FALSE;
        }
        g_lua_pushfstring = (lua_pushfstring_t)GetProcAddress(g_Lua54, "lua_pushfstring");
        if (g_lua_pushfstring == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushfstring\n");
            return FALSE;
        }
        g_lua_pushinteger = (lua_pushinteger_t)GetProcAddress(g_Lua54, "lua_pushinteger");
        if (g_lua_pushinteger == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushinteger\n");
            return FALSE;
        }
        g_lua_pushlstring = (lua_pushlstring_t)GetProcAddress(g_Lua54, "lua_pushlstring");
        if (g_lua_pushlstring == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushlstring\n");
            return FALSE;
        }
        g_lua_pushnil = (lua_pushnil_t)GetProcAddress(g_Lua54, "lua_pushnil");
        if (g_lua_pushnil == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushnil\n");
            return FALSE;
        }
        g_lua_pushnumber = (lua_pushnumber_t)GetProcAddress(g_Lua54, "lua_pushnumber");
        if (g_lua_pushnumber == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushnumber\n");
            return FALSE;
        }
        g_lua_pushstring = (lua_pushstring_t)GetProcAddress(g_Lua54, "lua_pushstring");
        if (g_lua_pushstring == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushstring\n");
            return FALSE;
        }
        g_lua_pushvalue = (lua_pushvalue_t)GetProcAddress(g_Lua54, "lua_pushvalue");
        if (g_lua_pushvalue == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_pushvalue\n");
            return FALSE;
        }
        g_lua_rawequal = (lua_rawequal_t)GetProcAddress(g_Lua54, "lua_rawequal");
        if (g_lua_rawequal == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawequal\n");
            return FALSE;
        }
        g_lua_rawget = (lua_rawget_t)GetProcAddress(g_Lua54, "lua_rawget");
        if (g_lua_rawget == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawget\n");
            return FALSE;
        }
        g_lua_rawgeti = (lua_rawgeti_t)GetProcAddress(g_Lua54, "lua_rawgeti");
        if (g_lua_rawgeti == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawgeti\n");
            return FALSE;
        }
        g_lua_rawgetp = (lua_rawgetp_t)GetProcAddress(g_Lua54, "lua_rawgetp");
        if (g_lua_rawgetp == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawgetp\n");
            return FALSE;
        }
        g_lua_rawset = (lua_rawset_t)GetProcAddress(g_Lua54, "lua_rawset");
        if (g_lua_rawset == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawset\n");
            return FALSE;
        }
        g_lua_rawseti = (lua_rawseti_t)GetProcAddress(g_Lua54, "lua_rawseti");
        if (g_lua_rawseti == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawseti\n");
            return FALSE;
        }
        g_lua_rawsetp = (lua_rawsetp_t)GetProcAddress(g_Lua54, "lua_rawsetp");
        if (g_lua_rawsetp == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rawsetp\n");
            return FALSE;
        }
        g_lua_rotate = (lua_rotate_t)GetProcAddress(g_Lua54, "lua_rotate");
        if (g_lua_rotate == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_rotate\n");
            return FALSE;
        }
        g_lua_setfield = (lua_setfield_t)GetProcAddress(g_Lua54, "lua_setfield");
        if (g_lua_setfield == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_setfield\n");
            return FALSE;
        }
        g_lua_setglobal = (lua_setglobal_t)GetProcAddress(g_Lua54, "lua_setglobal");
        if (g_lua_setglobal == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_setglobal\n");
            return FALSE;
        }
        g_lua_setmetatable = (lua_setmetatable_t)GetProcAddress(g_Lua54, "lua_setmetatable");
        if (g_lua_setmetatable == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_setmetatable\n");
            return FALSE;
        }
        g_lua_settop = (lua_settop_t)GetProcAddress(g_Lua54, "lua_settop");
        if (g_lua_settop == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_settop\n");
            return FALSE;
        }
        g_lua_toboolean = (lua_toboolean_t)GetProcAddress(g_Lua54, "lua_toboolean");
        if (g_lua_toboolean == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_toboolean\n");
            return FALSE;
        }
        g_lua_tolstring = (lua_tolstring_t)GetProcAddress(g_Lua54, "lua_tolstring");
        if (g_lua_tolstring == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_tolstring\n");
            return FALSE;
        }
        g_lua_touserdata = (lua_touserdata_t)GetProcAddress(g_Lua54, "lua_touserdata");
        if (g_lua_touserdata == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_touserdata\n");
            return FALSE;
        }
        g_lua_type = (lua_type_t)GetProcAddress(g_Lua54, "lua_type");
        if (g_lua_type == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_type\n");
            return FALSE;
        }
        g_lua_typename = (lua_typename_t)GetProcAddress(g_Lua54, "lua_typename");
        if (g_lua_typename == NULL) {
            printf("Failed to find address for Lua 5.4 function lua_typename\n");
            return FALSE;
        }
        g_luaopen_base = (luaopen_base_t)GetProcAddress(g_Lua54, "luaopen_base");
        if (g_luaopen_base == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_base\n");
            return FALSE;
        }
        g_luaopen_coroutine = (luaopen_coroutine_t)GetProcAddress(g_Lua54, "luaopen_coroutine");
        if (g_luaopen_coroutine == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_coroutine\n");
            return FALSE;
        }
        g_luaopen_debug = (luaopen_debug_t)GetProcAddress(g_Lua54, "luaopen_debug");
        if (g_luaopen_debug == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_debug\n");
            return FALSE;
        }
        g_luaopen_math = (luaopen_math_t)GetProcAddress(g_Lua54, "luaopen_math");
        if (g_luaopen_math == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_math\n");
            return FALSE;
        }
        g_luaopen_string = (luaopen_string_t)GetProcAddress(g_Lua54, "luaopen_string");
        if (g_luaopen_string == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_string\n");
            return FALSE;
        }
        g_luaopen_table = (luaopen_table_t)GetProcAddress(g_Lua54, "luaopen_table");
        if (g_luaopen_table == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_table\n");
            return FALSE;
        }
        g_luaopen_utf8 = (luaopen_utf8_t)GetProcAddress(g_Lua54, "luaopen_utf8");
        if (g_luaopen_utf8 == NULL) {
            printf("Failed to find address for Lua 5.4 function luaopen_utf8\n");
            return FALSE;
        }
        break;

    case DLL_PROCESS_DETACH:
        // DLL is being unloaded
        if (g_Lua54 != NULL) {
            FreeLibrary(g_Lua54);
            g_Lua54 = NULL;
        }
        break;

    default:
        break;
    }

    return TRUE;
}

extern "C" __declspec(dllexport) int luaL_argerror(lua_State * L, int arg, const char* extramsg) {

    return g_luaL_argerror(L, arg, extramsg);
}

extern "C" __declspec(dllexport) lua_Integer luaL_checkinteger(lua_State * L, int arg) {
    return g_luaL_checkinteger(L, arg);
}

extern "C" __declspec(dllexport) const char* luaL_checklstring(lua_State * L, int arg, size_t * len) {
    return g_luaL_checklstring(L, arg, len);
}

extern "C" __declspec(dllexport) lua_Number luaL_checknumber(lua_State * L, int arg) {
    return g_luaL_checknumber(L, arg);
}

extern "C" __declspec(dllexport) int luaL_error(lua_State * L, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return g_luaL_error(L, fmt, args);
    va_end(args);
}

extern "C" __declspec(dllexport) int luaL_loadbufferx(lua_State * L, const char* buff, size_t size, const char* name, const char* mode) {
    return g_luaL_loadbufferx(L, buff, size, name, mode);
}

extern "C" __declspec(dllexport) int luaL_loadfilex(lua_State * L, const char* filename, const char* mode) {
    return g_luaL_loadfilex(L, filename, mode);
}

extern "C" __declspec(dllexport) lua_State * luaL_newstate() {
    return g_luaL_newstate();
}

extern "C" __declspec(dllexport) void luaL_openlibs(lua_State * L) {
    g_luaL_openlibs(L);
}

extern "C" __declspec(dllexport) int luaL_ref(lua_State * L, int t) {
    return g_luaL_ref(L, t);
}

extern "C" __declspec(dllexport) void luaL_requiref(lua_State * L, const char* modname, lua_CFunction openf, int glb) {
    g_luaL_requiref(L, modname, openf, glb);
}

extern "C" __declspec(dllexport) const char* luaL_tolstring(lua_State * L, int idx, size_t * len) {
    return g_luaL_tolstring(L, idx, len);
}

extern "C" __declspec(dllexport) void luaL_unref(lua_State * L, int t, int ref) {
    g_luaL_unref(L, t, ref);
}

extern "C" __declspec(dllexport) int lua_absindex(lua_State * L, int idx) {
    return g_lua_absindex(L, idx);
}

extern "C" __declspec(dllexport) lua_CFunction lua_atpanic(lua_State * L, lua_CFunction panicf) {
    return g_lua_atpanic(L, panicf);
}

extern "C" __declspec(dllexport) void lua_callk(lua_State * L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k) {
    g_lua_callk(L, nargs, nresults, ctx, k);
}

extern "C" __declspec(dllexport) void lua_close(lua_State * L) {
    g_lua_close(L);
}

extern "C" __declspec(dllexport) int lua_compare(lua_State * L, int index1, int index2, int op) {
    return g_lua_compare(L, index1, index2, op);
}

extern "C" __declspec(dllexport) void lua_copy(lua_State * L, int fromidx, int toidx) {
    g_lua_copy(L, fromidx, toidx);
}

extern "C" __declspec(dllexport) void lua_createtable(lua_State * L, int narr, int nrec) {
    g_lua_createtable(L, narr, nrec);
}

extern "C" __declspec(dllexport) int lua_gc(lua_State * L, int what, ...) {
    va_list args;
    va_start(args, what);
    return g_lua_gc(L, what, args);
    va_end(args);
}

extern "C" __declspec(dllexport) int lua_getfield(lua_State * L, int index, const char* k) {
    return g_lua_getfield(L, index, k);
}

extern "C" __declspec(dllexport) int lua_getglobal(lua_State * L, const char* name) {
    return g_lua_getglobal(L, name);
}

extern "C" __declspec(dllexport) int lua_getinfo(lua_State * L, const char* what, lua_Debug * ar) {
    return g_lua_getinfo(L, what, ar);
}

extern "C" __declspec(dllexport) int lua_getmetatable(lua_State * L, int index) {
    return g_lua_getmetatable(L, index);
}

extern "C" __declspec(dllexport) int lua_getstack(lua_State * L, int level, lua_Debug * ar) {
    return g_lua_getstack(L, level, ar);
}

extern "C" __declspec(dllexport) int lua_gettable(lua_State * L, int index) {
    return g_lua_gettable(L, index);
}

extern "C" __declspec(dllexport) int lua_gettop(lua_State * L) {
    return g_lua_gettop(L);
}

extern "C" __declspec(dllexport) int lua_iscfunction(lua_State * L, int index) {
    return g_lua_iscfunction(L, index);
}

extern "C" __declspec(dllexport) int lua_isstring(lua_State * L, int index) {
    return g_lua_isstring(L, index);
}

extern "C" __declspec(dllexport) int lua_isuserdata(lua_State * L, int index) {
    return g_lua_isuserdata(L, index);
}

extern "C" __declspec(dllexport) void lua_len(lua_State * L, int index) {
    g_lua_len(L, index);
}

extern "C" __declspec(dllexport) lua_State * lua_newstate(lua_Alloc f, void* ud) {
    return g_lua_newstate(f, ud);
}

#pragma push_macro("lua_newuserdata")
#undef lua_newuserdata

extern "C" __declspec(dllexport) void* lua_newuserdata(lua_State * L, size_t size) {
    return g_lua_newuserdatauv(L, size, 1);
}

#pragma pop_macro("lua_newuserdata")

extern "C" __declspec(dllexport) int lua_pcallk(lua_State * L, int nargs, int nresults, int msgh, lua_KContext ctx, lua_KFunction k) {
    return g_lua_pcallk(L, nargs, nresults, msgh, ctx, k);
}

extern "C" __declspec(dllexport) void lua_pushboolean(lua_State * L, int b) {
    g_lua_pushboolean(L, b);
}

extern "C" __declspec(dllexport) void lua_pushcclosure(lua_State * L, lua_CFunction fn, int n) {
    g_lua_pushcclosure(L, fn, n);
}

extern "C" __declspec(dllexport) const char* lua_pushfstring(lua_State * L, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    return g_lua_pushfstring(L, fmt, args);
    va_end(args);
}

extern "C" __declspec(dllexport) void lua_pushinteger(lua_State * L, lua_Integer n) {
    g_lua_pushinteger(L, n);
}

extern "C" __declspec(dllexport) const char* lua_pushlstring(lua_State * L, const char* s, size_t len) {
    return g_lua_pushlstring(L, s, len);
}

extern "C" __declspec(dllexport) void lua_pushnil(lua_State * L) {
    g_lua_pushnil(L);
}

extern "C" __declspec(dllexport) void lua_pushnumber(lua_State * L, lua_Number n) {
    g_lua_pushnumber(L, n);
}

extern "C" __declspec(dllexport) const char* lua_pushstring(lua_State * L, const char* s) {
    return g_lua_pushstring(L, s);
}

extern "C" __declspec(dllexport) void lua_pushvalue(lua_State * L, int index) {
    g_lua_pushvalue(L, index);
}

extern "C" __declspec(dllexport) int lua_rawequal(lua_State * L, int index1, int index2) {
    return g_lua_rawequal(L, index1, index2);
}

extern "C" __declspec(dllexport) int lua_rawget(lua_State * L, int index) {
    return g_lua_rawget(L, index);
}

extern "C" __declspec(dllexport) int lua_rawgeti(lua_State * L, int index, lua_Integer n) {
    return g_lua_rawgeti(L, index, n);
}

extern "C" __declspec(dllexport) int lua_rawgetp(lua_State * L, int index, const void* p) {
    return g_lua_rawgetp(L, index, p);
}

extern "C" __declspec(dllexport) void lua_rawset(lua_State * L, int index) {
    g_lua_rawset(L, index);
}

extern "C" __declspec(dllexport) void lua_rawseti(lua_State * L, int index, lua_Integer i) {
    g_lua_rawseti(L, index, i);
}

extern "C" __declspec(dllexport) void lua_rawsetp(lua_State * L, int index, const void* p) {
    g_lua_rawsetp(L, index, p);
}

extern "C" __declspec(dllexport) void lua_rotate(lua_State * L, int idx, int n) {
    g_lua_rotate(L, idx, n);
}

extern "C" __declspec(dllexport) void lua_setfield(lua_State * L, int index, const char* k) {
    g_lua_setfield(L, index, k);
}

extern "C" __declspec(dllexport) void lua_setglobal(lua_State * L, const char* name) {
    g_lua_setglobal(L, name);
}

extern "C" __declspec(dllexport) int lua_setmetatable(lua_State * L, int idx) {
    return g_lua_setmetatable(L, idx);
}

extern "C" __declspec(dllexport) void lua_settop(lua_State * L, int idx) {
    g_lua_settop(L, idx);
}

extern "C" __declspec(dllexport) int lua_toboolean(lua_State * L, int index) {
    return g_lua_toboolean(L, index);
}

extern "C" __declspec(dllexport) const char* lua_tolstring(lua_State * L, int index, size_t * len) {
    return g_lua_tolstring(L, index, len);
}

extern "C" __declspec(dllexport) void* lua_touserdata(lua_State * L, int index) {
    return g_lua_touserdata(L, index);
}

extern "C" __declspec(dllexport) int lua_type(lua_State * L, int index) {
    return g_lua_type(L, index);
}

extern "C" __declspec(dllexport) const char* lua_typename(lua_State * L, int tp) {
    return g_lua_typename(L, tp);
}

extern "C" __declspec(dllexport) int luaopen_base(lua_State * L) {
    return g_luaopen_base(L);
}

extern "C" __declspec(dllexport) int luaopen_coroutine(lua_State * L) {
    return g_luaopen_coroutine(L);
}

extern "C" __declspec(dllexport) int luaopen_debug(lua_State * L) {
    return g_luaopen_debug(L);
}

extern "C" __declspec(dllexport) int luaopen_math(lua_State * L) {
    return g_luaopen_math(L);
}

extern "C" __declspec(dllexport) int luaopen_string(lua_State * L) {
    return g_luaopen_string(L);
}

extern "C" __declspec(dllexport) int luaopen_table(lua_State * L) {
    return g_luaopen_table(L);
}

extern "C" __declspec(dllexport) int luaopen_utf8(lua_State * L) {
    return g_luaopen_utf8(L);
}
