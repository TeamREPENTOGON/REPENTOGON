#include "IsaacRepentance.h"

MOD_EXPORT ProceduralItemManager* L_ProceduralItemManager_Get() {
	return g_Game->GetProceduralItemManager();
}

MOD_EXPORT int L_ProceduralItemManager_CreateProceduralItem(unsigned int seed, unsigned int unk) {
	return g_Game->GetProceduralItemManager()->CreateProceduralItem(seed, unk);
}
