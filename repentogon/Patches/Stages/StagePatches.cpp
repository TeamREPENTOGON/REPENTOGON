#include "StagePatches.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../Stages/StageManager.h"

// TODO: Should probably refactor this to centralize the custom backdrop handling outside of XMLData
extern uint32_t hookedbackdroptype;

namespace StagePatches {

HOOK_METHOD(Backdrop, render_black_outside_rect, (Vector* param_1, Vector* param_2) -> void) {
	if (hookedbackdroptype != 0 && XMLStuff.BackdropData->GetAttributeById(hookedbackdroptype, "noblackbg") == "true") {
		return;
	}
	super(param_1, param_2);
}

// Use the actual stage ID of custom stages for the purposes of FXLayers
// Since custom stage IDs and the LevelStage-esque values used by vanilla will never overlap, this is fine.
void __stdcall EnableCustomStageFxLayers(FXLayers* fxlayers) {
	if (StageManager::GetCurrentOverride().HasOverride()) {
		fxlayers->_levelStage = StageManager::GetCurrentOverride().GetCustomStageID() + 4; // to counter dumb math later on in xml parsing
		fxlayers->_stageType = 0;
	}
	if (hookedbackdroptype != 0) {
		fxlayers->_backdropType = hookedbackdroptype;
	}
}
void ASMPatchFXLayersInit() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::FXLayers_Init_PreReadXml);

	printf("[REPENTOGON] Patching FXLayers::Init at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore overwritten bytes
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // FXLayers
		.AddInternalCall(EnableCustomStageFxLayers)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall CheckBackdropNoShading() {
	int backdropType = g_Game->_room->_backdrop.backdropId;
	if (g_Game->_room->_backdrop.configurations[backdropType].fullImage) {
		return true;
	}
	if (hookedbackdroptype != 0) {
		return XMLStuff.BackdropData->GetAttributeById(hookedbackdroptype, "noshading") == "true";
	}
	return false;
}
void ASMPatchLoadBackdropGraphicsNoShading() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::LoadBackdropGraphics_NoShading);

	printf("[REPENTOGON] Patching Room::LoadbackdropGraphics at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddInternalCall(CheckBackdropNoShading)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x8);
	sASMPatcher.PatchAt(addr, &patch);
}

void ApplyPatches() {
	ASMPatchFXLayersInit();
	ASMPatchLoadBackdropGraphicsNoShading();
}

}  // namespace StagePatches
