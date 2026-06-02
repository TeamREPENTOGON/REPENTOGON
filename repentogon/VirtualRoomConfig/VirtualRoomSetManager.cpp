#include "VirtualRoomSetManager.h"

#include "Log.h"
#include "../RoomConfigUtility.h"
#include "../MiscFunctions.h"
#include "HookSystem.h"
#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson
#include <map>
#include <fstream>
#include <unordered_set>

#undef ERROR

typedef std::optional<std::string> Error;
typedef LogUtility::LogContext LogContext;

#define LOG_INFO_HEADER "[INFO] [VirtualRoomSetManager] - "
#define LOG_WARN_HEADER "[WARN] [VirtualRoomSetManager] -"
#define LOG_ERROR_HEADER "[ERROR] [VirtualRoomSetManager] - "

namespace {
	static constexpr size_t CHUNK_SIZE = 64 * 1024 / sizeof(RoomConfig_Room); // 64 KB
	using RoomChunk = std::array<RoomConfig_Room, CHUNK_SIZE>;
	
	using _VirtualRoomSet = std::vector<RoomConfig_Room*>;
	static constexpr size_t NUM_VANILLA_ROOM_SETS = eStbType::NUM_STB * 2; // one for each mode
	
	struct RestoreRoomsDB
	{
		std::vector<RoomConfig_Room*> restoredRooms; // can be nullptr for failed restored room
		bool initialized = false; // used to encode if this DB has been loaded at least once.

		void reset();
		void init();
	};
}

#pragma region Data

struct Data
{
	static constexpr size_t NUM_SAVE_FILES = 3;
	// it's unlikely that the game will switch between SteamCloud and local saves, but just in case.
	static constexpr size_t TOTAL_SAVE_FILES = NUM_SAVE_FILES * 2; // 2 sets of 3 save slots, one set for cloud and the other for local saves.

	std::vector<std::unique_ptr<RoomChunk>> roomChunks;
	size_t totalRooms = 0;
	/// @brief RestoreRoomsDB maintains the set of rooms that are currently being used by the game when writing and reading the GameState.
	/// It is what makes the conversion DbGameStateRoom -> VirtualGameStateRoom possible.
	std::array<RestoreRoomsDB, TOTAL_SAVE_FILES> saveFileDB;
	std::vector<_VirtualRoomSet> roomSets;
};

static Data s_Data;

#pragma endregion

#pragma region FunctionDefinitions

// ExtraData

/// @brief Gets the encoded `isVirtualRoom` property of a RoomConfig_Room
static bool is_virtual_room(const RoomConfig_Room& roomConfig);
/// @brief Sets the encoded `isVirtualRoom` property of a RoomConfig_Room
static void set_is_virtual_room(RoomConfig_Room& roomConfig);

// GameStateRoom

/** 
 * GameStateRoomConfig is hijacked by this system to properly store newly added virtual rooms.
 * Normally the GameStateRoomConfig would simply store the "vanilla RoomConfig_Room id", but that is
 * not a reliable id for virtual rooms.
 * 
 * Because of this 2 new "structs" have been created:
 * 
 * struct VirtualGameStateRoom
 * {
 *     RoomConfig_Room* room;
 * };
 * 
 * struct DbGameStateRoom
 * {
 *     size_t dbIndex;
 * };
 * 
 * VirtualGameStateRoom is the canonical representation used at runtime.
 * It stores a direct pointer to the associated RoomConfig_Room, which is
 * safe due to the required pointer stability of all RoomConfig_Room instances.
 *
 * DbGameStateRoom is a serialization-only representation. It stores an index
 * into the RestoreRoomsDB and exists solely to reconstruct a
 * VirtualGameStateRoom when loading a save file.
 *
 * Conversion flow:
 * - Runtime:
 * 		VirtualGameStateRoom -> RoomConfig_Room*
 *
 * - Serialization:
 * 		VirtualGameStateRoom -> DbGameStateRoom -> save file
 *
 * - Deserialization:
 * 		save file -> DbGameStateRoom -> VirtualGameStateRoom
 */

/// @brief Gets the encoded `isVirtual` property of a GameStateRoomConfig
static bool GameStateRoom_is_virtual(const GameStateRoomConfig& room);
/// @brief Gets the encoded `isDatabase` property of a GameStateRoomConfig
static bool GameStateRoom_is_database(const GameStateRoomConfig& room);
/// @brief Gets the encoded `room` property of a VirtualGameStateRoom
static RoomConfig_Room* VirtualGameStateRoom_get_room(const GameStateRoomConfig& room);
/// @brief Gets the encoded `dbIndex` property of a DbGameStateRoom
static size_t DbGameStateRoom_get_db_index(const GameStateRoomConfig& room);

/// @brief Hijacks a GameStateRoomConfig and "constructs" a VirtualGameStateRoom.
static void VirtualGameStateRoom_FromRoomConfig(GameStateRoomConfig& room, const RoomConfig_Room& roomConfig);
/// @brief Converts a DbGameStateRoom into a VirtualGameStateRoom.
static bool VirtualGameStateRoom_FromDbGameStateRoom(GameStateRoomConfig& room, const RestoreRoomsDB& db);
/// @brief Hijacks a GameStateRoomConfig and "constructs" a DbGameStateRoom.
static void DbGameStateRoom_constructor(GameStateRoomConfig& room, size_t index);

/// @brief Iterates over all GameStateRoomConfig and returns only those that match the given filter.
template <typename Func>
static std::vector<GameStateRoomConfig*> get_game_state_rooms(GameState& gameState, Func&& filter);
/// @brief Returns all VirtualGameStateRooms in the gameState.
static std::vector<GameStateRoomConfig*> filter_virtual_game_state_rooms(GameState& gameState);
/// @brief Returns all DbGameStateRooms in the gameState.
static std::vector<GameStateRoomConfig*> filter_db_game_state_rooms(GameState& gameState);

// DataManagement

/// @brief Normalizes the given mode.
///
/// Practically just returns the current mode when passing -1.
static int normalize_mode(int mode);
/// @brief Returns whether the given virtual room set id is a vanilla room set extension.
static bool is_vanilla_set(size_t id);
/// @brief Converts a virtual room set id into a StageID, Mode pair.
/// @param id Expected to be a vanilla set.
/// See `is_vanilla_set`()
static std::pair<uint32_t, int> id_to_stage_mode(size_t id);
/// @brief Converts a StageID, Mode pair into it's virtual room set id.
static size_t stage_mode_to_id(uint32_t stageId, int mode);
/// @brief Gets the restore rooms DB for the specified slot.
static RestoreRoomsDB& get_db_for_slot(uint32_t slot, bool remote);

/// @brief Initializes the system's Data.
static void Init();
/// @brief Returns the vanilla room set extension for the given stage.
static _VirtualRoomSet& GetVanillaRoomSet(uint32_t stage, int mode);
/// @brief Resets room weights of all rooms.
static void ResetAllRoomWeights();
/// @brief Resets room weights for the given set.
static void ResetRoomWeights(_VirtualRoomSet& virtualSet);
/// @brief Returns the room matching the "vanilla RoomConfig_Room id".
static RoomConfig_Room* GetRoomById(uint32_t stageId, int type, uint32_t variant, int mode);
/// @brief Adds the rooms that match the given filter to the buffer.
static void FilterRooms(std::vector<RoomConfig_Room*>& buffer, _VirtualRoomSet& virtualSet, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType);

// AddRooms

/**
 * The main requirements for the room holder are:
 * - Guarantee pointer stability for added rooms throughout the entire runtime.
 * - Provide relatively fast iteration over rooms, primarily for filtering operations.
 * - Preserve the relative ordering of elements within a RoomSet, as it affects
 *   the outcome of seeded random room selection.
 */

struct OutAddLuaRooms
{
	size_t placedRooms_begin = 0;
	size_t placedRooms_end = 0;
	std::vector<bool> failedBuilds; // also encodes original table size
};

struct OutAddStbRooms
{
	size_t placedRooms_begin = 0;
	size_t placedRooms_end = 0;
};

/// @brief Loads the vanilla room set, if it has not already been loaded.
static void init_vanilla_room_set(uint32_t stageId, int mode);
/// @brief Commits the placement of room in the vanilla RoomConfig system,
/// as if it were part of the room set.
///
/// Details:
/// - Initializes the StageID and Mode of all placed rooms.
/// - Registers the variant in the VariantSet, and assigns a new variant if it is not unique.
static void commit_vanilla_room_set_insertion(_VirtualRoomSet& virtualSet, size_t begin, size_t end, uint32_t stageId, int mode);

/// @brief Adds the room to the room holder.
///
/// The input room is copied, not moved, into the room holder.
/// @param isRestored unused, but included for potential future use.
static RoomConfig_Room* add_room(const RoomConfig_Room& room, bool isRestored);
/// @brief Parses the LuaRooms in the given table and adds them to the room holder.
static const OutAddLuaRooms add_lua_rooms(_VirtualRoomSet& virtualSet, lua_State* L, int tableIndex);
/// @brief Parses the rooms in the given .stb and adds them to the room holder.
static const OutAddStbRooms add_stb_rooms(_VirtualRoomSet& virtualSet, const std::string& filename, uint32_t stageId);
/// @brief Builds the lua return table for AddLuaRooms
///
/// The return table is index-aligned with the input table.
/// If an input room could not be parsed, the corresponding output entry is nil.
/// @return number of values the function has pushed on the stack.
static int build_add_lua_rooms_out_table(lua_State* L, const _VirtualRoomSet& virtualSet, const OutAddLuaRooms& outLuaRooms);
/// @brief Builds the lua return table for AddStbRooms
/// @return number of values the function has pushed on the stack.
static int build_add_stb_rooms_out_table(lua_State* L, const _VirtualRoomSet& virtualSet, const OutAddStbRooms& outStbRooms);

/// @brief Adds the rooms stored in the passed table to the room holder and
/// ties them to the specified set.
///
/// Places the return table on the Lua stack.
/// @return number of values the function has pushed on the stack.
static int Lua_AddLuaRooms(lua_State* L, size_t id, int tableIndex);
/// @brief Adds the rooms stored in the .stb to the room holder and
/// ties them to the specified set.
///
/// Places the return table on the Lua stack.
/// @return number of values the function has pushed on the stack.
static int Lua_AddStbRooms(lua_State* L, size_t id, const std::string& filename);

// RestoreRoomDB

namespace
{
	// currently unused, meant to detect rooms on ReadDB and
	// avoid wasting memory, however the actual amount of memory
	// this would save is relatively small, due to it being based entirely
	// on the presence of duplicate rooms across save files (db initialization already
	// prevents duplicates on each save).

	// The implementation of content hash and comparison has not been deleted
	// as it might still be useful later down the line.
	class RoomContentHash
	{
		size_t operator()(const RoomConfig_Room* room) const;
	};
	
	class RoomContentEqual
	{
		bool operator()(const RoomConfig_Room* a, const RoomConfig_Room* b) const;
	};
}

/// @brief Returns the path to the DB file, with the given fileName.
static std::string get_db_path(const std::string& fileName);
/// @brief Writes the used rooms to the DB, and converts all VirtualGameStateRoom instances into DbGameStateRoom.
static void InitDB(GameState& gameState, RestoreRoomsDB& db);
/// @brief Converts all DbGameStateRoom instances into VirtualGameStateRoom.
static bool RestoreDB(GameState& gameState, const RestoreRoomsDB& db);
/// @brief Parses the DB file, adds the rooms to the room holder
/// and writes the used rooms to the DB.
///
/// Details:
/// - All parsed rooms are added to the room holder, even if a room with the same id and content already exists;
///   As such this function should be called once for each save slot.
/// - If a room could not be parsed, then the RoomConfig_Room* at that index will be nullptr.
/// @return Successful parse.
static bool ReadDB(const std::string& filePath, uint32_t gameChecksum, RestoreRoomsDB& db);
/// @brief Saves all rooms in the given DB into the specified file.
static bool WriteDB(const std::string& filePath, uint32_t gameChecksum, RestoreRoomsDB& db);

#pragma endregion

#pragma region DataManagement

static int normalize_mode(int mode)
{
	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	assert(0 <= mode && mode <= 1);
	return mode;
}

static bool is_vanilla_set(size_t id)
{
	return id < NUM_VANILLA_ROOM_SETS;
}

static std::pair<uint32_t, int> id_to_stage_mode(size_t id)
{
	assert(is_vanilla_set(id));
	uint32_t stageId = id / 2;
	int mode = id % 2;
	return {stageId, mode};
}

size_t stage_mode_to_id(uint32_t stageId, int mode)
{
	assert(stageId < NUM_STB);
	assert(0 <= mode && mode <= 1);
    return stageId * 2 + mode;
}

RestoreRoomsDB& get_db_for_slot(uint32_t slot, bool remote)
{
	slot = slot - 1; // slot is a 1-based index
    assert(slot < Data::NUM_SAVE_FILES);
	size_t setOffset = Data::NUM_SAVE_FILES * remote;
	return s_Data.saveFileDB[slot + setOffset];
}

void RestoreRoomsDB::init()
{
	this->restoredRooms.clear();
	this->initialized = true;
}

void RestoreRoomsDB::reset()
{
	this->restoredRooms.clear();
	this->initialized = false;
}

static _VirtualRoomSet& GetVanillaRoomSet(uint32_t stage, int mode)
{
	size_t index = stage_mode_to_id(stage, mode);
	return s_Data.roomSets[index];
}

static void FilterRooms(std::vector<RoomConfig_Room*>& buffer, _VirtualRoomSet& virtualSet, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType)
{
	for (auto* room : virtualSet)
	{
		if (RoomConfigUtility::RoomPassesFilter(*room, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType))
		{
			buffer.emplace_back(room);
		}
	}
}

static void Init()
{
	s_Data.roomChunks.clear();
	s_Data.totalRooms = 0;

	s_Data.roomSets.clear();
	s_Data.roomSets.resize(NUM_VANILLA_ROOM_SETS);

	for (RestoreRoomsDB& db : s_Data.saveFileDB)
	{
		db.reset();
	}
}

static void ResetRoomWeights(_VirtualRoomSet& virtualSet)
{
	for (auto* room : virtualSet)
	{
		room->Weight = room->InitialWeight;
	}
}

static void ResetAllRoomWeights()
{
	for (std::unique_ptr<RoomChunk>& chunkPtr: s_Data.roomChunks)
	{
		RoomChunk& chunk = *chunkPtr.get();
		for (auto& room : chunk)
		{
			room.Weight = room.InitialWeight;
		}
	}
}

static RoomConfig_Room* GetRoomById(uint32_t stageId, int type, uint32_t variant, int mode)
{
	mode = normalize_mode(mode);
	_VirtualRoomSet& virtualSet = GetVanillaRoomSet(stageId, mode);
	for (auto* room : virtualSet)
	{
		if (room->Type == type && room->Variant == variant)
		{
			return room;
		}
	}

	return nullptr;
}

#pragma endregion

#pragma region ExtraData

static bool is_virtual_room(const RoomConfig_Room& roomConfig)
{
	uint32_t* flags = (uint32_t*)(&roomConfig.Flags);
	return (*flags & RoomConfig_Room::VIRTUAL_ROOM_FLAG) != 0;
}

static void set_is_virtual_room(RoomConfig_Room& roomConfig)
{
	uint32_t* flags = (uint32_t*)(&roomConfig.Flags);
	*flags |= RoomConfig_Room::VIRTUAL_ROOM_FLAG;
}

#pragma endregion

#pragma region AddRooms

static RoomConfig_Room* add_room(const RoomConfig_Room& room, bool isRestored)
{
	size_t capacity = s_Data.roomChunks.size() * CHUNK_SIZE;
	assert(capacity >= s_Data.totalRooms); // the number of total rooms should never be above the capacity
	if (capacity <= s_Data.totalRooms) // this should only be triggered when capacity == totalRooms
	{
		s_Data.roomChunks.push_back(std::make_unique<RoomChunk>());
	}

	size_t chunkIndex = s_Data.totalRooms / CHUNK_SIZE;
	size_t roomIndex = s_Data.totalRooms % CHUNK_SIZE;

	RoomConfig_Room* storage = &s_Data.roomChunks[chunkIndex]->operator[](roomIndex);
	*storage = room;
	s_Data.totalRooms++;

	set_is_virtual_room(*storage);
	return storage;
}

static void init_vanilla_room_set(uint32_t stageId, int mode)
{
	RoomConfig& roomConfig = *g_Game->GetRoomConfig();
	auto& vanillaRoomSet = roomConfig._stages[stageId]._rooms[mode];
	if (!vanillaRoomSet._loaded)
	{
		roomConfig.LoadStageBinary(stageId, mode);
	}
}

static void commit_vanilla_room_set_insertion(_VirtualRoomSet& virtualSet, size_t begin, size_t end, uint32_t stageId, int mode)
{
	RoomConfig& roomConfig = *g_Game->GetRoomConfig();
	auto& vanillaRoomSet = roomConfig._stages[stageId]._rooms[mode];

	for (size_t i = begin; i < end; i++)
	{
		RoomConfig_Room& room = *virtualSet[i];
		room.StageId = stageId;
		room.Mode = mode;
		auto* variantSet = vanillaRoomSet.GetVariantSet(room.Type);
		room.Variant = variantSet->AddUnique(room.originalVariant);
	}
}

static const OutAddLuaRooms add_lua_rooms(_VirtualRoomSet& virtualSet, lua_State* L, int tableIndex)
{
	OutAddLuaRooms out;
	out.placedRooms_begin = virtualSet.size();
	size_t table_size = (size_t)lua_rawlen(L, tableIndex);

	out.failedBuilds.resize(table_size);
	virtualSet.reserve(virtualSet.size() + table_size);

	LogUtility::LogContext logContext;
	for (size_t lua_it = 1; lua_it <= table_size; lua_it++)
	{
		size_t c_it = lua_it - 1;
		lua_rawgeti(L, tableIndex, lua_it);

		auto room = RoomConfigUtility::BuildRoomFromLua(L, -1, logContext);
		if (!room)
		{
			out.failedBuilds[c_it] = true;
		}
		else
		{
			auto* placedRoom = add_room(room.value(), false);
			virtualSet.emplace_back(placedRoom);
			out.failedBuilds[c_it] = false;
		}
	}

	out.placedRooms_end = virtualSet.size();
	return out;
}

static int build_add_lua_rooms_out_table(lua_State* L, const _VirtualRoomSet& virtualSet, const OutAddLuaRooms& outLuaRooms)
{
	const std::vector<bool>& failedBuilds = outLuaRooms.failedBuilds;

	size_t table_size = failedBuilds.size();
	lua_createtable(L, table_size, 0); // outTable
	int outTable_index = lua_absindex(L, -1);
	size_t set_it = outLuaRooms.placedRooms_begin;

	for (size_t c_it = 0; c_it < table_size; c_it++)
	{
		size_t lua_it = c_it + 1;
		if (failedBuilds[c_it])
		{
			lua_pushnil(L);
		}
		else
		{
			assert(set_it < virtualSet.size());
			RoomConfig_Room* room = virtualSet[set_it];
			set_it++;
			lua::luabridge::UserdataPtr::push(L, room, lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
		}

		lua_rawseti(L, outTable_index, lua_it);
	}
	
	return 1; // outTable
}

static const OutAddStbRooms add_stb_rooms(_VirtualRoomSet& virtualSet, const std::string& filename, uint32_t stageId)
{
	OutAddStbRooms out;

	ZHL::Log(LOG_INFO_HEADER "Adding Rooms from `/content/rooms/%s`\n", filename.c_str());

	// Find the full expanded filepaths for matching files from ALL enabled mods.
	std::vector<std::string> modfilepaths = REPENTOGON::GetAllModContentPaths(filename);
	std::vector<RoomSet> buffer;
	buffer.reserve(modfilepaths.size());
	size_t buffer_totalRooms = 0; // Cached total of all rooms

	// parse all stbs and load them into buffer
	for (const std::string& filepath : modfilepaths)
	{
		KAGE_Filesys_IFile* file = g_FileManager.OpenRead(filepath.c_str());
		if (!file) continue;

		// Read & validate the 4-byte header from the stb like the game expects.
		char header[5];
		file->Read(&header, 1, 4);
		header[4] = '\0';
		if (strcmp(header, "STB1") != 0)
		{
			ZHL::Log(LOG_INFO_HEADER "Invalid room file header '%s' in %s", header, filepath.c_str());
		}
		else
		{
			// Read the number of rooms from the file.
			int numRooms = 0;
			file->Read(&numRooms, 4, 1);

			ZHL::Log(LOG_INFO_HEADER "Reading %d Rooms from '%s'\n", numRooms, filepath.c_str());

			// The game's logic to load a RoomConfig_Room from a .stb file is tied to the RoomSet class.
			// Additionally, we can run into issues if we try to move/destroy a RoomConfig_Room/RoomSpawns allocated by the game.
			// Read all rooms from the file into this RoomSet, then copy the room data into our own vector.
			RoomSet& bufferRoomSet = buffer.emplace_back();
			bufferRoomSet._configs = new RoomConfig_Room[numRooms];
			bufferRoomSet._count = numRooms;
			buffer_totalRooms += numRooms;

			for (int i = 0; i < numRooms; i++)
			{
				RoomConfig::read_room(&bufferRoomSet, i, stageId, file);
			}
		}

		// Close the file.
		file->Free(true);
	}

	// add rooms
	out.placedRooms_begin = virtualSet.size();

	for (size_t i = 0; i < buffer.size(); i++)
	{
		RoomSet& bufferRoomSet = buffer[i];
		size_t numRooms = bufferRoomSet._count;

		for (size_t j = 0; j < numRooms; j++)
		{
			const RoomConfig_Room& room = bufferRoomSet._configs[j];
			if (RoomConfigUtility::IsRoomTypeValid(room.Type))
			{
				auto* placedRoom = add_room(room, false);
				virtualSet.emplace_back(placedRoom);
			}
		}

		// This will properly destroy/free the RoomConfig_Rooms in the RoomSet.
		// (Note: RoomSet's real destructor does not call this function.)
		bufferRoomSet.unload();
	}

	out.placedRooms_end = virtualSet.size();
	return out;
}

static int build_add_stb_rooms_out_table(lua_State* L, const _VirtualRoomSet& virtualSet, const OutAddStbRooms& outStbRooms)
{
	size_t placedRooms_begin = outStbRooms.placedRooms_begin;
	size_t placedRooms_end = outStbRooms.placedRooms_end;

	size_t placedRooms_total = placedRooms_end - placedRooms_begin;
	lua_createtable(L, placedRooms_total, 0); // outTable
	int outTable_index = lua_absindex(L, -1);

	for (size_t i = placedRooms_begin; i < placedRooms_end; i++)
	{
		size_t relativeIt = (i - placedRooms_begin);
		RoomConfig_Room* room = virtualSet[i];
		lua::luabridge::UserdataPtr::push(L, room, lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
		lua_rawseti(L, outTable_index, relativeIt + 1);
	}

	return 1; // outTable
}

int Lua_AddLuaRooms(lua_State* L, size_t id, int tableIndex)
{
	int table_index = lua_absindex(L, tableIndex);
	assert(lua_istable(L, table_index));

	uint32_t stageId;
	int mode;

	bool isVanilla = is_vanilla_set(id);
	if (isVanilla)
	{
		auto& vanillaId = id_to_stage_mode(id);
		stageId = vanillaId.first;
		mode = vanillaId.second;
		init_vanilla_room_set(stageId, mode);
	}

	_VirtualRoomSet& virtualSet = s_Data.roomSets[id];
	const OutAddLuaRooms outLuaRooms = add_lua_rooms(virtualSet, L, table_index);
	size_t placedRooms_begin = outLuaRooms.placedRooms_begin;
	size_t placedRooms_end = outLuaRooms.placedRooms_end;

	if (isVanilla)
	{
		commit_vanilla_room_set_insertion(virtualSet, placedRooms_begin, placedRooms_end, stageId, mode);
	}

	return build_add_lua_rooms_out_table(L, virtualSet, outLuaRooms);
}

int Lua_AddStbRooms(lua_State* L, size_t id, const std::string& filename)
{
	uint32_t stageId;
	int mode;

	bool isVanilla = is_vanilla_set(id);
	if (isVanilla)
	{
		auto& vanillaId = id_to_stage_mode(id);
		stageId = vanillaId.first;
		mode = vanillaId.second;
		init_vanilla_room_set(stageId, mode);
	}

	_VirtualRoomSet& virtualSet = s_Data.roomSets[id];
	const OutAddStbRooms outAddStbRooms = add_stb_rooms(virtualSet, filename, stageId);

	if (isVanilla)
	{
		commit_vanilla_room_set_insertion(virtualSet, outAddStbRooms.placedRooms_begin, outAddStbRooms.placedRooms_end, stageId, mode);
	}

	return build_add_stb_rooms_out_table(L, virtualSet, outAddStbRooms);
}

#pragma endregion

#pragma region GameStateRoom

static_assert(std::extent<decltype(GameState::_rooms)>::value == std::extent<decltype(Game::_gridRooms)>::value + std::extent<decltype(Game::_negativeGridRooms)>::value, "GameState room count does not match Game room count");
static_assert(std::extent<decltype(GameState::_rooms)>::value == std::extent<decltype(GameState::_roomConfigs)>::value, "GameState room count does not match roomConfig count");
static_assert(std::extent<decltype(GameState::_rooms)>::value == MAX_ROOMS, "GameState room count does not match MAX_ROOMS enum value");
static_assert(std::extent<decltype(GameState::_backwardsStages)>::value == NUM_BACKWARDS_STAGES, "GameState backwards stage count does not match NUM_BACKWARDS_STAGES enum value");

template <typename Func>
static std::vector<GameStateRoomConfig*> get_game_state_rooms(GameState& gameState, Func&& filter)
{
	constexpr size_t MAX_BACKWARDS_ROOMS = 4 * NUM_BACKWARDS_STAGES;
	std::vector<GameStateRoomConfig*> gameStateRooms;
	gameStateRooms.reserve(gameState._roomCount + NUM_OFF_GRID_ROOMS + MAX_BACKWARDS_ROOMS);
	
	auto filter_room = [&](GameStateRoomConfig& room)
	{
		if (filter(room))
		{
			gameStateRooms.emplace_back(&room);
		}
	};

	for (size_t i = 0; i < gameState._roomCount; i++)
	{
		filter_room(gameState._roomConfigs[i]);
	}

	for (size_t i = MAX_GRID_ROOMS; i < MAX_ROOMS; i++) // Offgrid rooms
	{
		filter_room(gameState._roomConfigs[i]);
	}

	for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
	{
		auto& backwardsStage = gameState._backwardsStages[i];

		for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
		{
			filter_room(backwardsStage._bossRoomConfigs[j]);
		}

		for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
		{
			filter_room(backwardsStage._treasureRoomConfigs[j]);
		}
	}

	return gameStateRooms;
}

static std::vector<GameStateRoomConfig*> filter_virtual_game_state_rooms(GameState& gameState)
{
	return get_game_state_rooms(gameState, [](const GameStateRoomConfig& room) {
		return GameStateRoom_is_virtual(room);
	});
}

static std::vector<GameStateRoomConfig*> filter_db_game_state_rooms(GameState& gameState)
{
	return get_game_state_rooms(gameState, [](const GameStateRoomConfig& room) {
		return GameStateRoom_is_database(room);
	});
}

static bool GameStateRoom_is_virtual(const GameStateRoomConfig& room)
{
	return (room.GetFlags() & GameStateRoomConfig::VIRTUAL_ROOM_FLAG) != 0;
}

static bool GameStateRoom_is_database(const GameStateRoomConfig& room)
{
	return (room.GetFlags() & GameStateRoomConfig::DB_ROOM_FLAG) != 0;
}

static RoomConfig_Room* VirtualGameStateRoom_get_room(const GameStateRoomConfig& room)
{
	assert(GameStateRoom_is_virtual(room));
	return (RoomConfig_Room*)room._variant;
}

static size_t DbGameStateRoom_get_db_index(const GameStateRoomConfig& room)
{
	assert(GameStateRoom_is_database(room));
	return (size_t)room._variant;
}

constexpr int VR_STAGE_ID = 0x5652; // VR
constexpr int DB_STAGE_ID = 0x4442; // DB

static void VirtualGameStateRoom_FromRoomConfig(GameStateRoomConfig& room, const RoomConfig_Room& roomConfig)
{
	room.SetStageID(VR_STAGE_ID);
	room.SetMode(0);
	room.SetFlags(GameStateRoomConfig::VIRTUAL_ROOM_FLAG);
	room._type = eRoomType::ROOM_ERROR;
	room._variant = (unsigned int)&roomConfig;
}

static bool VirtualGameStateRoom_FromDbGameStateRoom(GameStateRoomConfig& room, const RestoreRoomsDB& db)
{
	assert(GameStateRoom_is_database(room));
	size_t db_index = DbGameStateRoom_get_db_index(room);

	bool success = db.restoredRooms.size() > db_index &&
		db.restoredRooms[db_index] != nullptr;
	
	if (success)
	{
		RoomConfig_Room* restoredRoom = db.restoredRooms[db_index];

		room.SetStageID(VR_STAGE_ID);
		room.SetMode(0);
		room.SetFlags(GameStateRoomConfig::VIRTUAL_ROOM_FLAG);
		room._type = eRoomType::ROOM_ERROR;
		room._variant = (unsigned int)restoredRoom;
	}

	return success;
}

static void DbGameStateRoom_constructor(GameStateRoomConfig& room, size_t index)
{
	assert(GameStateRoom_is_virtual(room)); // only virtual rooms should be converted into db rooms

	room.SetStageID(DB_STAGE_ID);
	room.SetMode(0);
	room.SetFlags(GameStateRoomConfig::DB_ROOM_FLAG);
	room._type = eRoomType::ROOM_ERROR;
	room._variant = (unsigned int)index;
}

#pragma endregion

#pragma region RestoreRoomDB

// the hash is fairly simple as we use it to avoid a great number of comparisons without going fully in depth
size_t RoomContentHash::operator()(const RoomConfig_Room* room) const
{
	size_t h = 17;

	h = h * 31 + std::hash<int>{}(room->StageId);
	h = h * 31 + std::hash<int>{}(room->Mode);
	h = h * 31 + std::hash<int>{}(room->Type);
	h = h * 31 + std::hash<int>{}(room->originalVariant);
	h = h * 31 + std::hash<int>{}(room->Shape);
	h = h * 31 + std::hash<int>{}(room->SpawnCount);

	return h;
}

bool RoomContentEqual::operator()(const RoomConfig_Room* a, const RoomConfig_Room* b) const
{
	// minus spawns
	bool sameCanonicalData = a->StageId == b->StageId &&
		a->Mode == b->Mode &&
		a->Type == b->Type &&
		a->originalVariant == b->originalVariant &&
		a->Subtype == b->Subtype &&
		a->Name == b->Name &&
		a->Difficulty == b->Difficulty &&
		a->InitialWeight == b->InitialWeight &&
		a->Doors == b->Doors &&
		a->Shape == b->Shape &&
		a->Width == b->Width &&
		a->Height == b->Height;

	if (!sameCanonicalData)
	{
		return false;
	}

	// compare spawn entries
	if (a->SpawnCount != b->SpawnCount)
	{
		return false;
	}

	// order is "important" as it affects spawn outcomes
	size_t spawnEntriesCount = a->SpawnCount;
	for (size_t i = 0; i < spawnEntriesCount; i++)
	{
		RoomSpawn& a_spawnEntry = a->Spawns[i];
		RoomSpawn& b_spawnEntry = b->Spawns[i];

		bool sameCoordinates = a_spawnEntry.X == b_spawnEntry.X &&
			a_spawnEntry.Y == b_spawnEntry.Y;

		if (!sameCoordinates)
		{
			return false;
		}

		if (a_spawnEntry.CountEntries != b_spawnEntry.CountEntries)
		{
			return false;
		}

		// compare spawns
		size_t spawnCount = a_spawnEntry.CountEntries;
		for (size_t j = 0; j < spawnCount; j++)
		{
			RoomEntry& a_spawn = a_spawnEntry.Entries[j];
			RoomEntry& b_spawn = b_spawnEntry.Entries[j];

			bool sameSpawn = a_spawn.type == b_spawn.type &&
				a_spawn.variant == b_spawn.variant &&
				a_spawn.subtype == b_spawn.subtype &&
				a_spawn.weight == b_spawn.weight;

			if (!sameSpawn)
			{
				return false;
			}
		}
	}

	return true;
}

static void InitDB(GameState& gameState, RestoreRoomsDB& db)
{
	db.init();

	std::vector<GameStateRoomConfig*> virtualRooms = filter_virtual_game_state_rooms(gameState);
	std::map<RoomConfig_Room*, size_t> processedRooms; 

	for (size_t i = 0; i < virtualRooms.size(); i++)
	{
		GameStateRoomConfig& room = *virtualRooms[i];
		if (GameStateRoom_is_database(room))
		{
			continue; // already processed room (don't know if it can happen but just in case)
		}

		size_t db_currentIndex = db.restoredRooms.size();
		RoomConfig_Room* roomData = VirtualGameStateRoom_get_room(room);
		auto [it, inserted] = processedRooms.try_emplace(roomData, db_currentIndex);

		size_t roomDBIndex;
		if (inserted)
		{
			roomDBIndex = db_currentIndex;
			db.restoredRooms.emplace_back(roomData);
		}
		else
		{
			roomDBIndex = it->second;
		}

		DbGameStateRoom_constructor(room, roomDBIndex);
	}
}

static bool RestoreDB(GameState& gameState, const RestoreRoomsDB& db)
{
	std::vector<GameStateRoomConfig*> dbRooms = filter_db_game_state_rooms(gameState);
	for (size_t i = 0; i < dbRooms.size(); i++)
	{
		GameStateRoomConfig& room = *dbRooms[i];
		if (GameStateRoom_is_virtual(room)) // already processed
		{
			continue;
		}

		bool success = VirtualGameStateRoom_FromDbGameStateRoom(room, db);

		if (!success)
		{
			ZHL::Log(LOG_ERROR_HEADER "Unable to restore DB\n");
			return false;
		}
	}

	return true;
}

constexpr uint32_t DB_VERSION = 1;

static std::string get_db_path(const std::string& fileName)
{
	return REPENTOGON::StringFormat("%s/VirtualRoomSetManager/%s.json", REPENTOGON::GetRepentogonDataPath(), fileName.c_str());
}

static bool WriteDB(const std::string& filePath, uint32_t gameChecksum, RestoreRoomsDB& db)
{
	// open file
	std::filesystem::path path(filePath);
	if (!REPENTOGON::EnsureParentPathExists(path))
	{
		ZHL::Log(LOG_ERROR_HEADER "Unable to create parent path for \"%s\"\n", filePath.c_str());
		return false;
	}

	std::ofstream file(path);
	if (!file)
	{
		ZHL::Log(LOG_ERROR_HEADER "Unable to open file for writing \"%s\"\n", filePath.c_str());
		return false;
	}
	
	// serialize
	rapidjson::Document doc;
	auto& save = doc.SetObject();
	auto& allocator = doc.GetAllocator();

	save.AddMember("Version", DB_VERSION, allocator);
	save.AddMember("Checksum", gameChecksum, allocator);
	save.AddMember("Rooms", rapidjson::Value(rapidjson::kArrayType), allocator);

	// serialize rooms
	rapidjson::Value& rooms = doc["Rooms"];
	for (RoomConfig_Room* room : db.restoredRooms)
	{
		assert(room); // Each saved room should never be nullptr
		rooms.PushBack(RoomConfigUtility::SerializeRoom(*room, allocator), allocator);
	}

	// write to file
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	file << buffer.GetString() << std::endl;

	ZHL::Log(LOG_INFO_HEADER "Successfully saved DB at \"%s\"\n", filePath.c_str());
	return true;
}

static bool ReadDB(const std::string& filePath, uint32_t gameChecksum, RestoreRoomsDB& db)
{
	db.init();

	// open file
	std::filesystem::path path(filePath);
	std::error_code error_code;
	if (!std::filesystem::exists(path, error_code))
	{
		if (error_code)
		{
			ZHL::Log(LOG_ERROR_HEADER "Error while checking if DB exits: %s\n", error_code.message().c_str());
			return false;
		}

		ZHL::Log(LOG_INFO_HEADER "DB not found at \"%s\", skipping read.\n", filePath.c_str());
		return false;
	}

	std::ifstream file(path);
	if (!file)
	{
		ZHL::Log(LOG_ERROR_HEADER "Unable to open file for reading \"%s\"\n", filePath.c_str());
		return false;
	}

	// parse
	rapidjson::Document doc;

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		ZHL::Log(LOG_ERROR_HEADER "Failed to parse json file\n");
		return false;
	}

	LogContext logContext;
	if (!LogUtility::Json::ValidateObject(doc, logContext))
	{
		return false;
	}

	auto& json_version = LogUtility::Json::ReadIntegerMember<uint32_t>(doc, "Version", logContext, false); if (!json_version) { return false; }
	if (json_version.value() != DB_VERSION)
	{
		ZHL::Log(LOG_ERROR_HEADER "version mismatch (current: %u, save: %u)\n", DB_VERSION, json_version.value());
		return false;
	}

	auto& json_checksum = LogUtility::Json::ReadIntegerMember<uint32_t>(doc, "Checksum", logContext, false); if (!json_checksum) { return false; }
	if (json_checksum.value() != gameChecksum)
	{
		ZHL::Log(LOG_ERROR_HEADER "checksum mismatch (current: %u, save: %u)\n", gameChecksum, json_checksum.value());
		return false;
	}

	auto& json_rooms = LogUtility::Json::GetArrayMember(doc, "Rooms", logContext, false); if (!json_rooms) { return false; } // treat this missing as a parse fail

	// deserialize rooms
	const auto& json_roomsArray = json_rooms.value()->GetArray();
	db.restoredRooms.reserve(json_roomsArray.Size());

	for (size_t i = 0; i < json_roomsArray.Size(); i++)
	{
		auto& json_room = json_roomsArray[i];
		std::optional<RoomConfig_Room> deserializedRoom = RoomConfigUtility::DeserializeRoom(json_room, logContext);
		if (!deserializedRoom.has_value())
		{
			ZHL::Log(LOG_ERROR_HEADER "Unable to deserialize json room %u\n", i);
			db.restoredRooms.emplace_back(nullptr);
			continue;
		}

		RoomConfig_Room& room = deserializedRoom.value();
		RoomConfig_Room* placedRoom = add_room(room, true);
		db.restoredRooms.emplace_back(placedRoom);
	}

	ZHL::Log(LOG_INFO_HEADER "Successfully loaded DB \"%s\"\n", filePath.c_str());
	return true;
}

#pragma endregion

#pragma region Interface

VirtualRoomSet VirtualRoomSetManager::GetVanillaSet(uint32_t stageId, int mode)
{
	size_t id = stage_mode_to_id(stageId, mode);
    return VirtualRoomSet(id);
}

RoomConfig_Room* VirtualRoomSet::operator[](size_t index)
{
	const _VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
	return virtualSet[index];
}

size_t VirtualRoomSet::size()
{
	const _VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
    return virtualSet.size();
}

VirtualRoomSet::Iterator VirtualRoomSet::begin()
{
    const _VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
	return virtualSet.cbegin();
}

VirtualRoomSet::Iterator VirtualRoomSet::end()
{
    const _VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
	return virtualSet.cend();
}

void VirtualRoomSet::GetRooms(std::vector<RoomConfig_Room*> buffer, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType)
{
	_VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
	FilterRooms(buffer, virtualSet, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType);
}

void VirtualRoomSet::ResetRoomWeights()
{
	_VirtualRoomSet& virtualSet = s_Data.roomSets[this->m_id];
	::ResetRoomWeights(virtualSet);
}

#pragma endregion

#pragma region detail

int VirtualRoomSetManager::detail::Lua_AddLuaRooms(lua_State *L, VirtualRoomSet &virtualSet, int tableIdx)
{
    return ::Lua_AddLuaRooms(L, virtualSet.m_id, tableIdx);
}

int VirtualRoomSetManager::detail::Lua_AddStbRooms(lua_State *L, VirtualRoomSet &virtualSet, const std::string &fileName)
{
	std::string fullFilename = "rooms/" + fileName;
    return ::Lua_AddStbRooms(L, virtualSet.m_id, fullFilename);
}

void VirtualRoomSetManager::detail::ClearDB(const GameStateSaveInfo& saveInfo)
{
	RestoreRoomsDB& db = get_db_for_slot(saveInfo.saveSlot, saveInfo.isRemote);
	db.init();
}

void VirtualRoomSetManager::detail::PreWriteGameState(GameState &gameState, const GameStateSaveInfo& saveInfo)
{
	RestoreRoomsDB& db = get_db_for_slot(saveInfo.saveSlot, saveInfo.isRemote);
	InitDB(gameState, db);
}

void VirtualRoomSetManager::detail::PostWriteGameState(GameState& gameState, const GameStateSaveInfo& saveInfo)
{
	RestoreRoomsDB& db = get_db_for_slot(saveInfo.saveSlot, saveInfo.isRemote);
	bool success = RestoreDB(gameState, db);
	assert(success); // this should always succeed.
}

bool VirtualRoomSetManager::detail::GameStateNeedsHandling(GameState& gameState)
{
    return filter_db_game_state_rooms(gameState).size() > 0;
}

bool VirtualRoomSetManager::detail::WriteSave(const GameState& gameState, const GameStateSaveInfo& saveInfo, bool isRerun)
{
	// rooms are not saved on Rerun
	if (isRerun)
	{
		return true;
	}

	size_t gameChecksum = gameState._checksum;
	RestoreRoomsDB& db = get_db_for_slot(saveInfo.saveSlot, saveInfo.isRemote);

	std::string filePath = get_db_path(saveInfo.fileName);
	bool successfulWrite = WriteDB(filePath, gameChecksum, db);
	bool failure = !successfulWrite &&
		db.restoredRooms.size() > 0; // at least one hijack

	return !failure;
}

bool VirtualRoomSetManager::detail::ReadSave(GameState& gameState, const GameStateSaveInfo& saveInfo, bool isRerun)
{
	// rooms are not saved on Rerun
	if (isRerun)
	{
		return true;
	}

	RestoreRoomsDB& db = get_db_for_slot(saveInfo.saveSlot, saveInfo.isRemote);
	// read db on first load for this slot
	if (!db.initialized)
	{
		uint32_t gameChecksum = gameState._checksum;
		std::string filePath = get_db_path(saveInfo.fileName);
		bool successfulRead = ReadDB(filePath, gameChecksum, db);
		if (!successfulRead)
		{
			// don't return false, we might still be able to handle the save file.
			ZHL::Log(LOG_WARN_HEADER "Failed DB read, restore might fail.\n");
		}
	}
	
	bool success = RestoreDB(gameState, db);
	if (!success)
	{
		ZHL::Log(LOG_ERROR_HEADER "Failed DB restore for slot %u.\n", saveInfo.saveSlot);
	}
	else
	{
		ZHL::Log(LOG_INFO_HEADER "Successfully restored DB for slot %u.\n", saveInfo.saveSlot);
	}
	return success;
}

void VirtualRoomSetManager::detail::DeleteSave(const GameStateSaveInfo& saveInfo, bool isRerun)
{
	// reset db
	RestoreRoomsDB& db = get_db_for_slot(saveInfo.isRemote, saveInfo.isRemote);
	db.init();

	// rooms are not saved on Rerun
	if (isRerun)
	{
		return;
	}

	// delete file
	std::filesystem::path filePath = get_db_path(saveInfo.fileName);
	std::error_code errorCode;
	if (!std::filesystem::exists(filePath, errorCode))
	{
		return;
	}

	bool deleteStatus = std::filesystem::remove(filePath, errorCode);
	if (errorCode)
	{
		ZHL::Log(LOG_ERROR_HEADER "Unable to delete DB at \"%s\": %s\n", filePath.string().c_str(), errorCode.message().c_str());
	}
	else if (!deleteStatus)
	{
		ZHL::Log(LOG_ERROR_HEADER "Unable to delete DB at \"%s\"\n", filePath.string().c_str());
	}
	else
	{
		ZHL::Log(LOG_INFO_HEADER "Deleted DB at \"%s\"\n", filePath.string().c_str());
	}
}

#pragma endregion

#pragma region Hooks

HOOK_METHOD(ModManager, LoadConfigs, () -> void)
{
	::Init();
	super();
}

HOOK_METHOD(Game, Exit, (bool ShouldSave) -> void)
{
	super(ShouldSave);
	::ResetAllRoomWeights();
}

HOOK_METHOD(RoomConfig, ResetRoomWeights, (uint32_t Stage, int Mode) -> void)
{
	super(Stage, Mode);

	// reset room weights
	Mode = normalize_mode(Mode);
	_VirtualRoomSet& virtualSet = GetVanillaRoomSet(Stage, Mode);
	::ResetRoomWeights(virtualSet);
}

HOOK_METHOD(RoomConfig, LoadStageBinary, (uint32_t Stage, uint32_t Mode) -> void)
{
	super(Stage, Mode);

	// recommit all insertions
	Mode = normalize_mode(Mode);
	_VirtualRoomSet& virtualSet = GetVanillaRoomSet(Stage, Mode);
	commit_vanilla_room_set_insertion(virtualSet, 0, virtualSet.size(), Stage, Mode);
}

HOOK_METHOD(RoomConfig, GetRoomByStageTypeAndVariant, (unsigned int stage, unsigned int type, unsigned int variant, int mode) -> RoomConfig_Room*)
{
	RoomConfig_Room* room = super(stage, type, variant, mode);
	if (!room)
	{
		room = ::GetRoomById(stage, type, variant, mode);
	}

	if (!room)
	{
		KAGE::_LogMessage(0, "[warn] StageID %d Room type %d, variant %d, mode %d not found!\n", stage, type, variant, mode);
	}

	return room;
}

HOOK_METHOD(RoomConfig, GetRooms, (int stage, int type, int shape, int minVariant, int maxVariant, int minDifficulty, int maxDifficulty, unsigned int* doors, unsigned int subtype, int mode) -> RoomConfigRoomPtrVector)
{
	auto rooms = super(stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subtype, mode);
	mode = normalize_mode(mode);
	_VirtualRoomSet& virtualSet = GetVanillaRoomSet(stage, mode);
	FilterRooms(rooms, virtualSet, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, *doors, subtype);

	return rooms;
}

HOOK_STATIC(Level, write_room_config, (RoomConfig_Room* roomConfig, GameStateRoomConfig* gameStateRoom, std_deque_RoomConfigRoom* deque) -> void, __stdcall)
{
	if (is_virtual_room(*roomConfig))
	{
		VirtualGameStateRoom_FromRoomConfig(*gameStateRoom, *roomConfig);
		return;
	}

	super(roomConfig, gameStateRoom, deque);
}

HOOK_STATIC(Level, read_room_config, (GameStateRoomConfig* room, std_deque_RoomConfigRoom* deque) -> RoomConfig_Room*, __stdcall)
{
	assert(!GameStateRoom_is_database(*room));
	if (GameStateRoom_is_virtual(*room))
	{
		RoomConfig_Room* restoredRoom = VirtualGameStateRoom_get_room(*room);
		return restoredRoom;
	}

	return super(room, deque);
}

#pragma endregion