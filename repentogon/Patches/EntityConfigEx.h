#pragma once

#include <unordered_map>

#include "IsaacRepentance.h"
#include "EvaluateStats.h"
#include "XMLData.h"

namespace EntityConfigEx {

class PlayerEx {
public:
	void Parse(const EntityConfig_Player& player, const XMLAttributes& xml);

	inline bool HasStatModifier(const EvaluateStats::PlayerStat stat) const {
		return statModifiers_.count(stat) > 0 && statModifiers_.at(stat) != GetPlayerStatDefaultValue(stat);
	}

	inline float GetStatModifier(const EvaluateStats::PlayerStat stat) const {
		if (statModifiers_.count(stat)) {
			return statModifiers_.at(stat);
		}
		return GetPlayerStatDefaultValue(stat);
	}

	inline bool NoShake() const { return noShake_; }
	inline bool HasBloodyTears() const { return bloodyTears_; }
	inline bool ForceFlying() const { return forceFlying_; }

private:
	std::unordered_map<EvaluateStats::PlayerStat, float> statModifiers_;

	bool noShake_ = false;
	bool bloodyTears_ = false;
	bool forceFlying_ = false;
};

// Scans XMLData to initialize all of the Ex configs.
void ParseXMLData();

PlayerEx* GetPlayerEx(int playertype);

}  // namespace EntityConfigEx
