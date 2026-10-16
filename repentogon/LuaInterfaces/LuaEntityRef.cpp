#include "IsaacRepentance.h"
#include "libzhl.h"

MOD_EXPORT void L_EntityRef_Init(EntityRef* ref, Entity* entity) {
	new (ref) EntityRef(entity);
}
