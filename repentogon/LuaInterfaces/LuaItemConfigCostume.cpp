#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) const char* L_ItemConfigCostume_GetAnm2Path(ItemConfig_Costume* costume) {
		return costume->anm2Path.c_str();
	}

	__declspec(dllexport) void L_ItemConfigCostume_SetAnm2Path(ItemConfig_Costume* costume, const char* path) {
		costume->anm2Path = path;
	}
}
