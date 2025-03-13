#pragma once

#include "IsaacRepentance.h"
#include "LogUtility.h"

#include "document.h" // rapidjson
#include <string>
#include <vector>
#include <utility>
#include <optional>

namespace RoomConfigUtility
{
	typedef std::optional<std::string> Error;
    typedef LogUtility::LogContext LogContext;

	struct RoomIdentifier
	{
		int stageId = STB_SPECIAL_ROOMS;
		int mode = 0;
		int type = ROOM_DEFAULT;
		uint32_t variant = 0;
		int shape = ROOMSHAPE_NULL;
		std::string name;

		RoomIdentifier(const RoomConfig_Room& roomConfig);
		auto tie() const noexcept { return std::tie(stageId, mode, type, variant, shape, name); }
		size_t hash() const noexcept;
		bool operator<(const RoomIdentifier& other) const noexcept { return tie() < other.tie(); }
		bool operator<=(const RoomIdentifier& other) const noexcept { return tie() <= other.tie(); }
		bool operator==(const RoomIdentifier& other) const noexcept { return tie() == other.tie(); }
		bool operator>=(const RoomIdentifier& other) const noexcept { return tie() >= other.tie(); }
		bool operator>(const RoomIdentifier& other) const noexcept { return tie() > other.tie(); }

		struct Hasher
		{
			size_t operator()(const RoomIdentifier& roomIdentifier) const noexcept { return roomIdentifier.hash(); }
		};
	};

	// Returns a pair of Width and Height of the shape
	const std::pair<const int, const int>& GetShapeDimensions(size_t shape) noexcept;

	void PostRoomInsert(RoomConfig_Room& room, uint32_t stageId, int mode) noexcept;
	void FinalizeSpawnEntryInsertion(RoomConfig_Room& room, RoomSpawn& roomSpawn, RoomEntry& spawnEntry) noexcept;
	bool RoomPassesFilter(RoomConfig_Room& room, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType) noexcept;

	inline bool IsStageValid(uint32_t stageId) noexcept { return STB_SPECIAL_ROOMS <= stageId && stageId < NUM_STB; }
	inline bool IsModeValid(int mode) noexcept { return 0 <= mode && mode <= 1; }
	inline bool IsRoomTypeValid(uint32_t roomType) noexcept { return ROOM_DEFAULT <= roomType && roomType < NUM_ROOMTYPES; }
	inline bool IsShapeValid(uint32_t roomShape) noexcept { return ROOMSHAPE_NULL < roomShape && roomShape < NUM_ROOMSHAPES; }

	void AssertRoomValidity(const RoomConfig_Room& room) noexcept;
	void AssertRoomSpawnValidity(const RoomSpawn& roomSpawn, const RoomConfig_Room* room = nullptr) noexcept;
	void AssertRoomSpawnEntryValidity(const RoomEntry& spawnEntry) noexcept;

	std::optional<RoomConfig_Room> BuildRoomFromLua(lua_State* L, int index, LogContext& logContext) noexcept;
	int GetDoorSlotFromLua(lua_State* L, int index, int shape, LogContext& logContext) noexcept;
	std::optional<RoomSpawn> BuildSpawnFromLua(lua_State* L, int index, RoomConfig_Room& room, LogContext& logContext) noexcept;
	std::optional<RoomEntry> BuildSpawnEntryFromLua(lua_State* L, int index, LogContext& logContext) noexcept;

	std::optional<RoomConfig_Room> DeserializeRoom(const rapidjson::Value& roomNode, LogContext& logContext) noexcept;
	std::optional<RoomSpawn> DeserializeRoomSpawn(const rapidjson::Value& spawnNode, RoomConfig_Room& room, LogContext& logContext) noexcept;
	std::optional<RoomEntry> DeserializeRoomSpawnEntry(const rapidjson::Value& spawnEntryNode, LogContext& logContext) noexcept;

	rapidjson::Value SerializeRoom(const RoomConfig_Room& room, rapidjson::Document::AllocatorType& allocator) noexcept;
	rapidjson::Value SerializeRoomSpawn(const RoomSpawn& roomSpawn, rapidjson::Document::AllocatorType& allocator) noexcept;
	rapidjson::Value SerializeRoomSpawnEntry(const RoomEntry& spawnEntry, rapidjson::Document::AllocatorType& allocator) noexcept;
}