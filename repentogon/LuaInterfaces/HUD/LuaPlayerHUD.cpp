#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetPlayerHUD) {
	HUD* hud = lua::GetUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	int index = (int)luaL_optinteger(L, 2, 0);

	if (index < 0 || index > 7) {
		return luaL_error(L, "Invalid index %d, value must be between 0 and 7", index);
	}

	PlayerHUD** ud = (PlayerHUD**)lua_newuserdata(L, sizeof(PlayerHUD*));
	*ud = hud->GetPlayerHUD(index);
	luaL_setmetatable(L, lua::metatables::PlayerHUDMT);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDGetPlayer) {
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

LUA_FUNCTION(Lua_PlayerHUDGetHUD) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	lua::luabridge::UserdataPtr::push(L, playerHUD->_HUD, lua::Metatables::HUD);
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDRenderActiveItem) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	unsigned int slot = (unsigned int)luaL_checkinteger(L, 2);
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	float alpha = (float)luaL_optnumber(L, 4, 1.0);
	float size = (float)luaL_optnumber(L, 5, 1.0);

	playerHUD->RenderActiveItem(slot, *pos, alpha, .0f, size);
	return 0;

}

LUA_FUNCTION(Lua_PlayerHUDGetHearts) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	PlayerHUDHeart* hearts = playerHUD->_heart;

	lua_newtable(L);
	int idx = 1;
	for (int i = 0; i < 24; i++) {
		PlayerHUDHeart* ud = (PlayerHUDHeart*)lua_newuserdata(L, sizeof(PlayerHUDHeart));
		*ud = hearts[i];
		luaL_setmetatable(L, lua::metatables::PlayerHUDHeartMT);
		lua_rawseti(L, -2, idx);
		idx++;
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerHUDGetHeartByIndex) {
	PlayerHUD* playerHUD = *lua::GetUserdata<PlayerHUD**>(L, 1, lua::metatables::PlayerHUDMT);
	int index = (int)luaL_checkinteger(L, 2);
	if (index < 0 || index > 23) {
		return luaL_error(L, "Invalid index: %d", index);
	}
	PlayerHUDHeart* hearts = playerHUD->_heart;
	PlayerHUDHeart* ud = (PlayerHUDHeart*)lua_newuserdata(L, sizeof(PlayerHUDHeart));
	*ud = hearts[index];
	luaL_setmetatable(L, lua::metatables::PlayerHUDHeartMT);
	return 1;
}

static void RegisterPlayerHUD(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::HUD, "GetPlayerHUD", Lua_GetPlayerHUD);

	luaL_Reg functions[] = {
		{ "GetPlayer", Lua_PlayerHUDGetPlayer },
		{ "GetHUD", Lua_PlayerHUDGetHUD },
		{ "RenderActiveItem", Lua_PlayerHUDRenderActiveItem },
		{ "GetHearts", Lua_PlayerHUDGetHearts},
		{ "GetHeartByIndex", Lua_PlayerHUDGetHeartByIndex},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::PlayerHUDMT, lua::metatables::PlayerHUDMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPlayerHUD(_state);
}