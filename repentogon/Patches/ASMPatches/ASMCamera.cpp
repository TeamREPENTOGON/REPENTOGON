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
	void* clampToRoomAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Camera_ClampOverride);

	ASMPatch clampToRoomPatch;

	clampToRoomPatch.AddInternalCall(ShouldClampCamera)
		.AddBytes("\x84\xC0") // test al, al
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)clampToRoomAddr + 0x14)
		.AddBytes("\x8B\x45\x08") // mov eax, dword ptr [ebp + 0x8]
		.AddRelativeJump((char*)clampToRoomAddr + 0x5);

	sASMPatcher.PatchAt(clampToRoomAddr, &clampToRoomPatch);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::ALL & ~ASMPatch::SavedRegisters::Registers::EDX, false);
	void* ultraSmoothAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Camera_SlowStopClampOverride);

	ASMPatch smoothUpdatePatch;
	smoothUpdatePatch.Push(ASMPatch::Registers::EDX)
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(ShouldClampCamera)       
		.AddBytes("\x0F\xB6\xD0") // movzx edx, al
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x85\xD2") // test edx, edx
		.Pop(ASMPatch::Registers::EDX)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, (char*)ultraSmoothAddr + 0x9)
		.AddBytes(ByteBuffer().AddAny((char*)ultraSmoothAddr, 0x9)) // Restore original instructions
		.AddRelativeJump((char*)ultraSmoothAddr + 0x9);

	sASMPatcher.PatchAt(ultraSmoothAddr, &smoothUpdatePatch);

}

// Rooms only have one camera instance, so we don't really need to stress about having
// a dedicated wrapper class like EntityPlus.

HOOK_METHOD(Camera, destructor, () -> void) {
	roomCameraData.Clamped = true;
	super();
}