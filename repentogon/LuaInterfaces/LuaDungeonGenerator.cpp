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

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, uint32_t col, uint32_t row, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
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

RoomConfig_Room* DungeonGeneratorRoom::GetRoomConfig(uint32_t seed, uint32_t required_doors, Level* level) {
    if (this->room != nullptr) {
        return this->room;
    }

    if (this->stage >= 0) {
        int current_stage = g_Game->_stage;

        RoomConfig_Room* config = g_Game->GetRoomConfig()->GetRandomRoom(
            seed, true, this->stage, this->type, this->shape,
            this->minVariant, this->maxVariant, this->minDifficulty,
            this->maxDifficulty, &required_doors, this->subtype, this->mode
        );

        if (config == nullptr && current_stage != this->stage) {
            config = g_Game->GetRoomConfig()->GetRandomRoom(
                seed, true, current_stage, this->type, this->shape,
                this->minVariant, this->maxVariant, this->minDifficulty,
                this->maxDifficulty, &required_doors, this->subtype, this->mode
            );
        }

        return config;  // nullptr is valid and expected on failure
    }

    return nullptr;
}

#pragma endregion

#pragma region DungeonGenerator Impl

DungeonGenerator::DungeonGenerator(RNG* rng, Level* level, DungeonGenerationType generation_type, bool reset_lil_portal) {
    this->rng = rng;
    this->level = level;
    this->generation_type = generation_type;
    this->reset_lil_portal = reset_lil_portal;
    this->level_generator._rng = *rng;
    
    // _isChapter6/_isStageVoid must stay false: setting them true makes place_room run hardcoded endgame logic that reads uninitialized memory
    this->level_generator._isChapter6 = false; 
    this->level_generator._isStageVoid = false; 
    this->level_generator._isXL = false;
    
    this->ResetLevelGenerator();
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
	if (room_config == nullptr) {
		return nullptr;
	}

	int col = base_coords.x;
	int row = base_coords.y;

	LevelGenerator_Room safe_room;
	safe_room.constructor(col, row, room_config->Shape);
	safe_room._doors = doors;

	// LevelGenerator::place_room (0x009b0330) returns LevelGenerator_Room*, not bool
	// ZHL bool truncation causes low byte 0x00 to register as false (roughly 5% false failures)
	// Room placement success must be verified via _rooms vector growth due to this
	size_t prev_size = this->level_generator._rooms.size();
	this->level_generator.place_room(&safe_room);
	if (this->level_generator._rooms.size() <= prev_size) {
		return nullptr;
	}

	LevelGenerator_Room& placed_room = this->level_generator._rooms.at(this->level_generator._rooms.size() - 1);

	int new_room_list_index = placed_room._generationIndex;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, room_config);
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
    int col = base_coords.x;
    int row = base_coords.y;
    
    LevelGenerator_Room safe_room;

    safe_room.constructor(col, row, shape);

    safe_room._doors = doors;
    safe_room._generationIndex = 0;
    safe_room._deadEnd = false;
    safe_room._unk1 = false;
    safe_room._unk8 = false;
    safe_room._setExtraField = 0;
    safe_room._linkColIdx = -1;
    safe_room._linkLineIdx = -1;
    safe_room._originNeighborConnectDir = -1;
    safe_room._originNeighborConnectDirAdjust = -1;
    safe_room._distanceFromStart = 0; 

	// LevelGenerator::place_room (0x009b0330) returns LevelGenerator_Room*, not bool
	// ZHL bool truncation causes low byte 0x00 to register as false (roughly 5% false failures)
	// Room placement success must be verified via _rooms vector growth due to this
    size_t prev_size = this->level_generator._rooms.size();
    this->level_generator.place_room(&safe_room);
    if (this->level_generator._rooms.size() <= prev_size) {
        return nullptr;
    }

    LevelGenerator_Room& placed_room = this->level_generator._rooms.back();
    int new_room_list_index = placed_room._generationIndex;

    this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode);
    
    return &this->rooms[new_room_list_index];
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config) {
	if (room_config == nullptr) {
		return nullptr;
	}

	doors = doors & room_config->Doors;

	if (this->CanRoomBePlaced(base_coords, room_config->Shape, doors, true)) {
		return this->PlaceRoom(base_coords, doors, room_config);
	}

	return nullptr;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
    bool canPlace = this->CanRoomBePlaced(base_coords, shape, doors, true);
    
    if (canPlace) {
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
	if (room_config == nullptr) {
		return nullptr;
	}

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

	if (doors == -1) {
		doors = CalculateAutoDoors(this->level_generator._roomMap, coords, ROOMSHAPE_1x1);
	}

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

void DungeonGenerator::InitializeDefaultOffGridRooms() {
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

		if (this->level->IsAltPath()) {
			if ((this->level->_stage == STAGE4_1 && (this->level->GetCurses() & 2) != 0) || this->level->_stage == STAGE4_2) {
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
	else if (this->generation_type == BLUE_WOMB) {
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

void DungeonGenerator::SetGreedGoldRoom(DungeonGeneratorRoom* gold_room) {
	if (gold_room) {
		this->greed_gold_room_index = gold_room->col + gold_room->row * 13;
	}
}

void DungeonGenerator::SetGreedGoldRoom(int grid_index) {
	this->greed_gold_room_index = grid_index;
}

bool DungeonGenerator::ValidateFloor() {
    if (this->level_generator._roomMap[84] == -1) {
        this->TryPlaceDefaultStartingRoom(15);
    }

    this->level_generator.calc_required_doors();

    bool has_final_room = this->final_boss_index >= 0;
    int initial_seed = this->rng->_seed;

    // Index-based iteration allows bounds-checking _generationIndex before array lookup
    for (size_t i = 0; i < this->level_generator._rooms.size(); i++)
    {
        LevelGenerator_Room& room = this->level_generator._rooms[i];
        

        if (room._generationIndex < 0 || room._generationIndex >= 169) {
            return false;
        }

        DungeonGeneratorRoom& generator_room = this->rooms[room._generationIndex];

        RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), room._doors, this->level);

        if (room_config == nullptr) {
            return false;
        }
        
        this->rng->Next();
    }
    
    this->rng->_seed = initial_seed;
    return has_final_room;
}

void DungeonGenerator::CleanFloor() {
	this->level->reset_room_list(this->reset_lil_portal);

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
	this->level_generator._rooms.reserve(169);
}

void DungeonGenerator::Reset() {
	this->CleanFloor();

	this->ResetLevelGenerator();

	this->final_boss_index = -1;
	this->greed_gold_room_index = -1;
	for (size_t i = 0; i < 169; i++)
	{
		this->rooms[i] = DungeonGeneratorRoom();
	}
}

bool DungeonGenerator::PlaceRoomsInFloor() {
    this->level_generator.calc_required_doors();

    for (LevelGenerator_Room& room : this->level_generator._rooms)
    {
        DungeonGeneratorRoom& generator_room = this->rooms[room._generationIndex];
        RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), room._doors, this->level);

        if (room_config == nullptr) {
            return false;
        }

        uint32_t seed = this->rng->Next();

        g_Game->PlaceRoom(&room, room_config, seed, 0);
    }

    for (int i = 0; i < 20; i++)
    {
        int offGridIndex = -(i + 1);
        DungeonGeneratorRoom& generator_room = this->off_grid_rooms[i];
        RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), 0, this->level);
        
        if (room_config == nullptr) {
            continue;
        }

        RoomDescriptor* room_desc = this->level->GetRoomByIdx(offGridIndex, -1);
        if (room_desc != nullptr) {
            room_desc->Data = room_config;
            room_desc->InitSeeds(this->rng);
        }
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

    if (g_Game->IsGreedMode() || this->generation_type == GREED) {
        int gold_idx = (this->greed_gold_room_index >= 0) ? this->greed_gold_room_index : 85;
        g_Game->_greedModeTreasureRoomIdx = gold_idx;
    }

    bool could_place_rooms = PlaceRoomsInFloor();
    if (!could_place_rooms) {
        KAGE::_LogMessage(1, "[WARN] Couldn't place the rooms in the level, clearing placed rooms...\n");
        Reset();
    }
    else {
        if (g_Game->IsGreedMode() || this->generation_type == GREED) {
            g_Game->_greedModeWave = 0;
            uint32_t* wave_seeds = (uint32_t*)((char*)g_Game + 0x18338);
            for (int i = 0; i < 12; i++) {
                uint32_t seed = this->rng->Next();
                if (seed == 0) seed = 1;
                wave_seeds[i] = seed;
            }
        }
        else if (this->level->IsAltPath()) {
            if (((this->level->_stage == STAGE1_1 && (this->level->GetCurses() & 2) != 0) || this->level->_stage == STAGE1_2)) {
                this->level->generate_mirror_world();
            }
            else if (((this->level->_stage == STAGE2_1 && (this->level->GetCurses() & 2) != 0) || this->level->_stage == STAGE2_2)) {
                this->level->generate_mines_dungeon();
            }
        }

        g_Game->_gameStateFlags &= ~(1 << 30); // STATE_DONATION_SLOT_BLOWN
        g_Game->_gameStateFlags &= ~(1 << 31); // STATE_SHOPKEEPER_KILLED

        g_Game->_donationModGreed = 0;
    }

    return could_place_rooms;
}

#pragma endregion

LUALIB_API DungeonGenerator* GetDungeonGenerator(lua_State* L) {
	return *lua::GetRawUserdata<DungeonGenerator**>(L, 1, lua::metatables::DungeonGeneratorMT);
}

LUA_FUNCTION(Lua_PlaceRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	RoomConfig_Room* config = lua::GetLuabridgeUserdata<RoomConfig_Room*>(L, 2, lua::Metatables::CONST_ROOM_CONFIG_ROOM, "RoomConfig");
	
	uint32_t col = 0;
	uint32_t row = 0;
	int allowed_doors = -1;

	int arg_count = lua_gettop(L);
	if (arg_count == 3) {
		uint32_t grid_index = (uint32_t)luaL_checkinteger(L, 3);
		col = grid_index % 13;
		row = grid_index / 13;
	} else if (arg_count == 4) {
		int arg3 = (int)luaL_checkinteger(L, 3);
		int arg4 = (int)luaL_checkinteger(L, 4);
		if (arg3 > 12 || arg4 < 0) {
			col = arg3 % 13;
			row = arg3 / 13;
			allowed_doors = arg4;
		} else {
			col = arg3;
			row = arg4;
		}
	} else if (arg_count >= 5) {
		col = (uint32_t)luaL_checkinteger(L, 3);
		row = (uint32_t)luaL_checkinteger(L, 4);
		allowed_doors = (int)luaL_optinteger(L, 5, -1);
	}

	XY coords(col, row);

	if (allowed_doors == -1) {
		allowed_doors = CalculateAutoDoors(generator->level_generator._roomMap, coords, config->Shape);
	}

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

    if (generator == nullptr) {
        lua_pushnil(L);
        return 1;
    }

    if (generator->level == nullptr) {
        lua_pushnil(L);
        return 1;
    }

    int arg_count = lua_gettop(L);

    uint32_t col = 0;
    uint32_t row = 0;
    
    int stage = generator->level->_stage;

    int type = ROOM_DEFAULT;
    int shape = ROOMSHAPE_1x1;
    int minVariant = 0;
    int maxVariant = -1;
    int minDifficulty = 0;
    int maxDifficulty = 10;
    int subtype = -1;
    int mode = -1;
    int allowed_doors = -1;

    bool has_grid_index = false;
    int table_idx = 0;

    if (arg_count >= 3 && lua_istable(L, 3)) {
        has_grid_index = true;
        table_idx = 3;
    } else if (arg_count >= 4 && lua_istable(L, 4)) {
        table_idx = 4;
    } else if (arg_count == 2) {
        has_grid_index = true;
    } else if (arg_count == 3 && !lua_istable(L, 3)) {
        has_grid_index = true;
    }

    if (has_grid_index) {
        uint32_t grid_index = (uint32_t)luaL_checkinteger(L, 2);
        col = grid_index % 13;
        row = grid_index / 13;
    } else {
        col = (uint32_t)luaL_checkinteger(L, 2);
        row = (uint32_t)luaL_checkinteger(L, 3);
    }

    if (table_idx > 0) {
        lua_getfield(L, table_idx, "Stage");
        if (!lua_isnil(L, -1)) stage = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "Type");
        if (!lua_isnil(L, -1)) type = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "Shape");
        if (!lua_isnil(L, -1)) shape = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "MinVariant");
        if (!lua_isnil(L, -1)) minVariant = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "MaxVariant");
        if (!lua_isnil(L, -1)) maxVariant = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "MinDifficulty");
        if (!lua_isnil(L, -1)) minDifficulty = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "MaxDifficulty");
        if (!lua_isnil(L, -1)) maxDifficulty = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "Subtype");
        if (!lua_isnil(L, -1)) subtype = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "Mode");
        if (!lua_isnil(L, -1)) mode = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, table_idx, "Doors");
        if (!lua_isnil(L, -1)) allowed_doors = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
    } else {
        if (!has_grid_index) {
            if (arg_count >= 4) stage = (int)luaL_checkinteger(L, 4);
            if (arg_count >= 5) type = (int)luaL_checkinteger(L, 5);
            if (arg_count >= 6) shape = (int)luaL_optinteger(L, 6, 1);
            if (arg_count >= 7) minVariant = (int)luaL_optinteger(L, 7, 0);
            if (arg_count >= 8) maxVariant = (int)luaL_optinteger(L, 8, -1);
            if (arg_count >= 9) minDifficulty = (int)luaL_optinteger(L, 9, 0);
            if (arg_count >= 10) maxDifficulty = (int)luaL_optinteger(L, 10, 10);
            if (arg_count >= 11) subtype = (int)luaL_optinteger(L, 11, -1);
            if (arg_count >= 12) mode = (int)luaL_optinteger(L, 12, -1);
            if (arg_count >= 13) allowed_doors = (int)luaL_optinteger(L, 13, -1);
        }
    }

    XY coords(col, row);
    if (allowed_doors == -1) {
        allowed_doors = CalculateAutoDoors(generator->level_generator._roomMap, coords, shape);
    }

    DungeonGeneratorRoom* generator_room = generator->TryPlaceRoom(
        coords, allowed_doors, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode);


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

	int off_grid_index = (int)luaL_checkinteger(L, 2);
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

	int off_grid_index = (int)luaL_checkinteger(L, 2);
	if (off_grid_index < -20 || off_grid_index > -1) {
		return luaL_error(L, "Invalid grid index %d\n", off_grid_index);
	}

	int stage = (int)luaL_checkinteger(L, 3);
	if (stage < 0 || (stage >= STB_UNUSED1 && stage <= STB_ULTRA_GREED) || stage == STB_THE_VOID || stage >= NUM_STB) {
		return luaL_error(L, "Invalid stage %d\n", stage);
	}

	int type = (int)luaL_checkinteger(L, 4);
	if (type < ROOM_DEFAULT || type >= NUM_ROOMTYPES) {
		return luaL_error(L, "Invalid type %d\n", type);
	}

	int shape = (int)luaL_optinteger(L, 5, NUM_ROOMSHAPES);
	if (shape < ROOMSHAPE_1x1 || shape > NUM_ROOMSHAPES) {
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

LUA_FUNCTION(Lua_TryPlaceDefaultStartingRoom) {
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

LUA_FUNCTION(Lua_SetGreedGoldRoom) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	if (lua_isnumber(L, 2)) {
		int grid_index = (int)lua_tointeger(L, 2);
		generator->SetGreedGoldRoom(grid_index);
	}
	else if (lua_isuserdata(L, 2)) {
		DungeonGeneratorRoom* generator_room = *lua::GetRawUserdata<DungeonGeneratorRoom**>(L, 2, lua::metatables::DungeonGeneratorRoomMT);
		generator->SetGreedGoldRoom(generator_room);
	}

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

LUA_FUNCTION(Lua_InitializeDefaultOffGridRooms) {
	DungeonGenerator* generator = GetDungeonGenerator(L);
	generator->InitializeDefaultOffGridRooms();
	
	return 0;
}

static void RegisterDungeonGenerator(lua_State* L) {
	luaL_Reg functions[] = {
		{"PlaceRoom", Lua_PlaceRoom},
		{"PlaceRandomRoom", Lua_PlaceRandomRoom},
		{"PlaceOffGridRoom", Lua_PlaceOffGridRoom},
		{"PlaceRandomOffGridRoom", Lua_PlaceRandomOffGridRoom},
		{"TryPlaceDefaultStartingRoom", Lua_TryPlaceDefaultStartingRoom},
		{"InitializeDefaultOffGridRooms", Lua_InitializeDefaultOffGridRooms},
		{"SetFinalBossRoom", Lua_SetFinalBossRoom},
		{"SetGreedGoldRoom", Lua_SetGreedGoldRoom},
		{"BlockIndex", Lua_BlockIndex},
		{"Validate", Lua_Validate},
		{"Reset", Lua_Reset},
		{NULL, NULL}
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
