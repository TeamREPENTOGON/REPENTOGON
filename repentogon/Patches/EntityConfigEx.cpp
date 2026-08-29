#include "EntityConfigEx.h"

#include <vector>

#include <Log.h>

#include "IsaacRepentance.h"
#include "EvaluateStats.h"
#include "XMLData.h"
#include "AchievementsStuff.h"
#include "HookSystem.h"

namespace EntityConfigEx {

static std::vector<PlayerEx> s_Players;

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

}  // EntityConfigEx
