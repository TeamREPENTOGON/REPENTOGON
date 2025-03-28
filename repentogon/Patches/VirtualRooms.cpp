#include "VirtualRooms.h"

#include "../RoomConfigUtility.h"
#include "HookSystem.h"
#include "GameStateManagerment.h"
#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson
#include <map>
#include <fstream>

#undef ERROR

typedef std::optional<std::string> Error;
typedef LogUtility::LogContext LogContext;

#pragma region Asserts and Checks

static_assert(std::extent<decltype(GameState::_rooms)>::value == std::extent<decltype(Game::_gridRooms)>::value + std::extent<decltype(Game::_negativeGridRooms)>::value, "GameState room count does not match Game room count");
static_assert(std::extent<decltype(GameState::_rooms)>::value == std::extent<decltype(GameState::_roomConfigs)>::value, "GameState room count does not match roomConfig count");
static_assert(std::extent<decltype(GameState::_rooms)>::value == MAX_ROOMS, "GameState room count does not match MAX_ROOMS enum value");
static_assert(std::extent<decltype(GameState::_backwardsStages)>::value == NUM_BACKWARDS_STAGES, "GameState backwards stage count does not match NUM_BACKWARDS_STAGES enum value");

size_t VirtualRoomManager::RoomSet::__calc_size() const noexcept
{
	size_t size = 0;

	for (auto& roomArray : m_Rooms)
	{
		size += roomArray.size();
	}

	return size;
}

std::vector<std::vector<const RoomConfig_Room*>> VirtualRoomManager::__store_room_set_pointers_pre_insertion(std::vector<RoomSetInsertionData>& roomSetInsertionData, bool isRestoringRooms) const noexcept
{
#ifdef NDEBUG
	return {};
#else
	std::vector<std::vector<const RoomConfig_Room*>> roomSetPtrsContainer;
	roomSetPtrsContainer.reserve(roomSetInsertionData.size());

	for (const auto& roomSetData : roomSetInsertionData)
	{
		auto& roomSetPtrs = roomSetPtrsContainer.emplace_back();
		auto& roomSet = this->get_room_set(roomSetData.stageId, roomSetData.mode, isRestoringRooms);

		roomSetPtrs.reserve(roomSet.m_Rooms.size());
		for (auto& roomsArray : roomSet.m_Rooms)
		{
			roomSetPtrs.push_back(roomsArray.data()); //Store the current Pointers for the room sets to make sure there has been no reallocation
		}
	}

	return roomSetPtrsContainer;
#endif // !NDEBUG
}

void VirtualRoomManager::__assert_valid_state_post_insertion(const InsertionData& insertionData, const std::vector<std::vector<const RoomConfig_Room*>>& roomSetPtrsContainer, bool isRestoringRooms, bool isOptional) const noexcept
{
#ifndef NDEBUG
	size_t roomsToInsert = 0;

	for (size_t i = 0; i < insertionData.roomSetData.size(); i++)
	{
		auto& roomSetData = insertionData.roomSetData[i];

		auto& roomSetPtrs = roomSetPtrsContainer[i];
		auto& roomSet = this->get_room_set(roomSetData.stageId, roomSetData.mode, isRestoringRooms);

		auto it = roomSet.m_Rooms.begin();
		for (auto* roomPtr : roomSetPtrs)
		{
			assert(roomPtr == it->data()); // Make sure no reallocation occurred
			it++;
		}

		assert(roomSet.m_Size == roomSet.__calc_size());

		for (size_t i = 0; i < roomSetData.roomData.size(); i++)
		{
			const auto& roomData = roomSetData.roomData[i];

			assert(!roomData.mapEntry.IsInInvalidState());
			assert(roomData.mapEntry.inserted || isRestoringRooms); // Make sure all rooms have their map entry marked as inserted unless we are restoring rooms
			assert(insertionData.outInsertedRooms[roomData.outputVectorIndex] != nullptr); // Make sure all rooms have a reference to the inserted room
		}

		roomsToInsert += roomSetData.roomData.size();
	}

	if (isOptional && !isRestoringRooms)
	{
		size_t numInsertedRooms = 0;
		for (RoomConfig_Room* room : insertionData.outInsertedRooms)
		{
			numInsertedRooms += room != nullptr;
		}

		assert(numInsertedRooms == roomsToInsert);
	}
#endif // !NDEBUG
}

#pragma endregion

#pragma region RoomSet Iteration

void VirtualRoomManager::RoomSet::iterator::try_move_to_next_array() noexcept
{
	while (m_Outer != m_OuterEnd && m_Inner == m_Outer->end())
	{
		++m_Outer;
		if (m_Outer != m_OuterEnd)
		{
			m_Inner = m_Outer->begin();
		}
	}
}

VirtualRoomManager::RoomSet::iterator::iterator(std::list<std::vector<RoomConfig_Room>>::iterator outer, std::list<std::vector<RoomConfig_Room>>::iterator outerEnd) noexcept
	: m_Outer(outer), m_OuterEnd(outerEnd)
{
	if (m_Outer != m_OuterEnd)
	{
		m_Inner = m_Outer->begin();
	}

	this->try_move_to_next_array();
}

RoomConfig_Room& VirtualRoomManager::RoomSet::iterator::operator*() noexcept
{
	return *m_Inner;
}

RoomConfig_Room* VirtualRoomManager::RoomSet::iterator::operator->() noexcept
{
	return &(*m_Inner);
}

VirtualRoomManager::RoomSet::iterator& VirtualRoomManager::RoomSet::iterator::operator++() noexcept
{
	++m_Inner;
	this->try_move_to_next_array();
	return *this;
}

VirtualRoomManager::RoomSet::iterator VirtualRoomManager::RoomSet::iterator::operator++(int) noexcept
{
	iterator tmp(*this);
	++(*this);
	return tmp;
}

bool VirtualRoomManager::RoomSet::iterator::operator==(const iterator& other) const noexcept
{
	return m_Outer == other.m_Outer && (m_Outer == m_OuterEnd || m_Inner == other.m_Inner);
}

bool VirtualRoomManager::RoomSet::iterator::operator!=(const iterator& other) const noexcept
{
	return !(*this == other);
}

size_t VirtualRoomManager::RoomSet::absolute_size() const noexcept
{
	size_t vanillaSize = g_Game->GetRoomConfig()->_stages[m_StageId]._rooms[m_Mode]._count;
	return vanillaSize + m_Size;
}

const RoomConfig_Room& VirtualRoomManager::RoomSet::at_index(size_t index) const noexcept
{
	assert(index < m_Size);
	assert(this->__calc_size() == m_Size);

	for (const auto& roomArray : m_Rooms)
	{
		if (index < roomArray.size())
		{
			return roomArray[index];
		}
		index -= roomArray.size();
	}

	throw std::runtime_error(__FUNCTION__ ": index out of bounds"); // this is only useful when using a debugger as noexcept forces std::terminate
}

const RoomConfig_Room& VirtualRoomManager::RoomSet::at_absolute_index(size_t index) const noexcept
{
	auto& roomSet = g_Game->GetRoomConfig()->_stages[m_StageId]._rooms[m_Mode];
	if (index < roomSet._count)
	{
		return roomSet._configs[index];
	}

	return this->at_index(index - roomSet._count);
}

RoomConfig_Room& VirtualRoomManager::RoomSet::at_absolute_index(size_t index) noexcept
{
	return const_cast<RoomConfig_Room&>(std::as_const(*this).at_absolute_index(index));
}

const RoomConfig_Room& VirtualRoomManager::RoomSet::operator[](size_t index) const noexcept
{
	return this->at_index(index);
}

RoomConfig_Room& VirtualRoomManager::RoomSet::operator[](size_t index) noexcept
{
	return const_cast<RoomConfig_Room&>(this->at_index(index));
}

#pragma endregion

#pragma region General Data Management

void VirtualRoomManager::RoomSet::reset() noexcept
{
	m_Rooms.clear();
	m_Size = 0;
}

void VirtualRoomManager::clear() noexcept
{
	m_RoomMap.clear();
	m_Stages.clear();
	m_RestoredRooms.reset();
	m_SavedRooms.clear();

	for (size_t i = 0; i < NUM_STB; i++)
	{
		m_Stages.emplace_back(i);
	}
}

void VirtualRoomManager::reset() noexcept
{
	for (auto& stage : m_Stages)
	{
		stage.reset_weights();
	}
}

void VirtualRoomManager::reset_weights(uint32_t stageId, int mode) noexcept
{
	this->get_room_set(stageId, mode, false).reset_weights();
}

void VirtualRoomManager::Stage::reset_weights() noexcept
{
	for (auto& roomSet : m_RoomSets)
	{
		roomSet.reset_weights();
	}
}

void VirtualRoomManager::RoomSet::reset_weights() noexcept
{
	for (auto& room : *this)
	{
		room.Weight = room.InitialWeight;
	}
}

void VirtualRoomManager::reprocess_rooms(uint32_t stageId, int mode) noexcept
{
	this->get_room_set(stageId, mode, false).reprocess_rooms();
}

void VirtualRoomManager::RoomSet::reprocess_rooms() noexcept
{
	for (auto& room : *this)
	{
		RoomConfigUtility::PostRoomInsert(room, room.StageId, room.Mode);
	}
}

void VirtualRoomManager::__Init() noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.clear();
}

void VirtualRoomManager::__Reset() noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.reset();
}

void VirtualRoomManager::__ResetWeights(uint32_t stageId, int mode) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.reset_weights(stageId, mode);
}

void VirtualRoomManager::__RefinalizeRooms(uint32_t stageId, int mode) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.reprocess_rooms(stageId, mode);
}

#pragma endregion

#pragma region Room Getters

RoomConfig_Room* VirtualRoomManager::get_room(uint32_t stageId, int type, uint32_t variant, int mode) noexcept
{
	auto& roomSet = this->get_room_set(stageId, mode, false);
	for (auto& room : roomSet)
	{
		if (room.Type == type && room.Variant == variant)
		{
			return &room;
		}
	}

	return nullptr;
}

void VirtualRoomManager::get_rooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode) noexcept
{
	auto& roomSet = this->get_room_set(stageId, mode, false);
	for (auto& room : roomSet)
	{
		if (RoomConfigUtility::RoomPassesFilter(room, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType))
		{
			result.emplace_back(&room);
		}
	}
}

RoomConfig_Room* VirtualRoomManager::__GetRoom(uint32_t stageId, int type, uint32_t variant, int mode) noexcept
{
	return Get().get_room(stageId, type, variant, mode);
}

void VirtualRoomManager::__GetRooms(std::vector<RoomConfig_Room*>& result, uint32_t stageId, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType, int mode) noexcept
{
	return Get().get_rooms(result, stageId, roomType, roomShape, minVariant, maxVariant, minDifficulty, maxDifficulty, doors, subType, mode);
}

#pragma endregion

#pragma region Room Insertion

static constexpr size_t MIN_ARRAY_SIZE = 20; // Set a reasonable min size so that if someone adds small groups we don't have small vectors

std::vector<RoomConfig_Room>& VirtualRoomManager::RoomSet::__create_new_array(size_t roomsCount) noexcept
{
	assert(m_Rooms.empty() || m_Rooms.back().size() == m_Rooms.back().capacity()); // There is no reason to create a new vector
	m_Rooms.emplace_back().reserve(std::max(roomsCount, MIN_ARRAY_SIZE));
	return m_Rooms.back();
}

RoomConfig_Room& VirtualRoomManager::RoomSet::__emplace_back(RoomConfig_Room&& room, size_t remainingRooms) noexcept
{
	if (m_Rooms.empty())
	{
		this->__create_new_array(remainingRooms);
	}

	std::vector<RoomConfig_Room>* array = &m_Rooms.back();
	if (array->size() == array->capacity())
	{
		array = &this->__create_new_array(remainingRooms);
	}

	auto& insertedRoom = array->emplace_back(std::move(room));
	m_Size++;
	assert(m_Size == this->__calc_size());

	return insertedRoom;
}

void VirtualRoomManager::RoomSet::__post_room_insert(RoomConfig_Room& insertedRoom, RoomInsertionData& roomData, bool isRestoringRooms) const noexcept
{
	if (isRestoringRooms)
	{
		insertedRoom.Variant = m_Size - 1;
	}
	else
	{
		RoomConfigUtility::PostRoomInsert(insertedRoom, insertedRoom.StageId, insertedRoom.Mode);
	}

	if (roomData.mapEntry.IsFree() || (roomData.mapEntry.IsRestoredRoom() && !isRestoringRooms))
	{
		roomData.mapEntry.reference = &insertedRoom;
		roomData.mapEntry.inserted = !isRestoringRooms;
	}
}

void VirtualRoomManager::RoomSet::insert_rooms(RoomSetInsertionData& roomSet, InsertionData& insertionData, bool isRestoringRooms) noexcept
{
	for (size_t i = 0; i < roomSet.roomData.size(); i++)
	{
		auto& roomData = roomSet.roomData[i];

		assert(!roomData.mapEntry.IsInInvalidState());
		assert(this == &VirtualRoomManager::Get().get_room_set(roomData.room.StageId, roomData.room.Mode, isRestoringRooms)); // the room set this room is being inserted into is the correct one
		assert((isRestoringRooms && roomData.mapEntry.IsFree()) || !isRestoringRooms);

		size_t remainingRooms = roomSet.roomData.size() - i;
		auto& insertedRoom = this->__emplace_back(std::move(roomData.room), remainingRooms);
		this->__post_room_insert(insertedRoom, roomData, isRestoringRooms);

		assert(m_Size == this->__calc_size());
		insertionData.outInsertedRooms[roomData.outputVectorIndex] = &insertedRoom;
	}
}

const VirtualRoomManager::RoomSet& VirtualRoomManager::get_room_set(uint32_t stageId, int mode, bool isRestoringRooms) const noexcept
{
	if (isRestoringRooms)
	{
		return this->m_RestoredRooms;
	}

	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	assert(stageId < this->m_Stages.size());
	assert(0 <= mode && mode < std::tuple_size<decltype(Stage::m_RoomSets)>::value);

	return this->m_Stages[stageId].m_RoomSets[mode];
}

VirtualRoomManager::RoomSet& VirtualRoomManager::get_room_set(uint32_t stageId, int mode, bool isRestoringRooms) noexcept
{
	return const_cast<RoomSet&>(std::as_const(*this).get_room_set(stageId, mode, isRestoringRooms));
}

void VirtualRoomManager::prepare_room_for_insertion(RoomConfig_Room& room, InsertionData& insertionData, std::map<std::pair<uint32_t, int>, uint32_t>& roomSetMap, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	RoomConfigUtility::RoomIdentifier identifier(room);
	auto& mapEntry = m_RoomMap[identifier];

	if (isRestoringRooms && !mapEntry.IsFree())
	{
		insertionData.outInsertedRooms.emplace_back(mapEntry.reference);
		return;
	}

	std::pair<uint32_t, int> roomSetKey = isRestoringRooms ? std::make_pair(-1, -1) : std::make_pair(room.StageId, room.Mode);
	auto [it, inserted] = roomSetMap.emplace(roomSetKey, -1);
	if (inserted)
	{
		insertionData.roomSetData.emplace_back(room.StageId, room.Mode);
		it->second = insertionData.roomSetData.size() - 1;
	}

	uint32_t roomSetDataIndex = it->second;
	assert(roomSetDataIndex != -1);
	RoomSetInsertionData& roomSetData = insertionData.roomSetData[roomSetDataIndex];
	RoomConfig_Room*& outInsertedRoom = insertionData.outInsertedRooms.emplace_back(nullptr);

	assert(!(mapEntry.reference != nullptr && isRestoringRooms)); // We should not place a room when restoring, if the map entry has a reference
	insertionData.roomSetData[roomSetDataIndex].roomData.emplace_back(std::move(room), mapEntry, insertionData.outInsertedRooms.size() - 1);

	if (mapEntry.inserted && !isRestoringRooms)
	{
		logContext.LogMessage(LogUtility::eLogType::WARN, REPENTOGON::StringFormat("A room with stage: %u, mode: %d, type: %d, variant: %u, shape: %d and name: \"%s\" already exists, room cannot be restored on continue", identifier.stageId, identifier.mode, identifier.type, identifier.variant, identifier.shape, identifier.name.c_str()).c_str());
	}
}

template<typename RoomsContainer>
VirtualRoomManager::InsertionData VirtualRoomManager::__prepare_insertion_data_implemenetation(RoomsContainer& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	static_assert(std::is_same_v<std::decay_t<RoomsContainer>, std::vector<RoomConfig_Room>> || std::is_same_v<std::decay_t<RoomsContainer>, std::vector<std::optional<RoomConfig_Room>>>, "Invalid RoomContainer for add_rooms implementation");
	constexpr bool isOptional = std::is_same_v<std::decay_t<RoomsContainer>, std::vector<std::optional<RoomConfig_Room>>>;

	InsertionData insertionData;
	insertionData.outInsertedRooms.reserve(rooms.size());
	std::map<std::pair<uint32_t, int>, uint32_t> roomSetMap;

	for (auto& room : rooms)
	{
		if constexpr (isOptional)
		{
			if (!room)
			{
				insertionData.outInsertedRooms.emplace_back(nullptr);
				continue;
			}

			this->prepare_room_for_insertion(std::move(room.value()), insertionData, roomSetMap, logContext, isRestoringRooms);
		}
		else
		{
			this->prepare_room_for_insertion(std::move(room), insertionData, roomSetMap, logContext, isRestoringRooms);
		}
	}

	return insertionData;
}

template<typename RoomsContainer>
inline std::vector<RoomConfig_Room*> VirtualRoomManager::__add_rooms_implementation(RoomsContainer&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	static_assert(std::is_same_v<std::decay_t<RoomsContainer>, std::vector<RoomConfig_Room>> || std::is_same_v<std::decay_t<RoomsContainer>, std::vector<std::optional<RoomConfig_Room>>>, "Invalid RoomContainer for add_rooms implementation");
	constexpr bool isOptional = std::is_same_v<std::decay_t<RoomsContainer>, std::vector<std::optional<RoomConfig_Room>>>;

	auto insertionData = this->prepare_insertion_data(rooms, logContext, isRestoringRooms);

#ifndef NDEBUG
	auto roomSetPtrsContainer = this->__store_room_set_pointers_pre_insertion(insertionData.roomSetData, isRestoringRooms);
#endif // !NDEBUG

	for (auto& roomSetData : insertionData.roomSetData)
	{
		auto& roomSet = this->get_room_set(roomSetData.stageId, roomSetData.mode, isRestoringRooms);
		roomSet.insert_rooms(roomSetData, insertionData, isRestoringRooms);
	}

#ifndef NDEBUG
	this->__assert_valid_state_post_insertion(insertionData, roomSetPtrsContainer, isRestoringRooms, isOptional);
#endif // !NDEBUG

	return std::move(insertionData.outInsertedRooms);
}

VirtualRoomManager::InsertionData VirtualRoomManager::prepare_insertion_data(std::vector<RoomConfig_Room>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	return this->__prepare_insertion_data_implemenetation<std::vector<RoomConfig_Room>>(rooms, logContext, isRestoringRooms);
}

VirtualRoomManager::InsertionData VirtualRoomManager::prepare_insertion_data(std::vector<std::optional<RoomConfig_Room>>& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	return this->__prepare_insertion_data_implemenetation<std::vector<std::optional<RoomConfig_Room>>>(rooms, logContext, isRestoringRooms);
}

std::vector<RoomConfig_Room*> VirtualRoomManager::add_rooms(std::vector<RoomConfig_Room>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	return this->__add_rooms_implementation<std::vector<RoomConfig_Room>>(std::move(rooms), logContext, isRestoringRooms);
}

std::vector<RoomConfig_Room*> VirtualRoomManager::add_rooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext, bool isRestoringRooms) noexcept
{
	return this->__add_rooms_implementation<std::vector<std::optional<RoomConfig_Room>>>(std::move(rooms), logContext, isRestoringRooms);
}

#pragma endregion

#pragma region Save Data

constexpr uint32_t GAMESTATE_VERSION = 1;

static std::string get_save_data_path(const std::string& fileName) noexcept
{
	std::string rawString = REPENTOGON::StringFormat("%s/VirtualRoomManager/%s.json", REPENTOGON::GetRepentogonDataPath(), fileName.c_str());
	std::error_code errorCode;
	auto path = std::filesystem::absolute(rawString, errorCode);
	if (errorCode)
	{
		return rawString;
	}

	return path.string();
}

#pragma region GameStateRoomConfig Management

static std::vector<GameStateRoomConfig*> get_game_state_rooms(GameState& gameState) noexcept
{
	constexpr size_t MAX_BACKWARDS_ROOMS = 4 * NUM_BACKWARDS_STAGES;
	std::vector<GameStateRoomConfig*> gameStateRooms;
	gameStateRooms.reserve(gameState._roomCount + NUM_OFF_GRID_ROOMS + MAX_BACKWARDS_ROOMS);

	for (size_t i = 0; i < gameState._roomCount; i++)
	{
		gameStateRooms.emplace_back(&gameState._roomConfigs[i]);
	}

	for (size_t i = MAX_GRID_ROOMS; i < MAX_ROOMS; i++) // Offgrid rooms
	{
		gameStateRooms.emplace_back(&gameState._roomConfigs[i]);
	}

	for (size_t i = 0; i < NUM_BACKWARDS_STAGES; i++)
	{
		auto& backwardsStage = gameState._backwardsStages[i];

		for (size_t j = 0; j < backwardsStage._bossRoomsCount; j++)
		{
			gameStateRooms.emplace_back(&backwardsStage._bossRoomConfigs[i]);
		}

		for (size_t j = 0; j < backwardsStage._treasureRoomsCount; j++)
		{
			gameStateRooms.emplace_back(&backwardsStage._treasureRoomConfigs[i]);
		}
	}

	return gameStateRooms;
}

static RoomConfig_Room* get_room_from_set(VirtualRoomManager::RoomSet& roomSet, size_t index) noexcept
{
	return roomSet.size() <= index ? nullptr : &roomSet[index];
}

static void assert_valid_restored_room(RoomConfig_Room* restoredRoom, GameStateRoomConfig& gameStateRoom)
{
#ifndef NDEBUG
	uint32_t flags = gameStateRoom.GetFlags();
	if ((flags & GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG) != 0)
	{
		return; // Rooms belonging to the RestoredSet are restored on Level::read_room_config
	}

	auto* room = g_Game->GetRoomConfig()->GetRoomByStageTypeAndVariant(gameStateRoom.GetStageID(), gameStateRoom._type, gameStateRoom._variant, gameStateRoom.GetMode());
	assert(restoredRoom == room); // the room identifier is correct and exists
#endif
}

bool VirtualRoomManager::is_restored_virtual_room(RoomConfig_Room& roomConfig) noexcept
{
	auto& roomSet = this->get_room_set(0, 0, true);
	return &roomConfig == get_room_from_set(roomSet, roomConfig.Variant);
}

bool VirtualRoomManager::normalize_game_state_room(GameStateRoomConfig& gameStateRoom, std::vector<RoomConfig_Room*>& restoredRooms) noexcept
{
	uint32_t flags = gameStateRoom.GetFlags();
	if ((flags & GameStateRoomConfig::VIRTUAL_ROOM_FLAG) == 0)
	{
		return true; // Already in it's normal form (at least when concerning virtual rooms)
	}

	size_t index = gameStateRoom._variant;
	if (index >= restoredRooms.size())
	{
		return false;
	}

	auto* restoredRoom = restoredRooms[index];
	if (!restoredRoom)
	{
		return false;
	}

	flags &= ~GameStateRoomConfig::VIRTUAL_ROOM_FLAG;
	if (this->is_restored_virtual_room(*restoredRoom))
	{
		flags |= GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG;
	}
	gameStateRoom.SetFlags(flags);

	gameStateRoom.SetStageID(restoredRoom->StageId);
	gameStateRoom.SetMode(restoredRoom->Mode);
	gameStateRoom._variant = restoredRoom->Variant;
	gameStateRoom._type = restoredRoom->Type;

	assert_valid_restored_room(restoredRoom, gameStateRoom);
	return true;
}

void VirtualRoomManager::write_restored_virtual_room(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom) noexcept
{
	assert(&roomConfig == &this->m_RestoredRooms[roomConfig.Variant]);

	uint32_t flags = gameStateRoom.GetFlags();
	flags |= GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG;
	gameStateRoom.SetFlags(flags);
	gameStateRoom._variant = roomConfig.Variant;

	gameStateRoom.SetStageID(0);
	gameStateRoom.SetMode(0);
	gameStateRoom._type = 0;
}

RoomConfig_Room* VirtualRoomManager::read_restored_virtual_room(GameStateRoomConfig& gameStateRoom) noexcept
{
	assert(gameStateRoom.GetFlags() & GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG);

	auto& roomSet = this->get_room_set(0, 0, true);
	return get_room_from_set(roomSet, gameStateRoom._variant);
}

#pragma endregion

#pragma region GameState Management

uint32_t VirtualRoomManager::save_room(RoomConfig_Room* room, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap) noexcept
{
	RoomConfigUtility::RoomIdentifier identifier(*room);
	auto [it, inserted] = savedRoomsMap.emplace(identifier, m_SavedRooms.size());

	if (inserted)
	{
		auto& mapEntry = m_RoomMap[identifier];
		assert(mapEntry.reference); // The saved room was not properly inserted in the map as a virtual room (This could be caused by it not being a virtual room at all)
		m_SavedRooms.emplace_back(room);
	}

	return it->second;
}

void VirtualRoomManager::try_save_room(GameStateRoomConfig& saveState, std::map<RoomConfigUtility::RoomIdentifier, uint32_t>& savedRoomsMap) noexcept
{
	uint32_t flags = saveState.GetFlags();
	if ((flags & ~GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG) != 0) // Only the Restored Virtual Rooms or rooms with no flag are allowed
	{
		return;
	}

	RoomConfig_Room* room = nullptr;
	if (flags & GameStateRoomConfig::RESTORED_VIRTUAL_ROOM_FLAG)
	{
		room = this->read_restored_virtual_room(saveState);
	}
	else
	{
		room = this->get_room(saveState.GetStageID(), saveState._type, saveState._variant, saveState.GetMode());
	}

	if (!room) // The room is not a virtual room
	{
		return;
	}

	uint32_t savedRoomIndex = this->save_room(room, savedRoomsMap);
	saveState.SetFlags(GameStateRoomConfig::VIRTUAL_ROOM_FLAG);
	saveState._variant = savedRoomIndex;
	saveState._type = 0;
}

void VirtualRoomManager::save_game_state(uint32_t slot) noexcept
{
	if (slot != GameStateSlot::SAVE_FILE)
	{
		return;
	}

	m_SavedRooms.clear();
	std::map<RoomConfigUtility::RoomIdentifier, uint32_t> savedRoomsMap;
	auto& gameState = g_Manager->_gamestate;
	auto gameStateRooms = get_game_state_rooms(gameState);

	for (auto* gameStateRoom : gameStateRooms)
	{
		this->try_save_room(*gameStateRoom, savedRoomsMap);
	}
}

void VirtualRoomManager::delete_save(const std::string& fileName, bool isRerun) noexcept
{
	if (isRerun)
	{
		return;
	}

	std::filesystem::path filePath = get_save_data_path(fileName);
	std::error_code errorCode;

	if (!std::filesystem::exists(filePath, errorCode))
	{
		return;
	}

	LogContext logContext;
	logContext.emplace_back(REPENTOGON::StringFormat("[VirtualRoomManager] deleting \"%s\": ", fileName.c_str()));

	bool status = std::filesystem::remove(filePath, errorCode);
	if (!status || errorCode)
	{
		if (errorCode)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("unable to delete file at \"%s\": %s", filePath.string().c_str(), errorCode.message().c_str()).c_str());
		}
		else
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("unable to delete file at \"%s\"", filePath.string().c_str()).c_str());
		}
	}
	else
	{
		logContext.LogMessage(LogUtility::eLogType::INFO, REPENTOGON::StringFormat("successfully deleted file at \"%s\"", filePath.string().c_str()).c_str());
	}
}

#pragma endregion

#pragma region Serialization

static std::optional<std::ofstream> open_game_state_file(std::filesystem::path filePath, Error& error) noexcept
{
	std::filesystem::path directory = filePath.parent_path();
	error.reset();

	std::error_code errorCode;
	if (!directory.empty() && !std::filesystem::exists(directory, errorCode))
	{
		if (errorCode)
		{
			error = REPENTOGON::StringFormat("failed to check existence of game state directory: \"%s\"", errorCode.message().c_str());
			return std::nullopt;
		}

		std::filesystem::create_directories(directory, errorCode);
		if (errorCode)
		{
			error = REPENTOGON::StringFormat("failed to create game state directory: \"%s\"", errorCode.message().c_str());
			return std::nullopt;
		}
	}

	std::ofstream file(filePath);
	if (!file)
	{
		error = REPENTOGON::StringFormat("failed to open file for writing: \"%s\"", filePath.string().c_str());
		return file;
	}

	return file;
}

rapidjson::Value VirtualRoomManager::serialize_rooms(rapidjson::Document::AllocatorType& allocator) noexcept
{
	rapidjson::Value rooms(rapidjson::kArrayType);

	for (auto* room : m_SavedRooms)
	{
		assert(room); // Each saved room should never be nullptr
		rooms.PushBack(RoomConfigUtility::SerializeRoom(*room, allocator), allocator);
	}

	return rooms;
}

rapidjson::Document VirtualRoomManager::seralize_save() noexcept
{
	rapidjson::Document doc;
	auto& save = doc.SetObject();
	auto& allocator = doc.GetAllocator();
	uint32_t gameChecksum = g_Manager->_gamestate._checksum;

	save.AddMember("Version", GAMESTATE_VERSION, allocator);
	save.AddMember("Checksum", gameChecksum, allocator);
	save.AddMember("Rooms", this->serialize_rooms(allocator), allocator);

	return doc;
}

bool VirtualRoomManager::write_save(const std::string& fileName, bool isRerun) noexcept
{
	if (isRerun)
	{
		return true;
	}

	LogContext logContext;
	logContext.emplace_back(REPENTOGON::StringFormat("[VirtualRoomManager] writing save \"%s\": ", fileName.c_str()));

	std::string filePath = get_save_data_path(fileName);
	Error error;

	auto file = open_game_state_file(filePath, error);
	if (error)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("unable to write save: %s", error.value().c_str()).c_str());
		return true;
	}

	rapidjson::Document doc = this->seralize_save();

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	file.value() << buffer.GetString() << std::endl;

	logContext.LogMessage(LogUtility::eLogType::INFO, REPENTOGON::StringFormat("successfully saved to \"%s\"", filePath.c_str()).c_str());
	return true;
}

#pragma endregion

#pragma region Deserialization

static inline rapidjson::Document get_json_document(const std::string& filePath, Error& error) noexcept
{
	error.reset();
	rapidjson::Document doc;

	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		error = REPENTOGON::StringFormat("Failed to open file \"%s\"", filePath.c_str());
		return doc;
	}

	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		error = REPENTOGON::StringFormat("Failed to parse json file \"%s\"", filePath.c_str());
		return doc;
	}

	return doc;
}

std::vector<std::optional<RoomConfig_Room>> VirtualRoomManager::deserialize_rooms(const rapidjson::Value& rooms, LogUtility::LogContext& logContext) noexcept
{
	std::vector<std::optional<RoomConfig_Room>> builtRooms;

	if (!LogUtility::Json::ValidateArray(rooms, logContext))
	{
		return builtRooms;
	}

	const auto& roomsArray = rooms.GetArray();
	builtRooms.reserve(roomsArray.Size());

	for (size_t i = 0; i < roomsArray.Size(); i++)
	{
		auto& room = roomsArray[i];
		logContext.emplace_back(REPENTOGON::StringFormat("room #%u -> ", i));

		builtRooms.emplace_back(RoomConfigUtility::DeserializeRoom(room, logContext));
		if (!builtRooms.back().has_value())
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build room.");
		}

		logContext.pop_back();
	}

	return builtRooms;
}

std::optional<VirtualRoomManager::SaveData> VirtualRoomManager::deserialize_save(const std::string& filePath, LogContext& logContext) noexcept
{
	uint32_t gameChecksum = g_Manager->_gamestate._checksum; // We should never enter this when loading a rerun so we don't handle that case

	Error error;
	auto doc = get_json_document(filePath, error);
	if (error)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, error.value().c_str());
		return std::nullopt;
	}

	if (!LogUtility::Json::ValidateObject(doc, logContext))
	{
		return std::nullopt;
	}

	auto& version = LogUtility::Json::ReadIntegerMember<uint32_t>(doc, "Version", logContext, false); if (!version) { return std::nullopt; }
	if (version.value() != GAMESTATE_VERSION)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("gamestate version mismatch (current: %u, game state: %u)", GAMESTATE_VERSION, version.value()).c_str());
		return std::nullopt;
	}

	auto& checksum = LogUtility::Json::ReadIntegerMember<uint32_t>(doc, "Checksum", logContext, false); if (!checksum) { return std::nullopt; }
	if (checksum.value() != gameChecksum)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("checksum mismatch (current: 0x%X, game state: 0x%X)", gameChecksum, checksum.value()).c_str());
		return std::nullopt;
	}

	auto& rooms = LogUtility::Json::GetArrayMember(doc, "Rooms", logContext, false); if (!rooms) { return std::nullopt; }

	SaveData saveData;

	auto deserializedRooms = this->deserialize_rooms(*rooms.value(), logContext);
	saveData.virtualRooms = this->add_rooms(std::move(deserializedRooms), logContext, true);

	return saveData;
}

bool VirtualRoomManager::normalize_game_state(GameState& gameState, std::vector<RoomConfig_Room*>& restoredRooms, LogContext& logContext) noexcept
{
	auto gameStateRooms = get_game_state_rooms(gameState);
	for (auto* gameStateRoom : gameStateRooms)
	{
		if (!this->normalize_game_state_room(*gameStateRoom, restoredRooms))
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to normalize game state room.");
			return false;
		}
	}

	return true;
}

bool VirtualRoomManager::read_save(const std::string& fileName, bool isRerun) noexcept
{
	if (isRerun)
	{
		return true;
	}

	std::error_code error;
	std::string filePath = get_save_data_path(fileName);

	LogContext logContext;
	logContext.emplace_back(REPENTOGON::StringFormat("[VirtualRoomManager] reading save \"%s\": ", fileName.c_str()));

	SaveData saveData;

	if (!std::filesystem::exists(filePath, error))
	{
		if (error)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, error.message().c_str());
		}
		else
		{
			logContext.LogMessage(LogUtility::eLogType::INFO, "save not found, skipping read.");
		}
	}
	else
	{
		auto deserializedSave = this->deserialize_save(filePath, logContext);
		if (!deserializedSave)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "failed to deserialize save.");
		}
		else
		{
			saveData = std::move(deserializedSave.value());
			if (logContext.GetLoggedMessagesCount(LogUtility::eLogType::ERROR) + logContext.GetLoggedMessagesCount(LogUtility::eLogType::WARN) > 0)
			{
				logContext.LogMessage(LogUtility::eLogType::WARN, "successfully deserialized save, but something went wrong along the way.");
			}
			else
			{
				logContext.LogMessage(LogUtility::eLogType::INFO, "successfully deserialized save.");
			}
		}
	}

	bool success = this->normalize_game_state(g_Manager->_gamestate, saveData.virtualRooms, logContext);
	if (!success)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to normalize game state. cannot continue save");
	}

	return success;
}

#pragma endregion

void VirtualRoomManager::__SaveGameState(uint32_t slot) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.save_game_state(slot);
}

bool VirtualRoomManager::__WriteSave(const std::string& fileName, bool isRerun) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	return instance.write_save(fileName, isRerun);
}

bool VirtualRoomManager::__ReadSave(const std::string& fileName, bool isRerun) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	return instance.read_save(fileName, isRerun);
}

void VirtualRoomManager::__DeleteSave(const std::string& fileName, bool isRerun) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	return instance.delete_save(fileName, isRerun);
}

bool VirtualRoomManager::__TryWriteRestoredVirtualRoom(RoomConfig_Room& roomConfig, GameStateRoomConfig& gameStateRoom) noexcept
{
	auto& instance = VirtualRoomManager::Get();
	if (!instance.is_restored_virtual_room(roomConfig))
	{
		return false;
	}

	instance.write_restored_virtual_room(roomConfig, gameStateRoom);
	return true;
}

RoomConfig_Room* VirtualRoomManager::__ReadRestoredVirtualRoom(GameStateRoomConfig& gameStateRoom) noexcept
{
	return VirtualRoomManager::Get().read_restored_virtual_room(gameStateRoom);
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

std::vector<RoomConfig_Room*> VirtualRoomManager::AddRooms(uint32_t stageId, int mode, std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext) noexcept
{
	for (auto& room : rooms)
	{
		if (room)
		{
			room.value().StageId = stageId;
			room.value().Mode = mode;
		}
	}

	return VirtualRoomManager::Get().add_rooms(std::move(rooms), logContext, false);
}

std::vector<RoomConfig_Room*> VirtualRoomManager::AddRooms(std::vector<std::optional<RoomConfig_Room>>&& rooms, LogUtility::LogContext& logContext) noexcept
{
	return VirtualRoomManager::Get().add_rooms(std::move(rooms), logContext, false);
}

VirtualRoomManager::RoomSet& VirtualRoomManager::GetRoomSet(uint32_t stageId, int mode) noexcept
{
	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	return VirtualRoomManager::Get().get_room_set(stageId, mode, false);
}

#pragma endregion

#pragma region Hooks

HOOK_METHOD(ModManager, LoadConfigs, () -> void)
{
	VirtualRoomManager::__Init();
	super();
}

HOOK_METHOD(Game, Exit, (bool ShouldSave) -> void)
{
	super(ShouldSave);
	VirtualRoomManager::__Reset();
}

HOOK_METHOD(RoomConfig, ResetRoomWeights, (uint32_t Stage, int Mode) -> void)
{
	super(Stage, Mode);
	VirtualRoomManager::__ResetWeights(Stage, Mode);
}

HOOK_METHOD(RoomConfig, LoadStageBinary, (uint32_t Stage, uint32_t Mode) -> void)
{
	super(Stage, Mode);
	VirtualRoomManager::__RefinalizeRooms(Stage, Mode);
}

HOOK_METHOD(RoomConfig, GetRoomByStageTypeAndVariant, (unsigned int stage, unsigned int type, unsigned int variant, int mode) -> RoomConfig_Room*)
{
	RoomConfig_Room* room = super(stage, type, variant, mode);
	if (!room)
	{
		room = VirtualRoomManager::__GetRoom(stage, type, variant, mode);
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
	VirtualRoomManager::__GetRooms(rooms, stage, type, shape, minVariant, maxVariant, minDifficulty, maxDifficulty, *doors, subtype, mode);

	return rooms;
}

HOOK_STATIC(Level, write_room_config, (RoomConfig_Room* roomConfig, GameStateRoomConfig* gameStateRoom, std_deque_RoomConfigRoom* deque) -> void, __stdcall)
{
	if (VirtualRoomManager::__TryWriteRestoredVirtualRoom(*roomConfig, *gameStateRoom))
	{
		return;
	}

	super(roomConfig, gameStateRoom, deque);
}

HOOK_STATIC(Level, read_room_config, (GameStateRoomConfig* room, std_deque_RoomConfigRoom* deque) -> RoomConfig_Room*, __stdcall)
{
	if (room->GetFlags() & GameStateRoomConfig::VIRTUAL_ROOM_FLAG)
	{
		auto* restoredRoom = VirtualRoomManager::__ReadRestoredVirtualRoom(*room);
		if (!restoredRoom)
		{
			ZHL::Log("[ASSERT] [VirtualRoomManager] - Level::read_room_config_room Hook - unable to resolve restored virtual room.\n");
		}
		return restoredRoom;
	}

	return super(room, deque);
}

#pragma endregion

#ifndef NDEBUG

static std::vector<std::optional<RoomConfig_Room>> build_rooms(lua_State* L, int index, LogUtility::LogContext& logContext)
{
	int absIndex = lua_absindex(L, index);
	assert(lua_istable(L, absIndex));

	size_t size = (size_t)lua_rawlen(L, absIndex);

	std::vector<std::optional<RoomConfig_Room>> rooms;
	rooms.reserve(size);

	for (size_t i = 1; i <= size; i++)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("room #%u -> ", i));
		lua_rawgeti(L, absIndex, i);

		auto room = RoomConfigUtility::BuildRoomFromLua(L, -1, logContext);
		if (!room)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build room.");
		}
		rooms.emplace_back(std::move(room));

		lua_pop(L, 1);
		logContext.pop_back();
	}

	return rooms;
}

static void build_lua_return_table(lua_State* L, int index, std::vector<RoomConfig_Room*>& placedRooms)
{
	int absIndex = lua_absindex(L, index);
	size_t size = (size_t)lua_rawlen(L, absIndex);
	assert(placedRooms.size() == size); // placed rooms's size must be equal to size of the table

	lua_newtable(L);

	for (size_t i = 1; i <= size; i++)
	{
		auto* room = placedRooms[i - 1];
		if (!room)
		{
			lua_pushnil(L);
		}
		else
		{
			lua::luabridge::UserdataPtr::push(L, room, lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
		}

		lua_rawseti(L, -2, i);
	}
}

void VirtualRoomManager::__AddLuaRooms(lua_State* L, uint32_t stageId, int mode, int tableIndex) noexcept
{
	int absTableIndex = lua_absindex(L, tableIndex);
	assert(lua_istable(L, absTableIndex));

	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	LogUtility::LogContext logContext;
	logContext.emplace_back(REPENTOGON::StringFormat("[LUA] Add Rooms to set (%u, %i): ", stageId, mode));
	std::string luaCaller = LogUtility::Lua::GetStackLevelInfo(L, 1);
	if (!luaCaller.empty())
	{
		logContext.emplace_back(luaCaller + " -> ");
	}

	auto rooms = build_rooms(L, absTableIndex, logContext);
	auto placedRooms = VirtualRoomManager::AddRooms(stageId, mode, std::move(rooms), logContext);
	build_lua_return_table(L, absTableIndex, placedRooms);
}

#pragma region UnitTests

LUA_FUNCTION(Lua_Debug_AddRooms)
{
	uint32_t stageId = (uint32_t)luaL_checkinteger(L, 1);
	int mode = (int)luaL_checkinteger(L, 2);

	if (0 > stageId || stageId >= NUM_STB)
	{
		return luaL_argerror(L, 1, REPENTOGON::StringFormat("invalid stage %d", stageId).c_str());
	}

	if (-1 > mode || mode > 1)
	{
		return luaL_argerror(L, 2, REPENTOGON::StringFormat("invalid mode %d", stageId).c_str());
	}

	if (!lua_istable(L, 3))
	{
		return luaL_argerror(L, 3, REPENTOGON::Lua::GenerateInvalidTypeMessage(L, 3, "table").c_str());
	}

	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	LogUtility::LogContext logContext;
	logContext.emplace_back("[DEBUG] Add Virtual Rooms: ");
	std::string luaCaller = LogUtility::Lua::GetStackLevelInfo(L, 1);
	if (!luaCaller.empty())
	{
		logContext.emplace_back(luaCaller + " -> ");
	}

	auto rooms = build_rooms(L, 3, logContext);
	auto placedRooms = VirtualRoomManager::AddRooms(stageId, mode, std::move(rooms), logContext);
	build_lua_return_table(L, 3, placedRooms);

	return 1;
}

static void RegisterVirtualRoomsUnitTests(lua_State* L)
{
	lua_getglobal(L, "UnitTests");
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);
		lua_setglobal(L, "UnitTests");
		lua_getglobal(L, "UnitTests");
	}
	lua_newtable(L);
	lua_setfield(L, -2, "VirtualRooms");
	lua_getfield(L, -1, "VirtualRooms");

	lua::TableAssoc(L, "AddRooms", Lua_Debug_AddRooms);

	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterVirtualRoomsUnitTests(_state);
}

#pragma endregion

#endif // !NDEBUG