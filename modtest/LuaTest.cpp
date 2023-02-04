#include "IsaacRepentance.h"
#include "HookSystem.h"

extern "C" {
#include "lapi.h"
#include "lauxlib.h"
}

extern "C" int Lua_TestLua(lua_State *L) {
	int a = lua_tointeger(L, 1);
	int b = lua_tointeger(L, 2);
	int result = a + b;

	lua_pushinteger(L, result);
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	printf("[REPENTOGON WAS HERE] (flame everywhere woah gif modding of isaac sticker)\n");
	lua_State *state = g_LuaEngine->_state; //Soon.
	printf("repentogonning all over the place\n");
	lua_register(state, "Lua_TestLua", Lua_TestLua);
	printf("i'm repeotogonnning!!!!!!\n");
};