#pragma once

struct DungeonGeneratorRoom {
	RoomConfig_Room* room;
	uint32_t row;
	uint32_t col;
	bool is_final_boss = false;
	
	DungeonGeneratorRoom() {
		this->room = nullptr;
		this->row = -1;
		this->col = -1;
	}

	DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t row, uint32_t col) {
		this->room = room;
		this->row = row;
		this->col = col;
	}
};

struct DungeonGenerator {
	int num_rooms;
	DungeonGeneratorRoom rooms[169];
	RNG* rng;

	DungeonGenerator(RNG* rng) {
		this->num_rooms = 0;
		this->rng = rng;
	}

	DungeonGeneratorRoom* PlaceRoom(RoomConfig_Room* room_config, uint32_t row, uint32_t col) {
		this->rooms[this->num_rooms] = DungeonGeneratorRoom(room_config, row, col);
		this->num_rooms++;
		DungeonGeneratorRoom* generatorRoom = &this->rooms[this->num_rooms - 1];

		return generatorRoom;
	}

	void SetFinalBossRoom(DungeonGeneratorRoom* boss_room) {
		for (size_t i = 0; i < this->num_rooms; i++)
		{
			this->rooms[i].is_final_boss = false;
		}

		boss_room->is_final_boss = true;
	}

	bool ValidateFloor() {
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

	void CleanFloor(Level* level) {
		level->reset_room_list(false);

		for (size_t i = 0; i < 507; i++)
		{
			g_Game->_roomOffset[i] = -1;
		}

		g_Game->_nbRooms = 0;
	}

	void PlaceRoomsInFloor() {
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

	bool Generate(Level* level) {
		if (!this->ValidateFloor()) {
			return false;
		}

		CleanFloor(level);

		PlaceRoomsInFloor();

		return true;
	}
};