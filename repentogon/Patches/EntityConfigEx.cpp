#include "EntityConfigEx.h"

#include <vector>
#include <unordered_map>
#include <tuple>

#include <Log.h>

#include "IsaacRepentance.h"
#include "EvaluateStats.h"
#include "XMLData.h"
#include "AchievementsStuff.h"
#include "HookSystem.h"

namespace EntityConfigEx {

static std::unordered_map<std::tuple<int, int, int>, EntityEx> s_Entities;
static std::vector<PlayerEx> s_Players;

static const std::unordered_map<std::string, int> bagOfCraftingTags = {
	{"-1", 0},
	{"none", 0},
	{"false", 0},
	{"redheart", 1},
	{"heart", 1},
	{"soulheart", 2},
	{"blackheart", 3},
	{"eternalheart", 4},
	{"goldheart", 5},
	{"boneheart", 6},
	{"rottenheart", 7},
	{"penny", 8},
	{"nickel", 9},
	{"dime", 10},
	{"luckypenny", 11},
	{"key", 12},
	{"goldkey", 13},
	{"chargedkey", 14},
	{"bomb", 15},
	{"goldbomb", 16},
	{"gigabomb", 17},
	{"minibattery", 18},
	{"battery", 19},
	{"megabattery", 20},
	{"card", 21},
	{"pill", 22},
	{"rune", 23},
	{"diceshard", 24},
	{"crackedkey", 25},
	{"goldpenny", 26},
	{"goldpill", 27},
	{"goldbattery", 28},
	{"poop", 29},
};

void ParseBagOfCraftingAttribute(const string& str, vector<int>& out) {
	const string tagsstr = stringlower(str.c_str());
	if (!tagsstr.empty()) {
		std::stringstream tagstream(tagsstr);
		std::string tag;
		while (std::getline(tagstream, tag, ' ')) {
			if (out.size() >= 8)
				break;
			if (!tag.empty() && bagOfCraftingTags.find(tag) != bagOfCraftingTags.end()) {
				const int n = bagOfCraftingTags.at(tag);
				if (n > 0 || out.empty())
					out.push_back(n);
				if (n == 0)
					break;
			} else {
				break;
			}
		}
	}
}

void ParseOverrideBooleanAttribute(const XMLAttributes& xml, const char* attr, std::optional<bool>& out) {
	out = std::nullopt;
	if (xml.count(attr)) {
		if (xml.at(attr) == "true") {
			out = true;
		} else if (xml.at(attr) == "false") {
			out = false;
		}
	}
}

void EntityEx::Parse(const EntityConfig_Entity& entity, const XMLAttributes& xml) {
	if (xml.count("customtags")) {
		XMLData::ParseTagsString(xml.at("customtags"), customTags_);
	}
	// Legacy attribute
	if (auto it = xml.find("familiarignorebffs"); it != xml.end() && it->second == "true") {
		customTags_.insert(CustomTags::FAMILIAR_IGNORE_BFFS);
	}
	if (entity.id == ENTITY_PICKUP) {
		if (xml.count("bagofcrafting")) {
			ParseBagOfCraftingAttribute(xml.at("bagofcrafting"), bagOfCraftingPickups_);
		}
		if (entity.variant != PICKUP_COIN) {
			coinValue_ = 0;
		} else if (entity.subtype >= 0 && entity.subtype < 8) {
			static const int s_VanillaCoinValues[8] = { 1, 1, 5, 10, 2, 1, 5, 1 };
			coinValue_ = s_VanillaCoinValues[entity.subtype];
		} else if (auto it = xml.find("coinvalue"); it != xml.end() && !it->second.empty() && isdigit(it->second[0])) {
			coinValue_ = std::atoi(it->second.c_str());
		} else {
			coinValue_ = 1;
		}
	}
	ParseOverrideBooleanAttribute(xml, "meleecollide", pickupMeleeCollideOverride_);
	ParseOverrideBooleanAttribute(xml, "boomerang", pickupBoomerangOverride_);
	ParseOverrideBooleanAttribute(xml, "isactiveenemy", isActiveEnemyOverride_);
	ParseOverrideBooleanAttribute(xml, "nosplit", noSplitOverride_);
}

void PlayerEx::Parse(const EntityConfig_Player& player, const XMLAttributes& xml) {
	// Legacy tag "firedelaymodifier" would modify negative values due to Eden-related logic.
	// Newer "tearsmodifier" tag does not have this behaviour, and will overwrite this if both are present.
	if (xml.count("firedelaymodifier")) {
		try {
			float legacyModifier = std::stof(xml.at("firedelaymodifier"));
			if (legacyModifier < 0) {
				legacyModifier *= 0.686655f;
			}
			statModifiers_[EvaluateStats::PlayerStat::TEARS_MODIFIER] = legacyModifier;
		} catch (const std::exception& e) {
			ZHL::Log("[EntityConfigEx::PlayerEx] Invalid xml value `%s` for attribute `firedelaymodifier` (%s)\n", xml.at("firedelaymodifier").c_str(), e.what());
		}
	}

	// Parse the primary stat modifier tags.
	for (const auto& [stat, tag] : EvaluateStats::PlayerStatXmlTags) {
		if (xml.count(tag)) {
			try {
				statModifiers_[stat] = std::stof(xml.at(tag));
			} catch (const std::exception& e) {
				ZHL::Log("[EntityConfigEx::PlayerEx] Invalid xml value `%s` for attribute `%s` (%s)\n", xml.at(tag).c_str(), tag.c_str(), e.what());
			}
		}
	}
	
	noShake_ = player._id == PLAYER_BLUEBABY || player._id == PLAYER_THEFORGOTTEN || player._id == PLAYER_THESOUL || player._id == PLAYER_KEEPER || player._id == PLAYER_BLUEBABY_B || player._id == PLAYER_THEFORGOTTEN_B || player._id == PLAYER_KEEPER_B;

	if (xml.count("noshake")) {
		const std::string value = xml.at("noshake");
		if (value == "true") {
			noShake_ = true;
		} else if (value == "false") {
			noShake_ = false;
		}
	}

	bloodyTears_ = player._id == PLAYER_ESAU || player._id == PLAYER_JUDAS_B || player._id == PLAYER_LAZARUS2_B || player._id == PLAYER_BETHANY_B;

	if (xml.count("bloodytears")) {
		const std::string value = xml.at("bloodytears");
		if (value == "true") {
			bloodyTears_ = true;
		} else if (value == "false") {
			bloodyTears_ = false;
		}
	}

	forceFlying_ = xml.count("flying") && xml.at("flying") == "true";
}

void ParseXMLData() {
	for (const auto& [key, xml] : XMLStuff.EntityData->nodes) {
		const auto [type, variant, subtype] = key;
		EntityConfig_Entity* entity = g_Manager->GetEntityConfig()->GetEntity(type, variant, subtype);
		if (entity && entity->id == type && entity->variant == variant && entity->subtype == subtype && !s_Entities.count(key)) {
			s_Entities[key].Parse(*entity, xml);
		}
	}

	// Parse achievement IDs for modded babies.
	// It is safe to put these here.
	for (EntityConfig_Baby& baby : *g_Manager->GetEntityConfig()->GetBabies()) {
		std::string sourceid = XMLStuff.BabyData->GetAttributeById(baby.id, "sourceid");
		if (!sourceid.empty() && sourceid != "BaseGame") {
			std::string achievement = XMLStuff.BabyData->GetAttributeById(baby.id, "achievement");
			if (!achievement.empty()) {
				baby.achievementID = GetAchievementIdByName(achievement);
			}
		}
		if (baby.achievementID <= 0) {
			baby.achievementID = -1;
		}
	}

	const auto& players = *g_Manager->GetEntityConfig()->GetPlayers();
	s_Players.resize(players.size());
	for (const auto& player : players) {
		if (player._id > 0) {
			if (const auto* xml = XMLStuff.PlayerData->GetNodeOrNullById(player._id)) {
				s_Players[player._id].Parse(player, *xml);
			}
		}
	}

	if (XMLStuff.CustomXMLData.count("edenhair.xml")) {
		if (auto* data = XMLStuff.CustomXMLData["edenhair.xml"]) {
			for (auto& [_, node] : data->nodes) {
				std::string path = node["root"] + node["gfx"];
				if (!path.empty()) {
					g_Manager->GetEntityConfig()->_edenHair.push_back(path);
				}
			}
		}
	}
}

EntityEx* GetEntityEx(EntityConfig_Entity* entity) {
	if (entity) {
		const std::tuple<int, int, int> key {entity->id, entity->variant, entity->subtype};
		if (s_Entities.count(key)) {
			return &s_Entities[key];
		}
	}
	return nullptr;
}

EntityEx* GetEntityEx(int type, int variant, int subtype) {
	return GetEntityEx(g_Manager->GetEntityConfig()->GetEntity(type, variant, subtype));
}

PlayerEx* GetPlayerEx(int playertype) {
	if (playertype > 0 && playertype < s_Players.size()) {
		return &s_Players[playertype];
	}
	return nullptr;
}

// Reimplement co-op baby selection to ensure cooperation with modded babies
EntityConfig_Baby* GetRandomBaby(uint32_t seed) {
	auto& babies = *g_Manager->GetEntityConfig()->GetBabies();
	std::vector<EntityConfig_Baby*> candidates;
	for (EntityConfig_Baby& baby : babies) {
		if (baby.achievementID <= 0 || g_Manager->GetPersistentGameData()->Unlocked(baby.achievementID)) {
			candidates.push_back(&baby);
		}
	}
	if (candidates.empty()) {
		// Sanity fallback that shouldn't happen
		return &babies.at((babies.size() > 71) ? 71 : 0);
	}
	RNG rng;
	rng.SetSeed((seed > 0u) ? seed : 1u, 35);
	return candidates[rng.RandomInt(candidates.size())];
}
HOOK_METHOD(Entity_Familiar, ai_buddy_inbox, () -> void) {
	if (this->_subtype == 0) {
		EntityConfig_Baby* baby = GetRandomBaby(this->_initSeed ^ g_Game->_dungeonPlacementSeed);
		for (int i = 0; i < this->_sprite.GetLayerCount(); i++) {
			this->_sprite.ReplaceSpritesheet(i, baby->gfx);
		}
		this->_sprite.LoadGraphics(true);
		this->_subtype = 1;
	}
	super();
}
HOOK_METHOD(PlayerManager, SpawnCoPlayerBaby, (int babyID) -> Entity_Player*) {
	if (babyID < 0) {
		babyID = GetRandomBaby(Isaac::genrand_int32())->id;
	}
	return super(babyID);
}

// Respect "noerase" CustomTag
HOOK_STATIC(Game, AddErasedEnemy, (Entity* entity) -> void, __stdcall) {
	if (EntityEx* ex = GetEntityEx(entity->_type, entity->_variant, entity->_subtype)) {
		if (ex->HasCustomTag(CustomTags::ENTITY_NO_ERASE)) {
			return;
		}
	}
	super(entity);
}
HOOK_STATIC(Game, ShouldEraseEnemy, (Entity* entity) -> bool, __stdcall) {
	if (EntityEx* ex = GetEntityEx(entity->_type, entity->_variant, entity->_subtype)) {
		if (ex->HasCustomTag(CustomTags::ENTITY_NO_ERASE)) {
			return false;
		}
	}
	return super(entity);
}

}  // EntityConfigEx
