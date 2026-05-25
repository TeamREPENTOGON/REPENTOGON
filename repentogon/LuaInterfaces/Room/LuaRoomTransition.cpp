#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetRoomTransition) {
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	RoomTransition** ud = (RoomTransition**)lua_newuserdata(L, sizeof(RoomTransition*));
	*ud = game->GetRoomTransition();
	luaL_setmetatable(L, lua::metatables::RoomTransitionMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_RoomTransitionGetVersusScreenSprite) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	ANM2* sprite = roomTransition->GetVersusScreenSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_RoomTransitionStartBossIntro) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	int bossID1 = (int)luaL_checkinteger(L, 1);
	int bossID2 = (int)luaL_optinteger(L, 2, 0);

	roomTransition->_roomIndex = g_Game->_startingRoomIdx; // safety measure to prevent crashes if current transition's roomIndex is invalid

	roomTransition->StartBossIntro(bossID1, bossID2);
	return 0;
}

LUA_FUNCTION(Lua_RoomTransitionGetTransitionMode) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	lua_pushinteger(L, roomTransition->GetTransitionMode());
	return 1;
}

LUA_FUNCTION(Lua_RoomTransitionIsRenderingBossIntro) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	lua_pushboolean(L, roomTransition->IsRenderingBossIntro());
	return 1;
}

static const int s_VersusScreenPlayerExtraPortraitLayers[4][2] = {
	{5, 12},
	{18, 21},
	{19, 22},
	{20, 23},
};

// Previously, RoomTransition contained a single sprite for the first player's "extra portrait" (ie tainted eden's glitchy effect)
// In REP+, with online co-op showing all players in the versus screen, this was replaced with an std::map of layer (int) to ANM2
// For backward compatability this function returns a reference to player 1's extra portrait by default.
// Also note that normally the game does not play the co-op version of the VS screen for local co-op.
// I think it did for earlier versions of Rep+. It still loads all the (extra) portraits though, so mods could trigger the animaion.
LUA_FUNCTION(Lua_RoomTransitionGetPlayerExtraPortraitSprite) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	int playerIndex = (int)luaL_optinteger(L, 1, 0);

	if (playerIndex < 0 || playerIndex > 3) {
		lua_pushnil(L);
		return 1;
	}

	// The "alt" layer is for the "no shake" version of the portrait.
	// Only one or the other is ever populated in the map, as it is recreated from scratch in StartBossIntro.
	int layer = s_VersusScreenPlayerExtraPortraitLayers[playerIndex][0];
	int altLayer = s_VersusScreenPlayerExtraPortraitLayers[playerIndex][1];

	auto& map = *roomTransition->GetExtraLayerANM2s();

	if (map.count(altLayer)) {
		lua::luabridge::UserdataPtr::push(L, &map[altLayer], lua::GetMetatableKey(lua::Metatables::SPRITE));
	} else if (map.count(layer)) {
		lua::luabridge::UserdataPtr::push(L, &map[layer], lua::GetMetatableKey(lua::Metatables::SPRITE));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

static void RegistertRoomTransition(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "GetVersusScreenSprite", Lua_RoomTransitionGetVersusScreenSprite );
	lua::TableAssoc(L, "StartBossIntro", Lua_RoomTransitionStartBossIntro );
	lua::TableAssoc(L, "GetTransitionMode", Lua_RoomTransitionGetTransitionMode );
	lua::TableAssoc(L, "IsRenderingBossIntro", Lua_RoomTransitionIsRenderingBossIntro );
	lua::TableAssoc(L, "GetPlayerExtraPortraitSprite", Lua_RoomTransitionGetPlayerExtraPortraitSprite );
	lua_setglobal(L, "RoomTransition");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegistertRoomTransition(state);
}