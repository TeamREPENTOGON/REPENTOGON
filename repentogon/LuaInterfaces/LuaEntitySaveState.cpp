#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../SaveStateManagement/EntitySaveStateManagement.h"
#include "../LuaClasses.h"

namespace ESSM = EntitySaveStateManagement;

MOD_EXPORT uint32_t* L_EntitySaveState_GetI7(EntitySaveState* saveState) {
	return &ESSM::EntitySaveState_GetI7(*saveState);
}

MOD_EXPORT int16_t* L_EntitySaveState_GetGridSpawnIdx(EntitySaveState* saveState) {
	return &ESSM::EntitySaveState_GetGridSpawnIdx(*saveState);
}

MOD_EXPORT void L_EntitiesSaveStateVector_Clear(std::vector<EntitySaveState>* vector) {
	ESSM::EntitySaveState_ClearBatch(*vector);
	vector->clear();
}
