#include "IsaacRepentance.h"
#include "libzhl.h"

MOD_EXPORT void L_EntityPtr_SetReference(EntityPtr* ptr, Entity* entity) {
	ptr->SetReference(entity);
}
