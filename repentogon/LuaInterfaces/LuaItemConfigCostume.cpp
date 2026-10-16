#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) void L_ItemConfigCostume_SetAnm2Path(ItemConfig_Costume* costume, const char* path) {
		costume->anm2Path = path;
	}
}
