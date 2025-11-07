#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaDungeonGenerator.h"
#include "vector"

#pragma region RoomCoords

struct RoomCoords {
	uint32_t col;
	uint32_t row;

	RoomCoords(uint32_t col, uint32_t row) {
		this->col = col;
		this->row = row;
	}

	bool IsValid() {
		return row >= 0 && row <= 13 && col >= 0 && col <= 13;
	}
};

#pragma endregion

#pragma region Helpers

std::vector<RoomCoords> GetOccupiedCoords(RoomCoords* base_coords, int shape) {
	std::vector<RoomCoords> occupied_coords = {};

	switch (shape)
	{
	case ROOMSHAPE_1x1:
	case ROOMSHAPE_IH:
	case ROOMSHAPE_IV:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		break;
	case ROOMSHAPE_1x2:
	case ROOMSHAPE_IIV:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row + 1));
		break;
	case ROOMSHAPE_2x1:
	case ROOMSHAPE_IIH:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row));
		break;
	case ROOMSHAPE_2x2:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row + 1));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row + 1));
		break;
	case ROOMSHAPE_LTL:
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row + 1));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row + 1));
		break;
	case ROOMSHAPE_LTR:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row + 1));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row + 1));
		break;
	case ROOMSHAPE_LBL:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row + 1));
		break;
	case ROOMSHAPE_LBR:
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col + 1, base_coords->row));
		occupied_coords.push_back(RoomCoords(base_coords->col, base_coords->row + 1));
		break;
	default:
		break;
	}

	return occupied_coords;
}

#pragma endregion

#pragma region DungeonGeneratorRoom Impl

DungeonGeneratorRoom::DungeonGeneratorRoom() {
	this->room = nullptr;
	this->col = -1;
	this->row = -1;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t col, uint32_t row) {
	this->room = room;
	this->col = col;
	this->row = row;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng) {
	this->num_rooms = 0;
	this->rng = rng;
}

bool DungeonGenerator::CanRoomBePlaced(uint32_t row, uint32_t col, int room_shape, uint32_t doors) {
	RoomCoords* base_coords = &RoomCoords(col, row);
	if (!base_coords->IsValid()) {
		KAGE::_LogMessage(0, "[SEX] Base coordinates aren't valid. %d-%d\n", base_coords->row, base_coords->col);
		return false;
	}

	std::vector<RoomCoords> occupied_coords = GetOccupiedCoords(base_coords, room_shape);
	for (RoomCoords coords : occupied_coords) {
		if (!coords.IsValid()) {
			KAGE::_LogMessage(0, "[SEX] Occupied coordinates aren't valid. %d-%d\n", base_coords->row, base_coords->col);
			return false;
		}
	}

	for (size_t i = 0; i < this->num_rooms; i++) {
		DungeonGeneratorRoom room = this->rooms[i];

		RoomCoords* other_base_coords = &RoomCoords(room.col, room.row);
		std::vector<RoomCoords> other_occupied_coords = GetOccupiedCoords(other_base_coords, room.room->Shape);

		for (RoomCoords coords : occupied_coords) {
			for (RoomCoords other_coords : other_occupied_coords) {
				if (coords.col == other_coords.col && coords.row == other_coords.row) {
					KAGE::_LogMessage(0, "[SEX] Occupied coordinates conflict with other. %d-%d\n", base_coords->row, base_coords->col);

					return false;
				}
			}
		}
	}

	return true;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(RoomConfig_Room* room_config, uint32_t col, uint32_t row) {
	this->rooms[this->num_rooms] = DungeonGeneratorRoom(room_config, col, row);
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
	uint32_t col = (uint32_t)luaL_checkinteger(L, 3);
	uint32_t row = (uint32_t)luaL_checkinteger(L, 4);
	uint32_t doors = (uint32_t)luaL_checkinteger(L, 5);

	doors = doors | config->Doors;

	if (generator->CanRoomBePlaced(row, col, config->Shape, doors)) {
		DungeonGeneratorRoom* generator_room = generator->PlaceRoom(config, col, row);

		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);

		return 1;
	} else {
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