#include "VirtualRooms.h"

#include "../RoomConfigUtility.h"
#include "HookSystem.h"
#include <map>

#undef ERROR

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
			assert(roomPtr == it->data()); // Make sure no reallocation occurred
			it++;
		}
	}

	for (size_t i = 0; i < insertionData.roomsData.size(); i++)
	{
		const auto& roomData = insertionData.roomsData[i];

		assert(!roomData.mapEntry.IsInInvalidState());
		assert(roomData.mapEntry.inserted || isRestoringRooms); // Make sure all rooms have their map entry marked as inserted unless we are restoring rooms
		assert(roomData.insertedRoom != nullptr); // Make sure all rooms have a reference to the inserted room
		assert(&roomData.insertedRoom == &insertionData.outInsertedRooms[i]); // The Indexes of the two vectors should be synchronized
	}
#endif // !NDEBUG
}

#pragma endregion

#pragma region RoomSet Iterator

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

#pragma endregion

#pragma region Init

void VirtualRoomManager::reset() noexcept
{
	m_Stages.clear();
	m_RoomMap.clear();

	m_Stages.resize(NUM_STB);
}

void VirtualRoomManager::__Init() noexcept
{
	auto& instance = VirtualRoomManager::Get();
	instance.reset();
}

#pragma endregion

#pragma region Room Getters

RoomConfig_Room* VirtualRoomManager::get_room(uint32_t stageId, int type, uint32_t variant, int mode) noexcept
{
	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	assert(stageId < m_Stages.size());
	assert(0 <= mode && mode <= 1);

	auto& roomSet = m_Stages[stageId].m_RoomSets[mode];
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
	if (mode == -1)
	{
		mode = g_Game->GetMode();
	}

	assert(stageId < m_Stages.size());
	assert(0 <= mode && mode <= 1);

	auto& roomSet = m_Stages[stageId].m_RoomSets[mode];
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
		roomData.mapEntry.inserted = !isRestoringRooms;
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
			logContext.LogMessage(LogUtility::eLogType::WARN, REPENTOGON::StringFormat("A room with stage: %u, mode: %d, type: %d, variant: %u, shape: %d and name: \"%s\" already exists, room cannot be restored on continue", identifier.stageId, identifier.mode, identifier.type, identifier.variant, identifier.shape, identifier.name.c_str()).c_str());
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

#pragma region Hooks

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

#pragma endregion

#ifndef NDEBUG

#pragma region Unit Tests

#include <fstream>

static std::pair<std::vector<RoomConfig_Room>, std::set<size_t>> debug_build_rooms(lua_State* L, int index, LogUtility::LogContext& logContext)
{
	int absIndex = lua_absindex(L, index);
	assert(lua_istable(L, absIndex));

	size_t size = (size_t)lua_rawlen(L, absIndex);

	std::vector<RoomConfig_Room> rooms;
	std::set<size_t> failedRooms;
	rooms.reserve(size);

	for (size_t i = 1; i <= size; i++)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("room #%u -> ", i));
		lua_rawgeti(L, absIndex, i);

		auto room = RoomConfigUtility::BuildRoomFromLua(L, -1, logContext);
		if (!room)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build room.");
			failedRooms.insert(i);
		}
		else
		{
			rooms.emplace_back(std::move(room.value()));
		}
		lua_pop(L, 1);
		logContext.pop_back();
	}

	return { rooms, failedRooms };
}

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

	auto rooms = debug_build_rooms(L, 3, logContext);
	auto placedRooms = VirtualRoomManager::AddRooms(stageId, mode, std::move(rooms.first), logContext);

	lua_newtable(L);
	auto placedRoomsIt = placedRooms.begin();

	size_t size = (size_t)lua_rawlen(L, 3);
	assert(placedRooms.size() + rooms.second.size() == size);

	for (size_t i = 1; i <= size; i++)
	{
		if (rooms.second.find(i) != rooms.second.end())
		{
			lua_pushnil(L);
		}
		else
		{
			lua::luabridge::UserdataPtr::push(L, *placedRoomsIt, lua::GetMetatableKey(lua::Metatables::CONST_ROOM_CONFIG_ROOM));
			placedRoomsIt++;
		}

		lua_rawseti(L, -2, i);
	}

	return 1;
}

static void RegisterRoomConfigUtilityUnitTests(lua_State* L)
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
	RegisterRoomConfigUtilityUnitTests(_state);
}

#pragma endregion

#endif // !NDEBUG