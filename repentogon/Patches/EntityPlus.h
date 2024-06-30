#pragma once

#include <optional>
#include <memory>

#include "IsaacRepentance.h"

class EntityPlus {
  public:
	virtual ~EntityPlus() {}

	std::optional<bool> isFlyingOverride = std::nullopt;
};

class EntityLaserPlus : public EntityPlus {
  public:
	bool recalculateSamplesNextUpdate = false;
};

struct EntityPlusHolder {
	std::unique_ptr<EntityPlus> data;
};

EntityPlus* GetEntityPlus(Entity* entity);
EntityLaserPlus* GetEntityPlus(Entity_Laser* laser);
