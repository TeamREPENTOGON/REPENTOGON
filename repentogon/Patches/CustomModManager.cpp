#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"

#include "CustomModManager.h"

static std::map<uint32_t, CurseSpriteData> curseSpriteMap;
static ModEntry* lastModEntry = nullptr;
static uint32_t currentFrameNum = 0;

const std::map<uint32_t, CurseSpriteData>& CustomModManager::GetCurseSpriteMap() {
	return curseSpriteMap;
}

HOOK_METHOD(ModEntry, constructor, () -> void) {
	super();

	CustomModManager::GetInstance().AddEx(this);
}

ModEntry* capturedModEntry = nullptr;

void __stdcall CaptureModEntry(ModEntry* mod) {
	capturedModEntry = mod;
}

void ASMPatchCaptureModEntryForCurse() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseCaptureModEntry);

	printf("[REPENTOGON] Patching ModManager::UpdateCurses for capturing modEntry at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\xff\x36")
		.AddInternalCall(CaptureModEntry)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

static void LoadCustomContentAnm2(ModEntry* mod, std::string filename, ANM2* out) {
	if (mod == nullptr || !mod->_loaded) return;

	std::string path;
	mod->GetContentPath(&path, &filename);

	if (g_ContentManager.MountedFileExists(path)) {
		out->Load(path, true);
	}
}

HOOK_METHOD(RoomConfig, LoadCurses, (char* xmlPath, bool isMod) -> void) {
	if (isMod && capturedModEntry) {
		if (ModEntryEx* ex = CustomModManager::GetInstance().GetEx(capturedModEntry)) {
			LoadCustomContentAnm2(capturedModEntry, "gfx/mapitemicons.anm2", &ex->_customMapItemIcons);
			LoadCustomContentAnm2(capturedModEntry, "gfx/minimap_icons.anm2", &ex->_customMinimapIcons);
			LoadCustomContentAnm2(capturedModEntry, "gfx/stageicons.anm2", &ex->_customStageIcons);
		}
	}
	
	super(xmlPath, isMod);
}

void __stdcall RegisterCurseSprite(uint32_t curseId, bool isMod) {
	if (!isMod || !capturedModEntry) return;  // No mod captured

	if (capturedModEntry != lastModEntry) {
		currentFrameNum = 0;
		lastModEntry = capturedModEntry;
	}

	ModEntryEx* ex = CustomModManager::GetInstance().GetEx(capturedModEntry);
	if (ex && ex->_customMapItemIcons._loaded) {
		curseSpriteMap[curseId] = { ex, &ex->_customMapItemIcons, currentFrameNum };
		currentFrameNum++;
	}

}

void ASMPatchRegisterCurseSprite() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	const uint32_t curseIdOffset = *(uint32_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseGuessIdOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseRegisterSprite);

	printf("[REPENTOGON] Patching RoomConfig::LoadCurses for registering custom sprite at %p\n", addr);
	
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, 0xC) //isMod
		.Push(ASMPatch::Registers::EBP, curseIdOffset)
		.AddInternalCall(RegisterCurseSprite)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0xA))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0xA);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForCustomModManager() {
	ASMPatchCaptureModEntryForCurse();
	ASMPatchRegisterCurseSprite();
}