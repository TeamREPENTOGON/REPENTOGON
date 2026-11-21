#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"

MOD_EXPORT void L_EntityBomb_UpdateDirtColor(Entity_Bomb* bomb) {
	bomb->UpdateDirtColor();
}

MOD_EXPORT ANM2* L_EntityBomb_GetCostumeLayerSprite(Entity_Bomb* bomb, int index) {
	return &bomb->_bombCostumesSprites[index];
}

MOD_EXPORT unsigned int L_EntityBomb_GetHitListSize(Entity_Bomb* bomb) {
	return (unsigned int)bomb->_hitList.size();
}

MOD_EXPORT unsigned int L_EntityBomb_GetHitListEntry(Entity_Bomb* bomb, unsigned int index) {
	return bomb->_hitList[index];
}