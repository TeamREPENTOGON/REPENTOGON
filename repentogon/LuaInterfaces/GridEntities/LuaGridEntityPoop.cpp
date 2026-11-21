#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) bool L_GridEntityPoop_Hurt(GridEntity_Poop* poop, int damage, EntityRef* source) {
		return poop->Hurt(damage, source);
	}

	__declspec(dllexport) void L_GridEntityPoop_PostInit(GridEntity_Poop* poop) {
		poop->PostInit();
	}

	__declspec(dllexport) void L_GridEntityPoop_Render(GridEntity_Poop* poop, Vector offset) {
		poop->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityPoop_Update(GridEntity_Poop* poop) {
		poop->Update();
	}
}