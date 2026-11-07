#define lstrlib_c
#define LUA_LIB
#include "../lua-compat-5.3/lprefix.h"

#undef lua_Unsigned
#define lua_Unsigned unsigned LUA_INTEGER

#include "../lua-compat-5.3/lstrlib.c"
