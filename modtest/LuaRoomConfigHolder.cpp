#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RoomConfigHolderMT = "RoomConfigHolder";

static int Lua_GameGetRoomConfigHolder(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	void** ud = (void**)lua_newuserdata(L, sizeof(void*));
	*ud = (char*)game + 0x1879C;
	// lua_pushlightuserdata(L, (char*)game + 0x1879C);
	luaL_setmetatable(L, RoomConfigHolderMT);
	return 1;
}

static int Lua_RoomConfigHolderGetRoomByStageTypeAndVariant(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 5) {
		return luaL_error(L, "Expected five parameters, got %d\n", n);
	}

	RoomConfigHolder* holder = *lua::GetUserdata<RoomConfigHolder**>(L, 1, RoomConfigHolderMT);
	int stage = luaL_checkinteger(L, 2);

	if (stage < 0 || stage > 36) {
		return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
	}

	int type = luaL_checkinteger(L, 3);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Type must be between 1 and 29 (both inclusive), got %d\n", type);
	}

	int variant = luaL_checkinteger(L, 4);
	int difficulty = luaL_checkinteger(L, 5);
	if (difficulty < -2 || difficulty > 1) {
		difficulty = -1;
	}

	RoomConfig* config = holder->GetRoomByStageTypeAndVariant(stage, type, variant, difficulty);
	if (!config) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, config, lua::GetMetatableKey(lua::Metatables::CONST_ROOM_CONFIG_ROOM));
	}

	return 1;
}

static void RegisterRoomConfigHolder(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetRoomConfigHolder");
	lua_pushcfunction(L, Lua_GameGetRoomConfigHolder);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, RoomConfigHolderMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetRoomByStageTypeAndVariant", Lua_RoomConfigHolderGetRoomByStageTypeAndVariant },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterRoomConfigHolder(state);
}