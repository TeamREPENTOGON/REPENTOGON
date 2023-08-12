#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PlayerHUDHeartIsVisible) {
	PlayerHUDHeart* heart = lua::GetUserdata<PlayerHUDHeart*>(L, 1, lua::metatables::PlayerHUDHeartMT);
	lua_pushboolean(L, heart->_visibility);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDHeartIsGoldenHeartOverlayVisible) {
	PlayerHUDHeart* heart = lua::GetUserdata<PlayerHUDHeart*>(L, 1, lua::metatables::PlayerHUDHeartMT);
	lua_pushboolean(L, heart->_goldenHeartVisibility);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDHeartGetFlashType) {
	PlayerHUDHeart* heart = lua::GetUserdata<PlayerHUDHeart*>(L, 1, lua::metatables::PlayerHUDHeartMT);
	lua_pushinteger(L, heart->_flashType);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDHeartGetHeartAnim) {
	PlayerHUDHeart* heart = lua::GetUserdata<PlayerHUDHeart*>(L, 1, lua::metatables::PlayerHUDHeartMT);
	lua_pushstring(L, heart->_spriteAnim);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDHeartGetHeartOverlayAnim) {
	PlayerHUDHeart* heart = lua::GetUserdata<PlayerHUDHeart*>(L, 1, lua::metatables::PlayerHUDHeartMT);
	lua_pushstring(L, heart->_spriteOverlayAnim);
	return 1;
}

static void RegisterPlayerHUDHeart(lua_State* L) {
	luaL_newmetatable(L, lua::metatables::PlayerHUDHeartMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "IsVisible", Lua_PlayerHUDHeartIsVisible },
		{ "IsGoldenHeartOverlayVisible", Lua_PlayerHUDHeartIsGoldenHeartOverlayVisible },
		{ "GetFlashType", Lua_PlayerHUDHeartGetFlashType },
		{ "GetHeartAnim", Lua_PlayerHUDHeartGetHeartAnim },
		{ "GetHeartOverlayAnim", Lua_PlayerHUDHeartGetHeartOverlayAnim },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterPlayerHUDHeart(state);
}