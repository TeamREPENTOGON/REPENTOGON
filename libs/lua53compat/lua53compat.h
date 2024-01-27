#if defined(COMPAT53_LIB)
#define COMPAT53_API __declspec(dllexport)
#else
#define COMPAT53_API __declspec(dllimport)
#endif

COMPAT53_API int lua_rawgetp(lua_State* L, int i, const void* p);
COMPAT53_API void lua_rawsetp(lua_State* L, int i, const void* p);
COMPAT53_API void luaL_requiref(lua_State* L, const char* modname, lua_CFunction openf, int glb);
COMPAT53_API void lua_seti(lua_State* L, int index, lua_Integer i);
COMPAT53_API int lua_geti(lua_State* L, int index, lua_Integer i);