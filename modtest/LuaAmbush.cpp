#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* AmbushMT = "Ambush";

static int Lua_GetAmbush(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1c31b8;
	luaL_setmetatable(L, AmbushMT);
	return 1;
}

int Lua_AmbushStartChallenge(lua_State* L)
{
	Ambush* ambush = *lua::GetUserdata<Ambush**>(L, 1, AmbushMT);
	ambush->StartChallenge();
	return 1;
}

static void RegisterAmbush(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetAmbush");
	lua_pushcfunction(L, Lua_GetAmbush);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, AmbushMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "StartChallenge", Lua_AmbushStartChallenge },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterAmbush(state);
}