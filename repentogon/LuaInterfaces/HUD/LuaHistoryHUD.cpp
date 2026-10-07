#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../LuaClasses.h"

MOD_EXPORT HistoryHUD* L_HistoryHUD_Get() {
	return &g_Game->GetHUD()->_historyHUD;
}

MOD_EXPORT void L_HistoryHUD_GetPosition(HistoryHUD* historyHUD, Vector* out) {
	*out = historyHUD->GetPosition();
}

MOD_EXPORT int L_HistoryHUD_GetNumVisibleItems(HistoryHUD* historyHUD) {
	return historyHUD->GetNumVisibleItems();
}

MOD_EXPORT void L_HistoryHUD_GetItemRenderOffset(HistoryHUD* historyHUD, int playerSlot, int index, Vector* out) {
	const int numColumns = historyHUD->GetNumColumns();
	Vector offset;
	offset.x = (float)(index % numColumns);
	offset.y = (float)std::floor(index / numColumns);
	offset *= historyHUD->GetIconSize();
	if (historyHUD->HasTwin()) {
		offset.x += -2 + 33 * playerSlot;
	}
	*out = offset;
}
