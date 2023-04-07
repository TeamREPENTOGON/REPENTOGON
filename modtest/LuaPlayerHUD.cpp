#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GetPlayerHUD(lua_State* L) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	PlayerHUD** ud = (PlayerHUD**)lua_newuserdata(L, sizeof(PlayerHUD*));
	int index = luaL_checkinteger(L, 2);
	*ud = hud->GetPlayerHUD(index);
	luaL_setmetatable(L, lua::metatables::PlayerHUDMT);
	return 1;
}

static int Lua_PlayerHUDGetPlayer(lua_State* L) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	Entity_Player* player = playerHUD->GetPlayer();
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

static int Lua_PlayerHUDRenderActiveItem(lua_State* L) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	unsigned int slot = luaL_checkinteger(L, 2);
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	float alpha = luaL_optnumber(L, 4, 1.0);
	float unk = luaL_optnumber(L, 5, 1.0);

	playerHUD->RenderActiveItem(slot, *pos, alpha, unk);
	return 0;

}

static void RegisterPlayerHUD(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::HUD);
	lua_pushstring(L, "GetPlayerHUD");
	lua_pushcfunction(L, Lua_GetPlayerHUD);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::PlayerHUDMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetPlayer", Lua_PlayerHUDGetPlayer },
		{ "RenderActiveItem", Lua_PlayerHUDRenderActiveItem },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterPlayerHUD(state);
}