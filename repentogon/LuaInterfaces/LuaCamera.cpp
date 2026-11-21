#include "IsaacRepentance.h"
#include "../Patches/ASMPatches/ASMCamera.h"

extern "C" {
	__declspec(dllexport) bool L_Camera_IsClampEnabled(Camera* camera) {
		return roomCameraData.Clamped;
	}

	__declspec(dllexport) bool L_Camera_IsPosVisible(Camera* camera, Vector* pos) {
		return camera->IsPosVisible(pos);
	}

	__declspec(dllexport) void L_Camera_SetClampEnabled(Camera* camera, bool clamped) {
		roomCameraData.Clamped = clamped;
	}

	__declspec(dllexport) void L_Camera_SetFocusPosition(Camera* camera, Vector* pos) {
		camera->SetFocusPosition(pos);
	}
	
	__declspec(dllexport) void L_Camera_SnapToPosition(Camera* camera, Vector pos) {
		camera->SnapToPosition(pos);
		camera->_shouldOverride = true;
	}

	__declspec(dllexport) void L_Camera_Update(Camera* camera, bool flag) {
		camera->DoUpdate(flag);
	}
}