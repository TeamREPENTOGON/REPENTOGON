#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* RoomConfigHolderMT = "RoomConfigHolder";

static int Lua_GameGetRoomConfigHolder(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	RoomConfigHolder** ud = (RoomConfigHolder**)lua_newuserdata(L, sizeof(RoomConfigHolder*));
	*ud = game->GetRoomConfigHolder();
	luaL_setmetatable(L, RoomConfigHolderMT);
	return 1;
}

static int Lua_RoomConfigHolderGetRoomByStageTypeAndVariant(lua_State* L) {
	int n = lua_gettop(L);
	if (n != 5) {
		return luaL_error(L, "Expected five parameters, got %d\n", n);
	}

	RoomConfigHolder* holder = *lua::GetUserdata<RoomConfigHolder**>(L, 1, RoomConfigHolderMT);
	int stage = (int)luaL_checkinteger(L, 2);

	if (stage < 0 || stage > 36) {
		return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
	}

	int type = (int)luaL_checkinteger(L, 3);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Type must be between 1 and 29 (both inclusive), got %d\n", type);
	}

	int variant = (int)luaL_checkinteger(L, 4);
	int difficulty = (int)luaL_checkinteger(L, 5);
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

LUA_FUNCTION(Lua_RoomConfigHolder_GetRandomRoom) {
	RoomConfigHolder* holder = *lua::GetUserdata<RoomConfigHolder**>(L, 1, RoomConfigHolderMT);
	int seed = luaL_checkinteger(L, 2);
	int stage = luaL_checkinteger(L, 3);
	if (stage < 0 || (stage > 17 && stage < 27) || stage > 36) {
		return luaL_error(L, "Invalid stage %d\n", stage);
	}

	int type = luaL_checkinteger(L, 4);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Invalid type %d\n", type);
	}

	int shape = luaL_checkinteger(L, 5);
	if (shape < 1 || shape > 13) {
		return luaL_error(L, "Invalid shape %d\n", shape);
	}

	int minVariant = luaL_checkinteger(L, 6);
	if (minVariant < 0) {
		minVariant = 0;
	}

	int maxVariant = luaL_checkinteger(L, 7);
	if (maxVariant < minVariant && maxVariant >= 0) {
		return luaL_error(L, "maxVariant is lower than minVariant (min = %d, max = %d)\n", minVariant, maxVariant);
	}
	else if (maxVariant < 0) {
		maxVariant = -1;
	}

	int minDifficulty = luaL_checkinteger(L, 8);
	if (minDifficulty < 1) {
		minDifficulty = 1;
	}

	int maxDifficulty = luaL_checkinteger(L, 9);
	if (maxDifficulty < minDifficulty) {
		return luaL_error(L, "maxDifficulty is lower than minDifficulty (min = %d, max = %d)\n", minDifficulty, maxDifficulty);
	}

	int doors = luaL_checkinteger(L, 10);
	if (doors < 0) {
		return luaL_error(L, "Invalid door mask %d\n", doors);
	}

	int subtype = luaL_checkinteger(L, 11);
	if (subtype < 0) {
		return luaL_error(L, "Invalid subtype %d\n", subtype);
	}

	RoomConfig* config = holder->GetRandomRoom(seed, 0, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, (unsigned int*) & doors, subtype, -1);
	lua::luabridge::UserdataPtr::push(L, config, lua::Metatables::CONST_ROOM_CONFIG_ROOM);
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
		{ "GetRandomRoom", Lua_RoomConfigHolder_GetRandomRoom },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterRoomConfigHolder(state);
}