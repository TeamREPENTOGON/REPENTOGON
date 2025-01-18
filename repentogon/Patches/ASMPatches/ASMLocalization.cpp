#include "IsaacRepentance.h"
#include "HookSystem.h"

#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

bool __stdcall TryToRedirectToLocalizedResources(std::string& resFileString, std::string& targetFile, ModEntry** modEntry, RedirectedPath* redirectPath) {
	auto* manager = g_Manager->GetModManager();
	auto* langCode = g_Manager->GetLanguage();

	if (targetFile[0] == '\0') {
		return false;
	}

	auto buildAndCheckPath = [&](const std::string& postfix, bool useLangCode) -> bool {
		std::string potentialPath = resFileString + postfix;

		if (useLangCode && g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
			potentialPath = potentialPath + "." + langCode;
		}

		potentialPath = potentialPath + "/" + targetFile;

		if (g_FileManager_kinda->GetExpandedPath(potentialPath.c_str()) != NULL) {
			resFileString = potentialPath;
			redirectPath->_modEntry = *modEntry;
			redirectPath->_filePath = resFileString;
			std::cout << "[REPENTOGON] Patched " << resFileString << std::endl;
			return true;
		}
		return false;
	};

	// Check for "resources-repentogon.langCode"
	if (buildAndCheckPath("-repentogon", true)) return true;

	// Check for "resources-repentogon" (without langCode)
	if (buildAndCheckPath("-repentogon", false)) return true;

	// Check for "resources-dlc3.langCode"
	if (buildAndCheckPath("-dlc3", true)) return true;

	// Check for "resources.langCode" (original behavior)
	if (g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
		std::string originalPath = resFileString + "." + langCode + "/" + targetFile;
		if (g_FileManager_kinda->GetExpandedPath(originalPath.c_str()) != NULL) {
			resFileString = originalPath;
			redirectPath->_modEntry = *modEntry;
			redirectPath->_filePath = resFileString;
			std::cout << "[REPENTOGON] Patched " << resFileString << std::endl;
			return true;
		}
	}

	return false;
}

void ASMPatchRedirectToLocalizedResources() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan signature("6a0668????????8d4d??c645??01");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching ModManager::TryRedirectPath for resources folder redirect at %p\n", addr);

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

bool __stdcall TryToRedirectToLocalizedContent(std::string& resFileString, std::string& targetFile) {
	auto copyOfFileString = resFileString;
	auto* manager = g_Manager->GetModManager();
	auto* langCode = g_Manager->GetLanguage();

	if (g_Manager->_stringTable.language == 0 || targetFile[0] == '\0') {
		return false;
	}

	//std::cout << targetFile << std::endl;
	copyOfFileString = resFileString + "." + langCode + "/" + targetFile;

	//printf("[RGON] kinda string filename: %s %s \n", copyOfFileString.c_str(), g_FileManager_kinda->GetExpandedPath(copyOfFileString.c_str()));

	if (g_FileManager_kinda->GetExpandedPath(copyOfFileString.c_str()) != NULL) {

		resFileString = copyOfFileString;

		std::cout << "[REPENTOGON] Patched path: " << resFileString << std::endl;

		return true;
	}


	return false;
}

//need to push to esp+4 carefully somehow, maybe will tweak later. For now using method hooking to redirect path.
void ASMPatchRedirectToLocalizedContents() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan signature("6a0668????????8d4d??c745??00000000");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching ModEntry::GetContentPath for content folder redirect at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI) // Target file
		.Push(ASMPatch::Registers::EAX) //Mod content folder
		.AddInternalCall(TryToRedirectToLocalizedContent)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddBytes("\x74\x08")
		.CopyRegister(ASMPatch::Registers::EDI, ASMPatch::Registers::EAX)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x16D) // jump for true
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x7))  // Restore the commands we overwrote
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD(ModEntry, GetContentPath, (std::string& resFileString, std::string& targetFile) -> void) {
	super(resFileString, targetFile);
	auto* manager = g_Manager->GetModManager();
	auto* langCode = g_Manager->GetLanguage();

	if (targetFile[0] == '\0') {
		return;
	}

	auto buildAndCheckPath = [&](const std::string& postfix, bool useLangCode) -> bool {
		auto copyOfContentDirectory = _contentDirectory.substr(0, _contentDirectory.length() - 1);
		std::string potentialPath = copyOfContentDirectory + postfix;

		if (useLangCode && g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
			potentialPath = potentialPath + "." + langCode;
		}

		potentialPath = potentialPath + "/" + targetFile;

		if (g_FileManager_kinda->GetExpandedPath(potentialPath.c_str()) != NULL) {
			resFileString = potentialPath;
			std::cout << "[REPENTOGON] Patched path: " << resFileString << std::endl;
			return true;
		}
		return false;
	};

	// Check for "content-repentogon.langCode"
	if (buildAndCheckPath("-repentogon", true)) return;

	// Check for "content-repentogon" (without langCode)
	if (buildAndCheckPath("-repentogon", false)) return;

	// Check for "content-dlc3.langCode"
	if (buildAndCheckPath("-dlc3", true)) return;

	// Check for "content.langCode" (original behavior)
	if (g_Manager->_stringTable.language != 0 && langCode && langCode[0] != '\0') {
		auto copyOfContentDirectory = _contentDirectory.substr(0, _contentDirectory.length() - 1);
		std::string originalPath = copyOfContentDirectory + "." + langCode + "/" + targetFile;
		if (g_FileManager_kinda->GetExpandedPath(originalPath.c_str()) != NULL) {
			resFileString = originalPath;
			std::cout << "[REPENTOGON] Patched path: " << resFileString << std::endl;
			return;
		}
	}
}

void ASMPatchRedirectToLocalizationFolders() {
	ASMPatchRedirectToLocalizedResources();
	//ASMPatchRedirectToLocalizedContents();
}