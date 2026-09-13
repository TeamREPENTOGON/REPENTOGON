#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityGravity_Update(GridEntity_Gravity* gravity) {
		gravity->Update();
	}
}