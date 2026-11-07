#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "Log.h"
#include "LuaCore.h"

#include "../REPENTOGONDelirium.h"

MOD_EXPORT void L_EntityDelirium_Transform(Entity_NPC* delirium, int type, int variant, bool callback) {
	delirium::ForcedTransformations[delirium] = std::make_tuple(type, variant, callback);
	*delirium->GetDeliriumTransformationTimer() = 1;
}
