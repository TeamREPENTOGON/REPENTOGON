#include <memory>
#include <vector>
#include <utility>

#include "IsaacRepentance.h"
#include "document.h" // rapidjson
#include "EntitySaveStateManagement.h"
#include "../Patches/ItemSpoofSystem.h"

// Contains the representation and handling of REPENTOGON-internal entity save states.
// Piggybacks on EntitySaveStateManagement, being controlled by that logic and using the same IDs.

namespace EntitySaveStateEx {

// Abtract base class for save state. Type-specific derived classes implemented below.
class EntitySaveStateEx {
  public:
	EntitySaveStateEx() {};

	virtual int GetType() = 0;

	virtual void Save(Entity& entity) = 0;
	virtual void Restore(Entity& entity) = 0;

	virtual void Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) = 0;
	virtual void Deserialize(const rapidjson::Value& node) = 0;

	virtual std::unique_ptr<EntitySaveStateEx> Copy() = 0;
};

class PlayerSaveStateEx : public EntitySaveStateEx {
  public:
	PlayerSaveStateEx() {};

	int GetType() { return ENTITY_PLAYER; }

	void Save(Entity& entity) override;
	void Restore(Entity& entity) override;

	void Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) override;
	void Deserialize(const rapidjson::Value& node) override;

	std::unique_ptr<EntitySaveStateEx> Copy() override {
		return std::make_unique<PlayerSaveStateEx>(*this);
	}

	// For twins, the index of the main twin gets written here.
	// We don't rely on their indices being the same after load - just that both twins have the same value here.
	// We'll use this to restore the twin relationship after load (see ASMPlayerManager.cpp).
	int _mainTwinIndex = -1;

    ItemSpoofSystem::InnateItemGroups _innateCollectibles;
    ItemSpoofSystem::InnateItemGroups _innateTrinkets;
};

class FamiliarSaveStateEx : public EntitySaveStateEx {
 public:
	FamiliarSaveStateEx() {};

	int GetType() { return ENTITY_FAMILIAR; }

	void Save(Entity& entity) override;
	void Restore(Entity& entity) override;

	void Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) override;
	void Deserialize(const rapidjson::Value& node) override;

	std::unique_ptr<EntitySaveStateEx> Copy() override {
		return std::make_unique<FamiliarSaveStateEx>(*this);
	}
};

class PickupSaveStateEx : public EntitySaveStateEx {
  public:
	PickupSaveStateEx() {};

	int GetType() { return ENTITY_PICKUP; }

	void Save(Entity& entity) override;
	void Restore(Entity& entity) override;

	void Serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) override;
	void Deserialize(const rapidjson::Value& node) override;

	std::unique_ptr<EntitySaveStateEx> Copy() override {
		return std::make_unique<PickupSaveStateEx>(*this);
	}
};

// Reset the states with the specified ESSM IDs.
void ClearSaveStates(const std::vector<uint32_t>& idsToClear);

// Copy the specified save states (sourceId, targetId).
void CopySaveStates(const std::vector<std::pair<uint32_t, uint32_t>>& idsToCopy);

// Store the current state for the given entity under the given ESSM ID.
void SaveEntity(Entity& entity, uint32_t id);

// Load the state for the given entity from the given ESSM ID.
void RestoreEntity(Entity& entity, uint32_t id);

// Write the current states to a json file.
// writeEntityIdPairs contains info for mapping ESSM IDs to serialized IDs to stay aligned with ESSM.
void WriteSaveFile(const std::string& filename, const std::vector<EntitySaveStateManagement::detail::SaveData::SerializeIdPair>& writeEntityIdPairs);

// Wipe the current states and load state from a json file.
// restoreEntityIdPairs contains info for mapping serialized IDs to their new ESSM IDs stay aligned with ESSM.
void LoadSaveFile(const std::string& filename, const std::vector<EntitySaveStateManagement::detail::SaveData::SerializeIdPair>& restoreEntityIdPairs);

// Delete any existing json state file.
void DeleteSaveFile(const std::string& filename);

}  // EntitySaveStateEx
