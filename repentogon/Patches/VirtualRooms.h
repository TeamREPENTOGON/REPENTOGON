#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../RoomConfigUtility.h"
#include "../LogUtility.h"

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

	struct RoomSetInsertionData
	{
		const uint32_t stageId = 0;
		const int mode = 0;
		size_t remainingRooms = 0;

		RoomSetInsertionData(uint32_t stageId, int mode) : stageId(stageId), mode(mode) {}
	};

	struct RoomInsertionData
	{
		RoomConfig_Room room;
		RoomMapEntry& mapEntry;
		RoomSetInsertionData& roomSetData;
		RoomConfig_Room*& insertedRoom;

		RoomInsertionData(RoomConfig_Room&& roomData, RoomMapEntry& mapEntry, RoomSetInsertionData& roomSetData, RoomConfig_Room*& insertedRoom)
			: room(std::move(roomData)),
			  mapEntry(mapEntry),
			  roomSetData(roomSetData),
			  insertedRoom(insertedRoom)
		{
			insertedRoom = nullptr;
		}
	};

	struct InsertionData
	{
		std::vector<RoomInsertionData> roomsData;
		std::vector<RoomSetInsertionData> roomSetData; // An auxiliary vector to hold the data so that it may not be duplicated in RoomInsertionData
		std::vector<RoomConfig_Room*> outInsertedRooms; // The output vector so that it is already constructed and filled during insertion, rather than afterwards
	};

	class RoomSet
	{
	private:
		std::list<std::vector<RoomConfig_Room>> m_Rooms;

	private:
		std::vector<RoomConfig_Room>& create_new_array(size_t roomsCount) noexcept;
		bool insert_room(RoomInsertionData& roomData, bool isRestoringRooms) noexcept;

		friend class VirtualRoomManager;
	};

	class Stage
	{
	private:
		std::array<RoomSet, 2> m_RoomSets;

		friend class VirtualRoomManager;
	};

private:
	std::vector<Stage> m_Stages;
	std::unordered_map<RoomConfigUtility::RoomIdentifier, RoomMapEntry, RoomConfigUtility::RoomIdentifier::Hasher> m_RoomMap;

private:
	void reset() noexcept;
	std::vector<std::vector<const RoomConfig_Room*>> __store_room_set_pointers_pre_insertion(std::vector<RoomSetInsertionData>& roomSetInsertionData) const noexcept; // FOR ASSERTS!!!
	void __assert_valid_state_post_insertion(const InsertionData& insertionData, const std::vector<std::vector<const RoomConfig_Room*>>& roomSetPtrsContainer, bool isRestoringRooms) const noexcept; // FOR ASSERTS!!!
	InsertionData prepare_insertion_data(std::vector<RoomConfig_Room>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
	std::vector<RoomConfig_Room*> add_rooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept;
public:
	static std::vector<RoomConfig_Room*> AddRooms(uint32_t stageId, int mode, std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static std::vector<RoomConfig_Room*> AddRooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static void __Init() noexcept;
	static std::vector<RoomConfig_Room*> __RestoreRooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept;
	static RoomConfig_Room* __GetRoom(uint32_t stageId, int type, uint32_t variant, int mode) noexcept;
	static void __GetRooms(std::vector<RoomConfig_Room*> rooms) noexcept;

public:
	static VirtualRoomManager& Get() noexcept
	{
		static VirtualRoomManager s_Instance;
		return s_Instance;
	}
private:
	VirtualRoomManager() noexcept = default;
	VirtualRoomManager(const VirtualRoomManager&) = delete;
};