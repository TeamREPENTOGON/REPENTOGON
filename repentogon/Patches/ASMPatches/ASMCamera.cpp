#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMCamera.h"
#include "../XMLData.h"

#include "ASMDefinition.h"
#include "HookSystem.h"

RoomCameraData roomCameraData;

bool __stdcall ShouldClampCamera() {
	return roomCameraData.Clamped;
}

void ASMPatchCameraBoundOverride() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Camera_ClampOverride);

	ASMPatch patch;

	patch.AddInternalCall(ShouldClampCamera)
		.AddBytes("\x85\xC0") // test eax, eax
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x2 + 0x12)
		.AddBytes("\x8B\x45\x08") // mov eax, dword ptr [ebp + 0x8]
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

// Rooms only have one camera instance, so we don't really need to stress about having
// a dedicated wrapper class like EntityPlus.

HOOK_METHOD(Camera, destructor, () -> void) {
	roomCameraData.Clamped = true;
}