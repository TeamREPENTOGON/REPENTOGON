#include "IsaacRepentance.h"
#include "../../RoomConfigUtility.h"
#include "../../VirtualRoomConfig/VirtualRoomSetManager.h"

struct LuaRoomEntryDesc {
	int type;
	uint32_t variant;
	int subtype;
	float weight;
};

struct LuaRoomSpawnDesc {
	int16_t x;
	int16_t y;
	uint32_t entryCount;
	LuaRoomEntryDesc* entries;
};

struct LuaRoomDesc {
	int type;
	uint32_t variant;
	int subtype;
	int difficulty;
	const char* name;
	float weight;
	int shape;
	uint32_t doors;
	uint32_t spawnCount;
	LuaRoomSpawnDesc* spawns;
};

MOD_EXPORT unsigned int L_RoomConfigSet_GetVirtualSize(unsigned int id) {
	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	return virtualSet.size();
}

MOD_EXPORT RoomConfig_Room* L_RoomConfigSet_GetVirtualRoom(unsigned int id, unsigned int index) {
	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	return virtualSet[index];
}

MOD_EXPORT unsigned int L_RoomConfigSet_BeginAddRooms(unsigned int id) {
	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	return VirtualRoomSetManager::detail::BeginAddRooms(virtualSet);
}

MOD_EXPORT RoomConfig_Room* L_RoomConfigSet_AddRoom(unsigned int id, const LuaRoomDesc* desc) {
	RoomConfig_Room room;

	room.Type = desc->type;
	room.originalVariant = desc->variant;
	room.Variant = room.originalVariant;
	room.Subtype = desc->subtype;
	room.Difficulty = desc->difficulty;
	room.Name = desc->name;
	room.Weight = desc->weight;
	room.InitialWeight = room.Weight;

	room.Shape = desc->shape;
	auto& shapeDimensions = RoomConfigUtility::GetShapeDimensions(room.Shape);
	room.Width = shapeDimensions.first;
	room.Height = shapeDimensions.second;

	room.Flags = 0;
	room.SpawnCount = 0;
	room.Spawns = nullptr;
	room.Doors |= desc->doors;

	std::vector<RoomSpawn> roomSpawns;
	roomSpawns.reserve(desc->spawnCount);

	for (uint32_t i = 0; i < desc->spawnCount; i++) {
		const LuaRoomSpawnDesc& spawnDesc = desc->spawns[i];
		RoomSpawn roomSpawn;
		roomSpawn.X = spawnDesc.x;
		roomSpawn.Y = spawnDesc.y;
		roomSpawn.Entries = nullptr;
		roomSpawn.CountEntries = 0;

		if (spawnDesc.entryCount > 0) {
			roomSpawn.CountEntries = (uint8_t)spawnDesc.entryCount;
			roomSpawn.Entries = new RoomEntry[roomSpawn.CountEntries];

			for (size_t j = 0; j < roomSpawn.CountEntries; j++) {
				const LuaRoomEntryDesc& entryDesc = spawnDesc.entries[j];
				RoomEntry& spawnEntry = roomSpawn.Entries[j];
				spawnEntry.type = entryDesc.type;
				spawnEntry.variant = entryDesc.variant;
				spawnEntry.subtype = entryDesc.subtype;
				spawnEntry.weight = entryDesc.weight;
				RoomConfigUtility::FinalizeSpawnEntryInsertion(room, roomSpawn, spawnEntry);
			}
		}

		roomSpawns.emplace_back(std::move(roomSpawn));
	}

	if (!roomSpawns.empty()) {
		room.SpawnCount = (uint16_t)roomSpawns.size();
		room.Spawns = new RoomSpawn[room.SpawnCount];

		for (size_t i = 0; i < room.SpawnCount; i++) {
			room.Spawns[i] = std::move(roomSpawns[i]);
		}
	}

	RoomConfigUtility::AssertRoomValidity(room);

	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	return VirtualRoomSetManager::detail::AddRoom(virtualSet, room);
}

MOD_EXPORT void L_RoomConfigSet_EndAddRooms(unsigned int id, unsigned int begin) {
	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	VirtualRoomSetManager::detail::EndAddRooms(virtualSet, begin);
}

MOD_EXPORT unsigned int L_RoomConfigSet_AddStbRooms(unsigned int id, const char* fileName) {
	VirtualRoomSet virtualSet = VirtualRoomSetManager::detail::FromId(id);
	return VirtualRoomSetManager::detail::AddStbRooms(virtualSet, fileName);
}
