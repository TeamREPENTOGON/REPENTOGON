#pragma once

enum DungeonGenerationType {
	DEFAULT,
	BLUE_WOMB,
	BACKWARDS,
	HOME,
	RED_REDEMPTION,
	GREED,
};

struct DungeonGeneratorRoom {
	RoomConfig_Room* room;
	uint32_t col;
	uint32_t row;
	int doors;
	int shape;
	bool is_final_boss = false;
	
	DungeonGeneratorRoom();

	DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t row, uint32_t col, int doors);
};

struct DungeonGenerator {
	int num_rooms;
	DungeonGeneratorRoom rooms[169];
	RNG* rng;
	std::bitset<169> occupied_grid_indexes;
	std::bitset<169> forbidden_grid_indexes;

	DungeonGenerator(RNG* rng);

	bool CanRoomBePlaced(uint32_t row, uint32_t col, int shape, int doors);

	void FillOccupiedAndForbiddenIndexes(uint32_t row, uint32_t col, int shape, int doors);

	DungeonGeneratorRoom* PlaceRoom(RoomConfig_Room* room_config, uint32_t row, uint32_t col, int doors);

	void SetFinalBossRoom(DungeonGeneratorRoom* boss_room);

	bool ValidateFloor();

	void CleanFloor(Level* level);

	void DungeonGenerator::PlaceRoomsInFloor();

	bool Generate(Level* level);
};