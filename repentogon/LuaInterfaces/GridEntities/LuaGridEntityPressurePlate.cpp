#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityPressurePlate_Render(GridEntity_PressurePlate* plate, Vector offset) {
		plate->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityPressurePlate_Reward(GridEntity_PressurePlate* plate) {
		plate->Reward();
	}

	__declspec(dllexport) void L_GridEntityPressurePlate_Update(GridEntity_PressurePlate* plate) {
		plate->Update();
	}
}