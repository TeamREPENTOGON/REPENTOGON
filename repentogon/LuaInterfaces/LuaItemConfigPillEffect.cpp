#include "IsaacRepentance.h"
#include "LuaCore.h"

MOD_EXPORT bool L_ItemConfigPillEffect_IsAvailable(ItemConfig_Pill* pill) {
	return pill->IsAvailable();
}
