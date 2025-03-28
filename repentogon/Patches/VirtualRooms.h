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

class VirtualRoomManager
{
private:
	struct RoomMapEntry
	{
		RoomConfig_Room* reference = nullptr;
		bool inserted = false;

		inline bool IsRestoredRoom() const noexcept { return inserted == false && reference != nullptr; }
		inline bool IsFree() const noexcept { return inserted == false && reference == nullptr; }
		inline bool IsOccupied() const noexcept { return inserted == true && reference != nullptr; }
		inline bool IsInInvalidState() const noexcept { return inserted == true && reference == nullptr; } // FOR ASSERTS!!!
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
	class RoomSet
	{
	public:
		class iterator
		{
			std::list<std::vector<RoomConfig_Room>>::iterator m_Outer;
			std::list<std::vector<RoomConfig_Room>>::iterator m_OuterEnd;
			std::vector<RoomConfig_Room>::iterator m_Inner;

		private:
			void try_move_to_next_array() noexcept;
		public:
			iterator(std::list<std::vector<RoomConfig_Room>>::iterator outer, std::list<std::vector<RoomConfig_Room>>::iterator outerEnd) noexcept;
			RoomConfig_Room& operator*() noexcept;
			RoomConfig_Room* operator->() noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;
		};
	private:
		const uint32_t m_StageId = STB_SPECIAL_ROOMS;
		const uint32_t m_Mode = 0;
		std::list<std::vector<RoomConfig_Room>> m_Rooms;
		size_t m_Size = 0;

	private:
		size_t __calc_size() const noexcept; // FOR ASSERTS!!!
		std::vector<RoomConfig_Room>& __create_new_array(size_t roomsCount) noexcept;
		RoomConfig_Room& __emplace_back(RoomConfig_Room&& room, size_t remainingRooms) noexcept;
		void __post_room_insert(RoomConfig_Room& roomRoom, RoomInsertionData& insertionData, bool isRestoringRooms) const noexcept;
		void reset() noexcept;
		void insert_rooms(RoomSetInsertionData& roomSet, InsertionData& insertionData, bool isRestoringRooms) noexcept;
		void reset_weights() noexcept;
		void reprocess_rooms() noexcept;
		const RoomConfig_Room& at_index(size_t index) const noexcept;
	public:
		inline iterator begin() { return iterator(m_Rooms.begin(), m_Rooms.end()); }
		inline iterator end() { return iterator(m_Rooms.end(), m_Rooms.end()); }
		inline size_t size() const noexcept { return m_Size; }
		size_t absolute_size() const noexcept;
		const RoomConfig_Room& at_absolute_index(size_t index) const noexcept;
		RoomConfig_Room& at_absolute_index(size_t index) noexcept;
		const RoomConfig_Room& operator[](size_t index) const noexcept;
		RoomConfig_Room& operator[](size_t index) noexcept;
		size_t GetStageId() const noexcept { return m_StageId; }
		size_t GetMode() const noexcept { return m_Mode; }

		RoomSet(uint32_t stageId = STB_SPECIAL_ROOMS, uint32_t mode = 0)
			: m_StageId(stageId), m_Mode(mode), m_Size(0) {}
		friend class Stage;
		friend class VirtualRoomManager;
	};

private:
	class Stage
	{
	private:
		std::array<RoomSet, 2> m_RoomSets;
		void reset_weights() noexcept;

	public:
		Stage(uint32_t stageId = STB_SPECIAL_ROOMS)
			: m_RoomSets{RoomSet(stageId, 0), RoomSet(stageId, 1)} {}
		friend class VirtualRoomManager;
	};

private:
	std::vector<Stage> m_Stages;
	std::unordered_map<RoomConfigUtility::RoomIdentifier, RoomMapEntry, RoomConfigUtility::RoomIdentifier::Hasher> m_RoomMap;
	RoomSet m_RestoredRooms;
	std::vector<RoomConfig_Room*> m_SavedRooms;

private:
	void clear() noexcept;
	void reset() noexcept;
	void reset_weights(uint32_t stageId, int mode) noexcept;
	void reprocess_rooms(uint32_t stageId, int mode) noexcept;
	std::vector<std::vector<const RoomConfig_Room*>> __store_room_set_pointers_pre_insertion(std::vector<RoomSetInsertionData>& roomSetInsertionData, bool isRestoringRooms) const noexcept; // FOR ASSERTS!!!
	void __assert_valid_state_post_insertion(const InsertionData& insertionData, const std::vector<std::vector<const RoomConfig_Room*>>& roomSetPtrsContainer, bool isRestoringRooms, bool isOptional) const noexcept; // FOR ASSERTS!!!
	const RoomSet& get_room_set(uint32_t stageId, int mode, bool isRestoringRooms) const noexcept;
	RoomSet& get_room_set(uint32_t stageId, int mode, bool isRestoringRooms) noexcept;
	void prepare_room_for_insertion(RoomConfig_Room& room, InsertionData& insertionData, std::map<std::pair<uint32_t, int>, uint32_t>& roomSetMap, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	InsertionData prepare_insertion_data(std::vector<RoomConfig_Room>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	InsertionData prepare_insertion_data(std::vector<std::optional<RoomConfig_Room>>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	std::vector<RoomConfig_Room*> add_rooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	std::vector<RoomConfig_Room*> add_rooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	RoomConfig_Room* get_room(uint32_t stageId, int type, uint32_t variant, int mode) noexcept;
	void get_rooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode) noexcept;
	bool is_restored_virtual_room(RoomConfig_Room& roomConfig) noexcept;
	void write_restored_virtual_room(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom) noexcept;
	RoomConfig_Room* read_restored_virtual_room(GameStateRoomConfig& gameStateRoom) noexcept;
	uint32_t save_room(RoomConfig_Room* room, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap) noexcept;
	void try_save_room(GameStateRoomConfig& saveState, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap) noexcept;
	void save_game_state(uint32_t slot) noexcept;
	rapidjson::Value serialize_rooms(rapidjson::Document::AllocatorType& allocator) noexcept;
	rapidjson::Document seralize_save() noexcept;
	bool write_save(const std::string& fileName, bool isRerun) noexcept;
	std::vector<std::optional<RoomConfig_Room>> deserialize_rooms(const rapidjson::Value& rooms, LogUtility::LogContext& logContext) noexcept;
	std::optional<SaveData> deserialize_save(const std::string& filePath, LogUtility::LogContext& logContext) noexcept;
	bool normalize_game_state_room(GameStateRoomConfig& gameStateRoom, std::vector<RoomConfig_Room*>& restoredRooms) noexcept;
	bool normalize_game_state(GameState& gameState, std::vector<RoomConfig_Room*>& restoredRooms, LogUtility::LogContext& logContext) noexcept;
	bool read_save(const std::string& fileName, bool isRerun) noexcept;
	void delete_save(const std::string& fileName, bool isRerun) noexcept;
public:
	static std::vector<RoomConfig_Room*> AddRooms(uint32_t stageId, int mode, std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static std::vector<RoomConfig_Room*> AddRooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static std::vector<RoomConfig_Room*> AddRooms(uint32_t stageId, int mode, std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static std::vector<RoomConfig_Room*> AddRooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static RoomSet& GetRoomSet(uint32_t stageId, int mode) noexcept;
	static void __Init() noexcept;
	static void __Reset() noexcept;
	static void __ResetWeights(uint32_t stage, int mode) noexcept;
	static void __RefinalizeRooms(uint32_t stageId, int mode) noexcept;
	static void __SaveGameState(uint32_t slot) noexcept;
	static bool __WriteSave(const std::string& fileName, bool isRerun) noexcept;
	static bool __ReadSave(const std::string& fileName, bool isRerun) noexcept;
	static void __DeleteSave(const std::string& fileName, bool isRerun) noexcept;
	static RoomConfig_Room* __GetRoom(uint32_t stageId, int type, uint32_t variant, int mode) noexcept;
	static void __GetRooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode) noexcept;
	static bool __TryWriteRestoredVirtualRoom(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom) noexcept;
	static void __AddLuaRooms(lua_State* L, uint32_t stageId, int mode, int tableIndex) noexcept;
	static RoomConfig_Room* __ReadRestoredVirtualRoom(GameStateRoomConfig& gameStateRoom) noexcept;

private:
	template <typename RoomsContainer>
	InsertionData __prepare_insertion_data_implemenetation(RoomsContainer& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	template <typename RoomsContainer>
	std::vector<RoomConfig_Room*> __add_rooms_implementation(RoomsContainer&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
public:
	static VirtualRoomManager& Get() noexcept
	{
		static VirtualRoomManager s_Instance;
		return s_Instance;
	}
private:
	VirtualRoomManager() noexcept { this->clear(); }
	VirtualRoomManager(const VirtualRoomManager&) = delete;
};