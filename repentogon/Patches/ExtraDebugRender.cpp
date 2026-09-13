#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "../REPENTOGONOptions.h"

HOOK_METHOD(EntityList, QueryRadius, (Vector* Position, float Radius, unsigned int Partitions) -> EntityList_EL) {
	if (repentogonOptions.renderDebugFindInRadius && (*g_Game->GetDebugFlags() >> 5 & 1)) {
		//Vector vec{ 1,1 };
		//Capsule capsule{ Position, &vec, 0, Radius };
		auto* shape = g_Game->GetDebugRenderer()->Get(-1, true);
		*shape->GetTimeout() = 1;
		shape->Circle(Position, Radius);
	}
	return super(Position, Radius, Partitions);
}

HOOK_METHOD(EntityList, QueryCapsule, (EntityList_EL* result, Capsule* Capsule, unsigned int Partitions)->EntityList_EL*) {
	if (repentogonOptions.renderDebugFindInRadius && (*g_Game->GetDebugFlags() >> 5 & 1)) {
		auto* shape = g_Game->GetDebugRenderer()->Get(-1, true);
		*shape->GetTimeout() = 1;
		shape->Capsula(Capsule);
	}
	return super(result,Capsule,Partitions);
}