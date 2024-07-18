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

// Attributes for EntityPlayer.
class EntityPlayerPlus : public EntityPlus {
public:
	std::set<std::string> customCacheTags;
	std::unordered_map<std::string, double> customCacheResults;
};

// Attributes for EntityFamiliar.
class EntityFamiliarPlus : public EntityPlus {
public:
	std::optional<float> cachedMultiplier = std::nullopt;
	uintptr_t cachedMultiplierPlayer = 0;
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
EntityPlayerPlus* GetEntityPlayerPlus(Entity_Player* player);
EntityFamiliarPlus* GetEntityFamiliarPlus(Entity_Familiar* familiar);
EntityLaserPlus* GetEntityLaserPlus(Entity_Laser* laser);

void ASMPatchesForEntityPlus();
