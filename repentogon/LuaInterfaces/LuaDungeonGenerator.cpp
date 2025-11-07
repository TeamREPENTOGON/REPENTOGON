#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaDungeonGenerator.h"

#pragma region DungeonGeneratorRoom Impl

DungeonGeneratorRoom::DungeonGeneratorRoom() {
	this->room = nullptr;
	this->row = -1;
	this->col = -1;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t row, uint32_t col) {
	this->room = room;
	this->row = row;
	this->col = col;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng) {
	this->num_rooms = 0;
	this->rng = rng;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(RoomConfig_Room* room_config, uint32_t row, uint32_t col) {
	this->rooms[this->num_rooms] = DungeonGeneratorRoom(room_config, row, col);
	this->num_rooms++;
	DungeonGeneratorRoom* generatorRoom = &this->rooms[this->num_rooms - 1];

	return generatorRoom;
}

void DungeonGenerator::SetFinalBossRoom(DungeonGeneratorRoom* boss_room) {
	for (size_t i = 0; i < this->num_rooms; i++)
	{
		this->rooms[i].is_final_boss = false;
	}

	boss_room->is_final_boss = true;
}

bool DungeonGenerator::ValidateFloor() {
	bool has_final_room = false;

	for (size_t i = 0; i < this->num_rooms; i++)
	{
		DungeonGeneratorRoom room = this->rooms[i];
		if (room.is_final_boss) {
			has_final_room = true;
			break;
		}
	}

	return has_final_room;
}

void DungeonGenerator::CleanFloor(Level* level) {
	level->reset_room_list(false);

	for (size_t i = 0; i < 507; i++)
	{
		g_Game->_roomOffset[i] = -1;
	}

	g_Game->_nbRooms = 0;
}

void DungeonGenerator::PlaceRoomsInFloor() {
	for (size_t i = 0; i < this->num_rooms; i++)
	{
		DungeonGeneratorRoom generator_room = this->rooms[i];

		if (generator_room.room != nullptr) {
			LevelGenerator_Room* level_generator_room = &LevelGenerator_Room();
			level_generator_room->_gridColIdx = generator_room.col;
			level_generator_room->_gridLineIdx = generator_room.row;
			level_generator_room->_doors = 15;

			uint32_t seed = this->rng->Next();

			g_Game->PlaceRoom(level_generator_room, generator_room.room, seed, 0);

			if (generator_room.is_final_boss) {
				g_Game->_lastBossRoomListIdx = i;
			}
		}
	}
}

bool DungeonGenerator::Generate(Level* level) {
	if (!this->ValidateFloor()) {
		return false;
	}

	CleanFloor(level);

	PlaceRoomsInFloor();

	return true;
}

#pragma endregion

DungeonGenerator* GetDungeonGenerator(lua_State* L) {
	return *lua::GetRawUserdata<DungeonGenerator**>(L, 1, lua::metatables::DungeonGeneratorMT);
}

LUA_FUNCTION(Lua_PlaceRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	RoomConfig_Room* config = lua::GetLuabridgeUserdata<RoomConfig_Room*>(L, 2, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	uint32_t row = (uint32_t)luaL_checkinteger(L, 3);
	uint32_t col = (uint32_t)luaL_checkinteger(L, 4);

	DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, row, col);

	DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
	*ud = generator_room;
	luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);

	return 1;
}

LUA_FUNCTION(Lua_SetFinalBossRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	DungeonGeneratorRoom* generator_room = *lua::GetRawUserdata<DungeonGeneratorRoom**>(L, 2, lua::metatables::DungeonGeneratorRoomMT);

	generator->SetFinalBossRoom(generator_room);

	return 0;
}

static void RegisterDungeonGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{"PlaceRoom", Lua_PlaceRoom},
		{"SetFinalBossRoom", Lua_SetFinalBossRoom},
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::DungeonGeneratorMT, lua::metatables::DungeonGeneratorMT, functions);
}

static void RegisterDungeonGeneratorRoom(lua_State* L) {
	luaL_Reg functions[] = {
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::DungeonGeneratorRoomMT, lua::metatables::DungeonGeneratorRoomMT, functions);
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	RegisterDungeonGenerator(_state);
	RegisterDungeonGeneratorRoom(_state);
}