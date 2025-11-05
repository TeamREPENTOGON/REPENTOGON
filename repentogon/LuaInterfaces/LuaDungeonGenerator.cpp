#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaDungeonGenerator.h"

DungeonGenerator* GetDungeonGenerator(lua_State* L) {
	return *lua::GetRawUserdata<DungeonGenerator**>(L, 1, lua::metatables::DungeonGeneratorMT);
}

LUA_FUNCTION(place_room) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	RoomConfig_Room* config = lua::GetLuabridgeUserdata<RoomConfig_Room*>(L, 2, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	uint32_t row = (uint32_t)luaL_checkinteger(L, 3);
	uint32_t col = (uint32_t)luaL_checkinteger(L, 4);
	uint32_t seed = (uint32_t)luaL_checkinteger(L, 5);

	DungeonGeneratorRoom* generatorRoom = new DungeonGeneratorRoom(config, row, col, seed);
	generator->rooms[generator->num_rooms] = *generatorRoom;

	generator->num_rooms++;

	lua_pushboolean(L, true);

	return 1;
}

LUA_FUNCTION(set_final_boss_room) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	uint32_t row = (uint32_t)luaL_checkinteger(L, 2);
	uint32_t col = (uint32_t)luaL_checkinteger(L, 3);

	for (size_t i = 0; i < generator->num_rooms; i++) {
		DungeonGeneratorRoom* generator_room = &(generator->rooms[i]);
		
		if (generator_room->room != NULL) {
			if (row == generator_room->row && col == generator_room->col) {
				generator_room->is_final_boss = true;
			} else {
				generator_room->is_final_boss = false;
			}
		} else {
			break;
		}
	}

	return 0;
}

static void RegisterDungeonGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{"PlaceRoom", place_room},
		{"SetFinalBossRoom", set_final_boss_room},
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::DungeonGeneratorMT, lua::metatables::DungeonGeneratorMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterDungeonGenerator(_state);
}