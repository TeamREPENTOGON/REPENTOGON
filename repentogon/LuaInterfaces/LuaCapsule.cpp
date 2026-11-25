#include "IsaacRepentance.h"

MOD_EXPORT void L_Capsule_Ctor(Capsule* buffer, Vector* position, Vector* sizeMult, float rotation, float size) {
	buffer->constructor(position, sizeMult, rotation, size);
}

MOD_EXPORT void L_Capsule_Ctor2(Capsule* buffer, Vector* position, Vector* targetPosition, float size) {
	buffer->constructor2(position, targetPosition, size);
}

MOD_EXPORT bool L_Capsule_Collide(Capsule* cap1, Capsule* cap2, Vector* point) {
	return cap1->Collide(cap1, cap2, point);
}