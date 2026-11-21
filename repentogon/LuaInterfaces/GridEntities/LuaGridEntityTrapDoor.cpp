#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityTrapDoor_Update(GridEntity_TrapDoor* trapDoor) {
		trapDoor->Update();
	}
}