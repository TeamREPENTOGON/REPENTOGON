#include "EntitySaveStateEx.h"

#include <memory>
#include <unordered_map>
#include <fstream>

#include "IsaacRepentance.h"
#include "document.h" // rapidjson
#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson
#include "../MiscFunctions.h"
#include "../Patches/EntityPlus.h"
#include "../Patches/ItemSpoofSystem.h"

namespace EntitySaveStateEx {

namespace {

// Map within which we store all of the current save states.
std::unordered_map<uint32_t, std::unique_ptr<EntitySaveStateEx>> ENTITY_SAVE_STATES;

// Identifies the current "format" of the json file. Incrementing this if we ever have to
// make a significant change to the format of the json file can allow us to invalidate any
// pre-existing states. Updating this is not necessary to add new types or data.
constexpr uint32_t JSON_FORMAT_VERSION = 1;

// JSON constants
constexpr char kVersion[] = "Version";
constexpr char kChecksum[] = "Checksum";
constexpr char kID[] = "ID";
constexpr char kType[] = "Type";
constexpr char kData[] = "Data";
constexpr char kEntitySaveStates[] = "EntitySaveStates";

// Fetches a pointer to the EntitySaveStateEx for the given ESSM ID, initializing it
// for the provided type if necessary. If `reset` is true, will always delete any
// existing data and re-initialize it. Result can be nullptr if the entity type is not
// supported by EntitySaveStateEx. Intended for internal use only.
EntitySaveStateEx* GetEntitySaveStateEx(const uint32_t id, const int type, const bool reset) {
	if (reset || !ENTITY_SAVE_STATES[id] || ENTITY_SAVE_STATES[id]->GetType() != type) {
		if (type == ENTITY_PLAYER) {
			ENTITY_SAVE_STATES[id] = std::make_unique<PlayerSaveStateEx>();
		} else if (type == ENTITY_FAMILIAR) {
			ENTITY_SAVE_STATES[id] = std::make_unique<FamiliarSaveStateEx>();
		} else if (type == ENTITY_PICKUP) {
			ENTITY_SAVE_STATES[id] = std::make_unique<PickupSaveStateEx>();
		} else {
			ENTITY_SAVE_STATES[id] = nullptr;
		}
	}
	return ENTITY_SAVE_STATES[id].get();
}

}  // namespace

// ----------------------------------------------------------------------------------------------------
// EntityPlayer

void PlayerSaveStateEx::Save(Entity& entity) {
	Entity_Player* player = entity.ToPlayer();
	if (!player) return;

	Entity_Player* twin = player->GetTwinPlayer();
	if (twin) {
		// The "main twin" is just whichever has the lower index.
		_mainTwinIndex = std::min(player->_playerIndex, twin->_playerIndex);
	} else {
		_mainTwinIndex = -1;
	}

    if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
        _innateCollectibles = playerPlus->itemSpoofs.GetCollectibleSpoof().SaveInnateItems();
        _innateTrinkets = playerPlus->itemSpoofs.GetTrinketSpoof().SaveInnateItems();
    }
}

void PlayerSaveStateEx::Restore(Entity& entity) {
	Entity_Player* player = entity.ToPlayer();
	if (!player) return;

	if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player)) {
		playerPlus->restoreTwinID = _mainTwinIndex;
        playerPlus->itemSpoofs.GetCollectibleSpoof().LoadInnateItems(_innateCollectibles);
        playerPlus->itemSpoofs.GetTrinketSpoof().LoadInnateItems(_innateTrinkets);
	}
}

void PlayerSaveStateEx::Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) {
	if (_mainTwinIndex > -1) {
		node.AddMember("mainTwinIndex", _mainTwinIndex, allocator);
	}

    if (!_innateCollectibles.empty()) {
        ZHL::Log("[PlayerSaveStateEx] [INFO] - Serializing Innate Collectibles...");
        node.AddMember("innateCollectibles", ItemSpoofSystem::SerializeInnateItems(_innateCollectibles, allocator), allocator);
    }
    if (!_innateTrinkets.empty()) {
        ZHL::Log("[PlayerSaveStateEx] [INFO] - Serializing Innate Trinkets...");
        node.AddMember("innateTrinkets", ItemSpoofSystem::SerializeInnateItems(_innateTrinkets, allocator), allocator);
    }
}

void PlayerSaveStateEx::Deserialize(const rapidjson::Value& node) {
	if (node.HasMember("mainTwinIndex") && node["mainTwinIndex"].IsInt()) {
		_mainTwinIndex = node["mainTwinIndex"].GetInt();
	} else {
		_mainTwinIndex = -1;
	}

    if (node.HasMember("innateCollectibles")) {
        ZHL::Log("[PlayerSaveStateEx] [INFO] - Deserializing Innate Collectibles...");
        _innateCollectibles = ItemSpoofSystem::DeserializeInnateItems(node["innateCollectibles"]);
    }
    if (node.HasMember("innateTrinkets")) {
        ZHL::Log("[PlayerSaveStateEx] [INFO] - Deserializing Innate Trinkets...");
        _innateTrinkets = ItemSpoofSystem::DeserializeInnateItems(node["innateTrinkets"]);
    }
}

// ----------------------------------------------------------------------------------------------------
// EntityFamiliar (stub)

void FamiliarSaveStateEx::Save(Entity& entity) {
	//Entity_Familiar* familiar = entity.ToFamiliar();
	//if (!familiar) return;
}
void FamiliarSaveStateEx::Restore(Entity& entity) {
	//Entity_Familiar* familiar = entity.ToFamiliar();
	//if (!familiar) return;
}
void FamiliarSaveStateEx::Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) {}
void FamiliarSaveStateEx::Deserialize(const rapidjson::Value& node) {}

// ----------------------------------------------------------------------------------------------------
// EntityPickup (stub)

void PickupSaveStateEx::Save(Entity& entity) {
	//Entity_Pickup* pickup = entity.ToPickup();
	//if (!pickup) return;
}
void PickupSaveStateEx::Restore(Entity& entity) {
	//Entity_Pickup* pickup = entity.ToPickup();
	//if (!pickup) return;
}
void PickupSaveStateEx::Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) {}
void PickupSaveStateEx::Deserialize(const rapidjson::Value& node) {}

// ----------------------------------------------------------------------------------------------------
// General Handling

void CopySaveStates(const std::vector<std::pair<uint32_t, uint32_t>>& idsToCopy) {
	for (const auto [sourceId, targetId] : idsToCopy) {
		if (ENTITY_SAVE_STATES[sourceId]) {
			ENTITY_SAVE_STATES[targetId] = ENTITY_SAVE_STATES[sourceId]->Copy();
		} else {
			ENTITY_SAVE_STATES.erase(targetId);
		}
	}
}

void ClearSaveStates(const std::vector<uint32_t>& idsToClear) {
	for (const uint32_t id : idsToClear) {
		ENTITY_SAVE_STATES.erase(id);
	}
}

void SaveEntity(Entity& entity, const uint32_t id) {
	if (EntitySaveStateEx* state = GetEntitySaveStateEx(id, entity._type, /*reset=*/true)) {
		state->Save(entity);
	}
}

void RestoreEntity(Entity& entity, const uint32_t id) {
	if (EntitySaveStateEx* state = GetEntitySaveStateEx(id, entity._type, /*reset=*/false)) {
		state->Restore(entity);
	}
}

// ----------------------------------------------------------------------------------------------------
// Save File Handling

static inline std::string GetSaveFilePath(const std::string& filename) {
	return REPENTOGON::StringFormat("%s/EntitySaveStateManagement/ex_%s.json", REPENTOGON::GetRepentogonDataPath(), filename.c_str());
}

void WriteSaveFile(const std::string& filename, const std::vector<EntitySaveStateManagement::detail::SaveData::SerializeIdPair>& writeEntityIdPairs) {
	std::filesystem::path filePath = GetSaveFilePath(filename);
	ZHL::Log("[EntitySaveStateEx] [INFO] - Writing state to file: %s\n", filePath.string().c_str());

	// Initialize directory, if needed
	try {
		std::filesystem::path directory = filePath.parent_path();
		if (!directory.empty() && !std::filesystem::exists(directory)) {
			std::filesystem::create_directories(directory);
		}
	} catch (const std::filesystem::filesystem_error& e) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Filesystem error attempting to create directories for state: %s\n", e.what());
		return;
	}

	// Open file
	std::ofstream file(filePath);
	if (!file) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Failed to open file for writing.\n");
		return;
	}

	// Initialize JSON document
	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember(kVersion, JSON_FORMAT_VERSION, doc.GetAllocator());
	doc.AddMember(kChecksum, g_Manager->_gamestate._checksum, doc.GetAllocator());

	// Create EntitySaveStates
	rapidjson::Value entityStates(rapidjson::kArrayType);
	for (const auto& idPair : writeEntityIdPairs) {
		if (EntitySaveStateEx* state = ENTITY_SAVE_STATES[idPair.realId].get()) {
			rapidjson::Value serializedData(rapidjson::kObjectType);
			state->Serialize(serializedData, doc.GetAllocator());
			if (!serializedData.ObjectEmpty()) {
				rapidjson::Value entry(rapidjson::kObjectType);
				entry.AddMember(kID, idPair.serializeId, doc.GetAllocator());
				entry.AddMember(kType, state->GetType(), doc.GetAllocator());
				entry.AddMember(kData, serializedData, doc.GetAllocator());
				entityStates.PushBack(entry, doc.GetAllocator());
			}
		}
	}
	doc.AddMember(kEntitySaveStates, entityStates, doc.GetAllocator());

	// Write the file
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	file << buffer.GetString() << std::endl;

	ZHL::Log("[EntitySaveStateEx] [INFO] - Successfully wrote state.\n");
}

void LoadSaveFile(const std::string& filename, const std::vector<EntitySaveStateManagement::detail::SaveData::SerializeIdPair>& restoreEntityIdPairs) {
	// Clear any existing data.
	ENTITY_SAVE_STATES.clear();

	std::filesystem::path filePath = GetSaveFilePath(filename);
	if (!std::filesystem::exists(filePath)) {
		// Nothing to restore.
		return;
	}

	ZHL::Log("[EntitySaveStateEx] [INFO] - Loading state from file: %s\n", filePath.string().c_str());

	rapidjson::Document doc;

	// Open file for reading
	std::ifstream stream(filePath);
	if (!stream) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Failed to open file for reading.\n");
		return;
	}

	// Read and parse the json from the file
	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	doc.Parse(content.c_str());

	if (doc.HasParseError()) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Failed to parse json file: %d\n", doc.GetParseError());
		return;
	}
	if (!doc.IsObject()) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Root of json file is not a valid Object (actual type %d)\n", doc.GetType());
		return;
	}

	// Validate version
	if (!doc.HasMember(kVersion) || !doc[kVersion].IsUint() || doc[kVersion].GetUint() != JSON_FORMAT_VERSION) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - State version mismatch, data will not be loaded.\n");
		return;
	}

	// Validate checksum
	if (!doc.HasMember(kChecksum) || !doc[kChecksum].IsUint() || doc[kChecksum].GetUint() != g_Manager->_gamestate._checksum) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Checksum mismatch, data will not be loaded.\n");
		return;
	}

	// Process restoreEntityIdPairs into a map to more easily reference later.
	std::unordered_map<uint32_t, uint32_t> idMap;
	for (const auto& idPair : restoreEntityIdPairs) {
		idMap[idPair.serializeId] = idPair.realId;
	}

	// Read the EntitySaveStates
	if (!doc.HasMember(kEntitySaveStates) || !doc[kEntitySaveStates].IsArray()) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - File contains no save state array!\n");
		return;
	}
	for (const auto& jsonSaveState : doc[kEntitySaveStates].GetArray()) {
		if (!jsonSaveState.IsObject()) {
			ZHL::Log("[EntitySaveStateEx] [ERROR] - Skipping non-object entry!\n");
			continue;
		}

		if (!jsonSaveState.HasMember(kID) || !jsonSaveState[kID].IsInt()) {
			ZHL::Log("[EntitySaveStateEx] [ERROR] - Skipping entry with no ID!\n");
			continue;
		}
		const int serializeId = jsonSaveState[kID].GetInt();

		if (idMap.find(serializeId) == idMap.end()) {
			ZHL::Log("[EntitySaveStateEx] [ERROR] - Skipping entry with unexpected ID!\n");
			continue;
		}
		const int realId = idMap[serializeId];

		if (!jsonSaveState.HasMember(kType) || !jsonSaveState[kType].IsInt()) {
			ZHL::Log("[EntitySaveStateEx] [ERROR] - Skipping entry with no Type!\n");
			continue;
		}
		const int type = jsonSaveState[kType].GetInt();

		if (!jsonSaveState.HasMember(kData) || !jsonSaveState[kData].IsObject()) {
			ZHL::Log("[EntitySaveStateEx] [ERROR] - Skipping entry with no Data!\n");
			continue;
		}
		const auto& jsonData = jsonSaveState[kData];

		if (EntitySaveStateEx* state = GetEntitySaveStateEx(realId, type, /*reset=*/true)) {
			state->Deserialize(jsonData);
		}
	}

	ZHL::Log("[EntitySaveStateEx] [INFO] - Successfully loaded state.\n");
}

void DeleteSaveFile(const std::string& filename) {
	try {
		std::filesystem::path filePath = GetSaveFilePath(filename);
		if (std::filesystem::exists(filePath)) {
			ZHL::Log("[EntitySaveStateEx] [INFO] - Deleting state: %s\n", filePath.string().c_str());
			std::filesystem::remove(filePath);
			ZHL::Log("[EntitySaveStateEx] [INFO] - Successfully deleted state.\n");
		}
	} catch (const std::filesystem::filesystem_error& e) {
		ZHL::Log("[EntitySaveStateEx] [ERROR] - Filesystem error attempting to delete save file: %s\n", e.what());
	}
}

}  // EntitySaveStateEx
