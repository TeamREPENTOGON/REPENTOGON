#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "HookSystem.h"

MOD_EXPORT ItemOverlay* L_ItemOverlay_Get() {
	return &g_Game->_itemOverlay;
}

LUA_FUNCTION(Lua_ItemOverlayShow)
{
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	int overlayID = (int)luaL_checkinteger(L, 1);
	int delay = (int)luaL_optinteger(L, 2, 0);
	Entity_Player* player = NULL;
	if (LuaEntityPlayer::IsUnderlyingType(L, 3)) {
		player = LuaEntityPlayer::Get(L, 3);
	}

	itemOverlay->Show(overlayID, delay, player);
	return 0;
}

LUA_FUNCTION(Lua_ItemOverlayGetPlayer) {
	ItemOverlay* itemOverlay = g_Game->GetItemOverlay();
	Entity_Player* player = itemOverlay->_player;
	if (!player) {
		lua_pushnil(L);
	}
	else {
		LuaEntityPlayer::PushPtr(L, player);
	}
	return 1;
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {

	lua_register(_state, "__Lua_ItemOverlay_Show", Lua_ItemOverlayShow);
	lua_register(_state, "__Lua_ItemOverlay_GetPlayer", Lua_ItemOverlayGetPlayer);

	super();
}