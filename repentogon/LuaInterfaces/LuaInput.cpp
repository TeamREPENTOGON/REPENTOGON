#include "IsaacRepentance.h"

extern float WINMouseWheelMove_Vert;
extern float WINMouseWheelMove_Hori;

MOD_EXPORT float L_Input_GetActionValue(int action, int controllerId) {
	return g_InputManagerBase.GetActionValue(action, controllerId, 0);
}

MOD_EXPORT float L_Input_GetButtonValue(int button, int controllerId) {
	return LuaEngine::GetButtonValue(button, controllerId);
}

MOD_EXPORT char* L_Input_GetDeviceNameByIdx(int controllerId) {
	return g_InputManagerBase.GetDeviceNameByIdx(controllerId);
}

MOD_EXPORT void L_Input_GetMousePosition(bool gameCoords, Vector* out) {
	g_LuaEngine->GetMousePosition(out, gameCoords);
}

MOD_EXPORT void L_Input_GetMouseWheel(Vector* out) {
	*out = Vector(WINMouseWheelMove_Hori, WINMouseWheelMove_Vert);
}

MOD_EXPORT bool L_Input_IsActionPressed(int action, int controllerId) {
	return g_InputManagerBase.IsActionPressed(action, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsActionTriggered(int action, int controllerId) {
	return g_InputManagerBase.IsActionTriggered(action, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsButtonPressed(int button, int controllerId) {
	return g_InputManagerBase.IsButtonPressed(button, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsButtonTriggered(int button, int controllerId) {
	return g_InputManagerBase.IsButtonTriggered(button, controllerId, 0);
}

MOD_EXPORT bool L_Input_IsMouseBtnPressed(uint32_t button) {
	return LuaEngine::IsMouseButtonPressed(button);
}