#pragma once

#include <optional>
#include <memory>

#include "IsaacRepentance.h"

// Attributes shared by all entities.
class EntityPlus {
  public:
	virtual ~EntityPlus() {}

	std::optional<bool> isFlyingOverride = std::nullopt;
};

// Attributes for EntityLaser.
class EntityLaserPlus : public EntityPlus {
  public:
	bool recalculateSamplesNextUpdate = false;
};

// Returns a ptr to the base EntityPlus class held by the entity.
// In theory should typically not be nullptr as it, or a child class, is initialized on entity Init.
EntityPlus* GetEntityPlus(Entity* entity);

// Gets the corresponding EntityPlus subclass for each Entity subclass.
// Will return nullptr if somehow called for the wrong entity type, or if the entity is not properly initialized.
EntityLaserPlus* GetEntityLaserPlus(Entity_Laser* laser);

void ASMPatchesForEntityPlus();
