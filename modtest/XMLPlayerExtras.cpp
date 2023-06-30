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

bool playerStatOverride = true;
int playerStatOverrideId;

HOOK_METHOD(Entity_Player, EvaluateItems, () -> void) {
	int id = this->GetPlayerType();

	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[id];
	playerStatOverride = false;
	std::map<std::string, float*> damageValues = {
	std::pair<std::string, float*> {"speedmodifier", this->GetEdenSpeed()},
	std::pair<std::string, float*> {"firedelaymodifier", this->GetEdenFireDelay()},
	std::pair<std::string, float*> {"damagemodifier", this->GetEdenDamage()},
	std::pair<std::string, float*> {"rangemodifier", this->GetEdenRange()},
	std::pair<std::string, float*> {"shotspeedmodifier", this->GetEdenShotSpeed()},
	std::pair<std::string, float*> {"luckmodifier", this->GetEdenLuck()},

	};

	for (std::pair<std::string, float*> value : damageValues) {
		if (!playerXML[value.first].empty()) {
			playerStatOverride = true;
			playerStatOverrideId = id;
			*value.second = stof(playerXML[value.first]);
		}
			
	}

	if (playerStatOverride) {
		*this->GetPlayerTypeMutable() = 9; // EDEN
	}

	super();

	if (playerStatOverride) {
		*this->GetPlayerTypeMutable() = id;
	}
}

HOOK_METHOD(LuaEngine, EvaluateItems, (Entity_Player* player, int cacheFlags) -> void) {
	if (playerStatOverride) {
		*player->GetPlayerTypeMutable() = playerStatOverrideId;
	}

	super(player, cacheFlags);

	if (playerStatOverride) {
		*player->GetPlayerTypeMutable() = 9;
	}
}


HOOK_METHOD_PRIORITY(Entity_Player, GetHealthLimit, 100, (bool keeper) -> int) {
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[this->GetPlayerType()];

	int orig = super(keeper);

	if (!playerXML["healthlimit"].empty()) {
		return stoi(playerXML["healthlimit"]);
	}

	return orig;
}