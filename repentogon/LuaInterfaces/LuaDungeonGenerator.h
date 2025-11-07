#pragma once

struct DungeonGeneratorRoom {
	RoomConfig_Room* room;
	uint32_t row;
	uint32_t col;
	bool is_final_boss = false;
	
	DungeonGeneratorRoom();

	DungeonGeneratorRoom(RoomConfig_Room* room, uint32_t row, uint32_t col);
};

struct DungeonGenerator {
	int num_rooms;
	DungeonGeneratorRoom rooms[169];
	RNG* rng;

	DungeonGenerator(RNG* rng);

	DungeonGeneratorRoom* PlaceRoom(RoomConfig_Room* room_config, uint32_t row, uint32_t col);

	void SetFinalBossRoom(DungeonGeneratorRoom* boss_room);

	bool ValidateFloor();

	void CleanFloor(Level* level);

	void DungeonGenerator::PlaceRoomsInFloor();

	bool Generate(Level* level);
};