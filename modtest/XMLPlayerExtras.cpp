#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "XMLData.h"

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {

	super(type, variant, subtype, initSeed);
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[subtype];
	std::map<std::string, int*> intValues = {
		std::pair<std::string, int*> {"heartcontainers", this->GetMaxHearts()},
		std::pair<std::string, int*> {"redhearts", this->GetRedHearts()},
		std::pair<std::string, int*> {"bonehearts", this->GetBoneHearts()},
		std::pair<std::string, int*> {"soulhearts", this->GetSoulHearts()},
		std::pair<std::string, int*> {"eternalheart", this->GetEternalHearts()},
		std::pair<std::string, int*> {"goldenhearts", this->GetGoldenHearts()},
		std::pair<std::string, int*> {"brokenhearts", this->GetBrokenHearts()},
	};

	for (std::pair<std::string, int*> value : intValues) {
		if (!playerXML[value.first].empty())
			*value.second = stoi(playerXML[value.first]);
	}

	// Special cases, where direct setting doesn't work right

	if (!playerXML["blackhearts"].empty()) {
		this->AddBlackHearts(stoi(playerXML["blackhearts"]));
	}

	if (!playerXML["rottenhearts"].empty()) {
		// WHY does HALF A HEART equal HALF A ROTTEN HEART? WHAT?????
		this->AddRottenHearts(stoi(playerXML["rottenhearts"]) * 2, false);
	}
}

HOOK_METHOD_PRIORITY(Entity_Player, GetHealthType, 100, () -> int) {
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[this->GetPlayerType()];

	int orig = super();

	if (!playerXML["healthtype"].empty()) {
		int healthType = stoi(playerXML["healthtype"]);
		if (healthType > 4 || healthType < 0) { // INVALID
			return orig;
		}
		return healthType;
	}
	return orig;
}