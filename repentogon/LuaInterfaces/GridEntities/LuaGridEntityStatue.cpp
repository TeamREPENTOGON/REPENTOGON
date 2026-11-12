#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityStatue_Update(GridEntity_Statue* statue) {
		statue->Update();
	}
}