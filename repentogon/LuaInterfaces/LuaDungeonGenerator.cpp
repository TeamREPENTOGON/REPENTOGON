#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaDungeonGenerator.h"
#include "vector"
#include "../Utils/LevelGenUtils.h"

#pragma region DungeonGeneratorRoom Impl

DungeonGeneratorRoom::DungeonGeneratorRoom() {
	this->list_index = -1;

	this->room = nullptr;
	this->col = -1;
	this->row = -1;

	this->doors = -1;
	this->shape = -1;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, RoomConfig_Room* room, uint32_t col, uint32_t row, int doors) {
	this->list_index = list_index;

	this->room = room;
	this->col = col;
	this->row = row;
	this->doors = doors;

	this->shape = room->Shape;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng) {
	this->num_rooms = 0;
	this->rng = rng;
}

bool DungeonGenerator::CanRoomBePlaced(uint32_t col, uint32_t row, int room_shape, int doors) {
	RoomCoords base_coords(col, row);
	if (!base_coords.IsValid()) {
		return false;
	}

	std::vector<RoomCoords> occupied_coords = GetOccupiedCoords(base_coords, room_shape);
	for (RoomCoords coords : occupied_coords) {
		if (!coords.IsValid()) {
			return false;
		}

		int gridIndex = coords.ToGridIndex();

		if (this->occupied_grid_indexes[gridIndex] || this->forbidden_grid_indexes[gridIndex]) {
			return false;
		}
	}

	std::vector<RoomCoords> forbidden_coords = GetForbiddenNeighbors(base_coords, room_shape, doors);
	for (RoomCoords coords : forbidden_coords) {
		int gridIndex = coords.ToGridIndex();

		if (this->occupied_grid_indexes[gridIndex]) {
			return false;
		}
	}

	return true;
}

void DungeonGenerator::FillOccupiedAndForbiddenIndexes(uint32_t row, uint32_t col, int shape, int doors) {
	RoomCoords coords(col, row);

	std::vector<RoomCoords> occupied_coords = GetOccupiedCoords(coords, shape);
	for (RoomCoords coords : occupied_coords) {
		this->occupied_grid_indexes.set(coords.ToGridIndex());
	}

	std::vector<RoomCoords> forbidden_coords = GetForbiddenNeighbors(coords, shape, doors);
	for (RoomCoords coords : occupied_coords) {
		this->forbidden_grid_indexes.set(coords.ToGridIndex());
	}
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(RoomConfig_Room* room_config, uint32_t col, uint32_t row, int doors) {
	int new_room_list_index = this->num_rooms;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, room_config, col, row, doors);
	this->num_rooms++;
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

	FillOccupiedAndForbiddenIndexes(row, col, room_config->Shape, doors);

	return generatorRoom;
}

void DungeonGenerator::SetFinalBossRoom(DungeonGeneratorRoom* boss_room) {
	this->final_boss_index = boss_room->list_index;
}

bool DungeonGenerator::ValidateFloor() {
	bool has_final_room = this->final_boss_index >= 0;

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
	for (int i = 0; i < this->num_rooms; i++)
	{
		DungeonGeneratorRoom generator_room = this->rooms[i];

		if (generator_room.room != nullptr) {
			LevelGenerator_Room level_generator_room;
			level_generator_room._gridColIdx = generator_room.col;
			level_generator_room._gridLineIdx = generator_room.row;
			level_generator_room._doors = generator_room.doors;

			uint32_t seed = this->rng->Next();

			g_Game->PlaceRoom(&level_generator_room, generator_room.room, seed, 0);
		}
	}

	g_Game->_lastBossRoomListIdx = this->final_boss_index;
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
	uint32_t col = (uint32_t)luaL_checkinteger(L, 3);
	uint32_t row = (uint32_t)luaL_checkinteger(L, 4);
	int doors = (int)luaL_optinteger(L, 5, 255);

	// Can't have more doors than what the config allows.
	doors = doors & config->Doors;

	if (generator->CanRoomBePlaced(col, row, config->Shape, doors)) {
		DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, col, row, doors);

		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);

		return 1;
	} else {
		return 0;
	}
}

LUA_FUNCTION(Lua_PlaceDefaultStartingRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	int doors = (int)luaL_optinteger(L, 2, 15);

	uint32_t col = 6;
	uint32_t row = 6;

	KAGE::_LogMessage(0, "[SEX] Before validation\n");
	if (generator->CanRoomBePlaced(col, row, ROOMSHAPE_1x1, doors)) {
		int required_doors = 0;

		KAGE::_LogMessage(0, "[SEX] Before room config\n");
		RoomConfig* room_config = g_Game->GetRoomConfig();
		KAGE::_LogMessage(0, "[SEX] Before random room\n");
		RoomConfig_Room* config = room_config->GetRandomRoom(
			generator->rng->Next(),
			false,
			STB_SPECIAL_ROOMS,
			ROOM_DEFAULT,
			ROOMSHAPE_1x1,
			2,
			2,
			0,
			10,
			(unsigned int*)&required_doors, // If I don't do it like this it shits itself
			0,
			-1
		);

		KAGE::_LogMessage(0, "[SEX] Before place room\n");

		DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, col, row, doors);

		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);

		return 1;
	}
	else {
		return 0;
	}
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
		{"PlaceDefaultStartingRoom", Lua_PlaceDefaultStartingRoom},
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