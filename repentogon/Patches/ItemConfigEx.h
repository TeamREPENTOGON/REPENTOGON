#pragma once

#include "EvaluateStats.h"

#include <unordered_map>
#include <string>
#include <set>

#include "IsaacRepentance.h"
#include "XMLData.h"
#include "EvaluateStats.h"

using namespace EvaluateStats;

namespace ItemConfigEx {

using ItemsStats = std::unordered_map<ItemStat, float>;
using StatsLookup = std::unordered_map<ItemStat, std::set<int>>;
using ReviveLookup = std::set<int>;


// Add a CustomCache to execute whenever CACHE_ALL runs.
void AddKnownCustomCache(const std::string& tag);

// Returns the set of all known CustomCaches that should get evaluated on CACHE_ALL.
const std::set<std::string>& GetKnownCustomCaches();


class ItemEx {
public:
	virtual void Parse(const ItemConfig_Item& item, const XMLAttributes& xml);

	inline void AddCustomTag(const std::string& tag) {
		customTags_.insert(tag);
		RefreshCustomTags();
	}
	inline void RemoveCustomTag(const std::string& tag) {
		customTags_.erase(tag);
		RefreshCustomTags();
	}
	inline const std::set<std::string>& GetCustomTags() const {
		return customTags_;
	}
	inline bool HasCustomTag(const std::string& tag) const {
		return customTags_.count(tag) > 0;
	}

	inline void AddCustomCache(const std::string& tag) {
		customCache_.insert(tag);
		RefreshCustomTags();
		AddKnownCustomCache(tag);
	}
	inline void RemoveCustomCache(const std::string& tag) {
		customCache_.erase(tag);
		RefreshCustomTags();
	}
	inline const std::set<std::string>& GetCustomCaches() const {
		return customCache_;
	}
	inline bool HasCustomCache(const std::string& tag) const {
		return customCache_.count(tag) > 0;
	}

	inline virtual float GetStat(const ItemStat stat) const {
		return GetStatInternal(stat, false);
	}

	inline float GetEffectStat(const ItemStat stat) const {
		return GetStatInternal(stat, true);
	}

	inline bool ShouldCountRevive(const bool includeHidden) const {
		return ShouldCountReviveInternal(false, includeHidden);
	}

	inline bool ShouldCountReviveEffect(const bool includeHidden) const {
		return ShouldCountReviveInternal(true, includeHidden);
	}

	inline bool IsChanceRevive() const {
		return customChanceRevive_;
	}

	virtual ItemConfig_Item* GetItemConfigItem() const = 0;

private:
	inline float GetStatInternal(const ItemStat stat, const bool effect) const {
		const ItemsStats& stats = effect ? effectStats_ : stats_;
		if (stats.count(stat))
			return stats.at(stat);
		return GetItemStatDefaultValue(stat);
	}

	inline bool ShouldCountReviveInternal(const bool effect, const bool includeHidden) const {
		if (customReviveHidden_ && !includeHidden) {
			return false;
		}
		if (effect) {
			return customReviveEffect_;
		}
		return customRevive_;
	}

	virtual bool IsNullItem() const {
		return false;
	}

	// Updates attributes and lookups related to CustomTags.
	void RefreshCustomTags();

	// Updates attributes and lookups related to ex stats.
	// May also update the corresponding ItemConfig_Item (for things like cache flags).
	void RefreshStats();

	virtual StatsLookup& GetStatsLookup() = 0;
	virtual ReviveLookup& GetReviveLookup() = 0;

protected:
	int id_ = -1;

	bool noMetronome_ = false;
	bool noExpansionPack_ = false;

	bool customRevive_ = false;  // Grants a revive when item/trinket is held.
	bool customReviveEffect_ = false;  // Grants a revive when the corresponding TemporaryEffect is applied.
	bool customReviveHidden_ = false;  // Revive is not counted on the HUD.
	bool customChanceRevive_ = false;  // Adds a "?" to the hud when held.

	std::set<std::string> customTags_;
	std::set<std::string> customCache_;
	ItemsStats stats_;
	ItemsStats effectStats_;  // For corresponding TemporaryEffect
};


class CollectibleEx : public ItemEx {
public:
	inline bool NoMetronome() const {
		return noMetronome_;
	}

	inline bool NoExpansionPack() const {
		return noExpansionPack_;
	}

	void Parse(const ItemConfig_Item& item, const XMLAttributes& xml) override;

	inline const std::string& GetCustomActiveGfx() {
		return customActiveGfx_;
	}

	inline ItemConfig_Item* GetItemConfigItem() const override {
		return g_Manager->GetItemConfig()->GetCollectible(id_);
	}

private:
	StatsLookup& GetStatsLookup() override;
	ReviveLookup& GetReviveLookup() override;

	std::string customActiveGfx_;
};


class TrinketEx : public ItemEx {
public:
	inline ItemConfig_Item* GetItemConfigItem() const override {
		return g_Manager->GetItemConfig()->GetTrinket(id_);
	}

private:
	StatsLookup& GetStatsLookup() override;
	ReviveLookup& GetReviveLookup() override;
};


class NullItemEx : public ItemEx {
public:
	inline float GetStat(const ItemStat stat) const override {
		return GetEffectStat(stat);
	}

	inline ItemConfig_Item* GetItemConfigItem() const override {
		return g_Manager->GetItemConfig()->GetNullItem(id_);
	}

private:
	bool IsNullItem() const override {
		return true;
	}

	StatsLookup& GetStatsLookup() override;
	ReviveLookup& GetReviveLookup() override;
};


// Scans XMLData to initialize all of the Ex configs.
void ParseXMLData();

// Get the ex config for a Collectible, or nullptr if the ID is invalid.
CollectibleEx* GetCollectibleEx(int id);

// Returns the ex config for a Trinket, or nullptr if the ID is invalid.
TrinketEx* GetTrinketEx(int id);

// Returns the ex config for a NullItem, or nullptr if the ID is invalid.
NullItemEx* GetNullItemEx(int id);

// Returns the ex config corresponding to an ItemConfig_Item, or nullptr if uhhhhhh
ItemEx* GetItemEx(ItemConfig_Item* item);

// Calculates the total ex stat change from the player's items. Do not use for multipliers.
float CalculateStatChange(Entity_Player* player, const ItemStat stat);

// Calculates the total ex stat multiplier from the player's items. Do not use for non-multiplier stats.
float CalculateStatMult(Entity_Player* player, const ItemStat stat);

// Returns the # of custom extra life items/effects that the player has.
int GetCustomReviveCount(Entity_Player* player, const bool includeHidden);

// Returns true if the player is holding any custom "chance" revive items.
bool HasCustomChanceRevive(Entity_Player* player);

}  // ItemConfigEx
