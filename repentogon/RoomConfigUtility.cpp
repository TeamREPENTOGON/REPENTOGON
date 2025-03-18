#include "RoomConfigUtility.h"

#include "IsaacRepentance.h"
#include "Log.h"
#include "LuaCore.h"
#include "LogUtility.h"
#include "MiscFunctions.h"

#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson
#include <utility>
#include <array>
#include <type_traits>

#undef ERROR

typedef RoomConfigUtility::Error Error;
typedef RoomConfigUtility::LogContext LogContext;

const static std::array<const std::pair<const int, const int>, NUM_ROOMSHAPES> s_ShapeDimensions = { {
	{0, 0}, // I don't know what else to put for ROOMSHAPE_NULL
	{13, 7}, // ROOMSHAPE_1x1
	{13, 3}, // ROOMSHAPE_IH
	{5, 7}, // ROOMSHAPE_IV
	{13, 14}, // ROOMSHAPE_1x2
	{5, 14}, // ROOMSHAPE_IIV
	{26, 7}, // ROOMSHAPE_2x1
	{26, 3}, // ROOMSHAPE_IIH
	{26, 14}, // ROOMSHAPE_2x2
	{26, 14}, // ROOMSHAPE_LTL
	{26, 14}, // ROOMSHAPE_LTR
	{26, 14}, // ROOMSHAPE_LBL
	{26, 14}, // ROOMSHAPE_LBR
}};

#pragma region RoomIdentifier

template <typename T>
static inline void hash_combine(std::size_t& seed, const T& value)
{
	seed ^= std::hash<T>{}(value)+0x9e3779b9 + (seed << 6) + (seed >> 2);
}

RoomConfigUtility::RoomIdentifier::RoomIdentifier(const RoomConfig_Room& roomConfig)
	: stageId(roomConfig.StageId),
	mode(roomConfig.Mode),
	type(roomConfig.Type),
	variant(roomConfig.originalVariant),
	shape(roomConfig.Shape),
	name(roomConfig.Name)
{
}

size_t RoomConfigUtility::RoomIdentifier::hash() const noexcept
{
	size_t seed = 0;

	hash_combine(seed, this->stageId);
	hash_combine(seed, this->mode);
	hash_combine(seed, this->type);
	hash_combine(seed, this->variant);
	hash_combine(seed, this->shape);
	hash_combine(seed, this->name);

	return seed;
}

#pragma endregion

#pragma region Asserts and Validations

static inline bool are_room_dimensions_valid(int width, int height, int shape) noexcept
{
	auto& shapeDimensions = RoomConfigUtility::GetShapeDimensions(shape);
	return width == shapeDimensions.first && height == shapeDimensions.second;
}

static bool is_spawn_entry_in_spawn_array(const RoomSpawn& roomSpawn, RoomEntry& spawnEntry) noexcept
{
	for (size_t i = 0; i < roomSpawn.CountEntries; i++)
	{
		if (&roomSpawn.Entries[i] == &spawnEntry)
		{
			return true;
		}
	}

	return false;
}

static void assert_spawn_entry_flags(const RoomConfig_Room& room, const RoomSpawn& roomSpawn, const RoomEntry& spawnEntry)
{
#ifndef NDEBUG
	if (spawnEntry.type == eEntityType::ENTITY_ENVIRONMENT && spawnEntry.variant == 2)
	{
		int doorSlot = RoomConfig::get_door_from_position(roomSpawn.X, roomSpawn.Y, room.Shape);
		if (doorSlot != NO_DOOR_SLOT)
		{
			assert(room.QuestDoors.test(doorSlot));
		}
	}
	else if (spawnEntry.type == eRoomSpawnType::SPAWN_PRESSURE_PLATE && spawnEntry.variant == 0)
	{
		assert(room.Flags.test(FLAG_HAS_CLEAR_PLATE));
	}
#endif
}

void RoomConfigUtility::AssertRoomSpawnEntryValidity(const RoomEntry& roomSpawnEntry) noexcept
{
#ifndef NDEBUG
	assert(roomSpawnEntry.weight >= 0.0f);
#endif // !NDEBUG
}

void RoomConfigUtility::AssertRoomSpawnValidity(const RoomSpawn& roomSpawn, const RoomConfig_Room* room) noexcept
{
#ifndef NDEBUG
	float sumWeights = 0.0f;

	for (size_t i = 0; i < roomSpawn.CountEntries; i++)
	{
		RoomConfigUtility::AssertRoomSpawnEntryValidity(roomSpawn.Entries[i]);
		sumWeights += roomSpawn.Entries[i].weight;
		if (room)
		{
			assert_spawn_entry_flags(*room, roomSpawn, roomSpawn.Entries[i]);
		}
	}

	assert(roomSpawn.SumWeights == sumWeights);
#endif // !NDEBUG
}

void RoomConfigUtility::AssertRoomValidity(const RoomConfig_Room& room) noexcept
{
#ifndef NDEBUG
	assert(IsStageValid(room.StageId));
	assert(IsModeValid(room.Mode));
	assert(IsRoomTypeValid(room.Type));
	assert(IsShapeValid(room.Shape));
	assert(are_room_dimensions_valid(room.Width, room.Height, room.Shape));
	assert(room.InitialWeight >= 0.0f);
	assert(room.Weight >= 0.0f);

	for (size_t i = 0; i < room.SpawnCount; i++)
	{
		RoomConfigUtility::AssertRoomSpawnValidity(room.Spawns[i]);
	}
#endif // !NDEBUG
}

#pragma endregion

#pragma region General

const std::pair<const int, const int>& RoomConfigUtility::GetShapeDimensions(size_t shape) noexcept
{
	assert(shape < NUM_ROOMSHAPES); // Assert, even if handled, cause something went wrong somewhere

	shape = shape < NUM_ROOMSHAPES ? shape : ROOMSHAPE_NULL;
	return s_ShapeDimensions[shape];
}

void RoomConfigUtility::PostRoomInsert(RoomConfig_Room& room, uint32_t stageId, int mode) noexcept
{
	if (mode == -1)
	{
		mode = g_Game->IsGreedMode() ? 1 : 0;
	}

	assert(IsStageValid(stageId));
	assert(IsModeValid(mode));
	assert(IsRoomTypeValid(room.Type));

	room.StageId = stageId;
	room.Mode = mode;

	auto& roomSet = g_Game->GetRoomConfig()->_stages[stageId]._rooms[mode];
	if (!roomSet._loaded)
	{
		g_Game->GetRoomConfig()->LoadStageBinary(stageId, mode);
	}

	auto* variantSet = roomSet.GetVariantSet(room.Type);
	room.Variant = variantSet->AddUnique(room.originalVariant);
}

void RoomConfigUtility::FinalizeSpawnEntryInsertion(RoomConfig_Room& room, RoomSpawn& roomSpawn, RoomEntry& spawnEntry) noexcept
{
	assert(is_spawn_entry_in_spawn_array(roomSpawn, spawnEntry));

	roomSpawn.SumWeights += spawnEntry.weight;

	if (spawnEntry.type == eEntityType::ENTITY_ENVIRONMENT && spawnEntry.variant == 2)
	{
		int doorSlot = RoomConfig::get_door_from_position(roomSpawn.X, roomSpawn.Y, room.Shape);
		if (doorSlot != NO_DOOR_SLOT)
		{
			room.QuestDoors.set(doorSlot);
		}
	}
	else if (spawnEntry.type == eRoomSpawnType::SPAWN_PRESSURE_PLATE && spawnEntry.variant == 0)
	{
		room.Flags.set(FLAG_HAS_CLEAR_PLATE);
	}
}

bool RoomConfigUtility::RoomPassesFilter(RoomConfig_Room& room, uint32_t roomType, uint32_t roomShape, uint32_t minVariant, uint32_t maxVariant, int minDifficulty, int maxDifficulty, uint32_t doors, int subType) noexcept
{
	if ((room.Type == roomType) &&
		(roomShape == NUM_ROOMSHAPES || room.Shape == roomShape) &&
		(minVariant <= room.Variant && room.Variant <= maxVariant) &&
		(minDifficulty <= room.Difficulty && room.Difficulty <= maxDifficulty) &&
		((room.Doors & doors) == doors) &&
		(subType == -1 || room.Subtype == subType))
	{
		return !g_Game->_seedEffects.HasSeedEffect(SEED_G_FUEL) || !room.Flags.test(FLAG_HAS_CLEAR_PLATE);
	}

	return false;
}

#pragma endregion

#pragma region Lua

std::optional<RoomConfig_Room> RoomConfigUtility::BuildRoomFromLua(lua_State* L, int index, LogContext& logContext) noexcept
{
	int absIndex = lua_absindex(L, index);

	if (!LogUtility::Lua::ValidateTable(L, absIndex, logContext, nullptr, false))
	{
		return std::nullopt;
	}

	auto type = LogUtility::Lua::ReadIntegerField<int>(L, absIndex, "TYPE", logContext, false); if (!type) { return std::nullopt; }
	auto variant = LogUtility::Lua::ReadIntegerField<uint32_t>(L, absIndex, "VARIANT", logContext, false); if (!variant) { return std::nullopt; }
	auto name = LogUtility::Lua::ReadStringField(L, absIndex, "NAME", logContext, false); if (!name) { return std::nullopt; }
	auto shape = LogUtility::Lua::ReadIntegerField<int8_t>(L, absIndex, "SHAPE", logContext, false); if (!shape) { return std::nullopt; }

	if (!IsRoomTypeValid(type.value()))
	{
		LogUtility::Lua::LogInvalidArg(logContext, REPENTOGON::StringFormat("invalid argument for \"TYPE\" (invalid room type %d)", type.value()).c_str(), "TYPE");
		type = ROOM_DEFAULT;
	}

	if (!IsShapeValid(shape.value()))
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, REPENTOGON::StringFormat("invalid argument for \"SHAPE\" (invalid room shape %d) ", shape.value()).c_str());
		shape = ROOMSHAPE_1x1;
	}

	RoomConfig_Room room;

	room.Type = type.value();
	room.originalVariant = variant.value();
	room.Variant = room.originalVariant;

	room.Subtype = LogUtility::Lua::ReadIntegerField<int>(L, absIndex, "SUBTYPE", logContext).value_or(0);
	room.Difficulty = LogUtility::Lua::ReadIntegerField<int>(L, absIndex, "DIFFICULTY", logContext).value_or(1);
	room.Name = name.value();

	room.Weight = LogUtility::Lua::ReadNumberField<float>(L, absIndex, "WEIGHT", logContext).value_or(0.0f);
	if (room.Weight < 0.0f)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, "invalid argument for \"WEIGHT\" (weight cannot be negative)");
		room.Weight = 0.0f;
	}
	room.InitialWeight = room.Weight;

	room.Shape = shape.value();
	auto& shapeDimensions = RoomConfigUtility::GetShapeDimensions(room.Shape); // Let's just use the correct ones, rather than reading from the table
	room.Width = shapeDimensions.first;
	room.Height = shapeDimensions.second;
	
	room.Flags = 0;
	room.SpawnCount = 0;
	room.Spawns = nullptr;

	size_t roomSpawnCount = (size_t)lua_rawlen(L, absIndex);
	std::vector<RoomSpawn> roomSpawns;
	roomSpawns.reserve(roomSpawnCount);

	for (size_t i = 1; i <= roomSpawnCount; i++)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("spawn #%u -> ", i));
		lua_rawgeti(L, absIndex, i);

		if (!LogUtility::Lua::ValidateTable(L, -1, logContext, nullptr, false))
		{
			continue;
		}

		bool isDoor = LogUtility::Lua::ReadBoolField(L, -1, "ISDOOR", logContext).value_or(false);
		if (isDoor)
		{
			int doorSlot = RoomConfigUtility::GetDoorSlotFromLua(L, -1, room.Shape, logContext);
			if (doorSlot != NO_DOOR_SLOT)
			{
				room.Doors |= 1 << doorSlot;
			}
		}
		else
		{
			auto roomSpawn = RoomConfigUtility::BuildSpawnFromLua(L, -1, room, logContext);
			if (!roomSpawn)
			{
				logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build spawn.");
			}
			else
			{
				roomSpawns.emplace_back(std::move(roomSpawn.value()));
			}
		}

		lua_pop(L, 1);
		logContext.pop_back();
	}

	if (!roomSpawns.empty())
	{
		room.SpawnCount = (uint16_t)roomSpawns.size();
		room.Spawns = new RoomSpawn[room.SpawnCount];

		for (size_t i = 0; i < room.SpawnCount; i++)
		{
			room.Spawns[i] = std::move(roomSpawns[i]);
		}
	}

	RoomConfigUtility::AssertRoomValidity(room);
	return room;
}

std::optional<RoomSpawn> RoomConfigUtility::BuildSpawnFromLua(lua_State* L, int index, RoomConfig_Room& room, LogContext& logContext) noexcept
{
	int absIndex = lua_absindex(L, index);

	if (!LogUtility::Lua::ValidateTable(L, absIndex, logContext, nullptr, false))
	{
		return std::nullopt;
	}

	auto posX = LogUtility::Lua::ReadIntegerField<int16_t>(L, index, "GRIDX", logContext, false);
	auto posY = LogUtility::Lua::ReadIntegerField<int16_t>(L, index, "GRIDY", logContext, false);

	if (!posX || !posY)
	{
		return std::nullopt;
	}

	RoomSpawn roomSpawn;

	//The game allows multiple spawns to have the same coordinates, so there is no check for duplicate entry coordinates in here.
	roomSpawn.X = posX.value();
	roomSpawn.Y = posY.value();

	roomSpawn.Entries = nullptr;
	roomSpawn.CountEntries = 0;

	size_t spawnEntriesCount = (size_t)lua_rawlen(L, absIndex);
	std::vector<RoomEntry> spawnEntries;
	spawnEntries.reserve(spawnEntriesCount);


	for (size_t i = 1; i <= spawnEntriesCount; ++i)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("spawn entry #%u -> ", i));
		lua_rawgeti(L, absIndex, i);

		auto spawnEntry = RoomConfigUtility::BuildSpawnEntryFromLua(L, -1, logContext);
		if (!spawnEntry)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build spawn entry.");
		}
		else
		{
			spawnEntries.emplace_back(std::move(spawnEntry.value()));
		}

		lua_pop(L, 1);
		logContext.pop_back();
	}

	if (spawnEntries.empty())
	{
		return roomSpawn;
	}

	roomSpawn.CountEntries = (uint16_t)spawnEntries.size();
	roomSpawn.Entries = new RoomEntry[roomSpawn.CountEntries];

	for (size_t i = 0; i < roomSpawn.CountEntries; i++)
	{
		roomSpawn.Entries[i] = std::move(spawnEntries[i]);
		RoomConfigUtility::FinalizeSpawnEntryInsertion(room, roomSpawn, roomSpawn.Entries[i]);
	}

	return roomSpawn;
}

static int RoomConfigUtility::GetDoorSlotFromLua(lua_State* L, int index, int shape, LogContext& logContext) noexcept
{
	int absIndex = lua_absindex(L, index);

	if (!LogUtility::Lua::ValidateTable(L, absIndex, logContext, nullptr, false))
	{
		return NO_DOOR_SLOT;
	}

	bool exists = LogUtility::Lua::ReadBoolField(L, index, "EXISTS", logContext).value_or(false);
	if (!exists)
	{
		return NO_DOOR_SLOT;
	}

	auto slot = LogUtility::Lua::ReadIntegerField<uint32_t>(L, index, "SLOT", logContext);
	if (slot)
	{
		if (LevelGenerator::has_shape_slot(shape, slot.value(), false))
		{
			return slot.value();
		}

		logContext.LogMessage(LogUtility::eLogType::WARN, REPENTOGON::StringFormat("provided door slot is invalid for shape %d: %d", shape, slot.value()).c_str());
	}

	auto posX = LogUtility::Lua::ReadIntegerField<int16_t>(L, index, "GRIDX", logContext, false);
	auto posY = LogUtility::Lua::ReadIntegerField<int16_t>(L, index, "GRIDY", logContext, false);

	if (!posX || !posY)
	{
		return NO_DOOR_SLOT;
	}

	int doorSlot = RoomConfig::get_door_from_position(posX.value(), posY.value(), shape);

	if (doorSlot <= NO_DOOR_SLOT || doorSlot >= NUM_DOOR_SLOTS)
	{
		logContext.LogMessage(LogUtility::eLogType::WARN, REPENTOGON::StringFormat("door slot position is invalid for shape %d: (%d, %d)", shape, posX.value(), posY.value()).c_str());
		doorSlot = NO_DOOR_SLOT;
	}

	return doorSlot;
}

std::optional<RoomEntry> RoomConfigUtility::BuildSpawnEntryFromLua(lua_State* L, int index, LogContext& logContext) noexcept
{
	int absIndex = lua_absindex(L, index);

	if (!LogUtility::Lua::ValidateTable(L, absIndex, logContext, nullptr, false))
	{
		return std::nullopt;
	}

	auto type = LogUtility::Lua::ReadIntegerField<int>(L, absIndex, "TYPE", logContext, false);
	if (!type)
	{
		return std::nullopt;
	}

	RoomEntry spawnEntry;

	spawnEntry.type = type.value();

	spawnEntry.variant = LogUtility::Lua::ReadIntegerField<uint32_t>(L, absIndex, "VARIANT", logContext).value_or(0);
	spawnEntry.subtype = LogUtility::Lua::ReadIntegerField<int>(L, absIndex, "SUBTYPE", logContext).value_or(0);
	spawnEntry.weight = LogUtility::Lua::ReadNumberField<float>(L, absIndex, "WEIGHT", logContext).value_or(0.0f);
	if (spawnEntry.weight < 0.0f)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, "invalid argument for \"WEIGHT\" (weight cannot be negative)");
		spawnEntry.weight = 0.0f;
	}


	return spawnEntry;
}

#pragma endregion

#pragma region Json

constexpr uint32_t ROOM_SERIALIZATION_VERSION = 1;
constexpr uint32_t SPAWN_SERIALIZATION_VERSION = 1;
constexpr uint32_t SPAWN_ENTRY_SERIALIZATION_VERSION = 1;

rapidjson::Value RoomConfigUtility::SerializeRoom(const RoomConfig_Room& room, rapidjson::Document::AllocatorType& allocator) noexcept
{
	RoomConfigUtility::AssertRoomValidity(room);
	rapidjson::Value jsonRoom(rapidjson::kObjectType);

	jsonRoom.AddMember("Version", ROOM_SERIALIZATION_VERSION, allocator);

	jsonRoom.AddMember("StageId", room.StageId, allocator);
	jsonRoom.AddMember("Mode", room.Mode, allocator);
	jsonRoom.AddMember("Type", room.Type, allocator);
	jsonRoom.AddMember("Variant", room.originalVariant, allocator);
	jsonRoom.AddMember("SubType", room.Subtype, allocator);
	rapidjson::Value name(room.Name.c_str(), room.Name.length(), allocator);
	jsonRoom.AddMember("Name", name, allocator);
	jsonRoom.AddMember("Difficulty", room.Difficulty, allocator);
	jsonRoom.AddMember("Weight", room.InitialWeight, allocator);
	jsonRoom.AddMember("Shape", room.Shape, allocator);

	rapidjson::Value jsonSpawns(rapidjson::kArrayType);
	for (size_t i = 0; i < room.SpawnCount; i++)
	{
		jsonSpawns.PushBack(RoomConfigUtility::SerializeRoomSpawn(room.Spawns[i], allocator), allocator);
	}
	jsonRoom.AddMember("Spawns", jsonSpawns, allocator);

	return jsonRoom;
}

rapidjson::Value RoomConfigUtility::SerializeRoomSpawn(const RoomSpawn& roomSpawn, rapidjson::Document::AllocatorType& allocator) noexcept
{
	RoomConfigUtility::AssertRoomSpawnValidity(roomSpawn);
	rapidjson::Value jsonSpawn(rapidjson::kObjectType);

	jsonSpawn.AddMember("Version", SPAWN_SERIALIZATION_VERSION, allocator);

	jsonSpawn.AddMember("X", roomSpawn.X, allocator);
	jsonSpawn.AddMember("Y", roomSpawn.Y, allocator);
	
	rapidjson::Value jsonSpawnEntries(rapidjson::kArrayType);
	for (size_t i = 0; i < roomSpawn.CountEntries; i++)
	{
		jsonSpawnEntries.PushBack(RoomConfigUtility::SerializeRoomSpawnEntry(roomSpawn.Entries[i], allocator), allocator);
	}
	jsonSpawn.AddMember("Entries", jsonSpawnEntries, allocator);

	return jsonSpawn;
}

rapidjson::Value RoomConfigUtility::SerializeRoomSpawnEntry(const RoomEntry& spawnEntry, rapidjson::Document::AllocatorType& allocator) noexcept
{
	RoomConfigUtility::AssertRoomSpawnEntryValidity(spawnEntry);
	rapidjson::Value jsonSpawnEntry(rapidjson::kObjectType);

	jsonSpawnEntry.AddMember("Version", SPAWN_ENTRY_SERIALIZATION_VERSION, allocator);

	jsonSpawnEntry.AddMember("Type", spawnEntry.type, allocator);
	jsonSpawnEntry.AddMember("Variant", spawnEntry.variant, allocator);
	jsonSpawnEntry.AddMember("SubType", spawnEntry.subtype, allocator);
	jsonSpawnEntry.AddMember("Weight", spawnEntry.weight, allocator);

	return jsonSpawnEntry;
}

static std::vector<RoomSpawn> deserialize_spawns_node(const rapidjson::Value& spawnsNode, RoomConfig_Room& room, LogContext& logContext) noexcept
{
	std::vector<RoomSpawn> roomSpawns;

	if (!LogUtility::Json::ValidateArray(spawnsNode, logContext, "Spawns"))
	{
		return roomSpawns;
	}

	size_t index = 0;
	roomSpawns.reserve(spawnsNode.Size());
	for (auto itr = spawnsNode.Begin(); itr != spawnsNode.End(); ++itr)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("spawn #%u -> ", index));
		auto spawn = RoomConfigUtility::DeserializeRoomSpawn(*itr, room, logContext);
		if (!spawn)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to deserialize spawn.");
		}
		else
		{
			roomSpawns.emplace_back(std::move(spawn.value()));
		}
		logContext.pop_back();
		++index;
	}

	return roomSpawns;
}

static std::vector<RoomEntry> deserialize_spawn_entries_node(const rapidjson::Value& spawnEntriesNode, LogContext& logContext) noexcept
{
	std::vector<RoomEntry> spawnEntries;

	if (!LogUtility::Json::ValidateArray(spawnEntriesNode, logContext, "Entries"))
	{
		return spawnEntries;
	}

	size_t index = 0;
	spawnEntries.reserve(spawnEntriesNode.Size());
	for (auto itr = spawnEntriesNode.Begin(); itr != spawnEntriesNode.End(); ++itr)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("spawn entry #%u -> ", index));
		auto spawnEntry = RoomConfigUtility::DeserializeRoomSpawnEntry(*itr, logContext);
		if (!spawnEntry)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to deserialize spawn entry.");
		}
		else
		{
			spawnEntries.emplace_back(std::move(spawnEntry.value()));
		}
		logContext.pop_back();
		++index;
	}

	return spawnEntries;
}

std::optional<RoomConfig_Room> RoomConfigUtility::DeserializeRoom(const rapidjson::Value& roomNode, LogContext& logContext) noexcept
{
	if (!LogUtility::Json::ValidateObject(roomNode, logContext))
	{
		return std::nullopt;
	}

	auto version = LogUtility::Json::ReadIntegerMember<uint32_t>(roomNode, "Version", logContext, false); if (!version) { return std::nullopt; }

	if (version.value() != ROOM_SERIALIZATION_VERSION)
	{
		logContext.LogMessage(LogUtility::eLogType::INFO, REPENTOGON::StringFormat("serialization version mismatch, current version %u, room version %u, skipping deserialization.", ROOM_SERIALIZATION_VERSION, version.value()).c_str());
		return std::nullopt;
	}

	auto stageId = LogUtility::Json::ReadIntegerMember<int>(roomNode, "StageId", logContext, false); if (!stageId) { return std::nullopt; }

	if (!IsStageValid(stageId.value()))
	{
		LogUtility::Json::LogInvalidArg(logContext, REPENTOGON::StringFormat("invalid stage id: %d", stageId.value()).c_str(), "StageId");
		return std::nullopt;
	}

	auto mode = LogUtility::Json::ReadIntegerMember<int>(roomNode, "Mode", logContext, false); if (!mode) { return std::nullopt; }

	if (!IsModeValid(mode.value()))
	{
		LogUtility::Json::LogInvalidArg(logContext, REPENTOGON::StringFormat("invalid mode: %d", mode.value()).c_str(), "Mode");
		return std::nullopt;
	}

	RoomConfig_Room room;

	room.StageId = stageId.value();
	room.Mode = mode.value();

	room.Type = LogUtility::Json::ReadIntegerMember<int>(roomNode, "Type", logContext, false).value_or(ROOM_DEFAULT);

	if (!IsRoomTypeValid(room.Type))
	{
		LogUtility::Json::LogInvalidArg(logContext, REPENTOGON::StringFormat("invalid room type: %d", room.Type).c_str(), "Mode");
		room.Type = ROOM_DEFAULT;
	}

	room.originalVariant = LogUtility::Json::ReadIntegerMember<uint32_t>(roomNode, "Variant", logContext, false).value_or(0);
	room.Variant = room.originalVariant;
	room.Subtype = LogUtility::Json::ReadIntegerMember<int>(roomNode, "SubType", logContext, false).value_or(0);
	room.Name = LogUtility::Json::ReadStringMember(roomNode, "Name", logContext, false).value_or("");
	room.Difficulty = LogUtility::Json::ReadIntegerMember<int>(roomNode, "Difficulty", logContext, false).value_or(0);
	room.Weight = LogUtility::Json::ReadNumberMember<float>(roomNode, "Weight", logContext, false).value_or(0.0f);

	if (room.Weight < 0.0f)
	{
		LogUtility::Json::LogInvalidArg(logContext, "weight cannot be negative", "Weight");
		room.Weight = 0.0f;
	}

	room.Shape = LogUtility::Json::ReadIntegerMember<int>(roomNode, "Shape", logContext, false).value_or(ROOMSHAPE_1x1);

	if (!IsShapeValid(room.Shape))
	{
		LogUtility::Json::LogInvalidArg(logContext, REPENTOGON::StringFormat("invalid room shape: %d", room.Shape).c_str(), "Shape");
		room.Shape = ROOMSHAPE_1x1;
	}

	auto& shapeDimensions = RoomConfigUtility::GetShapeDimensions(room.Shape);
	room.Width = shapeDimensions.first;
	room.Height = shapeDimensions.second;

	room.Flags = 0;
	room.Spawns = nullptr;
	room.SpawnCount = 0;

	auto it = roomNode.FindMember("Spawns");

	if (it == roomNode.MemberEnd())
	{
		LogUtility::Json::LogInvalidArg(logContext, "member not found", "Spawns");
	}
	else
	{
		auto spawns = deserialize_spawns_node(it->value, room, logContext);
		if (!spawns.empty())
		{
			room.SpawnCount = (uint16_t)spawns.size();
			room.Spawns = new RoomSpawn[room.SpawnCount];
			for (size_t i = 0; i < room.SpawnCount; i++)
			{
				room.Spawns[i] = std::move(spawns[i]);
			}
		}
	}

	RoomConfigUtility::AssertRoomValidity(room);
	return room;
}

std::optional<RoomSpawn> RoomConfigUtility::DeserializeRoomSpawn(const rapidjson::Value& spawnNode, RoomConfig_Room& room, LogContext& logContext) noexcept
{
	if (!LogUtility::Json::ValidateObject(spawnNode, logContext))
	{
		return std::nullopt;
	}

	auto version = LogUtility::Json::ReadIntegerMember<uint32_t>(spawnNode, "Version", logContext, false); if (!version) { return std::nullopt; }

	if (version.value() != SPAWN_SERIALIZATION_VERSION)
	{
		logContext.LogMessage(LogUtility::eLogType::INFO, REPENTOGON::StringFormat("serialization version mismatch, current version %u, spawn version %u.", SPAWN_SERIALIZATION_VERSION, version.value()).c_str());
		return std::nullopt;
	}

	auto posX = LogUtility::Json::ReadIntegerMember<int>(spawnNode, "X", logContext, false);
	auto posY = LogUtility::Json::ReadIntegerMember<int>(spawnNode, "Y", logContext, false);

	if (!posX || !posY)
	{
		return std::nullopt;
	}

	RoomSpawn roomSpawn;
	roomSpawn.X = posX.value();
	roomSpawn.Y = posY.value();

	roomSpawn.Entries = nullptr;
	roomSpawn.CountEntries = 0;

	auto it = spawnNode.FindMember("Entries");

	if (it == spawnNode.MemberEnd())
	{
		LogUtility::Json::LogInvalidArg(logContext, "member not found", "Entries");
	}
	else
	{
		auto spawnEntries = deserialize_spawn_entries_node(it->value, logContext);
		if (!spawnEntries.empty())
		{
			roomSpawn.CountEntries = (uint16_t)spawnEntries.size();
			roomSpawn.Entries = new RoomEntry[roomSpawn.CountEntries];
			for (size_t i = 0; i < roomSpawn.CountEntries; i++)
			{
				roomSpawn.Entries[i] = std::move(spawnEntries[i]);
				RoomConfigUtility::FinalizeSpawnEntryInsertion(room, roomSpawn, roomSpawn.Entries[i]);
			}
		}
	}

	RoomConfigUtility::AssertRoomSpawnValidity(roomSpawn);
	return roomSpawn;
}

std::optional<RoomEntry> RoomConfigUtility::DeserializeRoomSpawnEntry(const rapidjson::Value& spawnEntryNode, LogContext& logContext) noexcept
{
	if (!LogUtility::Json::ValidateObject(spawnEntryNode, logContext))
	{
		return std::nullopt;
	}

	auto version = LogUtility::Json::ReadIntegerMember<uint32_t>(spawnEntryNode, "Version", logContext, false); if (!version) { return std::nullopt; }

	if (version.value() != SPAWN_ENTRY_SERIALIZATION_VERSION)
	{
		logContext.LogMessage(LogUtility::eLogType::INFO, REPENTOGON::StringFormat("serialization version mismatch, current version %u, spawn entry version %u.", SPAWN_ENTRY_SERIALIZATION_VERSION, version.value()).c_str());
		return std::nullopt;
	}

	auto type = LogUtility::Json::ReadIntegerMember<uint32_t>(spawnEntryNode, "Type", logContext, false); if (!type) { return std::nullopt; }
	auto variant = LogUtility::Json::ReadIntegerMember<uint32_t>(spawnEntryNode, "Variant", logContext, false); if (!variant) { return std::nullopt; }

	RoomEntry spawnEntry;

	spawnEntry.type = type.value();
	spawnEntry.variant = variant.value();
	spawnEntry.subtype = LogUtility::Json::ReadIntegerMember<uint16_t>(spawnEntryNode, "SubType", logContext, false).value_or(0);
	spawnEntry.weight = LogUtility::Json::ReadNumberMember<float>(spawnEntryNode, "Weight", logContext, false).value_or(0.0f);
	if (spawnEntry.weight < 0.0f)
	{
		LogUtility::Json::LogInvalidArg(logContext, "weight cannot be negative", "Weight");
		spawnEntry.weight = 0.0f;
	}

	RoomConfigUtility::AssertRoomSpawnEntryValidity(spawnEntry);
	return spawnEntry;
}

#pragma endregion

#ifndef NDEBUG

#pragma region Unit Tests

#include <fstream>
#include <HookSystem.h>

#include <Windows.h>

static RoomConfig_Room s_TestRoom;

static std::string get_path_string(const std::filesystem::path& path)
{
	auto& string = path.native();

	int size_needed = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, string.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size_needed == 0) return "";

	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, string.c_str(), -1, result.data(), size_needed, "?", nullptr);
	result.pop_back(); // Remove null terminator
	return result;
}

static std::optional<std::ofstream> open_file_for_writing(std::filesystem::path filePath, Error& error, std::ios_base::openmode mode = 0) noexcept
{
	error.reset();
	assert((mode & std::ios::in) == 0);

	std::filesystem::path directory = filePath.parent_path();

	std::error_code errorCode;
	if (!directory.empty() && !std::filesystem::exists(directory, errorCode))
	{
		if (errorCode)
		{
			error = REPENTOGON::StringFormat("failed to check existence of parent directory: %s", errorCode.message().c_str());
			return std::nullopt;
		}

		std::filesystem::create_directories(directory, errorCode);
		if (errorCode)
		{
			error = REPENTOGON::StringFormat("failed to create parent directory: %s", errorCode.message().c_str());
			return std::nullopt;
		}
	}

	std::ofstream file(filePath, mode);
	if (!file)
	{
		error = REPENTOGON::StringFormat("failed to open file for writing: \"%s\"", get_path_string(filePath).c_str());
		return std::nullopt;
	}

	return file;
}

static inline rapidjson::Document get_json_document(const std::string& filePath, Error& error) noexcept
{
	error.reset();
	rapidjson::Document doc;

	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		error = "Failed to open file \"" + filePath + "\"";
		return doc;
	}

	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		error = "Failed to parse json file \"" + filePath + "\"";
		return doc;
	}

	return doc;
}

LUA_FUNCTION(Lua_Debug_GetTestRoom)
{
	lua::luabridge::UserdataPtr::push(L, &s_TestRoom, lua::GetMetatableKey(lua::Metatables::CONST_ROOM_CONFIG_ROOM));
	return 1;
}

static void debug_build(lua_State* L, int index, LogContext& logContext)
{
	int absIndex = lua_absindex(L, index);

	size_t size = (size_t)lua_rawlen(L, absIndex);

	for (size_t i = 1; i <= size; i++)
	{
		logContext.emplace_back(REPENTOGON::StringFormat("room #%u -> ", i));
		lua_rawgeti(L, absIndex, i);

		auto room = RoomConfigUtility::BuildRoomFromLua(L, -1, logContext);
		if (!room)
		{
			logContext.LogMessage(LogUtility::eLogType::ERROR, "unable to build room.");
		}
		else
		{
			s_TestRoom = std::move(room.value());
		}
		lua_pop(L, 1);
		logContext.pop_back();
	}
}

LUA_FUNCTION(Lua_Debug_Build)
{
	if (!lua_istable(L, 1))
	{
		return luaL_argerror(L, 1, REPENTOGON::Lua::GenerateInvalidTypeMessage(L, 1, "table").c_str());
	}

	bool expectErrors = !lua_isnoneornil(L, 2) ? lua_toboolean(L, 2) : false;

	LogContext logContext;
	logContext.emplace_back("[DEBUG] Build Lua Room: ");

	debug_build(L, 1, logContext);

	assert((logContext.GetLoggedMessagesCount(LogUtility::eLogType::ERROR) + logContext.GetLoggedMessagesCount(LogUtility::eLogType::WARN) > 0) == expectErrors);

	return 0;
}

LUA_FUNCTION(Lua_Debug_Serialize)
{
	Error error;
	auto file = open_file_for_writing("test_room.json", error);
	if (error)
	{
		ZHL::Log("%s", error.value().c_str());
		return 0;
	}

	rapidjson::MemoryPoolAllocator<> allocator; // initialize allocator to not create rapidjson::Document
	rapidjson::Value roomValue = RoomConfigUtility::SerializeRoom(s_TestRoom, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	roomValue.Accept(writer);

	file.value() << buffer.GetString() << std::endl;

	return 0;
}

static void debug_deserialize(LogContext& logContext)
{
	Error error;
	auto doc = get_json_document("test_room.json", error);
	if (error)
	{
		logContext.LogMessage(LogUtility::eLogType::ERROR, error.value().c_str());
		return;
	}

	if (!LogUtility::Json::ValidateObject(doc, logContext))
	{
		return;
	}

	auto room = RoomConfigUtility::DeserializeRoom(doc, logContext);
	if (room)
	{
		s_TestRoom = std::move(room.value());
	}
}

LUA_FUNCTION(Lua_Debug_Deserialize)
{
	bool expectErrors = !lua_isnoneornil(L, 1) ? lua_toboolean(L, 1) : false;

	LogContext logContext;
	logContext.emplace_back("[DEBUG] deserializing test_room: ");
	
	debug_deserialize(logContext);
	
	assert((logContext.GetLoggedMessagesCount(LogUtility::eLogType::ERROR) + logContext.GetLoggedMessagesCount(LogUtility::eLogType::WARN) > 0) == expectErrors);

	return 0;
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
	lua_setfield(L, -2, "RoomConfigUtility");
	lua_getfield(L, -1, "RoomConfigUtility");

	lua::TableAssoc(L, "GetTestRoom", Lua_Debug_GetTestRoom);
	lua::TableAssoc(L, "Build", Lua_Debug_Build);
	lua::TableAssoc(L, "Serialize", Lua_Debug_Serialize);
	lua::TableAssoc(L, "Deserialize", Lua_Debug_Deserialize);

	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfigUtilityUnitTests(_state);
}

#pragma endregion

#endif // !NDEBUG