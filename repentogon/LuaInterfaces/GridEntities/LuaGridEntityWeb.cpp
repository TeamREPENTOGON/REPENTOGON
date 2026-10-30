#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityWeb_Update(GridEntity_Web* web) {
		web->Update();
	}
}