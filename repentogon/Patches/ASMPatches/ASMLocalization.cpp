#include "ASMLocalization.h"

#include <string>

#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "ASMDefinition.h"

bool __stdcall TryToRedirectToLocalizedResources(std::string& resFileString, std::string& targetFile, ModEntry** modEntry, RedirectedPath* redirectPath) {
	ModManager* manager = g_Manager->GetModManager();
	char* langCode = g_Manager->GetLanguage();

	if (targetFile[0] == '\0') {
		return false;
	}

	auto buildAndCheckPath = [&](const std::string& postfix, bool useLangCode) -> bool {
		std::string potentialPath = resFileString + postfix;

		if (useLangCode && g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
			potentialPath = potentialPath + "." + langCode;
		}

		potentialPath = potentialPath + "/" + targetFile;

		if (g_ContentManager.MountedFileExists(potentialPath)) {
			resFileString = potentialPath;
			redirectPath->_modEntry = *modEntry;
			redirectPath->_filePath = resFileString;
			return true;
		}
		return false;
	};

	if (buildAndCheckPath("-repentogon", false)) return true;
	
	return false;
}

void ASMPatchRedirectToLocalizedResources() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::RedirectToLocalizedResources);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EBP, -0xa0) // RedirectedPath*
		.Push(ASMPatch::Registers::ESI) // ModEntry
		.Push(ASMPatch::Registers::EBP, -0xa4) // Target file
		.Push(ASMPatch::Registers::EAX) //Mod resources folder
		.AddInternalCall(TryToRedirectToLocalizedResources)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + 0x2D4) // jump for true
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD(ModEntry, GetContentPath, (std::string* resFileString, const std::string* targetFile) -> void) {
	super(resFileString, targetFile);
	ModManager* manager = g_Manager->GetModManager();
	char* langCode = g_Manager->GetLanguage();

	if (targetFile->empty()) {
		return;
	}

	auto buildAndCheckPath = [&](const std::string& postfix, bool useLangCode) -> bool {
		std::string copyOfContentDirectory = _contentDirectory.substr(0, _contentDirectory.length() - 1);
		std::string potentialPath = copyOfContentDirectory + postfix;

		if (useLangCode && g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
			potentialPath = potentialPath + "." + langCode;
		}

		potentialPath = potentialPath + "/" + *targetFile;

		if (g_ContentManager.MountedFileExists(potentialPath.c_str())) {
			*resFileString = potentialPath;
			return true;
		}
		return false;
	};

	if (buildAndCheckPath("-repentogon", false)) return;
	
}

void ASMPatchRedirectToLocalizationFolders() {
	ASMPatchRedirectToLocalizedResources();
}

std::string GetLocalizedPlayerAnimationForAnm2(EntityConfig_Player* player, ANM2* sprite) {
	std::string displayName = player->GetDisplayName(nullptr);
	if (!sprite->GetAnimationData(displayName) && !player->_name.empty() && player->_name.front() == '#') {
		// No animation available for the translated name. Look for a fallback.
		if (sprite->GetAnimationData(player->_name)) {
			// Fall back to raw string.
			return player->_name;
		} else {
			std::string stringkey = player->_name;
			stringkey.erase(0, 1);  // Remove the '#'
			bool failed = false;
			if (const char* english = g_Manager->GetStringTable()->GetString("Players", 0, stringkey.c_str(), &failed); !failed && english && sprite->GetAnimationData(english)) {
				// Fall back to English.
				return english;
			}
		}
	}
	return displayName;
}

// Patch overtop calls to GetDisplayName that will be used to look for an animation in a particular Player-related menu ANM2.
// We'll provide the untranslated or english names as fallbacks if needed to find an available animation.
void __stdcall LocalizedPlayerAnimationNameTrampoline(EntityConfig_Player* player, ANM2* sprite, std::string* buffer) {
	new (buffer) std::string(GetLocalizedPlayerAnimationForAnm2(player, sprite));
}
void ASMPatchLocalizedPlayerAnimationName(char* sig, std::optional<size_t> offset = std::nullopt) {
	void* addr = sASMDefinitionHolder->GetDefinition(sig);

	printf("[REPENTOGON] Patching EntityConfig_Player::GetDisplayName call for localized ANM2 animations @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.AddBytes("\x83\xC4\x04")  // add esp,0x4
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX);
	if (offset) {
		patch.Push(ASMPatch::Registers::ECX, *offset);
	} else {
		patch.Push(ASMPatch::Registers::ESI);
	}
	patch.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(LocalizedPlayerAnimationNameTrampoline)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// For some reason the game uses the raw name string to match the tainted with their regular counterpart,
// but then goes through GetDisplayName to match the regular character with their tainted.
// This can prevent the tainted from appearing on the menu.
// Here we make it use the raw name too.
void __stdcall GetNameToMatchBSkinParent(EntityConfig_Player* player, std::string* buffer) {
	new (buffer) std::string(player->_name);
}
void ASMPatchGetBSkinParentName() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityConfig_PostLoadMods_BSkinParentGetName);

	printf("[REPENTOGON] Patching EntityConfig::PostLoadMods for BSkinParent check @ %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.Pop(ASMPatch::Registers::EAX)
		.AddBytes("\x83\xC4\x04")  // add esp,0x4
		.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)
		.Push(ASMPatch::Registers::ECX)
		.AddInternalCall(GetNameToMatchBSkinParent)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchLocalizedPlayerAnimations() {
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CharacterMenuAlt);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CharacterPortraitsAlt);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_DeathScreenAlt);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CoopMenuAlt);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_ControlsAlt);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CharacterMenu);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CharacterPortraits);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_DeathScreen);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CoopMenu);
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_Controls);

	ASMPatchGetBSkinParentName();
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::EntityConfig_LoadPlayers_CustomBackgroundCheck, offsetof(EntityConfig_Player, _moddedMenuBackgroundANM2));

	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::RenderCustomCharacters_GetAnimName, offsetof(EntityConfig_Player, _moddedMenuPortraitANM2));
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::RenderCustomCharacterMenu_GetAnimName, offsetof(EntityConfig_Player, _moddedMenuBackgroundANM2));
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::RenderCustomCharacterCoopMenu_GetAnimName, offsetof(EntityConfig_Player, _moddedCoopMenuANM2));
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::Backdrop_PreRenderControls_GetCharacterAnimName, offsetof(EntityConfig_Player, _moddedControlsANM2));
	ASMPatchLocalizedPlayerAnimationName(&AsmDefinitions::GameOver_Render_GetCharacterAnimName, offsetof(EntityConfig_Player, _moddedGameOverANM2));
}
