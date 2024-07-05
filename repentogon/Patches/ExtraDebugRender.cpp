#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"

HOOK_METHOD(EntityList, QueryRadius, (Vector* Position, float Radius, unsigned int Partitions) -> EntityList_EL) {
	printf("FindInRadius: %.2f %.2f\n", Position->x, Position->y); // DEBUG
	if (repentogonOptions.renderDebugFindInRadius && (*g_Game->GetDebugFlags() >> 5 & 1)) {
		//Vector vec{ 1,1 };
		//Capsule capsule{ Position, &vec, 0, Radius };
		auto* shape = g_Game->GetDebugRenderer()->Get(-1, true);
		*shape->GetTimeout() = 1;
		shape->Circle(Position, Radius);
	}
	return super(Position, Radius, Partitions);
}