#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityDecoration_Render(GridEntity_Decoration* decoration, Vector offset) {
		decoration->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityDecoration_Update(GridEntity_Decoration* decoration) {
		decoration->Update();
	}
}