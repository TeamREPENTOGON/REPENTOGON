#pragma once

#include <unordered_map>
#include <string>

#include "IsaacRepentance.h"

namespace EvaluateStats {

enum EvaluateStatStage {
	STAT_TEARS_UP,
	STAT_FLAT_TEARS,
	STAT_DAMAGE_UP,
	STAT_FLAT_DAMAGE,
	STAT_SHOTSPEED_UP,
	STAT_SPEED_UP,
	STAT_RANGE_UP,
	STAT_LUCK_UP,
	STAT_TEARS_MULT,
	STAT_DAMAGE_MULT,
};

static const std::unordered_map<EvaluateStatStage, std::string> evaluteStatXmlTags = {
	{ STAT_TEARS_UP, "tears" },
	{ STAT_FLAT_TEARS, "flattears" },
	{ STAT_DAMAGE_UP, "damage" },
	{ STAT_FLAT_DAMAGE, "flatdamage" },
	{ STAT_SHOTSPEED_UP, "shotspeed" },
	{ STAT_SPEED_UP, "speed" },
	{ STAT_RANGE_UP, "range" },
	{ STAT_LUCK_UP, "luck" },
	{ STAT_TEARS_MULT, "tearsmult" },
	{ STAT_DAMAGE_MULT, "damagemult" },
};

static const std::unordered_map<EvaluateStatStage, CacheFlag> evaluteStatCacheFlags = {
	{ STAT_TEARS_UP, CACHE_FIREDELAY },
	{ STAT_FLAT_TEARS, CACHE_FIREDELAY },
	{ STAT_DAMAGE_UP, CACHE_DAMAGE },
	{ STAT_FLAT_DAMAGE, CACHE_DAMAGE },
	{ STAT_SHOTSPEED_UP, CACHE_SHOTSPEED },
	{ STAT_SPEED_UP, CACHE_SPEED },
	{ STAT_RANGE_UP, CACHE_RANGE },
	{ STAT_LUCK_UP, CACHE_LUCK },
	{ STAT_TEARS_MULT, CACHE_FIREDELAY },
	{ STAT_DAMAGE_MULT, CACHE_DAMAGE },
};

void UpdateItemConfig();
void ApplyASMPatches();

}  // namespace EvaluateStats
