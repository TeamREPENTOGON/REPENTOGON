#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"

#include "CustomModManager.h"

struct CurseSpriteData {
	ModEntryEx* modEx;
	ANM2* customANM2;
	uint32_t frameNum;
};

static std::map<uint32_t, CurseSpriteData> curseSpriteMap;
static ModEntry* lastModEntry = nullptr;
static uint32_t currentFrameNum = 0;

HOOK_METHOD(ModEntry, constructor, () -> void) {
	super();

	CustomModManager::GetInstance().AddEx(this);
}

void __stdcall LoadCustomMinimapANM2(ModEntry* mod) {
    if (mod == nullptr || !mod->_loaded) return;

    std::string filename = "gfx/mapitemicons.anm2";
    std::string path;
    mod->GetContentPath(&path, &filename);

    const char* expanded = g_ContentManager.GetMountedFilePath(path.c_str());
    if (expanded != nullptr) {
        ModEntryEx* ex = CustomModManager::GetInstance().GetEx(mod);
        if (ex != nullptr) {
            ex->_customMinimapANM2.Load(path, true);
			printf("are we here? %s\n", ex->_customMinimapANM2._filename.c_str());
        }
    }
}

void ASMPatchLoadModEntryExAssets() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_LoadCustomSprites);

	printf("[REPENTOGON] Patching ModManager::LoadConfigs for loading custom sprites at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.AddBytes("\xff\x36") //push ptr esi
		.AddInternalCall(LoadCustomMinimapANM2)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x5))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
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

HOOK_METHOD(RoomConfig, LoadCurses, (char* xmlPath, bool isMod) -> void) {
	if (!isMod) super(xmlPath, isMod);

	LoadCustomMinimapANM2(capturedModEntry);

	super(xmlPath, isMod);

}

void __stdcall RegisterCurseSprite(uint32_t curseId) {
	if (!capturedModEntry) return;  // No mod captured

	const unsigned int adjustedCurseId = curseId - 1;

	if (capturedModEntry != lastModEntry) {
		currentFrameNum = 0;
		lastModEntry = capturedModEntry;
	}

	ModEntryEx* ex = CustomModManager::GetInstance().GetEx(capturedModEntry);
	if (ex && &ex->_customMinimapANM2) {
		curseSpriteMap[adjustedCurseId] = { ex, &ex->_customMinimapANM2, currentFrameNum };
		currentFrameNum++;
	}

}

void ASMPatchRegisterCurseSprite() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	const uint32_t curseIdOffset = *(uint32_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseGuessIdOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseRegisterSprite);
	const int jumpOffset = 0x6 + *(int8_t*)((char*)addr + 0x5);

	printf("[REPENTOGON] Patching RoomConfig::LoadCurses for registering custom sprite at %p\n", addr);
	
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, curseIdOffset)
		.AddInternalCall(RegisterCurseSprite)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x4))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForCustomModManager() {
	ASMPatchLoadModEntryExAssets();
	ASMPatchCaptureModEntryForCurse();
	ASMPatchRegisterCurseSprite();
}

//void AddModdedCurseIcons(uint32_t curseBitmask, uint32_t& iconCount, std::array<int, 8>& icons) {
//	if (curseSpriteMap.empty() || iconCount >= 7) return;
//
//	for (const auto& entry : curseSpriteMap) {
//		uint32_t adjustedCurseId = entry.first;
//		uint32_t originalCurseId = adjustedCurseId + 1;
//
//		if (adjustedCurseId >= 9 && (curseBitmask & (1U << (originalCurseId - 1)))) {
//			icons[iconCount] = -(adjustedCurseId + 1);
//			iconCount++;
//			if (iconCount >= 7) break;
//		}
//	}
//}

HOOK_METHOD(ModManager, LoadConfigs, () -> void) {
	super();

	if (capturedModEntry != nullptr) {
		printf("captured mode entry %s", capturedModEntry->GetName().c_str());
	}
	for (auto mod : _mods) {
		if (mod->_loaded) {
			printf("Loaded super mod: %s\n", mod->GetName().c_str());
		}
	}
	CurseSpriteData& curseSpriteEntry = curseSpriteMap[9];
	printf("something something %s\n", curseSpriteEntry.customANM2->_filename.c_str());
}