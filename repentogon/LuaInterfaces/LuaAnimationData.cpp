#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) const char* L_AnimationData_GetName(AnimationData* data) {
		return data->_name.c_str();
	}

	__declspec(dllexport) bool L_AnimationData_IsEventTriggered(AnimationData* data, const char* name) {
		std::string nameStr = name;
		return data->IsEventTriggered(&nameStr);
	}
}