#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "XMLData.h"

constexpr char PICKUP_IS_CHEST[] = "pickupischest";

HOOK_STATIC(Entity_Pickup, IsChest, (unsigned int variant) -> bool) {
	if (XMLStuff.EntityData->HasCustomTag(5, variant, 1, PICKUP_IS_CHEST)) {
		return true;
	}
	return super(variant);
}
