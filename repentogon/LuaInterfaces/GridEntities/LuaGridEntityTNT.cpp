#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) bool L_GridEntityTNT_Destroy(GridEntity_TNT* tnt, bool immediate, EntityRef* source) {
		return tnt->Destroy(immediate, source);
	}

	__declspec(dllexport) bool L_GridEntityTNT_Hurt(GridEntity_TNT* tnt, int damage, EntityRef* source) {
		return tnt->Hurt(damage, source);
	}

	__declspec(dllexport) void L_GridEntityTNT_Update(GridEntity_TNT* tnt) {
		tnt->Update();
	}
}