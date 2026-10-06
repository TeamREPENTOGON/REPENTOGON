#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include "../../Patches/EntityPlus.h"
#include "../../Patches/ASMPatches/ASMSplitTears.h"

MOD_EXPORT void L_EntityTear_SetHeight(Entity_Tear* tear, float height) {
	tear->SetHeight(height);
}

MOD_EXPORT void L_EntityTear_SetScale(Entity_Tear* tear, float scale) {
	tear->SetScale(scale);
}

MOD_EXPORT void L_EntityTear_ResetSpriteScale(Entity_Tear* tear, bool force) {
	if (force) {
		tear->_scaleAnimNum = -1;
	}
	tear->ResetSpriteScale();
}

MOD_EXPORT void L_EntityTear_ChangeVariant(Entity_Tear* tear, int variant) {
	tear->ChangeVariant(variant);
}

MOD_EXPORT void L_EntityTear_SetDeadEyeIntensity(Entity_Tear* tear, float intensity) {
	tear->SetDeadEyeIntensity(intensity);
}

MOD_EXPORT Entity_Tear* L_EntityTear_MakeMultidimensionalCopy(Entity_Tear* tear) {
	return tear->MakeMultidimensionalCopy(nullptr);
}

MOD_EXPORT ANM2* L_EntityTear_GetTearHaloSprite(Entity_Tear* tear) {
	return &tear->_tearHaloANM2;
}

MOD_EXPORT ANM2* L_EntityTear_GetTearEffectSprite(Entity_Tear* tear) {
	return &tear->_tearEffectANM2;
}

MOD_EXPORT ANM2* L_EntityTear_GetDeadEyeSprite(Entity_Tear* tear) {
	return &tear->_deadEyeANM2;
}

MOD_EXPORT unsigned int L_EntityTear_GetHitListSize(Entity_Tear* tear) {
	return (unsigned int)tear->_hitList.size();
}

MOD_EXPORT unsigned int L_EntityTear_GetHitListEntry(Entity_Tear* tear, unsigned int index) {
	return tear->_hitList[index];
}

MOD_EXPORT void L_EntityTear_ClearHitList(Entity_Tear* tear) {
	tear->_hitList.clear();
}

MOD_EXPORT void L_EntityTear_RemoveFromHitList(Entity_Tear* tear, Entity* entity) {
	auto iterator = std::find(tear->_hitList.begin(), tear->_hitList.end(), entity->GetHitListIndex());

	if (iterator != tear->_hitList.end()) {
		std::swap(*iterator, tear->_hitList.back());
		tear->_hitList.pop_back();
	}
}

MOD_EXPORT void L_EntityTear_AddToHitList(Entity_Tear* tear, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto& hitList = tear->_hitList;

	if (std::find(hitList.begin(), hitList.end(), hitListIndex) == hitList.end()) {
		hitList.push_back(hitListIndex);
	}
}

MOD_EXPORT bool L_EntityTear_InHitList(Entity_Tear* tear, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto& hitList = tear->_hitList;

	return std::find(hitList.begin(), hitList.end(), hitListIndex) != hitList.end();
}

MOD_EXPORT bool L_EntityTear_SetInitSound(Entity_Tear* tear, unsigned int soundId) {
	if (soundId >= g_Manager->_sfxManager._sounds.size()) {
		return false;
	}

	EntityTearPlus* tearPlus = GetEntityTearPlus(tear);
	assert(tearPlus);

	tearPlus->initSound = soundId;
	return true;
}

MOD_EXPORT Entity_Tear* L_Entity_FireSplitTear(Entity* source, Vector* position, Vector* velocity, float damageMultiplier, float sizeMultiplier, int variant, int splitType, const char* splitTypeName) {
	SplitTears::CustomSplitTearType type = (SplitTears::SplitTearType)splitType;
	if (splitTypeName) {
		type = std::string(splitTypeName);
	}
	return SplitTears::FireSplitTear(source, *position, *velocity, damageMultiplier, sizeMultiplier, variant, type);
}