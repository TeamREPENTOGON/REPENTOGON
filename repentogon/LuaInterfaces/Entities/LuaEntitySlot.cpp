#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"

MOD_EXPORT void L_EntitySlot_CreateDropsFromExplosion(Entity_Slot* slot) {
	slot->CreateDropsFromExplosion();
}

MOD_EXPORT void L_EntitySlot_SetPrizeCollectible(Entity_Slot* slot, int collectible) {
	slot->SetPrizeCollectible(collectible);
}

MOD_EXPORT ANM2* L_EntitySlot_GetPrizeSprite(Entity_Slot* slot) {
	return &slot->_prizeAnm2;
}

MOD_EXPORT int L_EntitySlot_RandomCoinJamIndex() {
	return Isaac::Random(4);
}
