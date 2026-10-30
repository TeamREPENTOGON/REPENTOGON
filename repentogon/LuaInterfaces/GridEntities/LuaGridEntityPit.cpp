#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityPit_MakeBridge(GridEntity_Pit* pit, GridEntity* parent) {
		pit->MakeBridge(parent);
	}

	__declspec(dllexport) void L_GridEntityPit_PostInit(GridEntity_Pit* pit) {
		pit->PostInit();
	}

	__declspec(dllexport) void L_GridEntityPit_Render(GridEntity_Pit* pit, Vector offset) {
		pit->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityPit_Update(GridEntity_Pit* pit) {
		pit->Update();
	}

	__declspec(dllexport) void L_GridEntityPit_UpdateCollision(GridEntity_Pit* pit) {
		pit->UpdateCollision();
	}
}