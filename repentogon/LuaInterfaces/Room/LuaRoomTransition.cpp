#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetRoomTransition) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
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

// REP+ temporarily disabled
/*LUA_FUNCTION(Lua_RoomTransitionGetPlayerExtraPortraitSprite) {
	RoomTransition* roomTransition = g_Game->GetRoomTransition();
	ANM2* sprite = &roomTransition->_playerExtraPortraitANM2;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}*/

static void RegistertRoomTransition(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetRoomTransition", Lua_GetRoomTransition);
	lua_newtable(L);
		lua::TableAssoc(L, "GetVersusScreenSprite", Lua_RoomTransitionGetVersusScreenSprite );
		lua::TableAssoc(L, "StartBossIntro", Lua_RoomTransitionStartBossIntro );
		lua::TableAssoc(L, "GetTransitionMode", Lua_RoomTransitionGetTransitionMode );
		lua::TableAssoc(L, "IsRenderingBossIntro", Lua_RoomTransitionIsRenderingBossIntro );
		// REP+ temporarily disabled
		//lua::TableAssoc(L, "GetPlayerExtraPortraitSprite", Lua_RoomTransitionGetPlayerExtraPortraitSprite );
	//lua::RegisterNewClass(L, lua::metatables::RoomTransitionMT, lua::metatables::RoomTransitionMT, functions);
	lua_setglobal(L, "RoomTransition");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegistertRoomTransition(state);
}