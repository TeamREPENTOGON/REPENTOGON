#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityTeleporter_Render(GridEntity_Teleporter* teleporter, Vector offset) {
		teleporter->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityTeleporter_Update(GridEntity_Teleporter* teleporter) {
		teleporter->Update();
	}
}