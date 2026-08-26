#include "EntityConfigEx.h"

#include <vector>

#include <Log.h>

#include "IsaacRepentance.h"
#include "EvaluateStats.h"
#include "XMLData.h"

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
}

void ParseXMLData() {
	const auto& players = *g_Manager->GetEntityConfig()->GetPlayers();
	s_Players.resize(players.size());
	for (const auto& player : players) {
		if (player._id > 0) {
			if (const auto* xml = XMLStuff.PlayerData->GetNodeOrNullById(player._id)) {
				s_Players[player._id].Parse(player, *xml);
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

}  // EntityConfigEx
