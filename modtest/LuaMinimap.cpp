#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GameGetMinimap(lua_State* L)
{
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	Minimap** toLua = (Minimap**)lua_newuserdata(L, sizeof(Minimap*));
	*toLua = game->GetMinimap();
	luaL_setmetatable(L, lua::metatables::MinimapMT);
	return 1;
}

static int Lua_MinimapGetDisplayedSize(lua_State* L) //crashes, not sure why
{
	Minimap* minimap = *lua::GetUserdata<Minimap**>(L, 1, lua::metatables::MinimapMT);

	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), minimap->GetDisplayedSize());

	return 1;
}

static void RegisterMinimap(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetMinimap");
	lua_pushcfunction(L, Lua_GameGetMinimap);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::MinimapMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		//{ "GetDisplayedSize", Lua_MinimapGetDisplayedSize },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMinimap(state);
}