#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"

#include "../LuaEntityBridge.h"
#include "../CustomCallbacks.h"
#include "../../Patches/XMLData.h"
#include "../../Patches/ASMPatches/ASMEntityNPC.h"
#include "../../Patches/EntityPlus.h"

namespace {
	std::vector<Entity_Projectile*> s_projectileResults;

	int StoreProjectileResults(std::vector<Entity_Projectile*>& projectiles) {
		s_projectileResults = projectiles;
		projectilesStorage.projectiles.clear();
		projectilesStorage.inUse = false;
		return (int)s_projectileResults.size();
	}

	int StoreQueryResults(EntityList_EL& result) {
		unsigned int size = result._size;
		StoreEntityResults(result._data, size);
		if (size) {
			result.Destroy();
		}
		return (int)size;
	}
}

MOD_EXPORT void L_EntityNPC_Morph(Entity_NPC* npc, int type, int variant, int subType, int championColorIdx) {
	npc->Morph(type, variant, subType, championColorIdx);
}

MOD_EXPORT void L_EntityNPC_KillUnique(Entity_NPC* npc) {
	npc->KillUnique();
}

MOD_EXPORT void L_EntityNPC_SetCanShutDoors(Entity_NPC* npc, bool canShutDoors) {
	npc->SetCanShutDoors(canShutDoors);
}

MOD_EXPORT void L_EntityNPC_SetScale(Entity_NPC* npc, float scale) {
	npc->SetScale(scale);
}

MOD_EXPORT void L_EntityNPC_ResetPathFinderTarget(Entity_NPC* npc) {
	npc->ResetPathFinderTarget();
}

MOD_EXPORT bool L_EntityNPC_CanReroll(Entity_NPC* npc) {
	return npc->CanReroll();
}

MOD_EXPORT void L_EntityNPC_MakeChampion(Entity_NPC* npc, unsigned int seed, int championColorIdx, bool init) {
	npc->MakeChampion(seed, championColorIdx, init);
}

MOD_EXPORT Entity_Effect* L_EntityNPC_MakeSplat(Entity_NPC* npc, float scale) {
	return npc->MakeSplat(scale);
}

MOD_EXPORT int L_EntityNPC_GetAliveEnemyCount(Entity_NPC* npc) {
	return npc->GetAliveEnemyCount();
}

MOD_EXPORT void L_EntityNPC_AnimWalkFrame(Entity_NPC* npc, const char* horizontalAnim, const char* verticalAnim, float threshold) {
	std::string horizontal(horizontalAnim);
	std::string vertical(verticalAnim);
	npc->AnimWalkFrame(horizontal, vertical, threshold);
}

MOD_EXPORT int L_EntityNPC_QueryNPCsType(Entity_NPC* npc, int type, int variant) {
	EntityList_EL result(*g_Game->GetCurrentRoom()->GetEntityList()->GetUpdateEL());
	npc->QueryNPCsType(&result, type, variant);
	return StoreQueryResults(result);
}

MOD_EXPORT int L_EntityNPC_QueryNPCsSpawnerType(Entity_NPC* npc, int type, int variant, bool onlyEnemies) {
	EntityList_EL result(*g_Game->GetCurrentRoom()->GetEntityList()->GetUpdateEL());
	npc->QueryNPCsSpawnerType(&result, type, variant, onlyEnemies);
	return StoreQueryResults(result);
}

MOD_EXPORT int L_EntityNPC_QueryNPCsGroup(Entity_NPC* npc, int groupIdx) {
	EntityList_EL result(*g_Game->GetCurrentRoom()->GetEntityList()->GetUpdateEL());
	npc->QueryNPCsGroup(&result, groupIdx);
	return StoreQueryResults(result);
}

MOD_EXPORT Entity* L_EntityNPC_GetPlayerTarget(Entity_NPC* npc) {
	return npc->GetPlayerTarget();
}

MOD_EXPORT void L_EntityNPC_CalcTargetPosition(Entity_NPC* npc, float distanceLimit, Vector* result) {
	npc->CalcTargetPosition(result, distanceLimit);
}

MOD_EXPORT bool L_EntityNPC_CanBeDamagedFromVelocity(Entity_NPC* npc, Vector* velocity) {
	return npc->CanBeDamagedFromVelocity(velocity);
}

MOD_EXPORT Entity_Projectile* L_EntityNPC_FireBossProjectiles(Entity_NPC* npc, int numProjectiles, Vector* targetPosition, float trajectoryModifier, ProjectileParams* params) {
	return npc->FireBossProjectiles(numProjectiles, *targetPosition, trajectoryModifier, *params);
}

MOD_EXPORT void L_EntityNPC_FireProjectiles(Entity_NPC* npc, Vector* position, Vector* velocity, unsigned int mode, ProjectileParams* params) {
	npc->FireProjectiles(position, velocity, mode, params);
}

MOD_EXPORT int L_EntityNPC_FireBossProjectilesEx(Entity_NPC* npc, int numProjectiles, Vector* targetPosition, float trajectoryModifier, ProjectileParams* params) {
	std::vector<Entity_Projectile*>& projectiles = InitProjectileStorage();
	npc->FireBossProjectiles(numProjectiles, *targetPosition, trajectoryModifier, *params);
	return StoreProjectileResults(projectiles);
}

MOD_EXPORT int L_EntityNPC_FireProjectilesEx(Entity_NPC* npc, Vector* position, Vector* velocity, unsigned int mode, ProjectileParams* params) {
	std::vector<Entity_Projectile*>& projectiles = InitProjectileStorage();
	npc->FireProjectiles(position, velocity, mode, params);
	return StoreProjectileResults(projectiles);
}

MOD_EXPORT Entity_Projectile* L_EntityNPC_GetProjectileResult(unsigned int index) {
	return s_projectileResults[index];
}

MOD_EXPORT int L_EntityNPC_GetBackdropId() {
	return g_Game->_room->GetBackdrop()->backdropId;
}

MOD_EXPORT Entity_Projectile* L_EntityNPC_FireGridEntity(Entity_NPC* npc, ANM2* sprite, GridEntityDesc* desc, Vector* velocity, int backdrop) {
	return npc->FireGridEntity(sprite, desc, velocity, backdrop);
}

MOD_EXPORT void L_EntityNPC_PlaySound(Entity_NPC* npc, int id, float volume, int frameDelay, bool loop, float pitch) {
	npc->PlaySound(id, volume, frameDelay, loop, pitch);
}

MOD_EXPORT Entity_Effect* L_EntityNPC_MakeBloodCloud(Entity_NPC* npc, Vector* position, ColorMod* color) {
	Vector pos = position ? *position : *npc->GetPosition();
	ColorMod effectColor = color ? *color : ColorMod();
	return npc->MakeBloodCloud(&pos, &effectColor);
}

MOD_EXPORT void L_EntityNPC_MakeBloodSplash(Entity_NPC* npc) {
	npc->MakeBloodSplash();
}

MOD_EXPORT void L_EntityNPC_UpdateDirtColor(Entity_NPC* npc, bool lerp) {
	npc->UpdateDirtColor(lerp);
}

MOD_EXPORT bool L_EntityNPC_TryForceTarget(Entity_NPC* npc, Entity* target, int duration) {
	return npc->TryForceTarget(target, duration);
}

MOD_EXPORT unsigned int L_EntityNPC_GetHitListSize(Entity_NPC* npc) {
	return (unsigned int)npc->GetHitList()->size();
}

MOD_EXPORT unsigned int L_EntityNPC_GetHitListEntry(Entity_NPC* npc, unsigned int index) {
	return (*npc->GetHitList())[index];
}

MOD_EXPORT void L_EntityNPC_SetEntityRef(Entity_NPC* npc, Entity* entity) {
	reinterpret_cast<EntityPtr*>(reinterpret_cast<char*>(npc) + 0xbec)->SetReference(entity);
}

MOD_EXPORT int L_EntityNPC_GetFlyingOverride(Entity_NPC* npc) {
	EntityPlus* entityPlus = GetEntityPlus(npc);
	if (entityPlus && entityPlus->isFlyingOverride.has_value()) {
		return *entityPlus->isFlyingOverride ? 1 : 0;
	}
	return -1;
}

MOD_EXPORT void L_EntityNPC_SetFlyingOverride(Entity_NPC* npc, bool isFlying) {
	EntityPlus* entityPlus = GetEntityPlus(npc);
	if (entityPlus) {
		entityPlus->isFlyingOverride = isFlying;
	}
}

MOD_EXPORT void L_EntityNPC_ClearFlyingOverride(Entity_NPC* npc) {
	EntityPlus* entityPlus = GetEntityPlus(npc);
	if (entityPlus) {
		entityPlus->isFlyingOverride = std::nullopt;
	}
}

MOD_EXPORT void L_EntityNPC_ApplyTearflagEffects(Entity_NPC* npc, Vector* position, BitSet128* flags, Entity* source, float damage) {
	Entity_Tear::ApplyTearFlagEffects(npc, position, *flags, source, damage);
}

MOD_EXPORT bool L_EntityNPC_IsBossColor(Entity_NPC* npc) {
	std::tuple idx = { npc->_type, npc->_variant };
	if (XMLStuff.BossColorData->bytypevar.find(idx) != XMLStuff.BossColorData->bytypevar.end()) {
		vector<XMLAttributes> vecnodes = XMLStuff.BossColorData->childs[XMLStuff.BossColorData->bytypevar[idx]]["color"];
		if ((npc->_subtype > 0) && (vecnodes.size() > (npc->_subtype - 1))) {
			return true;
		}
	}
	return false;
}

MOD_EXPORT bool L_EntityNPC_TrySplit(Entity_NPC* npc, float defaultDamage, EntityRef* source, bool doScreenEffects) {
	return npc->TrySplit(defaultDamage, source, doScreenEffects);
}

MOD_EXPORT bool L_EntityNPC_ReplaceSpritesheet(Entity_NPC* npc, int layerId, const char* newSpriteSheet, bool loadGraphics) {
	std::string sheet(newSpriteSheet);
	std::string input;

	npc->translate_gfx_path(input, sheet);

	bool successful = npc->_sprite.ReplaceSpritesheet(layerId, input);

	if (successful && loadGraphics) {
		npc->_sprite.LoadGraphics(false);
	}
	return successful;
}

MOD_EXPORT void L_EntityNPC_UpdatePickupGhosts(Entity_NPC* npc) {
	npc->UpdatePickupGhosts();
}

MOD_EXPORT void L_EntityNPC_GetLootList(Entity_NPC* npc, bool shouldAdvance, LootList* out) {
	LootList list = CustomCallbacks::GetNpcLootList(*npc, shouldAdvance);
	out->~LootList();
	new (out) LootList(std::move(list));
}

MOD_EXPORT void L_EntityNPC_GetFireplaceLoot(Entity_NPC* npc, bool shouldAdvance, LootList* out) {
	LootList list = npc->fireplace_get_loot(shouldAdvance);
	out->~LootList();
	new (out) LootList(std::move(list));
}

MOD_EXPORT void L_EntityNPC_GetShopkeeperLoot(Entity_NPC* npc, bool shouldAdvance, LootList* out) {
	LootList list = npc->shopkeeper_get_loot(shouldAdvance);
	out->~LootList();
	new (out) LootList(std::move(list));
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowSpider(Vector* position, Entity* spawner, Vector* targetPosition, bool big, float yOffset) {
	return (Entity_NPC*)Entity_NPC::ThrowSpider(position, spawner, *targetPosition, big, yOffset);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowMaggot(Vector* origin, Vector* target, float yOffset, float fallSpeed) {
	return Entity_NPC::ThrowMaggot(origin, yOffset, target, fallSpeed);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowMaggotAtPos(Vector* origin, Vector* target, float yOffset) {
	return Entity_NPC::ThrowMaggotAtPos(origin, target, yOffset);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ShootMaggotProjectile(Vector* origin, Vector* target, float velocity, float yOffset) {
	return Entity_NPC::ShootMaggotProjectile(origin, yOffset, target, velocity);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowStrider(Vector* origin, Entity* spawner, Vector* target) {
	return Entity_NPC::ThrowStrider(origin, spawner, target);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowRockSpider(Vector* origin, Entity* spawner, Vector* target, int variant, float yPosOffset) {
	return Entity_NPC::ThrowRockSpider(origin, target, spawner, variant, yPosOffset);
}

MOD_EXPORT Entity_NPC* L_EntityNPC_ThrowLeech(Vector* origin, Entity* spawner, Vector* target, float yPosOffset, bool big) {
	return Entity_NPC::ThrowLeech(origin, spawner, yPosOffset, target, big);
}
