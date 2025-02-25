#include "VirtualRooms.h"
#include "../RoomConfigUtility.h"
#include "map"

#pragma region Asserts and Checks

std::vector<std::vector<const RoomConfig_Room*>> VirtualRoomManager::__store_room_set_pointers_pre_insertion(std::vector<RoomSetInsertionData>& roomSetInsertionData) const noexcept
{
#ifdef NDEBUG
	return {};
#else
	std::vector<std::vector<const RoomConfig_Room*>> roomSetPtrsContainer;
	roomSetPtrsContainer.reserve(roomSetInsertionData.size());

	for (const auto& roomSetData : roomSetInsertionData)
	{
		auto& roomSetPtrs = roomSetPtrsContainer.emplace_back();
		auto& roomSet = m_Stages[roomSetData.stageId].m_RoomSets[roomSetData.mode];

		roomSetPtrs.reserve(roomSet.m_Rooms.size());
		for (auto& roomsArray : roomSet.m_Rooms)
		{
			roomSetPtrs.push_back(roomsArray.data()); //Store the current Pointers for the room sets to make sure there has been no reallocation
		}
	}

	return roomSetPtrsContainer;
#endif // !NDEBUG
}

void VirtualRoomManager::__assert_valid_state_post_insertion(const InsertionData& insertionData, const std::vector<std::vector<const RoomConfig_Room*>>& roomSetPtrsContainer, bool isRestoringRooms) const noexcept
{
#ifndef NDEBUG
	for (size_t i = 0; i < insertionData.roomSetData.size(); i++)
	{
		auto& roomSetData = insertionData.roomSetData[i];
		assert(roomSetData.remainingRooms == 0); // All rooms should have been inserted

		auto& roomSetPtrs = roomSetPtrsContainer[i];
		auto& roomSet = m_Stages[roomSetData.stageId].m_RoomSets[roomSetData.mode];

		auto it = roomSet.m_Rooms.begin();
		for (auto* roomPtr : roomSetPtrs)
		{
			assert(roomPtr == it->data()); // Make sure no reallocation occured
			it++;
		}
	}

	for (size_t i = 0; i < insertionData.roomsData.size(); i++)
	{
		const auto& roomData = insertionData.roomsData[i];

		assert(!roomData.mapEntry.IsInInvalidState());
		assert(roomData.mapEntry.inserted || isRestoringRooms); // Make sure all rooms have their map entry marked as inserted unless we are restoring rooms
		assert(roomData.insertedRoom != nullptr); // Make sure all rooms have a reference to the inserted room
		assert(&roomData.insertedRoom == &insertionData.outInsertedRooms[i]); // The Indexes of the two vectors should be syncronized
	}
#endif // !NDEBUG
}

#pragma endregion


#pragma region Insertion

static constexpr size_t MIN_ARRAY_SIZE = 20; // Set a reasonable min size so that if someone adds small groups we don't have small vectors

std::vector<RoomConfig_Room>& VirtualRoomManager::RoomSet::create_new_array(size_t roomsCount) noexcept
{
	assert(m_Rooms.empty() || m_Rooms.back().size() == m_Rooms.back().capacity()); // There is no reason to create a new vector
	m_Rooms.emplace_back().reserve(std::max(roomsCount, MIN_ARRAY_SIZE));
	return m_Rooms.back();
}

bool VirtualRoomManager::RoomSet::insert_room(RoomInsertionData& roomData, bool isRestoringRooms) noexcept
{
	assert(!roomData.mapEntry.IsInInvalidState());
	assert(roomData.room.StageId == roomData.roomSetData.stageId && roomData.room.Mode == roomData.roomSetData.mode);

	if (isRestoringRooms && !roomData.mapEntry.IsFree())
	{
		roomData.insertedRoom = roomData.mapEntry.reference;
		return false;
	}

	if (roomData.mapEntry.IsRestoredRoom())
	{
		roomData.room.Variant = roomData.mapEntry.reference->Variant;
		*roomData.mapEntry.reference = std::move(roomData.room);
		roomData.mapEntry.inserted = true;

		roomData.insertedRoom = roomData.mapEntry.reference;
		return false;
	}

	if (m_Rooms.empty())
	{
		this->create_new_array(roomData.roomSetData.remainingRooms);
	}

	std::vector<RoomConfig_Room>* array = &m_Rooms.back();
	if (array->size() == array->capacity())
	{
		array = &this->create_new_array(roomData.roomSetData.remainingRooms);
	}

	auto& insertedRoom = array->emplace_back(std::move(roomData.room));
	RoomConfigUtility::FinalizeRoom(insertedRoom, roomData.roomSetData.stageId, roomData.roomSetData.mode);

	if (roomData.mapEntry.IsFree())
	{
		roomData.mapEntry.reference = &insertedRoom;
		roomData.mapEntry.inserted = isRestoringRooms;
	}

	assert(roomData.roomSetData.remainingRooms != 0); // A room was inserted when no rooms were supposed to be inserted
	roomData.roomSetData.remainingRooms--;
	roomData.insertedRoom = &insertedRoom;
	return true;
}

VirtualRoomManager::InsertionData VirtualRoomManager::prepare_insertion_data(std::vector<RoomConfig_Room>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	InsertionData insertionData;
	insertionData.roomsData.reserve(rooms.size());
	insertionData.outInsertedRooms.reserve(rooms.size());
	std::map<std::pair<uint32_t, int>, RoomSetInsertionData*> roomSetMap;

	for (auto& room : rooms)
	{
		std::pair<uint32_t, int> roomSetKey = std::make_pair(room.StageId, room.Mode);
		auto [it, inserted] = roomSetMap.emplace(roomSetKey, nullptr);
		if (inserted)
		{
			insertionData.roomSetData.emplace_back(room.StageId, room.Mode);
			it->second = &insertionData.roomSetData.back();
		}

		assert(it->second != nullptr);
		it->second->remainingRooms++;

		RoomConfigUtility::RoomIdentifier identifier(room);
		auto& mapEntry = m_RoomMap[identifier];
		RoomConfig_Room*& outInsertedRoom = insertionData.outInsertedRooms.emplace_back(nullptr);
		insertionData.roomsData.emplace_back(std::move(room), mapEntry, *it->second, outInsertedRoom);

		if (mapEntry.inserted && !isRestoringRooms)
		{
			logContext.LogMessage(LogUtility::eLogType::WARN, REPENTOGON::StringFormat("A room with stage: %u, mode: %d, type: %d, variant: %u, shape: %d and name: \"%s\" already exists, room cannot be restored if program is closed", identifier.stageId, identifier.mode, identifier.type, identifier.variant, identifier.shape, identifier.name.c_str()).c_str());
		}
	}

	return insertionData;
}

std::vector<RoomConfig_Room*> VirtualRoomManager::add_rooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	auto insertionData = this->prepare_insertion_data(rooms, logContext, isRestoringRooms);

#ifndef NDEBUG
	auto roomSetPtrsContainer = this->__store_room_set_pointers_pre_insertion(insertionData.roomSetData);
#endif // !NDEBUG

	for (auto& roomData : insertionData.roomsData)
	{
		auto& roomSet = this->m_Stages[roomData.roomSetData.stageId].m_RoomSets[roomData.roomSetData.mode];
		roomSet.insert_room(roomData, isRestoringRooms);
	}

#ifndef NDEBUG
	this->__assert_valid_state_post_insertion(insertionData, roomSetPtrsContainer, isRestoringRooms);
#endif // !NDEBUG

	return std::move(insertionData.outInsertedRooms);
}

#pragma endregion

#pragma region Interface

std::vector<RoomConfig_Room*> VirtualRoomManager::AddRooms(uint32_t stageId, int mode, std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept
{
	for (auto& room : rooms)
	{
		room.StageId = stageId;
		room.Mode = mode;
	}

	return VirtualRoomManager::Get().add_rooms(std::move(rooms), logContext, false);
}

std::vector<RoomConfig_Room*> VirtualRoomManager::AddRooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext) noexcept
{
	return VirtualRoomManager::Get().add_rooms(std::move(rooms), logContext, false);
}

#pragma endregion
