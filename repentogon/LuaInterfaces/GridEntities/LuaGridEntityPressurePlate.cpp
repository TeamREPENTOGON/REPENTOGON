#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) const char* L_GridEntityPressurePlate_GetNextGreedAnimation(GridEntity_PressurePlate* plate) {
		return plate->_nextGreedAnimation.c_str();
	}

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