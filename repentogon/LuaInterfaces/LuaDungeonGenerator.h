#pragma once

struct DungeonGeneratorRoom {
	RoomConfig_Room* room;
	uint32_t row;
	uint32_t col;
	uint32_t seed;
	bool is_final_boss = false;
	
	DungeonGeneratorRoom() {
		this->room = NULL;
		this->row = -1;
		this->col = -1;
		this->seed = -1;
	}

	DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t row, uint32_t col, uint32_t seed) {
		this->room = room;
		this->row = row;
		this->col = col;
		this->seed = seed;
	}
};

struct DungeonGenerator {
	int a;
	int num_rooms;
	DungeonGeneratorRoom rooms[169];

	DungeonGenerator() {
		a = 0;
		num_rooms = 0;
	}
};