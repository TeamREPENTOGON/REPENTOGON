#pragma once

#include <unordered_map>

#include "IsaacRepentance.h"
#include "EvaluateStats.h"
#include "XMLData.h"

namespace CustomTags {
	static const std::string FAMILIAR_IGNORE_BFFS = "familiarignorebffs";  // Makes the familiar ignore the default effects of BFFs.
	static const std::string FAMILIAR_CAN_TAKE_DAMAGE = "familiarcantakedamage";  // Allows familiars to take damage from enemies/hazards by default.
	static const std::string FAMILIAR_BLOCK_PROJECTILES = "familiarblockprojectiles";  // Allows familiars to block projectiles automatically.
	static const std::string FAMILIAR_NO_CHARM = "nocharm";  // Makes the familiar not be charmed by Siren.
	static const std::string ENTITY_NO_ERASE = "noerase";  // Makes the entity immune to being erased by the Eraser effect.
}

namespace EntityConfigEx {

class EntityEx {
public:
	void Parse(const EntityConfig_Entity& entity, const XMLAttributes& xml);

	inline void AddCustomTag(const std::string& tag) {
		customTags_.insert(tag);
	}
	inline void RemoveCustomTag(const std::string& tag) {
		customTags_.erase(tag);
	}
	inline const std::set<std::string>& GetCustomTags() const {
		return customTags_;
	}
	inline bool HasCustomTag(const std::string& tag) const {
		return customTags_.count(tag) > 0;
	}

	inline const std::vector<int>& GetBagOfCraftingPickups() {
		return bagOfCraftingPickups_;
	}

	inline int GetCoinValue() {
		return coinValue_;
	}

	inline const std::optional<bool>& GetPickupMeleeCollideOverride() {
		return pickupMeleeCollideOverride_;
	}
	inline const std::optional<bool>& GetPickupBoomerangOverride() {
		return pickupBoomerangOverride_;
	}
	inline const std::optional<bool>& GetIsActiveEnemyOverride() {
		return isActiveEnemyOverride_;
	}
	inline const std::optional<bool>& GetNoSplitOverride() {
		return noSplitOverride_;
	}

private:
	std::set<std::string> customTags_;
	std::vector<int> bagOfCraftingPickups_;
	int coinValue_ = 0;

	// "Override" type attributes.
	// When nullopt (not specified in xml) we should fall back to whatever the game's default beheaviour would be.

	std::optional<bool> pickupMeleeCollideOverride_;
	std::optional<bool> pickupBoomerangOverride_;
	std::optional<bool> isActiveEnemyOverride_;
	std::optional<bool> noSplitOverride_;
};

class PlayerEx {
public:
	void Parse(const EntityConfig_Player& player, const XMLAttributes& xml);

	inline bool HasStatModifier(const EvaluateStats::PlayerStat stat) const {
		return statModifiers_.count(stat) > 0 && statModifiers_.at(stat) != GetPlayerStatDefaultValue(stat);
	}

	inline float GetStatModifier(const EvaluateStats::PlayerStat stat) const {
		if (statModifiers_.count(stat)) {
			return statModifiers_.at(stat);
		}
		return GetPlayerStatDefaultValue(stat);
	}

	inline bool NoShake() const { return noShake_; }
	inline bool HasBloodyTears() const { return bloodyTears_; }
	inline bool ForceFlying() const { return forceFlying_; }

private:
	std::unordered_map<EvaluateStats::PlayerStat, float> statModifiers_;

	bool noShake_ = false;
	bool bloodyTears_ = false;
	bool forceFlying_ = false;
};

// Scans XMLData to initialize all of the Ex configs.
void ParseXMLData();

EntityEx* GetEntityEx(EntityConfig_Entity* entity);
EntityEx* GetEntityEx(int type, int variant, int subtype);
PlayerEx* GetPlayerEx(int playertype);

}  // namespace EntityConfigEx
