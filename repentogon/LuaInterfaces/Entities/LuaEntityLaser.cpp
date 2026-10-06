#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Patches/EntityPlus.h"

MOD_EXPORT Entity_Laser* L_EntityLaser_ShootAngle(int variant, Vector* sourcePos, float angleDegrees, int timeout, Vector* posOffset, Entity* source) {
	return Entity_Laser::ShootAngle(variant, sourcePos, angleDegrees, timeout, posOffset, source, false);
}

MOD_EXPORT void L_EntityLaser_CalculateEndPoint(Vector* start, Vector* dir, Vector* positionOffset, Entity* parent, float margin, Vector* result) {
	Entity_Laser::CalculateEndPosition(result, start, dir, positionOffset, parent, margin);
}

MOD_EXPORT void L_EntityLaser_SetAngle(Entity_Laser* laser, float angle) {
	laser->SetAngle(angle);
}

MOD_EXPORT void L_EntityLaser_SetActiveRotation(Entity_Laser* laser, int delay, float degrees, float speed, bool setTimeout) {
	laser->SetActiveRotation(delay, degrees, speed, setTimeout);
}

MOD_EXPORT int L_EntityLaser_GetRenderZ(Entity_Laser* laser) {
	void** vtable = *reinterpret_cast<void***>(laser);
	return reinterpret_cast<int(__thiscall*)(Entity*)>(vtable[0x34 / sizeof(void*)])(laser);
}

MOD_EXPORT void L_EntityLaser_ResetSpriteScale(Entity_Laser* laser) {
	laser->ResetSpriteScale();
}

MOD_EXPORT void L_EntityLaser_RotateToAngle(Entity_Laser* laser, float angle, float speed) {
	laser->RotateToAngle(angle, speed);
}

MOD_EXPORT void L_EntityLaser_RecalculateSamplesNextUpdate(Entity_Laser* laser) {
	EntityLaserPlus* laserPlus = GetEntityLaserPlus(laser);
	if (laserPlus) {
		laserPlus->recalculateSamplesNextUpdate = true;
	}
}

MOD_EXPORT bool L_EntityLaser_SetInitSound(Entity_Laser* laser, unsigned int soundId) {
	if (soundId >= g_Manager->_sfxManager._sounds.size()) {
		return false;
	}

	EntityLaserPlus* laserPlus = GetEntityLaserPlus(laser);
	assert(laserPlus);
	if (laserPlus) {
		laserPlus->initSound = soundId;
	}
	return true;
}

MOD_EXPORT void L_EntityLaser_SetBounceLaser(Entity_Laser* laser, Entity* bounceLaser) {
	reinterpret_cast<EntityPtr*>(reinterpret_cast<char*>(laser) + 0x4e8)->SetReference(bounceLaser);
}

MOD_EXPORT unsigned int L_EntityLaser_GetHitListSize(Entity_Laser* laser) {
	return (unsigned int)laser->GetHitList()->size();
}

MOD_EXPORT unsigned int L_EntityLaser_GetHitListEntry(Entity_Laser* laser, unsigned int index) {
	return (*laser->GetHitList())[index];
}

MOD_EXPORT void L_EntityLaser_RemoveFromHitList(Entity_Laser* laser, Entity* entity) {
	auto hitList = laser->GetHitList();
	auto iterator = std::find(hitList->begin(), hitList->end(), entity->GetHitListIndex());

	if (iterator != hitList->end()) {
		std::swap(*iterator, hitList->back());
		hitList->pop_back();
	}
}

MOD_EXPORT void L_EntityLaser_AddToHitList(Entity_Laser* laser, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = laser->GetHitList();

	if (std::find(hitList->begin(), hitList->end(), hitListIndex) == hitList->end()) {
		hitList->push_back(hitListIndex);
	}
}

MOD_EXPORT bool L_EntityLaser_InHitList(Entity_Laser* laser, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = laser->GetHitList();

	return std::find(hitList->begin(), hitList->end(), hitListIndex) != hitList->end();
}
