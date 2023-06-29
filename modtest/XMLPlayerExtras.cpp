#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "XMLData.h"

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {

	super(type, variant, subtype, initSeed);
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[subtype];
	std::map<std::string, int*> intValues = {
		std::pair<std::string, int*> {"maxhearts", this->GetMaxHearts()},
		std::pair<std::string, int*> {"redhearts", this->GetRedHearts()},
		std::pair<std::string, int*> {"bonehearts", this->GetBoneHearts()},
		std::pair<std::string, int*> {"soulhearts", this->GetSoulHearts()},
		std::pair<std::string, int*> {"eternalheart", this->GetEternalHearts()},
	};

	for (std::pair<std::string, int*> value : intValues) {
		if (!playerXML[value.first].empty())
			*value.second = stoi(playerXML[value.first]);
	}
}