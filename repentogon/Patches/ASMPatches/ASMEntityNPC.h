#pragma once
#include <vector>
#include <stdexcept>

struct FireProjectilesStorage {
	std::vector<Entity_Projectile*> projectiles;
	bool inUse = false;
};

extern thread_local FireProjectilesStorage projectilesStorage;

static std::vector<Entity_Projectile*>& InitProjectileStorage() {
	std::vector<Entity_Projectile*>& projectiles = projectilesStorage.projectiles;
	projectiles.clear();
	projectilesStorage.inUse = true;
	return projectiles;
}

void ASMPatchHushBug();
void ASMPatchFireProjectiles();
void ASMPatchFireBossProjectiles();
void ASMPatchAddWeakness();
void ASMPatchApplyFrozenEnemyDeathEffects();