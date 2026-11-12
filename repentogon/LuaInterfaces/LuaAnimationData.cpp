#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) bool L_AnimationData_IsEventTriggered(AnimationData* data, const char* name) {
		std::string nameStr = name;
		return data->IsEventTriggered(&nameStr);
	}
}