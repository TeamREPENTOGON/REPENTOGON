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

	int list_index;
	
	DungeonGeneratorRoom();

	DungeonGeneratorRoom(int list_index, RoomConfig_Room* room, uint32_t row, uint32_t col, int doors);

	RoomConfig_Room* GetRoomConfig(uint32_t seed, int required_doors);
};

struct DungeonGenerator {
	DungeonGeneratorRoom rooms[169];
	RNG* rng;
	Level* level;
	LevelGenerator level_generator;

	int final_boss_index = -1;

	DungeonGenerator(RNG* rng, Level* level);

	bool CanRoomBePlaced(XY& base_coords, int shape, int allowed_doors, bool allow_unconnected);

	void BlockPositionsFromAllowedDoords(XY& base_coords, int shape, int allowed_doors);

	DungeonGeneratorRoom* PlaceRoom(RoomConfig_Room* room_config, uint32_t row, uint32_t col, int doors);

	void SetFinalBossRoom(DungeonGeneratorRoom* boss_room);

	bool ValidateFloor();

	void CleanFloor();

	bool DungeonGenerator::PlaceRoomsInFloor();

	bool Generate();

	void Reset();

	void ResetLevelGenerator();
};