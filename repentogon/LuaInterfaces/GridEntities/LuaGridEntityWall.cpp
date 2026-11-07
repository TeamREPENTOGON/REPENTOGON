#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityWall_Render(GridEntity_Wall* wall, Vector offset) {
		wall->Render(offset);
	}
}