#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../RoomConfigUtility.h"
#include "../LogUtility.h"
#include "LuaCore.h"

#include <string>
#include <vector>
#include <list>
#include <array>
#include <optional>

class VirtualRoomSetManager
{
private:
	struct RoomMapEntry
	{
		RoomConfig_Room* reference = nullptr;
		bool inserted = false;

		inline bool IsRestoredRoom() const { return inserted == false && reference != nullptr; }
		inline bool IsFree() const { return inserted == false && reference == nullptr; }
		inline bool IsOccupied() const { return inserted == true && reference != nullptr; }
		inline bool IsInInvalidState() const { return inserted == true && reference == nullptr; } // FOR ASSERTS!!!
	};

	struct RoomInsertionData
	{
		RoomConfig_Room room;
		RoomMapEntry& mapEntry;
		size_t outputVectorIndex;

		RoomInsertionData(RoomConfig_Room&& roomData, RoomMapEntry& mapEntry, size_t outputVectorIndex)
			: room(std::move(roomData)), mapEntry(mapEntry), outputVectorIndex(outputVectorIndex) {}
	};

	struct RoomSetInsertionData
	{
		const uint32_t stageId = 0;
		const int mode = 0;
		std::vector<RoomInsertionData> roomData;

		RoomSetInsertionData(uint32_t stageId, int mode) : stageId(stageId), mode(mode) {}
	};

	// An auxiliary structure used when Adding rooms to a room set, meant to encapsulate both input and output data.
	struct InsertionData
	{
		std::vector<RoomSetInsertionData> roomSetData;
		std::vector<RoomConfig_Room*> outInsertedRooms; // The output vector so that it is already constructed and filled during insertion, rather then afterwards
	};

	struct SaveData
	{
		std::vector<RoomConfig_Room*> virtualRooms;
	};

	class Stage;

public:
	/**
	 * @class RoomSet
	 * @brief The main container for virtual rooms
	 * 
     * REQUIREMENTS:
	 * - The container must emulate the indexing behavior of a RoomConfig_Room array.
	 * - The container must be capable of dynamically increasing in size, WITHOUT reallocating any of it's elements.
	 * - The container owns all the RoomConfig_Room objects it contains.
	 * 
	 * IMPLEMENTATION DETAILS:
	 * - A list of vectors has been chosen as the primary container to both prevent reallocation, and allow fast iteration and indexing
	 * - Each individual vector has a variable size, usually enough to insert all the rooms passed to the insert function,
	 *   unless the amount is small, in which case we allocate a bigger vector that will then be filled with other rooms on subsequent insertions.
	 *   As such it is suggested insert a batch of as many rooms as possible.
	*/
	class RoomSet
	{
	public:
		class iterator
		{
			std::list<std::vector<RoomConfig_Room>>::iterator m_Outer;
			std::list<std::vector<RoomConfig_Room>>::iterator m_OuterEnd;
			std::vector<RoomConfig_Room>::iterator m_Inner;

		private:
			void try_move_to_next_array();
		public:
			iterator(std::list<std::vector<RoomConfig_Room>>::iterator outer, std::list<std::vector<RoomConfig_Room>>::iterator outerEnd);
			RoomConfig_Room& operator*();
			RoomConfig_Room* operator->();
			iterator& operator++();
			iterator operator++(int);
			bool operator==(const iterator& other) const;
			bool operator!=(const iterator& other) const;
		};
	private:
		const uint32_t m_StageId = STB_SPECIAL_ROOMS;
		const uint32_t m_Mode = 0;
		std::list<std::vector<RoomConfig_Room>> m_Rooms;
		size_t m_Size = 0;

	private:
		size_t __calc_size() const; // FOR ASSERTS!!!
		std::vector<RoomConfig_Room>& __create_new_array(size_t roomsCount);
		/**
		 * @brief Places the room inside of the container.
		 * Might require the allocation of a new vector if there is not enough space.
		 * 
		 * @param remainingRooms: represent the amount of rooms that still need to be allocated in this specific room set.
		 * Used when allocating a new vector, when max capacity is reached.
		 */
		RoomConfig_Room& __emplace_back(RoomConfig_Room&& room, size_t remainingRooms);
		/**
		 * @brief Performs all necessary actions after a room has been inserted.
		 * 
		 * The primary required actions are:
		 * - Registering a Unique Variant in the Game's original room set.
		 * - Setting the room's StageId and Mode to match those of the room set.
		 * 
		 * Any other actions are implementation dependent.
		 */
		void __post_room_insert(RoomConfig_Room& roomRoom, RoomInsertionData& insertionData, bool isRestoringRooms) const;
		/**
		* @brief Emulates the reinsertion of the rooms, by only calling __post_room_insert on all the inserted rooms.
		* This is necessary when the game reloads the room set and we must once again "insert" all of them.
		*/
		void __reprocess_room_insertion();
		void reset();
		void insert_rooms(RoomSetInsertionData& roomSet, InsertionData& insertionData, bool isRestoringRooms);
		void reset_weights();
		const RoomConfig_Room& at_index(size_t index) const;
	public:
		inline iterator begin() { return iterator(m_Rooms.begin(), m_Rooms.end()); }
		inline iterator end() { return iterator(m_Rooms.end(), m_Rooms.end()); }
		inline size_t size() const { return m_Size; }
		// Gets the size of the room set, while also considering the Game's original room set.
		size_t absolute_size() const;
		// Gets the room at the specified index, while also considering the Game's original room set.
		// The Game's rooms are "placed" before the virtual room set.
		const RoomConfig_Room& at_absolute_index(size_t index) const;
		// Gets the room at the specified index, while also considering the Game's original room set.
		// The Game's rooms are "placed" before the virtual room set.
		RoomConfig_Room& at_absolute_index(size_t index);
		const RoomConfig_Room& operator[](size_t index) const;
		RoomConfig_Room& operator[](size_t index);
		size_t GetStageId() const { return m_StageId; }
		size_t GetMode() const { return m_Mode; }

		RoomSet(uint32_t stageId = STB_SPECIAL_ROOMS, uint32_t mode = 0)
			: m_StageId(stageId), m_Mode(mode), m_Size(0) {}
		friend class Stage;
		friend class VirtualRoomSetManager;
	};

private:
	class Stage
	{
	private:
		std::array<RoomSet, 2> m_RoomSets;
		void reset_weights();

	public:
		Stage(uint32_t stageId = STB_SPECIAL_ROOMS)
			: m_RoomSets{RoomSet(stageId, 0), RoomSet(stageId, 1)} {}
		friend class VirtualRoomSetManager;
	};

private:
	std::vector<Stage> m_Stages;
	/**
	 * @brief A map that ties a room's identifier to the first ever room with that id that was inserted with, or to the first restored room with that identifier (if no room has that identifier yet).
	 * When a room is added and the map entry is occupied by a restored room, then the newly added room takes precedence, otherwise the old virtual room remains.
	 * 
	 * This is used on Save and Load to avoid creating duplicates, and to quickly being able to link an encoded GameStateRoomConfig to the virtual room.
	 * If two or more rooms have the same identifier, then only the first one will be saved, and all instances in which the others are used will be replaced with the first one on Load.
	 */
	std::unordered_map<RoomConfigUtility::RoomIdentifier, RoomMapEntry, RoomConfigUtility::RoomIdentifier::Hasher> m_RoomMap;
	/**
	 * @brief A room set not tied to any particular stage, that holds rooms loaded from json on game state restore.
	 * 
	 * A Room should only be inserted into the set if, on restore, the room's map does not already have an entry for this specific identifier.
	 * Rooms from this set are only accessed when reading encoded GameStateRoomConfig with the Restored Room flag, and should remain inaccessible to the public API.
	 */
	RoomSet m_RestoredRooms;
	/**
	 * @brief An auxiliary variable to store all the rooms saved on game state store, before being serialized into json on game state write.
	 */
	std::vector<RoomConfig_Room*> m_SavedRooms;

private:
	void clear();
	void reset();
	void reset_weights(uint32_t stageId, int mode);
	/**
	* @brief Emulates the reinsertion of the rooms.
	* This is necessary when the game reloads the room set and we must once again "insert" all of them.
	*/
	void reprocess_room_insertion(uint32_t stageId, int mode);
	std::vector<std::vector<const RoomConfig_Room*>> __store_room_set_pointers_pre_insertion(std::vector<RoomSetInsertionData>& roomSetInsertionData, bool isRestoringRooms) const; // FOR ASSERTS!!!
	void __assert_valid_state_post_insertion(const InsertionData& insertionData, const std::vector<std::vector<const RoomConfig_Room*>>& roomSetPtrsContainer, bool isRestoringRooms, bool isOptional) const; // FOR ASSERTS!!!
	const RoomSet& get_room_set(uint32_t stageId, int mode, bool isRestoringRooms) const;
	RoomSet& get_room_set(uint32_t stageId, int mode, bool isRestoringRooms);
	void prepare_room_for_insertion(RoomConfig_Room& room, InsertionData& insertionData, std::map<std::pair<uint32_t, int>, uint32_t>& roomSetMap, LogUtility::LogContext& logContext, bool isRestoringRooms);
	InsertionData prepare_insertion_data(std::vector<RoomConfig_Room>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
	InsertionData prepare_insertion_data(std::vector<std::optional<RoomConfig_Room>>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
	std::vector<RoomConfig_Room*> add_rooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
	std::vector<RoomConfig_Room*> add_rooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
	RoomConfig_Room* get_room(uint32_t stageId, int type, uint32_t variant, int mode);
	void get_rooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode);
	bool is_restored_virtual_room(RoomConfig_Room& roomConfig);
	void write_restored_virtual_room(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom);
	RoomConfig_Room* read_restored_virtual_room(GameStateRoomConfig& gameStateRoom);
	uint32_t save_room(RoomConfig_Room* room, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap);
	void try_save_room(GameStateRoomConfig& saveState, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap);
	void save_game_state(uint32_t slot);
	rapidjson::Value serialize_rooms(rapidjson::Document::AllocatorType& allocator);
	rapidjson::Document serialize_save();
	bool write_save(const std::string& fileName, bool isRerun);
	std::vector<std::optional<RoomConfig_Room>> deserialize_rooms(const rapidjson::Value& rooms, LogUtility::LogContext& logContext);
	std::optional<SaveData> deserialize_save(const std::string& filePath, LogUtility::LogContext& logContext);
	bool normalize_game_state_room(GameStateRoomConfig& gameStateRoom, std::vector<RoomConfig_Room*>& restoredRooms);
	bool normalize_game_state(GameState& gameState, std::vector<RoomConfig_Room*>& restoredRooms, LogUtility::LogContext& logContext);
	bool read_save(const std::string& fileName, bool isRerun);
	void delete_save(const std::string& fileName, bool isRerun);
public:
	static std::vector<RoomConfig_Room*> AddRooms(uint32_t stageId, int mode, std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext);
	static std::vector<RoomConfig_Room*> AddRooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext);
	static std::vector<RoomConfig_Room*> AddRooms(uint32_t stageId, int mode, std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext);
	static std::vector<RoomConfig_Room*> AddRooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext);
	static RoomSet& GetRoomSet(uint32_t stageId, int mode);
	static void __Init();
	static void __Reset();
	static void __ResetWeights(uint32_t stage, int mode);
	static void __ReprocessRoomInsertion(uint32_t stageId, int mode);
	static void __SaveGameState(uint32_t slot);
	static bool __WriteSave(const std::string& fileName, bool isRerun);
	static bool __ReadSave(const std::string& fileName, bool isRerun);
	static void __DeleteSave(const std::string& fileName, bool isRerun);
	static RoomConfig_Room* __GetRoom(uint32_t stageId, int type, uint32_t variant, int mode);
	static void __GetRooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode);
	static bool __TryWriteRestoredVirtualRoom(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom);
	static void __AddLuaRooms(lua_State* L, uint32_t stageId, int mode, int tableIndex);
	static RoomConfig_Room* __ReadRestoredVirtualRoom(GameStateRoomConfig& gameStateRoom);

private:
	template <typename RoomsContainer>
	InsertionData __prepare_insertion_data_implementation(RoomsContainer& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
	template <typename RoomsContainer>
	std::vector<RoomConfig_Room*> __add_rooms_implementation(RoomsContainer&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms);
public:
	static VirtualRoomSetManager& Get()
	{
		static VirtualRoomSetManager s_Instance;
		return s_Instance;
	}
private:
	VirtualRoomSetManager() { this->clear(); }
	VirtualRoomSetManager(const VirtualRoomSetManager&) = delete;
};