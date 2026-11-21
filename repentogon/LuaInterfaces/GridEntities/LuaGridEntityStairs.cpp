#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityStairs_Update(GridEntity_Stairs* stairs) {
		stairs->Update();
	}
}