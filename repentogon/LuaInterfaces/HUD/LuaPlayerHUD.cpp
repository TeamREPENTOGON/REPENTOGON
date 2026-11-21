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
