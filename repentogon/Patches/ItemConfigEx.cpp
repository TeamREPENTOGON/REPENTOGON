#include "ItemConfigEx.h"

#include <vector>
#include <string>

#include "IsaacRepentance.h"
#include "XMLData.h"
#include "EvaluateStats.h"
#include <Log.h>

using namespace EvaluateStats;

namespace ItemConfigEx {

// The extended configs for each item. Vectors are sized to match their respective ItemConfig vectors.
static std::vector<CollectibleEx> s_Collectibles;
static std::vector<TrinketEx> s_Trinkets;
static std::vector<NullItemEx> s_NullItems;

// List of CustomCaches to execute whenever CACHE_ALL is evaluated.
static std::set<std::string> s_AllCustomCaches = { "familiarmultiplier", "maxcoins", "maxkeys" , "maxbombs", "tearscap", "statmultiplier" };

// Lookup tables for items that have ex stats, for efficiency reasons.
static StatsLookup s_CollectibleStatLookup;
static StatsLookup s_TrinketStatLookup;
static StatsLookup s_NullStatLookup;

StatsLookup& CollectibleEx::GetStatsLookup() { return s_CollectibleStatLookup; }
StatsLookup& TrinketEx::GetStatsLookup() { return s_TrinketStatLookup; }
StatsLookup& NullItemEx::GetStatsLookup() { return s_NullStatLookup; }

// Lookup tables for items with specific CustomTags, for efficiency and convenience.
static CustomTagsLookup s_CollectibleCustomTagsLookup;
static CustomTagsLookup s_TrinketCustomTagsLookup;
static CustomTagsLookup s_NullCustomTagsLookup;

CustomTagsLookup& CollectibleEx::GetCustomTagsLookup() { return s_CollectibleCustomTagsLookup; }
CustomTagsLookup& TrinketEx::GetCustomTagsLookup() { return s_TrinketCustomTagsLookup; }
CustomTagsLookup& NullItemEx::GetCustomTagsLookup() { return s_NullCustomTagsLookup; }


float CalculateStatChange(Entity_Player* player, const ItemStat stat) {
	TemporaryEffects* effects = &player->_temporaryeffects;
	float totalChange = 0;
	for (const int id : s_CollectibleStatLookup[stat]) {
		if (const CollectibleEx* ex = GetCollectibleEx(id)) {
			float value = ex->GetStat(stat);
			if (value != 0) {
				totalChange += player->GetCollectibleNum(id, false) * value;
			}
			float effectValue = ex->GetEffectStat(stat);
			if (effectValue != 0) {
				totalChange += effects->GetCollectibleEffectNum(id) * effectValue;
			}
		}
	}
	for (const int id : s_TrinketStatLookup[stat]) {
		if (const TrinketEx* ex = GetTrinketEx(id)) {
			float value = ex->GetStat(stat);
			if (value != 0) {
				totalChange += player->GetTrinketMultiplier(id) * value;
			}
			float effectValue = ex->GetEffectStat(stat);
			if (effectValue != 0) {
				totalChange += effects->GetTrinketEffectNum(id) * effectValue;
			}
		}
	}
	for (const int id : s_NullStatLookup[stat]) {
		if (const NullItemEx* ex = GetNullItemEx(id)) {
			float value = ex->GetEffectStat(stat);
			if (value != 0) {
				totalChange += effects->GetNullEffectNum(id) * value;
			}
		}
	}
	return totalChange;
}

inline float CalcStatMult(const float mult, const int n) {
	if (mult > 0 && n > 0) {
		return (float)std::pow(mult, n);
	}
	return 1;
}
float CalculateStatMult(Entity_Player* player, const ItemStat stat) {
	TemporaryEffects* effects = &player->_temporaryeffects;
	float finalMult = 1;
	for (const int id : s_CollectibleStatLookup[stat]) {
		if (const CollectibleEx* ex = GetCollectibleEx(id)) {
			float value = ex->GetStat(stat);
			if (value != 1) {
				finalMult *= CalcStatMult(value, player->GetCollectibleNum(id, false));
			}
			float effectValue = ex->GetEffectStat(stat);
			if (effectValue != 1) {
				finalMult *= CalcStatMult(value, effects->GetCollectibleEffectNum(id));
			}
		}
	}
	for (const int id : s_TrinketStatLookup[stat]) {
		if (const TrinketEx* ex = GetTrinketEx(id)) {
			float value = ex->GetStat(stat);
			if (value != 1) {
				finalMult *= CalcStatMult(value, player->GetTrinketMultiplier(id));
			}
			float effectValue = ex->GetEffectStat(stat);
			if (effectValue != 1) {
				finalMult *= CalcStatMult(value, effects->GetTrinketEffectNum(id));
			}
		}
	}
	for (const int id : s_NullStatLookup[stat]) {
		if (const NullItemEx* ex = GetNullItemEx(id)) {
			float value = ex->GetEffectStat(stat);
			if (value != 1) {
				finalMult *= CalcStatMult(value, effects->GetNullEffectNum(id));
			}
		}
	}
	return finalMult;
}

const std::set<int>& GetCollectiblesWithCustomTag(const std::string& tag) {
	return s_CollectibleCustomTagsLookup[tag];
}

const std::set<int>& GetTrinketsWithCustomTag(const std::string& tag) {
	return s_TrinketCustomTagsLookup[tag];
}

const std::set<int>& GetNullItemsWithCustomTag(const std::string& tag) {
	return s_NullCustomTagsLookup[tag];
}

bool HasCollectibleWithCustomTag(Entity_Player* player, const std::string& tag, const bool ignoreModifiers) {
	for (const int id : GetCollectiblesWithCustomTag(tag)) {
		if (player->HasCollectible(id, ignoreModifiers)) {
			return true;
		}
	}
	return false;
}

bool HasCollectibleEffectWithCustomTag(Entity_Player* player, const std::string& tag) {
	for (const int id : GetCollectiblesWithCustomTag(tag)) {
		if (player->_temporaryeffects.HasCollectibleEffect(id)) {
			return true;
		}
	}
	return false;
}

bool HasTrinketWithCustomTag(Entity_Player* player, const std::string& tag, const bool ignoreModifiers) {
	for (const int id : GetTrinketsWithCustomTag(tag)) {
		if (player->HasTrinket(id, ignoreModifiers)) {
			return true;
		}
	}
	return false;
}

bool HasTrinketEffectWithCustomTag(Entity_Player* player, const std::string& tag) {
	for (const int id : GetTrinketsWithCustomTag(tag)) {
		if (player->_temporaryeffects.HasTrinketEffect(id)) {
			return true;
		}
	}
	return false;
}

bool HasNullEffectWithCustomTag(Entity_Player* player, const std::string& tag) {
	for (const int id : GetNullItemsWithCustomTag(tag)) {
		if (player->_temporaryeffects.HasNullEffect(id)) {
			return true;
		}
	}
	return false;
}

bool HasItemWithCustomTag(Entity_Player* player, const std::string& tag, const bool ignoreModifiers) {
	return HasCollectibleWithCustomTag(player, tag, ignoreModifiers) || HasTrinketWithCustomTag(player, tag, ignoreModifiers);
}

bool HasEffectWithCustomTag(Entity_Player* player, const std::string& tag) {
	return HasCollectibleEffectWithCustomTag(player, tag) || HasTrinketEffectWithCustomTag(player, tag) || HasNullEffectWithCustomTag(player, tag);
}

bool HasItemOrEffectWithCustomTag(Entity_Player* player, const std::string& tag) {
	return HasItemWithCustomTag(player, tag, false) || HasEffectWithCustomTag(player, tag);
}

int GetCustomReviveCount(Entity_Player* player, const bool includeHidden) {
	int numLives = 0;
	TemporaryEffects& effects = player->_temporaryeffects;
	for (const int id : s_CollectibleCustomTagsLookup[CustomTags::REVIVE]) {
		if (CollectibleEx* ex = GetCollectibleEx(id)) {
			if (ex->ShouldCountRevive(includeHidden)) {
				numLives += player->GetCollectibleNum(id, !includeHidden);
			}
		}
	}
	for (const int id : s_CollectibleCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (CollectibleEx* ex = GetCollectibleEx(id)) {
			if (ex->ShouldCountReviveEffect(includeHidden)) {
				numLives += effects.GetCollectibleEffectNum(id);
			}
		}
	}
	for (const int id : s_TrinketCustomTagsLookup[CustomTags::REVIVE]) {
		if (TrinketEx* ex = GetTrinketEx(id)) {
			if (ex->ShouldCountRevive(includeHidden)) {
				if ((player->_trinketsID[0] & TRINKET_ID_MASK) == id) {
					numLives += 1;
				}
				if ((player->_trinketsID[1] & TRINKET_ID_MASK) == id) {
					numLives += 1;
				}
				numLives += player->GetSmeltedTrinket()->at(id)._trinketNum + player->GetSmeltedTrinket()->at(id)._goldenTrinketNum;
			}
		}
	}
	for (const int id : s_TrinketCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (TrinketEx* ex = GetTrinketEx(id)) {
			if (ex->ShouldCountReviveEffect(includeHidden)) {
				numLives += effects.GetTrinketEffectNum(id);
			}
		}
	}
	for (const int id : s_NullCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (NullItemEx* ex = GetNullItemEx(id)) {
			if (ex->ShouldCountReviveEffect(includeHidden)) {
				numLives += effects.GetNullEffectNum(id);
			}
		}
	}
	return numLives;
}

bool HasCustomChanceRevive(Entity_Player* player) {
	TemporaryEffects& effects = player->_temporaryeffects;
	for (const int id : s_CollectibleCustomTagsLookup[CustomTags::REVIVE]) {
		if (CollectibleEx* ex = GetCollectibleEx(id); ex && ex->IsChanceRevive()) {
			if (ex->ShouldCountRevive(false) && player->HasCollectible(id, false)) {
				return true;
			}
		}
	}
	for (const int id : s_CollectibleCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (CollectibleEx* ex = GetCollectibleEx(id); ex && ex->IsChanceRevive()) {
			if (ex->ShouldCountReviveEffect(false) && effects.HasCollectibleEffect(id)) {
				return true;
			}
		}
	}
	for (const int id : s_TrinketCustomTagsLookup[CustomTags::REVIVE]) {
		if (TrinketEx* ex = GetTrinketEx(id); ex && ex->IsChanceRevive()) {
			if (ex->ShouldCountRevive(false) && player->HasTrinket(id, false)) {
				return true;
			}
		}
	}
	for (const int id : s_TrinketCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (TrinketEx* ex = GetTrinketEx(id); ex && ex->IsChanceRevive()) {
			if (ex->ShouldCountReviveEffect(false) && effects.HasTrinketEffect(id)) {
				return true;
			}
		}
	}
	for (const int id : s_NullCustomTagsLookup[CustomTags::REVIVE_EFFECT]) {
		if (NullItemEx* ex = GetNullItemEx(id); ex && ex->ShouldCountReviveEffect(false) && ex->IsChanceRevive()) {
			return true;
		}
	}
	return false;
}

void ItemEx::RefreshCustomTags() {
	customRevive_ = customTags_.count(CustomTags::REVIVE);
	customReviveEffect_ = customTags_.count(CustomTags::REVIVE_EFFECT);
	customReviveHidden_ = customTags_.count(CustomTags::HIDDEN_REVIVE);
	customChanceRevive_ = customTags_.count(CustomTags::CHANCE_REVIVE);

	if (customTags_.count(CustomTags::FLYING) || customTags_.count(CustomTags::FLYING_EFFECT)) {
		if (ItemConfig_Item* item = GetItemConfigItem()) {
			item->cacheFlags |= CACHE_FLYING;
		}
	}

	auto& customTagsLookup = GetCustomTagsLookup();
	for (auto& [k, v] : customTagsLookup) {
		v.erase(id_);
	}
	for (const std::string& tag : customTags_) {
		customTagsLookup[tag].insert(id_);
	}
}

void ItemEx::RefreshStats() {
	auto& lookups = GetStatsLookup();

	for (int i = 0; i < (int)ItemStat::NUM_STATS; i++) {
		const ItemStat stat = (ItemStat)i;
		const float defaultValue = GetItemStatDefaultValue(stat);
		const bool hasStat = stats_.count(stat) && stats_.at(stat) != defaultValue;
		const bool hasEffectStat = effectStats_.count(stat) && effectStats_.at(stat) != defaultValue;
		if (hasStat || hasEffectStat) {
			lookups[stat].insert(id_);
			if (ItemConfig_Item* item = GetItemConfigItem(); item && ItemStatCacheFlags.count(stat)) {
				// Update CacheFlags so that the item triggers the necessary cache evals.
				item->cacheFlags |= ItemStatCacheFlags.at(stat);

				if (item->type == 3 && hasStat) {
					// Allow active item to trigger cache evals when held, instead of on use.
					item->passiveCache = true;
				}
			}
		} else {
			stats_.erase(stat);
			effectStats_.erase(stat);
			lookups[stat].erase(id_);
		}
	}
}

void ItemEx::Parse(const ItemConfig_Item& item, const XMLAttributes& xml) {
	id_ = item.id;

	// Parse tags
	if (xml.count("customtags")) {
		XMLData::ParseTagsString(xml.at("customtags"), customTags_);
		// Backwards compatability for "revive" tag being used on Null Items
		// But I don't want to keep doing this sort of thing for every other "effect" tag.
		if (IsNullItem() && customTags_.count(CustomTags::REVIVE)) {
			customTags_.erase(CustomTags::REVIVE);
			customTags_.insert(CustomTags::REVIVE_EFFECT);
		}
	}
	if (xml.count("customcache")) {
		XMLData::ParseTagsString(xml.at("customcache"), customCache_);
		XMLData::ParseTagsString(xml.at("customcache"), s_AllCustomCaches);
	}

	RefreshCustomTags();

	// XML Stats
	for (const auto& [stat, tag] : ItemStatXmlTags) {
		if (xml.count(tag)) {
			try {
				float value = std::stof(xml.at(tag));
				if (item.type == 0) {  // Null
					effectStats_[stat] = value;
				} else {
					stats_[stat] = value;
				}
			} catch (const std::exception& e) {
				ZHL::Log("[ItemConfigEx] Invalid xml value `%s` for attribute `%s` (%s)\n", xml.at(tag).c_str(), tag.c_str(), e.what());
			}
		}

		const std::string effecttag = "effect" + tag;
		if (xml.count(effecttag)) {
			try {
				effectStats_[stat] = std::stof(xml.at(effecttag));
			} catch (const std::exception& e) {
				ZHL::Log("[ItemConfigEx] Invalid xml value `%s` for attribute `%s` (%s)\n", xml.at(effecttag).c_str(), effecttag.c_str(), e.what());
			}
		}
	}

	RefreshStats();
}

void CollectibleEx::Parse(const ItemConfig_Item& item, const XMLAttributes& xml) {
	ItemEx::Parse(item, xml);

	if (item.id == COLLECTIBLE_BFFS || item.id == COLLECTIBLE_HIVE_MIND) {
		customCache_.insert("familiarmultiplier");
	} else if (item.id == COLLECTIBLE_DEEP_POCKETS) {
		customCache_.insert("maxcoins");
	}

	// Custom active gfx
	if (item.type == 3 && xml.count("gfxroot") && xml.count("activegfx")) {
		customActiveGfx_ = xml.at("gfxroot") + "collectibles/" + xml.at("activegfx");
	}
}

void ParseXMLData() {
	const auto& collectibles = *g_Manager->GetItemConfig()->GetCollectibles();
	s_Collectibles.resize(collectibles.size());
	for (const auto* item : collectibles) {
		if (item && item->id > 0) {
			if (const auto* xml = XMLStuff.ItemData->GetNodeOrNullById(item->id)) {
				s_Collectibles[item->id].Parse(*item, *xml);
			}
		}
	}

	const auto& trinkets = *g_Manager->GetItemConfig()->GetTrinkets();
	s_Trinkets.resize(trinkets.size());
	for (const auto* item : trinkets) {
		if (item && item->id > 0) {
			if (const auto* xml = XMLStuff.TrinketData->GetNodeOrNullById(item->id)) {
				s_Trinkets[item->id].Parse(*item, *xml);
			}
		}
	}

	const auto& nullItems = *g_Manager->GetItemConfig()->GetNullItems();
	s_NullItems.resize(nullItems.size());
	for (const auto* item : nullItems) {
		if (item && item->id > 0) {
			if (const auto* xml = XMLStuff.NullItemData->GetNodeOrNullById(item->id)) {
				s_NullItems[item->id].Parse(*item, *xml);
			}
		}
	}
}

void AddKnownCustomCache(const std::string& tag) {
	s_AllCustomCaches.insert(tag);
}

const std::set<std::string>& GetKnownCustomCaches() {
	return s_AllCustomCaches;
}

CollectibleEx* GetCollectibleEx(int id) {
	if (id > 0 && id < s_Collectibles.size()) {
		return &s_Collectibles[id];
	}
	return nullptr;
}

TrinketEx* GetTrinketEx(int id) {
	id &= TRINKET_ID_MASK;
	if (id > 0 && id < s_Trinkets.size()) {
		return &s_Trinkets[id];
	}
	return nullptr;
}

NullItemEx* GetNullItemEx(int id) {
	if (id > 0 && id < s_NullItems.size()) {
		return &s_NullItems[id];
	}
	return nullptr;
}

ItemEx* GetItemEx(ItemConfig_Item* item) {
	if (item) {
		if (item->type == 0) {
			return GetNullItemEx(item->id);
		} else if (item->type == 2) {
			return GetTrinketEx(item->id);
		}
		return GetCollectibleEx(item->id);
	}
	return nullptr;
}

}  // ItemConfigEx
