#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Patches/FamiliarTags.h"
#include "../../Patches/EntityPlus.h"

MOD_EXPORT void L_EntityFamiliar_SetPlayer(Entity_Familiar* familiar, Entity_Player* player) {
	familiar->SetPlayer(player);
}

MOD_EXPORT void L_EntityFamiliar_AddCoins(Entity_Familiar* familiar, int coins) {
	familiar->AddCoins(coins);
}

MOD_EXPORT void L_EntityFamiliar_FollowParent(Entity_Familiar* familiar) {
	familiar->FollowParent();
}

MOD_EXPORT void L_EntityFamiliar_FollowPosition(Entity_Familiar* familiar, Vector* position) {
	familiar->FollowPosition(position);
}

MOD_EXPORT void L_EntityFamiliar_Shoot(Entity_Familiar* familiar) {
	familiar->Shoot();
}

MOD_EXPORT void L_EntityFamiliar_PlayChargeAnim(Entity_Familiar* familiar, int direction) {
	familiar->PlayChargeAnim(direction);
}

MOD_EXPORT void L_EntityFamiliar_PlayShootAnim(Entity_Familiar* familiar, int direction) {
	familiar->PlayShootAnim(direction);
}

MOD_EXPORT void L_EntityFamiliar_PlayFloatAnim(Entity_Familiar* familiar, int direction) {
	familiar->PlayFloatAnim(direction);
}

MOD_EXPORT void L_EntityFamiliar_MoveDelayed(Entity_Familiar* familiar, int frames) {
	familiar->MoveDelayed(frames);
}

MOD_EXPORT void L_EntityFamiliar_MoveDiagonally(Entity_Familiar* familiar, float speed) {
	familiar->MoveDiagonally(speed);
}

MOD_EXPORT int L_EntityFamiliar_RecalculateOrbitOffset(Entity_Familiar* familiar, int layer, bool add) {
	return familiar->RecalculateOrbitOffset(layer, add);
}

MOD_EXPORT void L_EntityFamiliar_AddToFollowers(Entity_Familiar* familiar) {
	familiar->AddToFollowers();
}

MOD_EXPORT void L_EntityFamiliar_AddToDelayed(Entity_Familiar* familiar) {
	familiar->AddToDelayed();
}

MOD_EXPORT void L_EntityFamiliar_AddToOrbit(Entity_Familiar* familiar, int layer) {
	familiar->AddToOrbit(layer);
}

MOD_EXPORT void L_EntityFamiliar_RemoveFromFollowers(Entity_Familiar* familiar) {
	familiar->RemoveFromFollowers();
}

MOD_EXPORT void L_EntityFamiliar_RemoveFromDelayed(Entity_Familiar* familiar) {
	familiar->RemoveFromDelayed();
}

MOD_EXPORT void L_EntityFamiliar_RemoveFromOrbit(Entity_Familiar* familiar) {
	familiar->RemoveFromOrbit();
}

MOD_EXPORT void L_EntityFamiliar_GetOrbitDistance(int layer, Vector* result) {
	Entity_Familiar::GetOrbitDistance(result, layer);
}

MOD_EXPORT void L_EntityFamiliar_GetOrbitPosition(Entity_Familiar* familiar, Vector* offset, Vector* result) {
	familiar->GetOrbitPosition(result, offset);
}

MOD_EXPORT Entity_Tear* L_EntityFamiliar_FireProjectile(Entity_Familiar* familiar, Vector* direction) {
	return familiar->FireProjectile(*direction, false);
}

MOD_EXPORT void L_EntityFamiliar_PickEnemyTarget(Entity_Familiar* familiar, float maxDistance, int frameInterval, int flags, Vector* coneDirection, float coneAngle) {
	familiar->PickEnemyTarget(maxDistance, frameInterval, flags, coneDirection, coneAngle);
}

MOD_EXPORT int L_EntityFamiliar_GetFollowerPriority(Entity_Familiar* familiar) {
	return familiar->GetFollowerPriority();
}

MOD_EXPORT NPCAI_Pathfinder* L_EntityFamiliar_GetPathfinder(Entity_Familiar* familiar) {
	return familiar->GetPathFinder();
}

MOD_EXPORT bool L_EntityFamiliar_TryAimAtMarkedTarget(Entity_Familiar* familiar, Vector* aimDirection, int* direction, Vector* targetPosition) {
	return familiar->TryAimAtMarkedTarget(aimDirection, direction, targetPosition);
}

MOD_EXPORT void L_EntityFamiliar_TriggerRoomClear(Entity_Familiar* familiar) {
	familiar->TriggerRoomClear();
}

MOD_EXPORT void L_EntityFamiliar_UpdateDirtColor(Entity_Familiar* familiar) {
	familiar->UpdateDirtColor(true);
}

MOD_EXPORT void L_EntityFamiliar_RemoveFromPlayer(Entity_Familiar* familiar) {
	familiar->RemoveFromPlayer(true);
}

MOD_EXPORT bool L_EntityFamiliar_CanCharm(Entity_Familiar* familiar) {
	return familiar->CanCharm();
}

MOD_EXPORT bool L_EntityFamiliar_IsCharmed(Entity_Familiar* familiar) {
	Entity_Player* player = familiar->_player;
	return player && (player->_spawnerType == 904 && player->_spawnerVariant == 0);
}

MOD_EXPORT bool L_EntityFamiliar_CanBeDamagedByEnemies(Entity_Familiar* familiar) {
	const int variant = *familiar->GetVariant();
	const int subtype = *familiar->GetSubType();
	// Ugh
	if (variant == 206) {
		// Wisps do get hurt by enemies, except the Vengeful Spirit ones.
		return subtype != 702;
	}
	if (variant == 201 || variant == 216 || variant == 217 || variant == 228 || variant == 237 || variant == 238) {
		// Friendly dips, Tinytomas, Minisaacs, Item Wisps and Blood Babies do, in fact, get hurt by enemy contact.
		return true;
	}
	return familiar->CanBeDamagedByEnemy();
}

MOD_EXPORT bool L_EntityFamiliar_CanBeDamagedByProjectiles(Entity_Familiar* familiar) {
	const int variant = *familiar->GetVariant();
	// Ugh 2
	if (variant == 201 || variant == 216 || variant == 217 || variant == 228 || variant == 238) {
		// Friendly dips, Tinytomas, Minisaacs and Blood Babies do, in fact, get hurt by projectiles.
		return true;
	}
	return FamiliarCanBeDamagedByProjectilesReimplementation(familiar);
}

MOD_EXPORT bool L_EntityFamiliar_CanBeDamagedByLasers(Entity_Familiar* familiar) {
	return FamiliarCanBeDamagedByLaserReimplementation(familiar);
}

MOD_EXPORT bool L_EntityFamiliar_CanBlockProjectiles(Entity_Familiar* familiar) {
	return familiar->CanBlockProjectiles();
}

MOD_EXPORT float L_EntityFamiliar_GetMultiplier(Entity_Familiar* familiar) {
	EntityFamiliarPlus* famPlus = GetEntityFamiliarPlus(familiar);
	if (famPlus && !famPlus->cachedMultiplier) {
		// We can't use the return value yet due to where the float value ends up in memory.
		// However, just calling it will trigger re-evaluation & cache the result via my ASM patch.
		familiar->GetMultiplier();
	}
	if (famPlus && famPlus->cachedMultiplier) {
		return *famPlus->cachedMultiplier;
	}
	// Uh oh
	return 1.0f;
}

MOD_EXPORT void L_EntityFamiliar_InvalidateCachedMultiplier(Entity_Familiar* familiar) {
	EntityFamiliarPlus* famPlus = GetEntityFamiliarPlus(familiar);
	if (famPlus) {
		famPlus->cachedMultiplier = std::nullopt;
	}
}

MOD_EXPORT void L_EntityFamiliar_SetLilDelirium(Entity_Familiar* familiar, bool isLilDelirium) {
	familiar->_isLilDelirium = isLilDelirium;

	if (isLilDelirium) {
		familiar->delirium_morph();
	}
}

MOD_EXPORT int L_EntityFamiliar_GetRandomWisp(RNG* rng) {
	return Entity_Familiar::GetRandomWisp(*rng);
}

MOD_EXPORT Entity* L_EntityFamiliar_GetActiveWeaponEntity(Entity_Familiar* familiar) {
	Weapon* weapon = familiar->_weapon;
	return weapon ? weapon->GetMainEntity() : nullptr;
}

// Returns -1 when there is no weapon
MOD_EXPORT int L_EntityFamiliar_GetActiveWeaponNumFired(Entity_Familiar* familiar) {
	Weapon* weapon = familiar->_weapon;
	return weapon ? weapon->_numFired : -1;
}