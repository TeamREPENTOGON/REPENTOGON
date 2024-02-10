#include "IsaacRepentance.h"
#include "LuaCore.h"

extern "C" {
	void L_EntityNPC_AnimWalkFrame(Entity_NPC* npc, const char* horizontal, const char* vertical, float threshold) {
		std::string namestr[2] = { horizontal, vertical };
		npc->AnimWalkFrame(&namestr[0], &namestr[1], threshold);
	}

	Vector* L_EntityNPC_CalculateTargetPosition(Entity_NPC* npc, float limit) {
		Vector ret;
		npc->CalcTargetPosition(&ret, limit);
		return &ret;
	}

	bool L_EntityNPC_CanBeDamagedFromVelocity(Entity_NPC* npc, Vector* vel) {
		return npc->CanBeDamagedFromVelocity(*vel);
	}

	bool L_EntityNPC_CanReroll(Entity_NPC* npc) {
		return npc->CanReroll();
	}

	Entity_Projectile* L_EntityNPC_FireBossProjectiles(Entity_NPC* npc, int num, Vector* target, float trajectoryMod, ProjectileParams* params) {
		return npc->FireBossProjectiles(num, *target, trajectoryMod, *params);
	}

	int L_EntityNPC_GetAliveEnemyCount(Entity_NPC* npc) {
		return npc->GetAliveEnemyCount();
	}

	int L_EntityNPC_GetBossColorIdx(Entity_NPC* npc) {
		return npc->_bossColorIdx;
	}

	int L_EntityNPC_GetChampionColorIdx(Entity_NPC* npc) {
		return npc->_championId;
	}

	Entity* L_EntityNPC_GetPlayerTarget(Entity_NPC* npc) {
		return npc->GetPlayerTarget();
	}

	bool L_EntityNPC_IsBoss(Entity_NPC* npc) {
		return npc->_isBoss;
	}

	bool L_EntityNPC_IsChampion(Entity_NPC* npc) {
		return npc->_isChampion;
	}

	void L_EntityNPC_KillUnique(Entity_NPC* npc) {
		npc->Kill();
		npc->_state = 17;
		npc->_stateFrame = 0;
	}

	void L_EntityNPC_MakeChampion(Entity_NPC* npc, unsigned int seed, int championColorIdx, bool init) {
		npc->MakeChampion(seed, championColorIdx, init);
	}

	Entity_Effect* L_EntityNPC_MakeSplat(Entity_NPC* npc, float splatSize) {
		return npc->MakeSplat(splatSize);
	}

	bool L_EntityNPC_Morph(Entity_NPC* npc, int type, int variant, int subtype, int championColorIdx) {
		return npc->Morph(type, variant, subtype, championColorIdx);
	}

	void L_EntityNPC_PlaySound(Entity_NPC* npc, int id, float volume, int delay, bool loop, float pitch) {
		npc->PlaySoundNPC(id, volume, delay, loop, pitch);
	}

	EntityList_EL* L_EntityNPC_QueryNPCsGroup(Entity_NPC* npc, int idx) {
		EntityList_EL list;
		npc->QueryNPCsGroup(&list, idx);
		return &list;
	}

	EntityList_EL* L_EntityNPC_QueryNPCsSpawnerType(Entity_NPC* npc, int spawnerType, int type, bool onlyEnemies) {
		EntityList_EL list;
		npc->QueryNPCsSpawnerType(&list, spawnerType, type, onlyEnemies);
		return &list;
	}

	EntityList_EL* L_EntityNPC_QueryNPCsType(Entity_NPC* npc, int type, int variant) {
		EntityList_EL list;
		npc->QueryNPCsType(&list, type, variant);
		return &list;
	}

	void L_EntityNPC_ResetPathFinderTarget(Entity_NPC* npc) {
		npc->_pathfinder._targetRelated1 = Vector();
		npc->_pathfinder._targetRelated2 = Vector();
	}

	// TODO: investigate unk default value
	Entity_NPC* L_EntityNPC_ThrowSpider(Vector* pos, Entity* spawner, Vector* target, bool big, float yOffset) {
		return Entity_NPC::ThrowSpider(pos, spawner, target, yOffset, big, 0.0f);
	}

	bool L_EntityNPC_GetCanShutDoors(Entity_NPC* npc) {
		return npc->CanShutDoors();
	}

	void L_EntityNPC_SetCanShutDoors(Entity_NPC* npc, bool value) {
		npc->_canShutDoors = value;
	}

	Entity_NPC* L_EntityNPC_GetChildNPC(Entity_NPC* npc) {
		return npc->GetChildNPC();
	}

	Entity_NPC* L_EntityNPC_GetParentNPC(Entity_NPC* npc) {
		return npc->GetParentNPC();
	}

	Entity* L_EntityNPC_GetEntityRef(Entity_NPC* npc) {
		return npc->_entityRef;
	}

	void L_EntityNPC_SetEntityRef(Entity_NPC* npc, Entity* value) {
		npc->_entityRef = value;
	}

	int L_EntityNPC_GetGroupIdx(Entity_NPC* npc) {
		return npc->_groupIdx;
	}

	void L_EntityNPC_SetGroupIdx(Entity_NPC* npc, int value) {
		npc->_groupIdx = value;
	}

	int L_EntityNPC_GetI1(Entity_NPC* npc) {
		return npc->_i1;
	}

	void L_EntityNPC_SetI1(Entity_NPC* npc, int value) {
		npc->_i1 = value;
	}

	int L_EntityNPC_GetI2(Entity_NPC* npc) {
		return npc->_i2;
	}

	void L_EntityNPC_SetI2(Entity_NPC* npc, int value) {
		npc->_i2 = value;
	}

	Vector* L_EntityNPC_GetV1(Entity_NPC* npc) {
		return &npc->_v1;
	}

	void L_EntityNPC_SetV1(Entity_NPC* npc, Vector* value) {
		npc->_v1 = *value;
	}

	Vector* L_EntityNPC_GetV2(Entity_NPC* npc) {
		return &npc->_v2;
	}

	void L_EntityNPC_SetV2(Entity_NPC* npc, Vector* value) {
		npc->_v2 = *value;
	}

	NPCAI_Pathfinder* L_EntityNPC_GetPathfinder(Entity_NPC* npc) {
		return &npc->_pathfinder;
	}

	int L_EntityNPC_GetProjectileCooldown(Entity_NPC* npc) {
		return npc->_projectileCooldown;
	}

	void L_EntityNPC_SetProjectileCooldown(Entity_NPC* npc, int value) {
		npc->_projectileCooldown = value;
	}

	float L_EntityNPC_GetScale(Entity_NPC* npc) {
		return npc->_npcScale;
	}

	void L_EntityNPC_SetScale(Entity_NPC* npc, float value) {
		npc->_npcScale = value;
	}

	int L_EntityNPC_GetState(Entity_NPC* npc) {
		return npc->_state;
	}

	void L_EntityNPC_SetState(Entity_NPC* npc, int value) {
		npc->_state = value;
	}

	int L_EntityNPC_GetStateFrame(Entity_NPC* npc) {
		return npc->_stateFrame;
	}

	void L_EntityNPC_SetStateFrame(Entity_NPC* npc, int value) {
		npc->_stateFrame = value;
	}
}