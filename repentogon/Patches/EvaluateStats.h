#pragma once

#include <unordered_map>
#include <string>

#include "IsaacRepentance.h"

namespace EvaluateStats {

// Stat evaluation "stages" sent to MC_EVALUATE_STAT
// Must be aligned with the lua enum `EvaluateStatStage`.
enum class LuaEvaluateStatStage {
	TEARS_UP,
	FLAT_TEARS,
	DAMAGE_UP,
	FLAT_DAMAGE,
	PRE_FLAT_DAMAGE,
};

// Internal enum to represent XML item stats.
enum class ItemStat {
	TEARS_UP,
	FLAT_TEARS,
	TEARS_MULT,
	DAMAGE_UP,
	FLAT_DAMAGE,
	DAMAGE_EARLY_MULT,
	DAMAGE_MULT,
	SHOTSPEED_UP,
	SPEED_UP,
	RANGE_UP,
	LUCK_UP,
	NUM_STATS,
};

inline float GetItemStatDefaultValue(const ItemStat stat) {
	if (stat == ItemStat::TEARS_MULT || stat == ItemStat::DAMAGE_EARLY_MULT || stat == ItemStat::DAMAGE_MULT) {
		return 1;
	}
	return 0;
}

// Maps item stats to their corresponding XML tags.
static const std::unordered_map<ItemStat, std::string> ItemStatXmlTags = {
	{ ItemStat::TEARS_UP, "tears" },
	{ ItemStat::FLAT_TEARS, "flattears" },
	{ ItemStat::DAMAGE_UP, "damage" },
	{ ItemStat::FLAT_DAMAGE, "flatdamage" },
	{ ItemStat::TEARS_MULT, "tearsmult" },
	{ ItemStat::DAMAGE_MULT, "damagemult" },
	{ ItemStat::DAMAGE_EARLY_MULT, "earlydamagemult" },
	{ ItemStat::SHOTSPEED_UP, "shotspeed" },
	{ ItemStat::SPEED_UP, "speed" },
	{ ItemStat::RANGE_UP, "range" },
	{ ItemStat::LUCK_UP, "luck" },
};

// Maps item stats to their corresponding CacheFlag.
static const std::unordered_map<ItemStat, CacheFlag> ItemStatCacheFlags = {
	{ ItemStat::TEARS_UP, CACHE_FIREDELAY },
	{ ItemStat::FLAT_TEARS, CACHE_FIREDELAY },
	{ ItemStat::DAMAGE_UP, CACHE_DAMAGE },
	{ ItemStat::FLAT_DAMAGE, CACHE_DAMAGE },
	{ ItemStat::TEARS_MULT, CACHE_FIREDELAY },
	{ ItemStat::DAMAGE_MULT, CACHE_DAMAGE },
	{ ItemStat::DAMAGE_EARLY_MULT, CACHE_DAMAGE },
	{ ItemStat::SHOTSPEED_UP, CACHE_SHOTSPEED },
	{ ItemStat::SPEED_UP, CACHE_SPEED },
	{ ItemStat::RANGE_UP, CACHE_RANGE },
	{ ItemStat::LUCK_UP, CACHE_LUCK },
};

// Internal enum to represent XML player stat modifiers.
enum class PlayerStat {
	TEARS_MODIFIER,
	TEARS_MULTIPLIER,
	DAMAGE_MODIFIER,
	DAMAGE_MULTIPLIER,
	SHOTSPEED_MODIFIER,
	SPEED_MODIFIER,
	RANGE_MODIFIER,
	LUCK_MODIFIER,
};

inline float GetPlayerStatDefaultValue(const PlayerStat stat) {
	if (stat == PlayerStat::DAMAGE_MULTIPLIER || stat == PlayerStat::TEARS_MULTIPLIER) {
		return 1;
	}
	return 0;
}

// Maps player stat modifiers to their corresponding XML tags.
static const std::unordered_map<PlayerStat, std::string> PlayerStatXmlTags = {
	{ PlayerStat::TEARS_MODIFIER, "tearsmodifier" },
	{ PlayerStat::TEARS_MULTIPLIER, "tearsmultiplier" },
	{ PlayerStat::DAMAGE_MODIFIER, "damagemodifier" },
	{ PlayerStat::DAMAGE_MULTIPLIER, "damagemultiplier" },
	{ PlayerStat::SHOTSPEED_MODIFIER, "shotspeedmodifier" },
	{ PlayerStat::SPEED_MODIFIER, "speedmodifier" },
	{ PlayerStat::RANGE_MODIFIER, "rangemodifier" },
	{ PlayerStat::LUCK_MODIFIER, "luckmodifier" },
};

void ApplyASMPatches();

}  // namespace EvaluateStats
