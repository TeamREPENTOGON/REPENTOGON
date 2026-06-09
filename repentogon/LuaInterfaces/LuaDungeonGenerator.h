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

	int stage;
	int type;
	int shape;
	int minVariant;
	int maxVariant;
	int minDifficulty;
	int maxDifficulty;
	int subtype;
	int mode;

	int list_index;
	
	DungeonGeneratorRoom();

	DungeonGeneratorRoom(int list_index, uint32_t row, uint32_t col, int doors, RoomConfig_Room* room);
	DungeonGeneratorRoom(int list_index, uint32_t row, uint32_t col, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode);

	RoomConfig_Room* GetRoomConfig(uint32_t seed, uint32_t required_doors);
};

struct DungeonGenerator {
	DungeonGeneratorRoom rooms[169];
	DungeonGeneratorRoom off_grid_rooms[20];
	RNG* rng;
	Level* level;
	LevelGenerator level_generator;
	DungeonGenerationType generation_type;

	bool reset_lil_portal = false;
	int final_boss_index = -1;

	DungeonGenerator(RNG* rng, Level* level, DungeonGenerationType generation_type, bool reset_lil_portal);

	bool CanRoomBePlaced(XY& base_coords, int shape, int allowed_doors, bool allow_unconnected);

	void BlockPositionsFromAllowedDoords(XY& base_coords, int shape, int allowed_doors);

	DungeonGeneratorRoom* TryPlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config);

	DungeonGeneratorRoom* TryPlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode);

	DungeonGeneratorRoom* PlaceRoom(XY& base_coords, int doors, RoomConfig_Room* room_config);

	DungeonGeneratorRoom* PlaceRoom(XY& base_coords, int doors, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode);

	DungeonGeneratorRoom* PlaceOffGridRoom(int off_grid_index, RoomConfig_Room* room_config);

	DungeonGeneratorRoom* PlaceOffGridRoom(int off_grid_index, int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, int subtype, int mode);

	DungeonGeneratorRoom* TryPlaceDefaultStartingRoom(int doors);

	void InitializeDefaultGridRooms();

	void SetFinalBossRoom(DungeonGeneratorRoom* boss_room);

	bool ValidateFloor();

	void CleanFloor();

	bool PlaceRoomsInFloor();

	bool Generate();

	void Reset();

	void ResetLevelGenerator();
};