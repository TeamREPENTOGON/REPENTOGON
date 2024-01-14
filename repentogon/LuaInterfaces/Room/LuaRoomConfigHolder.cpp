#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GameGetRoomConfigHolder) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	RoomConfigHolder** ud = (RoomConfigHolder**)lua_newuserdata(L, sizeof(RoomConfigHolder*));
	*ud = game->GetRoomConfigHolder();
	luaL_setmetatable(L, lua::metatables::RoomConfigHolderMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_RoomConfigHolderGetRoomByStageTypeAndVariant) {
	int n = lua_gettop(L);
	if (n != 4) {
		return luaL_error(L, "Expected four parameters, got %d\n", n);
	}

	RoomConfigHolder* holder = g_Game->GetRoomConfigHolder();
	int stage = (int)luaL_checkinteger(L, 1);

	if (stage < 0 || stage > 36) {
		return luaL_error(L, "StageID must be between 0 and 36 (both inclusive), got %d\n", stage);
	}

	int type = (int)luaL_checkinteger(L, 2);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Type must be between 1 and 29 (both inclusive), got %d\n", type);
	}

	int variant = (int)luaL_checkinteger(L, 3);
	int difficulty = (int)luaL_checkinteger(L, 4);
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
	RoomConfigHolder* holder = g_Game->GetRoomConfigHolder();
	int seed = (int)luaL_checkinteger(L, 1);
	bool reduceWeight = lua::luaL_checkboolean(L, 2);

	int stage = (int)luaL_checkinteger(L, 3);
	if (stage < 0 || (stage > 17 && stage < 27) || stage > 36) {
		return luaL_error(L, "Invalid stage %d\n", stage);
	}

	int type = (int)luaL_checkinteger(L, 4);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Invalid type %d\n", type);
	}

	int shape = (int)luaL_optinteger(L, 5, 13); //NUM_ROOMSHAPES
	if (shape < 1 || shape > 13) {
		return luaL_error(L, "Invalid shape %d\n", shape);
	}

	int minVariant = (int)luaL_optinteger(L, 6, 0);
	if (minVariant < 0) {
		minVariant = 0;
	}

	int maxVariant = (int)luaL_optinteger(L, 7, -1);
	if (maxVariant < minVariant && maxVariant >= 0) {
		return luaL_error(L, "maxVariant is lower than minVariant (min = %d, max = %d)\n", minVariant, maxVariant);
	}
	else if (maxVariant < 0) {
		maxVariant = -1;
	}

	int minDifficulty = (int)luaL_optinteger(L, 8, 0);
	if (minDifficulty < 0) {
		minDifficulty = 0;
	}

	int maxDifficulty = (int)luaL_optinteger(L, 9, 10);
	if (maxDifficulty < minDifficulty) {
		return luaL_error(L, "maxDifficulty is lower than minDifficulty (min = %d, max = %d)\n", minDifficulty, maxDifficulty);
	}

	int doors = (int)luaL_optinteger(L, 10, 0);
	if (doors < 0) {
		return luaL_error(L, "Invalid door mask %d\n", doors);
	}

	int subtype = (int)luaL_optinteger(L, 11, -1);
	if (subtype < -1) {
		return luaL_error(L, "Invalid subtype %d\n", subtype);
	}
	int mode = (int)luaL_optinteger(L, 12, -1);
	if (mode < -1 || mode > 1) {
		return luaL_error(L, "Invalid mode %d\n", mode);
	}


	RoomConfig* config = holder->GetRandomRoom(seed, reduceWeight, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, (unsigned int*)&doors, subtype, mode);
	lua::luabridge::UserdataPtr::push(L, config, lua::Metatables::CONST_ROOM_CONFIG_ROOM);
	return 1;
}

static void RegisterRoomConfigHolder(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetRoomConfigHolder", Lua_GameGetRoomConfigHolder);
	lua_newtable(L);
	lua::TableAssoc(L, "GetRoomByStageTypeAndVariant", Lua_RoomConfigHolderGetRoomByStageTypeAndVariant);
	lua::TableAssoc(L, "GetRandomRoom", Lua_RoomConfigHolder_GetRandomRoom);
	lua_setglobal(L, "RoomConfigHolder");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfigHolder(_state);
}