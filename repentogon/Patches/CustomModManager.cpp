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
	if (isMod) {
		LoadCustomMinimapANM2(capturedModEntry);
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
	if (ex && &ex->_customMinimapANM2) {
		curseSpriteMap[curseId] = { ex, &ex->_customMinimapANM2, currentFrameNum };
		currentFrameNum++;
	}

}

void ASMPatchRegisterCurseSprite() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	const uint32_t curseIdOffset = *(uint32_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseGuessIdOffset) + 0x2);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseRegisterSprite);
	//const int jumpOffset = 0x6 + *(int8_t*)((char*)addr + 0x5);

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

int __stdcall TestArrayPrint(int* mapIcons, int mapIndex) {
		for (int i = 0; i < 7; i++) {
			mapIcons[i] = -1;
		}
		mapIndex = 7;
		//printf("map index: %d\n", mapIndex);
		return mapIndex;
	
}

int __stdcall AddModdedCurseIcons(uint32_t curseBitmask, int* mapIcons, int iconCount) {
	if (curseSpriteMap.empty() || iconCount >= 7) return iconCount;

	for (const auto& entry : curseSpriteMap) {
		int adjustedCurseId = (int)entry.first;

		if (adjustedCurseId >= 9 && (curseBitmask & (1U << (adjustedCurseId - 1)))) {
			mapIcons[iconCount] = -(adjustedCurseId);
			iconCount++;
			if (iconCount >= 7) break;
		}
	}

	return iconCount;
}

void ASMPatchAssignCustomFrame() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::Registers::EAX, true);
	ASMPatch patch;

	const int8_t mapIconsOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseMapIconsArrayOffset) + 0x3);
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseRegisterFrameOffset);
	const int jumpOffset = 0x7 + *(int8_t*)((char*)addr + 0x6);

	printf("[REPENTOGON] Patching Minimap::render_icons for accessing mapIcons array at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.LoadEffectiveAddress(ASMPatch::Registers::ESP, mapIconsOffset + 0x14, ASMPatch::Registers::EDI, std::nullopt, 4u )
		.Push(ASMPatch::Registers::EBX)
		.Push(ASMPatch::Registers::EDI)
		.Push(ASMPatch::Registers::EAX)
		.AddInternalCall(AddModdedCurseIcons)
		.RestoreRegisters(savedRegisters)
		.CopyRegister(ASMPatch::Registers::EBX, ASMPatch::Registers::EAX)
		.AddRelativeJump((char*)addr + jumpOffset);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall RenderCustomCurses(int curseId, Vector* position) {
	if (curseId >= 8) {

		std::string curseName = "curses";
		auto& curseEntry = curseSpriteMap[curseId + 1];
		ANM2* curseSprite = curseEntry.customANM2;
		if (curseSprite->_loaded) {
			curseSprite->Play("curses", false);
			curseSprite->SetFrame(&curseName, curseEntry.frameNum);
			//Vector pos { 100, 25 };
			Vector zero { 0,0 };

			/*
			if (!(&curseSprite->_color == color)) {
				curseSprite->_color = *color;
			}
			*/

			curseSprite->Render(position, &zero, &zero);
			return true;
		}
		
	}
	return false;
}

void ASMPatchRenderCustomCurses() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseMapIconsCustomRender);
	const int8_t positionOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseMapIconsPositionOffset) + 0x3);
	const int8_t colorModOffset = *(int8_t*)((char*)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::CustomModManager_CurseMapIconsColorModOffset) + 0x3);
	//const int jumpOffset = 0x5 + *(int8_t*)((char*)addr + 0x4);

	printf("[REPENTOGON] Patching Minimap::render_icons for rendering custom curses at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		/*.LoadEffectiveAddress(ASMPatch::Registers::ESP, mapIconsOffset + 0x14, ASMPatch::Registers::EDI, std::nullopt, 4u)
		.Push(ASMPatch::Registers::EBX)

		.Push(ASMPatch::Registers::EAX)*/
		//.LoadEffectiveAddress(ASMPatch::Registers::EBP)
		//.LoadEffectiveAddress(ASMPatch::Registers::ESP, positionOffset + 0x10 + 0x1A8, ASMPatch::Registers::EBX, std::nullopt, 4u) // load colorMod ptr to EAX (idiotic magic number offset)
		//.Push(ASMPatch::Registers::EBX) //push ColorMod
		.LoadEffectiveAddress(ASMPatch::Registers::ESP, positionOffset + 0x10, ASMPatch::Registers::EAX, std::nullopt, 4u) // load position ptr to EAX
		.Push(ASMPatch::Registers::EAX) //push position
		.Push(ASMPatch::Registers::EDI) // push curseId
		.AddInternalCall(RenderCustomCurses)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x80) //skipping vanilla curse icons rendering
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x7); 
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchesForCustomModManager() {
	ASMPatchLoadModEntryExAssets();
	ASMPatchCaptureModEntryForCurse();
	ASMPatchRegisterCurseSprite();
	ASMPatchAssignCustomFrame();
	ASMPatchRenderCustomCurses();
}

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
}