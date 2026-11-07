#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) bool L_GridEntityFire_Hurt(GridEntity_Fire* fire, int damage, EntityRef* source) {
		return fire->Hurt(damage, source);
	}

	__declspec(dllexport) void L_GridEntityFire_Render(GridEntity_Fire* fire, Vector offset) {
		fire->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityFire_Update(GridEntity_Fire* fire) {
		fire->Update();
	}
}