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

RoomConfig_Room* DungeonGeneratorRoom::GetRoomConfig(uint32_t seed, int required_doors) {
	if (this->room != nullptr) {
		return this->room;
	}

	return nullptr;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng) {
	this->num_rooms = 0;
	this->rng = rng;

	this->level_generator._rng = *rng;
	std::fill_n(this->level_generator._roomMap, 169, -1);
	std::fill_n(this->level_generator._blockedPositions, 169, false);
	this->level_generator._isChapter6 = false;
	this->level_generator._isStageVoid = false;
	this->level_generator._isXL = false;
}

bool DungeonGenerator::CanRoomBePlaced(XY& base_coords, int shape, int allowed_doors, bool allow_unconnected) {
	int base_grid_index = base_coords.ToGridIdx();
	if (!this->level_generator.IsPositionInBounds(base_coords)) {
		return false;
	}

	if (!this->level_generator.is_pos_free(&base_coords, shape)) {
		return false;
	}

	if (!allow_unconnected) {
		if (!this->level_generator.is_placement_valid((unsigned int*)&base_grid_index, shape)) {
			return false;
		}
	}

	std::vector<XY> forbbidden_neighbors = GetForbiddenNeighbors(base_coords, shape, allowed_doors);
	for (XY coords : forbbidden_neighbors) {
		int grid_index = coords.ToGridIdx();
		if (this->level_generator._roomMap[grid_index] > -1) {
			return false;
		}
	}

	return true;
}

void DungeonGenerator::BlockPositionsFromAllowedDoords(XY& base_coords, int shape, int allowed_doors) {
	std::vector<XY> forbbidden_neighbors = GetForbiddenNeighbors(base_coords, shape, allowed_doors);
	for (XY coords : forbbidden_neighbors) {
		this->level_generator.BlockPosition(coords);
	}
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(RoomConfig_Room* room_config, uint32_t col, uint32_t row, int doors) {
	LevelGenerator_Room level_generator_room;
	level_generator_room._gridColIdx = col;
	level_generator_room._gridLineIdx = row;
	level_generator_room._shape = room_config->Shape;
	bool result = this->level_generator.place_room(&level_generator_room);

	LevelGenerator_Room placed_room = this->level_generator._rooms.at(this->level_generator._rooms.size() - 1);

	int new_room_list_index = placed_room._generationIndex;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, room_config, col, row, doors);
	this->num_rooms++;
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

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

bool DungeonGenerator::PlaceRoomsInFloor() {
	this->level_generator.calc_required_doors();

	for (LevelGenerator_Room room : this->level_generator._rooms)
	{
		DungeonGeneratorRoom generator_room = this->rooms[room._generationIndex];
		RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), room._doors);

		if (room_config == nullptr) {
			return false;
		}

		uint32_t seed = this->rng->Next();

		g_Game->PlaceRoom(&room, room_config, seed, 0);
	}

	g_Game->_lastBossRoomListIdx = this->final_boss_index;

	return true;
}

bool DungeonGenerator::Generate(Level* level) {
	if (!this->ValidateFloor()) {
		KAGE::_LogMessage(1, "[WARN] Failed to validate custom floor, not placing rooms.\n");

		return false;
	}

	CleanFloor(level);

	bool could_place_rooms = PlaceRoomsInFloor();
	if (!could_place_rooms) {
		KAGE::_LogMessage(1, "[WARN] Couldn't place the rooms in the level, clearing placed rooms...\n");
		CleanFloor(level);
	}

	return could_place_rooms;
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
	int allowed_doors = (int)luaL_optinteger(L, 5, 255);

	// Can't have more doors than what the config allows.
	allowed_doors = allowed_doors & config->Doors;

	XY coords(col, row);

	if (generator->CanRoomBePlaced(coords, config->Shape, allowed_doors, true)) {
		DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, col, row, allowed_doors);

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

	XY coords(col, row);

	if (generator->CanRoomBePlaced(coords, ROOMSHAPE_1x1, doors, true)) {
		unsigned int required_doors = 0;

		RoomConfig* room_config = g_Game->GetRoomConfig();
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
			&required_doors, // If I don't do it like this it shits itself
			0,
			-1
		);

		DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, col, row, doors);

		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_SetFinalBossRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	DungeonGeneratorRoom* generator_room = *lua::GetRawUserdata<DungeonGeneratorRoom**>(L, 2, lua::metatables::DungeonGeneratorRoomMT);

	generator->SetFinalBossRoom(generator_room);

	return 0;
}

LUA_FUNCTION(Lua_BlockIndex) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	int grid_index = (int)luaL_checkinteger(L, 2);

	generator->level_generator.BlockPosition(grid_index);

	return 0;
}


static void RegisterDungeonGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{"PlaceRoom", Lua_PlaceRoom},
		{"SetFinalBossRoom", Lua_SetFinalBossRoom},
		{"PlaceDefaultStartingRoom", Lua_PlaceDefaultStartingRoom},
		{"BlockIndex", Lua_BlockIndex},
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