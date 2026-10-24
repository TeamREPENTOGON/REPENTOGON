#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../LuaClasses.h"
#include "../../Patches/EntityPlus.h"

MOD_EXPORT void L_EntityKnife_Shoot(Entity_Knife* knife, float percent, float range) {
	knife->Shoot(percent, range);
}

MOD_EXPORT void L_EntityKnife_Reset(Entity_Knife* knife) {
	knife->Reset();
}

MOD_EXPORT int L_EntityKnife_GetRenderZ(Entity_Knife* knife) {
	void** vtable = *reinterpret_cast<void***>(knife);
	return reinterpret_cast<int(__thiscall*)(Entity*)>(vtable[0x34 / sizeof(void*)])(knife);
}

MOD_EXPORT unsigned int L_EntityKnife_GetHitListSize(Entity_Knife* knife) {
	return (unsigned int)knife->GetHitEntities()->size();
}

MOD_EXPORT unsigned int L_EntityKnife_GetHitListEntry(Entity_Knife* knife, unsigned int index) {
	return (*knife->GetHitEntities())[index];
}

MOD_EXPORT void L_EntityKnife_RemoveFromHitList(Entity_Knife* knife, Entity* entity) {
	auto hitList = knife->GetHitEntities();
	auto iterator = std::find(hitList->begin(), hitList->end(), entity->GetHitListIndex());

	if (iterator != hitList->end()) {
		std::swap(*iterator, hitList->back());
		hitList->pop_back();
	}
}

MOD_EXPORT void L_EntityKnife_AddToHitList(Entity_Knife* knife, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = knife->GetHitEntities();

	if (std::find(hitList->begin(), hitList->end(), hitListIndex) == hitList->end()) {
		hitList->push_back(hitListIndex);
	}
}

MOD_EXPORT bool L_EntityKnife_InHitList(Entity_Knife* knife, Entity* entity) {
	int hitListIndex = entity->GetHitListIndex();
	auto hitList = knife->GetHitEntities();

	return std::find(hitList->begin(), hitList->end(), hitListIndex) != hitList->end();
}

MOD_EXPORT void L_EntityKnife_InitHomingPath(Entity_Knife* knife, Vector* direction, Entity* source) {
	if (!source) {
		source = knife;
		if (Entity* parent = knife->GetParent()) {
			source = parent;
		}
	}
	knife->InitHomingPath(*direction, source, knife->_pathOffset);
}

MOD_EXPORT Entity_Knife* L_EntityKnife_GetHitboxParentKnife(Entity_Knife* knife) {
	EntityKnifePlus* entityPlus = GetEntityKnifePlus(knife);
	Entity* parentKnife = entityPlus ? entityPlus->hitboxSource.GetReference() : nullptr;
	if (parentKnife && parentKnife->_type == ENTITY_KNIFE) {
		return (Entity_Knife*)parentKnife;
	}
	return nullptr;
}

MOD_EXPORT void L_EntityKnife_SetHitboxParentKnife(Entity_Knife* knife, Entity_Knife* parentKnife) {
	EntityKnifePlus* entityPlus = GetEntityKnifePlus(knife);
	if (entityPlus) {
		entityPlus->hitboxSource.SetReference(parentKnife);
	}
}
