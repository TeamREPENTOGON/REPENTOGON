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

		if (g_ContentManager.GetMountedFilePath(potentialPath.c_str()) != NULL) {
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

		if (g_ContentManager.GetMountedFilePath(potentialPath.c_str()) != NULL) {
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