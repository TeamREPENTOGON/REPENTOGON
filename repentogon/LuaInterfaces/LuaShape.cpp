#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaClasses.h"

MOD_EXPORT void L_Shape_Capsule(Shape* shape, Capsule* capsule) {
	shape->Capsula(capsule);
}

MOD_EXPORT void L_Shape_Circle(Shape* shape, Vector* position, float size) {
	shape->Circle(position, size);
}