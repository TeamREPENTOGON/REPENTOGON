#pragma once

struct RoomCameraData {
	bool Clamped = true;
};

extern RoomCameraData roomCameraData;

void ASMPatchCameraBoundClampOverride();
void ASMPatchCameraBoundSlowStopOverride();