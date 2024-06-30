#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "../LuaInit.h"
#include "EntityPlus.h"

void SetIsFlyingOverride(Entity* entity, const bool value) {
	EntityPlus* entityPlus = GetEntityPlus(entity);
	if (entityPlus) {
		entityPlus->isFlyingOverride = value;
	}
}

void ClearIsFlyingOverride(Entity* entity) {
	EntityPlus* entityPlus = GetEntityPlus(entity);
	if (entityPlus) {
		entityPlus->isFlyingOverride = std::nullopt;
	}
}

HOOK_METHOD(Entity, IsFlying, ()->bool) {
	EntityPlus* entityPlus = GetEntityPlus(this);
	if (entityPlus && entityPlus->isFlyingOverride) {
		return *entityPlus->isFlyingOverride;
	}
	return super();
}
