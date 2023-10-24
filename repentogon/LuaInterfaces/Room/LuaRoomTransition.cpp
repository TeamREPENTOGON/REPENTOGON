#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetRoomTransition) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	RoomTransition** ud = (RoomTransition**)lua_newuserdata(L, sizeof(RoomTransition*));
	*ud = game->GetRoomTransition();
	luaL_setmetatable(L, lua::metatables::RoomTransitionMT);
	return 1;
}

LUA_FUNCTION(Lua_RoomTransitionGetVersusScreenSprite) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, lua::metatables::RoomTransitionMT);
	ANM2* sprite = roomTransition->GetVersusScreenSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_RoomTransitionStartBossIntro) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, lua::metatables::RoomTransitionMT);
	int bossID1 = (int)luaL_checkinteger(L, 2);
	int bossID2 = (int)luaL_optinteger(L, 3, 0);

	roomTransition->StartBossIntro(bossID1, bossID2);
	return 0;
}

LUA_FUNCTION(Lua_RoomTransitionGetTransitionMode) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, lua::metatables::RoomTransitionMT);
	lua_pushinteger(L, roomTransition->GetTransitionMode());
	return 1;
}

LUA_FUNCTION(Lua_RoomTransitionIsRenderingBossIntro) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, lua::metatables::RoomTransitionMT);
	lua_pushboolean(L, roomTransition->IsRenderingBossIntro());
	return 1;
}

static void RegistertRoomTransition(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetRoomTransition", Lua_GetRoomTransition);

	luaL_Reg functions[] = {
		{ "GetVersusScreenSprite", Lua_RoomTransitionGetVersusScreenSprite },
		{ "StartBossIntro", Lua_RoomTransitionStartBossIntro },
		{ "GetTransitionMode", Lua_RoomTransitionGetTransitionMode },
		{ "IsRenderingBossIntro", Lua_RoomTransitionIsRenderingBossIntro },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::RoomTransitionMT, lua::metatables::RoomTransitionMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegistertRoomTransition(state);
}