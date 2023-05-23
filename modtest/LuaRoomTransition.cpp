#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RoomTransitionMT = "RoomTransition";

static int Lua_GetRoomTransition(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	RoomTransition** ud = (RoomTransition**)lua_newuserdata(L, sizeof(RoomTransition*));
	*ud = game->GetRoomTransition();
	luaL_setmetatable(L, RoomTransitionMT);
	return 1;
}

static int Lua_RoomTransitionGetVersusScreenSprite(lua_State* L) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, RoomTransitionMT);
	ANM2* sprite = roomTransition->GetVersusScreenSprite();
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

static int Lua_RoomTransitionStartBossIntro(lua_State* L) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, RoomTransitionMT);
	int bossID1 = luaL_checkinteger(L, 2);
	int bossID2 = luaL_optinteger(L, 3, 0);

	roomTransition->StartBossIntro(bossID1, bossID2);
	return 0;
}

static int Lua_RoomTransitionGetTransitionMode(lua_State* L) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, RoomTransitionMT);
	lua_pushinteger(L, roomTransition->GetTransitionMode());
	return 1;
}

static int Lua_RoomTransitionIsRenderingBossIntro(lua_State* L) {
	RoomTransition* roomTransition = *lua::GetUserdata<RoomTransition**>(L, 1, RoomTransitionMT);
	lua_pushboolean(L, roomTransition->IsRenderingBossIntro());
	return 1;
}

static void RegistertRoomTransition(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetRoomTransition");
	lua_pushcfunction(L, Lua_GetRoomTransition);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomTransitionMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetVersusScreenSprite", Lua_RoomTransitionGetVersusScreenSprite },
		{ "StartBossIntro", Lua_RoomTransitionStartBossIntro },
		{ "GetTransitionMode", Lua_RoomTransitionGetTransitionMode },
		{ "IsRenderingBossIntro", Lua_RoomTransitionIsRenderingBossIntro },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegistertRoomTransition(state);
}