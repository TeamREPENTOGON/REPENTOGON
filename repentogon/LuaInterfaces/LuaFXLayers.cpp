#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_FXLayers_AddPoop(FXLayers* fxLayers, ColorMod* color) {
		fxLayers->AddPoopFx(color);
	}
}