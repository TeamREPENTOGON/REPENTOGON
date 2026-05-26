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

	this->stage = -1;
	this->type = -1;
	this->shape = -1;
	this->shape = -1;
	this->minVariant = -1;
	this->maxVariant = -1;
	this->minDifficulty = -1;
	this->maxDifficulty = -1;
	this->subtype = -1;
	this->mode = -1;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, uint32_t col, uint32_t row, int doors, RoomConfig_Room* room) {
	this->list_index = list_index;

	this->room = room;
	this->col = col;
	this->row = row;
	this->doors = doors;


	this->stage = room->StageId;
	this->type = room->Type;
	this->shape = room->Shape;
	this->minVariant = room->Variant;
	this->maxVariant = room->Variant;
	this->minDifficulty = room->Difficulty;
	this->maxDifficulty = room->Difficulty;
	this->subtype = room->Subtype;
	this->mode = 0;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, uint32_t row, uint32_t col, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
	this->list_index = list_index;

	this->room = nullptr;
	this->col = col;
	this->row = row;
	this->doors = doors;

	this->stage = stage;
	this->type = type;
	this->shape = shape;
	this->minVariant = minVariant;
	this->maxVariant = maxVariant;
	this->minDifficulty = minDifficulty;
	this->maxDifficulty = maxDifficulty;
	this->subtype = subtype;
	this->mode = mode;
}

RoomConfig_Room* DungeonGeneratorRoom::GetRoomConfig(uint32_t seed, uint32_t required_doors) {
	if (this->room != nullptr) {
		return this->room;
	}

	if (this->stage >= 0) {
		return g_Game->GetRoomConfig()->GetRandomRoom(
			seed,
			true,
			this->stage,
			this->type,
			this->shape,
			this->minVariant,
			this->maxVariant,
			this->minDifficulty,
			this->maxDifficulty,
			&required_doors,
			this->subtype,
			this->mode
		);
	}

	return nullptr;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng, Level* level, DungeonGenerationType generation_type) {
	this->rng = rng;
	this->level = level;
	this->generation_type = generation_type;

	this->level_generator._rng = *rng;
	this->level_generator._isChapter6 = false;
	this->level_generator._isStageVoid = false;
	this->level_generator._isXL = false;
	
	this->ResetLevelGenerator();
	this->InitializeDefaultGridRooms();
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

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config) {
	int col = base_coords.x;
	int row = base_coords.y;

	LevelGenerator_Room level_generator_room;
	level_generator_room._gridColIdx = col;
	level_generator_room._gridLineIdx = row;
	level_generator_room._shape = room_config->Shape;
	bool result = this->level_generator.place_room(&level_generator_room);

	LevelGenerator_Room placed_room = this->level_generator._rooms.at(this->level_generator._rooms.size() - 1);

	int new_room_list_index = placed_room._generationIndex;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, room_config);
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
	int col = base_coords.x;
	int row = base_coords.y;
	
	LevelGenerator_Room level_generator_room;
	level_generator_room._gridColIdx = col;
	level_generator_room._gridLineIdx = row;
	level_generator_room._shape = shape;
	bool result = this->level_generator.place_room(&level_generator_room);

	LevelGenerator_Room placed_room = this->level_generator._rooms.at(this->level_generator._rooms.size() - 1);

	int new_room_list_index = placed_room._generationIndex;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode);
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config) {
	// Can't have more doors than what the config allows.
	doors = doors & room_config->Doors;

	if (this->CanRoomBePlaced(base_coords, room_config->Shape, doors, true)) {
		return this->PlaceRoom(base_coords, doors, room_config);
	}

	return nullptr;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
	if (this->CanRoomBePlaced(base_coords, shape, doors, true)) {
		return this->PlaceRoom(
			base_coords,
			doors,
			stage,
			type,
			shape,
			minVariant,
			maxVariant,
			minDifficulty,
			maxDifficulty,
			subtype,
			mode
		);
	}

	return nullptr;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceOffGridRoom(int off_grid_index, RoomConfig_Room* room_config) {
	int index = -off_grid_index - 1;
	
	this->off_grid_rooms[index] = DungeonGeneratorRoom(-1, -1, -1, 0, room_config);
	DungeonGeneratorRoom* generatorRoom = &this->off_grid_rooms[index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceOffGridRoom(int off_grid_index, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
	int index = -off_grid_index - 1;

	this->off_grid_rooms[index] = DungeonGeneratorRoom(-1, -1, -1, 0, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode);
	DungeonGeneratorRoom* generatorRoom = &this->off_grid_rooms[index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceDefaultStartingRoom(int doors) {
	uint32_t col = 6;
	uint32_t row = 6;
	XY coords(col, row);

	unsigned int required_doors = 0;

	RoomConfig* room_config = g_Game->GetRoomConfig();
	RoomConfig_Room* config = room_config->GetRandomRoom(
		this->rng->Next(),
		false,
		STB_SPECIAL_ROOMS,
		ROOM_DEFAULT,
		ROOMSHAPE_1x1,
		2,
		2,
		0,
		10,
		&required_doors,
		0,
		-1
	);

	return this->TryPlaceRoom(coords, doors, config);
}

void DungeonGenerator::InitializeDefaultGridRooms() {
	this->PlaceOffGridRoom(
		ROOM_ERROR_IDX,
		STB_SPECIAL_ROOMS,
		ROOM_ERROR,
		NUM_ROOMSHAPES,
		0,
		-1,
		1,
		10,
		-1,
		-1
	);

	this->PlaceOffGridRoom(
		ROOM_DUNGEON_IDX,
		STB_SPECIAL_ROOMS,
		ROOM_DUNGEON,
		NUM_ROOMSHAPES,
		0,
		-1,
		1,
		10,
		-1,
		-1
	);

	this->PlaceOffGridRoom(
		ROOM_BLACK_MARKET_IDX,
		STB_SPECIAL_ROOMS,
		ROOM_BLACK_MARKET,
		NUM_ROOMSHAPES,
		0,
		-1,
		1,
		10,
		-1,
		-1
	);

	if (this->generation_type != BACKWARDS && this->generation_type != HOME) {
		PersistentGameData* data = g_Manager->GetPersistentGameData();
		int a = 1;
		if (data->Unlocked(152)) { // Shop upgrade 2
			a = 2;
		}

		int b = 1;
		if (data->Unlocked(153)) { // Shop upgrade 3
			b = 2;
		}

		if (data->Unlocked(154)) { // Shop upgrade 4
			a++;
		}

		int shop_subtype = rng->RandomInt(a) + rng->RandomInt(b);

		this->PlaceOffGridRoom(
			ROOM_SECRET_SHOP_IDX,
			STB_SPECIAL_ROOMS,
			ROOM_SHOP,
			NUM_ROOMSHAPES,
			0,
			-1,
			1,
			10,
			shop_subtype,
			-1
		);
	}
	
	if (this->generation_type != HOME) {
		this->PlaceOffGridRoom(
			ROOM_ANGEL_SHOP_IDX,
			STB_SPECIAL_ROOMS,
			ROOM_ANGEL,
			NUM_ROOMSHAPES,
			0,
			-1,
			1,
			10,
			1,
			-1
		);
	}

	if (this->generation_type == DEFAULT)
	{
		Entity_Player* player_with_broken_shovel = g_Game->GetPlayerManager()->FirstCollectibleOwner(COLLECTIBLE_BROKEN_SHOVEL_1, nullptr, false);
		if (player_with_broken_shovel == nullptr) {
			this->PlaceOffGridRoom(
				ROOM_BOSSRUSH_IDX,
				STB_SPECIAL_ROOMS,
				ROOM_BOSSRUSH,
				NUM_ROOMSHAPES,
				0,
				-1,
				1,
				10,
				-1,
				-1
			);
		}
		else {
			this->PlaceOffGridRoom(
				ROOM_BOSSRUSH_IDX,
				g_Game->GetRoomConfig()->GetRoomByStageTypeAndVariant(STB_SPECIAL_ROOMS, ROOM_BOSSRUSH, 0, -1)
			);
		}

		this->PlaceOffGridRoom(
			ROOM_MEGA_SATAN_IDX,
			STB_SPECIAL_ROOMS,
			ROOM_BOSS,
			NUM_ROOMSHAPES,
			0,
			-1,
			1,
			10,
			BOSS_MEGA_SATAN,
			-1
		);

		if (g_Game->_stage != STAGE4_3) {
			this->PlaceOffGridRoom(
				ROOM_BLUE_WOOM_IDX,
				STB_BLUE_WOMB,
				ROOM_DEFAULT,
				NUM_ROOMSHAPES,
				0,
				-1,
				0,
				0,
				1,
				-1
			);
		} else {
			this->PlaceOffGridRoom(
				ROOM_THE_VOID_IDX,
				STB_BLUE_WOMB,
				ROOM_DEFAULT,
				NUM_ROOMSHAPES,
				0,
				-1,
				0,
				0,
				1,
				-1
			);
		}

		if (this->level->_stageType == STAGETYPE_REPENTANCE || this->level->_stageType == STAGETYPE_REPENTANCE_B) {
			if ((this->level->_stage == STAGE4_1 && this->level->GetCurses()) || this->level->_stage == STAGE4_2) {
				this->PlaceOffGridRoom(
					ROOM_SECRET_EXIT_IDX,
					this->level->GetStageID(),
					ROOM_BOSS,
					NUM_ROOMSHAPES,
					0,
					-1,
					1,
					10,
					BOSS_MOTHER,
					-1
				);
			}
		}
	}
	else if (this->generation_type == HOME) {
		this->PlaceOffGridRoom(
			ROOM_SECRET_EXIT_IDX,
			g_Game->GetRoomConfig()->GetRoomByStageTypeAndVariant(STB_HOME, ROOM_DUNGEON, 666, -1)
		);
	}
}

void DungeonGenerator::SetFinalBossRoom(DungeonGeneratorRoom* boss_room) {
	this->final_boss_index = boss_room->list_index;
}

bool DungeonGenerator::ValidateFloor() {
	this->level_generator.calc_required_doors();

	bool has_final_room = this->final_boss_index >= 0;

	// Check if all rooms can fetch a room config
	int initial_seed = this->rng->_seed;

	for (LevelGenerator_Room room : this->level_generator._rooms)
	{
		DungeonGeneratorRoom generator_room = this->rooms[room._generationIndex];
		RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), room._doors);

		if (room_config == nullptr) {
			return false;
		}
		
		// When placing the rooms the rng is advanced here too.
		this->rng->Next();
	}
	
	this->rng->_seed = initial_seed;

	return has_final_room;
}

void DungeonGenerator::CleanFloor() {
	this->level->reset_room_list(true);

	for (size_t i = 0; i < 507; i++)
	{
		g_Game->_roomOffset[i] = -1;
	}

	g_Game->_nbRooms = 0;
}

void DungeonGenerator::ResetLevelGenerator() {
	std::fill_n(this->level_generator._roomMap, 169, -1);
	std::fill_n(this->level_generator._blockedPositions, 169, false);

	this->level_generator._rooms.clear();
}

void DungeonGenerator::Reset() {
	this->CleanFloor();

	this->ResetLevelGenerator();

	this->final_boss_index = -1;
	for (size_t i = 0; i < 169; i++)
	{
		this->rooms[i] = DungeonGeneratorRoom();
	}
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

	for (int i = 0; i < 20; i++)
	{
		int offGridIndex = -(i + 1);
		DungeonGeneratorRoom generator_room = this->off_grid_rooms[i];
		RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), 0);
		
		if (room_config == nullptr) {
			continue;
		}

		RoomDescriptor* room_desc = this->level->GetRoomByIdx(offGridIndex, -1);
		room_desc->Data = room_config;
		room_desc->InitSeeds(this->rng);
	}

	RoomDescriptor* lil_portal_room = this->level->GetRoomByIdx(ROOM_LIL_PORTAL_IDX, -1);
	if (lil_portal_room == nullptr || lil_portal_room->Data == nullptr) {
		this->level->initialize_lil_portal_room();
	}

	g_Game->_lastBossRoomListIdx = this->final_boss_index;

	return true;
}

bool DungeonGenerator::Generate() {
	if (!this->ValidateFloor()) {
		KAGE::_LogMessage(1, "[WARN] Failed to validate custom floor, not placing rooms.\n");
		Reset();

		return false;
	}

	CleanFloor();

	bool could_place_rooms = PlaceRoomsInFloor();
	if (!could_place_rooms) {
		KAGE::_LogMessage(1, "[WARN] Couldn't place the rooms in the level, clearing placed rooms...\n");
		Reset();
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
	XY coords(col, row);
	int allowed_doors = (int)luaL_optinteger(L, 5, 255);

	DungeonGeneratorRoom* generator_room = generator->TryPlaceRoom(coords, allowed_doors, config);

	if (generator_room != nullptr) {
		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlaceRandomRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	uint32_t col = (uint32_t)luaL_checkinteger(L, 2);
	uint32_t row = (uint32_t)luaL_checkinteger(L, 3);
	XY coords(col, row);

	int stage = (int)luaL_checkinteger(L, 4);
	if (stage < 0 || (stage >= STB_UNUSED1 && stage <= STB_ULTRA_GREED) || stage == STB_THE_VOID || stage >= NUM_STB) {
		return luaL_error(L, "Invalid stage %d\n", stage);
	}

	int type = (int)luaL_checkinteger(L, 5);
	if (type < 1 || type > 29) {
		return luaL_error(L, "Invalid type %d\n", type);
	}

	int shape = (int)luaL_optinteger(L, 6, 13); //NUM_ROOMSHAPES
	if (shape < 1 || shape > 13) {
		return luaL_error(L, "Invalid shape %d\n", shape);
	}

	int minVariant = (int)luaL_optinteger(L, 7, 0);
	if (minVariant < 0) {
		minVariant = 0;
	}

	int maxVariant = (int)luaL_optinteger(L, 8, -1);
	if (maxVariant < minVariant && maxVariant >= 0) {
		return luaL_error(L, "maxVariant is lower than minVariant (min = %d, max = %d)\n", minVariant, maxVariant);
	}
	else if (maxVariant < 0) {
		maxVariant = -1;
	}

	int minDifficulty = (int)luaL_optinteger(L, 9, 0);
	if (minDifficulty < 0) {
		minDifficulty = 0;
	}

	int maxDifficulty = (int)luaL_optinteger(L, 10, 10);
	if (maxDifficulty < minDifficulty) {
		return luaL_error(L, "maxDifficulty is lower than minDifficulty (min = %d, max = %d)\n", minDifficulty, maxDifficulty);
	}

	int subtype = (int)luaL_optinteger(L, 11, -1);
	if (subtype < -1) {
		return luaL_error(L, "Invalid subtype %d\n", subtype);
	}
	int mode = (int)luaL_optinteger(L, 12, -1);
	if (mode < -1 || mode > 1) {
		return luaL_error(L, "Invalid mode %d\n", mode);
	}

	int allowed_doors = (int)luaL_optinteger(L, 13, 255);

	DungeonGeneratorRoom* generator_room = generator->TryPlaceRoom(
		coords,
		allowed_doors,
		stage,
		type,
		shape,
		minVariant,
		maxVariant,
		minDifficulty,
		maxDifficulty,
		subtype,
		mode
	);

	if (generator_room != nullptr) {
		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);
	}
	else {
		lua_pushnil(L);
	}

	return 1;

}

LUA_FUNCTION(Lua_PlaceOffGridRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	int off_grid_index = luaL_checkinteger(L, 2);
	if (off_grid_index < -20 || off_grid_index > -1) {
		return luaL_error(L, "Invalid grid index %d\n", off_grid_index);
	}

	RoomConfig_Room* config = lua::GetLuabridgeUserdata<RoomConfig_Room*>(L, 3, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");

	DungeonGeneratorRoom* generator_room = generator->PlaceOffGridRoom(off_grid_index, config);

	if (generator_room != nullptr) {
		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlaceRandomOffGridRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	int off_grid_index = luaL_checkinteger(L, 2);
	if (off_grid_index < -20 || off_grid_index > -1) {
		return luaL_error(L, "Invalid grid index %d\n", off_grid_index);
	}

	int stage = (int)luaL_checkinteger(L, 3);
	if (stage < 0 || (stage >= STB_UNUSED1 && stage <= STB_ULTRA_GREED) || stage == STB_THE_VOID || stage >= NUM_STB) {
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

	int subtype = (int)luaL_optinteger(L, 10, -1);
	if (subtype < -1) {
		return luaL_error(L, "Invalid subtype %d\n", subtype);
	}
	int mode = (int)luaL_optinteger(L, 11, -1);
	if (mode < -1 || mode > 1) {
		return luaL_error(L, "Invalid mode %d\n", mode);
	}

	DungeonGeneratorRoom* generator_room = generator->PlaceOffGridRoom(
		off_grid_index,
		stage,
		type,
		shape,
		minVariant,
		maxVariant,
		minDifficulty,
		maxDifficulty,
		subtype,
		mode
	);

	if (generator_room != nullptr) {
		DungeonGeneratorRoom** ud = (DungeonGeneratorRoom**)lua_newuserdata(L, sizeof(DungeonGeneratorRoom*));
		*ud = generator_room;
		luaL_setmetatable(L, lua::metatables::DungeonGeneratorRoomMT);
	}
	else {
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlaceDefaultStartingRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	int doors = (int)luaL_optinteger(L, 2, 15);

	DungeonGeneratorRoom* generator_room = generator->TryPlaceDefaultStartingRoom(doors);

	if (generator_room != nullptr) {
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

LUA_FUNCTION(Lua_Validate) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	bool result = generator->ValidateFloor();

	lua_pushboolean(L, result);

	return 1;
}

LUA_FUNCTION(Lua_Reset) {
	DungeonGenerator* generator = GetDungeonGenerator(L);

	generator->Reset();

	return 0;
}


static void RegisterDungeonGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{"PlaceRoom", Lua_PlaceRoom},
		{"PlaceRandomRoom", Lua_PlaceRandomRoom},
		{"PlaceOffGridRoom", Lua_PlaceOffGridRoom},
		{"PlaceRandomOffGridRoom", Lua_PlaceRandomOffGridRoom},
		{"SetFinalBossRoom", Lua_SetFinalBossRoom},
		{"PlaceDefaultStartingRoom", Lua_PlaceDefaultStartingRoom},
		{"BlockIndex", Lua_BlockIndex},
		{"Validate", Lua_Validate},
		{"Reset", Lua_Reset},
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