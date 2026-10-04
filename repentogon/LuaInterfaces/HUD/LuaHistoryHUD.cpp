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

LUA_FUNCTION(Lua_HistoryHUD_GetPlayer) {
	HistoryHUD* historyHUD = LuaHistoryHUD::Get(L, 1);
	int playerIdx = (int)luaL_checkinteger(L, 2);

	const HistoryHUD_Player& historyPlayer = historyHUD->_playerHistoryHuds[playerIdx];
	if (historyPlayer._player) {
		LuaEntityPlayer::PushPtr(L, historyPlayer._player);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_HistoryHUD_GetPlayer", Lua_HistoryHUD_GetPlayer);

	super();
}
