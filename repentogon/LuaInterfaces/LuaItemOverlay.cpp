#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "HookSystem.h"

MOD_EXPORT ItemOverlay* L_ItemOverlay_Get() {
	return &g_Game->_itemOverlay;
}

MOD_EXPORT void L_ItemOverlay_Show(int overlayID, int delay, Entity_Player* player) {
	g_Game->GetItemOverlay()->Show(overlayID, delay, player);
}
