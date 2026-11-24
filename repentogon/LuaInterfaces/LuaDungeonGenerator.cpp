#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "Exception.h"
#include "Log.h"
#include "LuaDungeonGenerator.h"
#include "vector"
#include <unordered_set>
#include "../Utils/LevelGenUtils.h"
#include "Room/RoomPlacement.h"
#include "../Patches/Stages/StageManager.h"
static void PrintGeneratorError(const std::string& message) {
    g_Game->GetConsole()->PrintError(message);
    KAGE::LogMessage(3, message.c_str());
}

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

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, uint32_t col, uint32_t row, int doors, RoomConfig_Room* room, bool explicit_doors) {
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
	this->explicit_doors = explicit_doors;
}

DungeonGeneratorRoom::DungeonGeneratorRoom(int list_index, uint32_t col, uint32_t row, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode, bool explicit_doors) {
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
	this->explicit_doors = explicit_doors;
}

RoomConfig_Room* DungeonGeneratorRoom::GetRoomConfig(uint32_t seed, uint32_t required_doors, Level* level) {
    if (this->room != nullptr) {
        return this->room;
    }

    if (this->stage >= 0) {
        return g_Game->GetRoomConfig()->GetRandomRoom(
            seed, true, this->stage, this->type, this->shape,
            this->minVariant, this->maxVariant, this->minDifficulty,
            this->maxDifficulty, &required_doors, this->subtype, this->mode
        );
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

bool DungeonGenerator::CanRoomBePlaced(XY& base_coords, int shape, int allowed_doors, bool allow_unconnected, bool explicit_doors) {
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

	if (!explicit_doors) {
		std::vector<XY> forbbidden_neighbors = GetForbiddenNeighbors(base_coords, shape, allowed_doors);
		for (XY coords : forbbidden_neighbors) {
			int grid_index = coords.ToGridIdx();
			if (this->level_generator._roomMap[grid_index] > -1) {
				return false;
			}
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

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config, bool explicit_doors) {
	if (room_config == nullptr) {
		return nullptr;
	}

	int col = base_coords.x;
	int row = base_coords.y;

	LevelGenerator_Room safe_room;
	safe_room.constructor(col, row, room_config->Shape);
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

	LevelGenerator_Room& placed_room = this->level_generator._rooms.at(this->level_generator._rooms.size() - 1);

	int new_room_list_index = placed_room._generationIndex;

	this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, room_config, explicit_doors);
	DungeonGeneratorRoom* generatorRoom = &this->rooms[new_room_list_index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode, bool explicit_doors) {
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

    this->rooms[new_room_list_index] = DungeonGeneratorRoom(new_room_list_index, col, row, doors, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode, explicit_doors);
    
    return &this->rooms[new_room_list_index];
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config, bool explicit_doors) {
	if (room_config == nullptr) {
		return nullptr;
	}

	doors = doors & room_config->Doors;

	if (this->CanRoomBePlaced(base_coords, room_config->Shape, doors, true, explicit_doors)) {
		return this->PlaceRoom(base_coords, doors, room_config, explicit_doors);
	}

	return nullptr;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode, bool explicit_doors) {
    bool canPlace = this->CanRoomBePlaced(base_coords, shape, doors, true, explicit_doors);
    
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
            mode,
            explicit_doors
        );
    }

    return nullptr;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceOffGridRoom(int off_grid_index, RoomConfig_Room* room_config) {
	if (room_config == nullptr || off_grid_index < -20 || off_grid_index > -1) {
		return nullptr;
	}

	int index = -off_grid_index - 1;
	
	this->off_grid_rooms[index] = DungeonGeneratorRoom(-1, -1, -1, 0, room_config);
	DungeonGeneratorRoom* generatorRoom = &this->off_grid_rooms[index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::PlaceOffGridRoom(int off_grid_index, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode) {
	if (off_grid_index < -20 || off_grid_index > -1) {
		return nullptr;
	}

	int index = -off_grid_index - 1;

	this->off_grid_rooms[index] = DungeonGeneratorRoom(-1, -1, -1, 0, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode);
	DungeonGeneratorRoom* generatorRoom = &this->off_grid_rooms[index];

	return generatorRoom;
}

DungeonGeneratorRoom* DungeonGenerator::TryPlaceDefaultStartingRoom(int doors, bool explicit_doors) {
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

	return this->TryPlaceRoom(coords, doors, config, explicit_doors);
}

void DungeonGenerator::InitializeDefaultOffGridRooms() {
	auto PlaceIfEmpty = [this](int off_grid_index, auto&&... args) {
		int index = -off_grid_index - 1;
		if (index >= 0 && index < 20) {
			if (this->off_grid_rooms[index].room != nullptr || this->off_grid_rooms[index].type != -1) {
				return;
			}
		}
		this->PlaceOffGridRoom(off_grid_index, std::forward<decltype(args)>(args)...);
	};

	PlaceIfEmpty(
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

	PlaceIfEmpty(
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

	PlaceIfEmpty(
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

		PlaceIfEmpty(
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
		PlaceIfEmpty(
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
			PlaceIfEmpty(
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
			PlaceIfEmpty(
				ROOM_BOSSRUSH_IDX,
				g_Game->GetRoomConfig()->GetRoomByStageTypeAndVariant(STB_SPECIAL_ROOMS, ROOM_BOSSRUSH, 0, -1)
			);
		}

		PlaceIfEmpty(
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

		PlaceIfEmpty(
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
				PlaceIfEmpty(
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
		PlaceIfEmpty(
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

		PlaceIfEmpty(
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
		PlaceIfEmpty(
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

void DungeonGenerator::SanitizeRoomDoors() {
    this->level_generator.calc_required_doors();

    for (size_t i = 0; i < this->level_generator._rooms.size(); i++) {
        LevelGenerator_Room& room = this->level_generator._rooms[i];
        if (room._generationIndex < 169) {
            DungeonGeneratorRoom& generator_room = this->rooms[room._generationIndex];
            if (generator_room.explicit_doors) {
                room._doors = generator_room.doors;
            } else {
                XY coords(generator_room.col, generator_room.row);
                room._doors = CalculateAutoDoors(this->level_generator._roomMap, coords, generator_room.shape);
            }
        }
    }

    for (size_t i = 0; i < this->level_generator._rooms.size(); i++) {
        LevelGenerator_Room& room = this->level_generator._rooms[i];
        if (room._generationIndex >= 169) continue;

        DungeonGeneratorRoom& generator_room = this->rooms[room._generationIndex];
        int grid_idx = generator_room.col + generator_room.row * 13;

        for (int slot = 0; slot < NUM_DOOR_SLOTS; slot++) {
            if ((room._doors & (1 << slot)) == 0) continue;

            DoorSourceTarget my_door = GetDoorSourceTarget(grid_idx, generator_room.shape, slot, false);
            if (!my_door.IsValid()) {
                if (generator_room.explicit_doors) {
                    PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room at grid index " + std::to_string(grid_idx) + 
                        " specifies invalid door slot " + std::to_string(slot) + " for room shape " + std::to_string(generator_room.shape));
                }
                room._doors &= ~(1 << slot);
                continue;
            }

            int target_grid_idx = my_door.GetTargetIdx();
            if (target_grid_idx < 0 || target_grid_idx >= 169) {
                if (generator_room.explicit_doors) {
                    PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room at grid index " + std::to_string(grid_idx) + 
                        " specifies a door facing out of bounds, but no room exists there");
                }
                room._doors &= ~(1 << slot);
                continue;
            }

            int neighbor_idx = this->level_generator._roomMap[target_grid_idx];
            if (neighbor_idx < 0 || neighbor_idx >= 169) {
                if (generator_room.explicit_doors) {
                    PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room at grid index " + std::to_string(grid_idx) + 
                        " specifies a door facing grid index " + std::to_string(target_grid_idx) + ", but no room exists there");
                }
                room._doors &= ~(1 << slot);
                continue;
            }

            LevelGenerator_Room* neighbor_room = nullptr;
            for (size_t j = 0; j < this->level_generator._rooms.size(); j++) {
                if (this->level_generator._rooms[j]._generationIndex == (uint32_t)neighbor_idx) {
                    neighbor_room = &this->level_generator._rooms[j];
                    break;
                }
            }

            if (!neighbor_room) {
                if (generator_room.explicit_doors) {
                    PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room at grid index " + std::to_string(grid_idx) + 
                        " specifies a door facing grid index " + std::to_string(target_grid_idx) + ", but no room exists there");
                }
                room._doors &= ~(1 << slot);
                continue;
            }

            DungeonGeneratorRoom& neighbor_gen_room = this->rooms[neighbor_idx];
            int neighbor_grid_idx = neighbor_gen_room.col + neighbor_gen_room.row * 13;

            bool neighbor_has_matching_door = false;

            for (int n_slot = 0; n_slot < NUM_DOOR_SLOTS; n_slot++) {
                if ((neighbor_room->_doors & (1 << n_slot)) == 0) continue;

                DoorSourceTarget neighbor_door = GetDoorSourceTarget(neighbor_grid_idx, neighbor_gen_room.shape, n_slot, false);
                if (my_door.ConnectsTo(neighbor_door)) {
                    neighbor_has_matching_door = true;
                    break;
                }
            }

            if (!neighbor_has_matching_door) {
                if (generator_room.explicit_doors) {
                    PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room at grid index " + std::to_string(grid_idx) + 
                        " specifies a door facing Room at grid index " + std::to_string(neighbor_grid_idx) + 
                        ", but Room at grid index " + std::to_string(neighbor_grid_idx) + " does not allow a connection");
                }
                room._doors &= ~(1 << slot);
            }
        }
    }

    for (size_t i = 0; i < this->level_generator._rooms.size(); i++) {
        LevelGenerator_Room& room = this->level_generator._rooms[i];
        if (room._generationIndex < 169) {
            this->rooms[room._generationIndex].doors = room._doors;
        }
    }
}

bool DungeonGenerator::ValidateFloor() {
    if (this->level_generator._roomMap[84] == -1) {
        this->TryPlaceDefaultStartingRoom(15, false);
    }

    this->SanitizeRoomDoors();

    int initial_seed = this->rng->_seed;

    bool valid = true;

    // Index-based iteration allows bounds-checking _generationIndex before array lookup
    for (size_t i = 0; i < this->level_generator._rooms.size(); i++)
    {
        LevelGenerator_Room& room = this->level_generator._rooms[i];

        if (room._generationIndex < 0 || room._generationIndex >= 169) {
            PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Room generation index " + std::to_string(room._generationIndex) + " is out of bounds");
            valid = false;
            continue;
        }

        DungeonGeneratorRoom& generator_room = this->rooms[room._generationIndex];

        RoomConfig_Room* room_config = generator_room.GetRoomConfig(this->rng->Next(), room._doors, this->level);

        if (room_config == nullptr) {
            int grid_index = generator_room.row * 13 + generator_room.col;
            std::string err = "MC_PRE_GENERATE_DUNGEON: Could not find valid RoomConfig for room at grid index " + 
                std::to_string(grid_index) + " (Stage: " + std::to_string(generator_room.stage) + 
                ", Type: " + std::to_string(generator_room.type) + 
                ", Shape: " + std::to_string(generator_room.shape) + 
                ", Doors: " + std::to_string(room._doors) + ")";
            PrintGeneratorError(err);
            valid = false;
        }
        
        this->rng->Next();
    }
    
    this->rng->_seed = initial_seed;

    if (this->final_boss_index < 0) {
        PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: No final boss room registered");
        valid = false;
    }

    return valid;
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
	for (size_t i = 0; i < 20; i++)
	{
		this->off_grid_rooms[i] = DungeonGeneratorRoom();
	}
}

bool DungeonGenerator::PlaceRoomsInFloor() {
    this->SanitizeRoomDoors();

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

    for (size_t i = 0; i < this->level_generator._rooms.size(); i++)
    {
        LevelGenerator_Room& room = this->level_generator._rooms[i];
        if (i >= g_Game->_nbRooms) break;
        RoomDescriptor* d = &g_Game->_gridRooms[i];
        for (int s = 0; s < NUM_DOOR_SLOTS; s++) {
            if ((room._doors & (1 << s)) != 0) {
                if (d->Data != nullptr) {
                    DoorSourceTarget target = GetDoorSourceTarget(d->GridIndex, d->Data->Shape, s, false);
                    d->Doors[s] = target.IsValid() ? target.GetTargetIdx() : -1;
                }
            } else {
                d->Doors[s] = -1;
            }
        }
        d->AllowedDoors = room._doors;
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

    g_Game->_lastBossRoomListIdx = this->final_boss_index;

    return true;
}

bool DungeonGenerator::Generate() {
    if (!this->ValidateFloor()) {
        PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Failed to validate custom floor, not placing rooms");
        Reset();
        return false;
    }

    CleanFloor();

    if (g_Game->IsGreedMode() || this->generation_type == GREED) {
        int gold_idx = (this->greed_gold_room_index >= 0) ? this->greed_gold_room_index : 85;
        g_Game->_greedModeTreasureRoomIdx = gold_idx;
    }

    this->InitializeDefaultOffGridRooms();

    bool could_place_rooms = PlaceRoomsInFloor();
    if (!could_place_rooms) {
        PrintGeneratorError("MC_PRE_GENERATE_DUNGEON: Couldn't place the rooms in the level, clearing placed rooms...");
        Reset();
    }
    else {
        if (g_Game->IsGreedMode() || this->generation_type == GREED) {
            g_Game->_greedModeWave = 0;
            for (int i = 0; i < 12; i++) {
                uint32_t seed = this->rng->Next();
                if (seed == 0) seed = 1;
                g_Game->_greedWaveSeed[i] = seed;
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

        auto link_dimension_doors = [&](int dimension) {
            std::unordered_set<RoomDescriptor*> seen;
            for (int g = 0; g < 169; g++) {
                RoomDescriptor* d = this->level->GetRoomByIdx(g, dimension);
                if (d != nullptr && d->Data != nullptr && seen.insert(d).second) {
                    for (int s = 0; s < NUM_DOOR_SLOTS; s++) {
                        if ((d->AllowedDoors & (1 << s)) != 0) {
                            DoorSourceTarget target = GetDoorSourceTarget(d->GridIndex, d->Data->Shape, s, false);
                            if (target.IsValid()) {
                                d->Doors[s] = target.GetTargetIdx();
                            } else {
                                d->Doors[s] = -1;
                            }
                        } else {
                            d->Doors[s] = -1;
                        }
                    }
                }
            }
        };

        link_dimension_doors(1);
        link_dimension_doors(2);

        g_Game->_gameStateFlags &= ~(1ULL << STATE_DONATION_SLOT_BLOWN);
        g_Game->_gameStateFlags &= ~(1ULL << STATE_SHOPKEEPER_KILLED);

        g_Game->_donationModGreed = 0;
    }

    return could_place_rooms;
}

#pragma endregion

MOD_EXPORT DungeonGeneratorRoom* L_DungeonGenerator_PlaceRoom(DungeonGenerator* generator, int idx, RoomConfig_Room* room, int allowed_doors) {
	bool explicit_doors = false;

	XY coords = RoomIndexToCoords(idx);
	if (allowed_doors < 0) {
		allowed_doors = CalculateAutoDoors(generator->level_generator._roomMap, coords, room->Shape);
	} else {
		explicit_doors = true;
	}

	return generator->TryPlaceRoom(coords, allowed_doors, room, explicit_doors);
}

MOD_EXPORT DungeonGeneratorRoom* L_DungeonGenerator_PlaceRandomRoom(DungeonGenerator* generator, int idx, int stage, int roomType, int roomShape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int allowed_doors, int subtype, int mode) {
	if (stage == -1) {
		stage = RoomConfig::GetStageID(generator->level->_stage, generator->level->_stageType, -1);
		if (StageManager::GetCurrentOverride().IsOverridden(stage)) {
			stage = StageManager::GetCurrentOverride().GetCustomStageID();
		}
	}

	bool explicit_doors = false;

	XY coords = RoomIndexToCoords(idx);
	if (allowed_doors < 0) {
		allowed_doors = CalculateAutoDoors(generator->level_generator._roomMap, coords, roomShape);
	} else {
		explicit_doors = true;
	}

	return generator->TryPlaceRoom(coords, allowed_doors, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, subtype, mode, explicit_doors);
}

MOD_EXPORT DungeonGeneratorRoom* L_DungeonGenerator_PlaceOffGridRoom(DungeonGenerator* generator, int idx, RoomConfig_Room* room) {
	return generator->PlaceOffGridRoom(idx, room);
}

MOD_EXPORT DungeonGeneratorRoom* L_DungeonGenerator_PlaceRandomOffGridRoom(DungeonGenerator* generator, int idx, int stage, int roomType, int roomShape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subType, int mode) {
	return generator->PlaceOffGridRoom(idx, stage, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, subType, mode);
}

MOD_EXPORT RNG* L_DungeonGenerator_GetRNG(DungeonGenerator* generator) {
	return generator->rng;
}

MOD_EXPORT DungeonGeneratorRoom* L_DungeonGenerator_TryPlaceDefaultStartingRoom(DungeonGenerator* generator, int doors) {
	bool explicit_doors = true;

	if (doors < 0) {
		doors = 15;
		explicit_doors = false;
	}

	return generator->TryPlaceDefaultStartingRoom(doors, explicit_doors);
}

MOD_EXPORT void L_DungeonGenerator_SetFinalBossRoom(DungeonGenerator* generator, DungeonGeneratorRoom* generator_room) {
	generator->SetFinalBossRoom(generator_room);
}

MOD_EXPORT void L_DungeonGenerator_SetGreedGoldRoom(DungeonGenerator* generator, DungeonGeneratorRoom* generator_room) {
	generator->SetGreedGoldRoom(generator_room);
}

MOD_EXPORT void L_DungeonGenerator_BlockIndex(DungeonGenerator* generator, int grid_index) {
	generator->level_generator.BlockPosition(grid_index);
}

MOD_EXPORT bool L_DungeonGenerator_Validate(DungeonGenerator* generator) {
	return generator->ValidateFloor();
}

MOD_EXPORT void L_DungeonGenerator_Reset(DungeonGenerator* generator) {
	generator->Reset();
}

MOD_EXPORT void L_DungeonGenerator_InitializeDefaultOffGridRooms(DungeonGenerator* generator) {
	generator->InitializeDefaultOffGridRooms();
}
