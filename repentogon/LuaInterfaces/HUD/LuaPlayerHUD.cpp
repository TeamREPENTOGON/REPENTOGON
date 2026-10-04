#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"

MOD_EXPORT int L_PlayerHUD_GetLayout(PlayerHUD* playerHUD) {
	return g_Game->GetHUD()->GetPlayerHUDLayout(playerHUD->_playerHudIndex, playerHUD->_player);
}

MOD_EXPORT void L_PlayerHUD_RenderActiveItem(PlayerHUD* playerHUD, unsigned int activeSlot, Vector* pos, float alpha, float size) {
	const int layout = g_Game->GetHUD()->GetPlayerHUDLayout(playerHUD->_playerHudIndex, playerHUD->GetPlayer());
	playerHUD->RenderActiveItem(activeSlot, *pos, layout, size, alpha, false);
}

LUA_FUNCTION(Lua_PlayerHUDGetPlayer) {
	PlayerHUD* playerHUD = LuaPlayerHUD::Get(L, 1);
	Entity_Player* player = playerHUD->GetPlayer();
	if (!player) {
		lua_pushnil(L);
	}
	else {
		LuaEntityPlayer::PushPtr(L, player);
	}
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_PlayerHUD_GetPlayer", Lua_PlayerHUDGetPlayer);

	super();
}
