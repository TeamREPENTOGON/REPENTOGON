#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

//to-do:
/*
* Add content support
* Add DLC3 support for resources/content folders
*/
bool __stdcall TryToRedirectToLocalizedResources(std::string& resFileString, std::string& targetFile, ModEntry** modEntry, RedirectedPath* redirectPath) {
	auto copyOfFileString = resFileString;
	auto* manager = g_Manager->GetModManager();
	auto* langCode = g_Manager->GetLanguage();

	if (g_Manager->_stringTable.language == 0 || targetFile[0] == '\0') {
		return false;
	}

	std::cout << targetFile << std::endl;
	copyOfFileString = resFileString + "." + langCode + "/" + targetFile;

	//printf("[RGON] kinda string filename: %s %s \n", copyOfFileString.c_str(), g_FileManager_kinda->GetExpandedPath(copyOfFileString.c_str()));

	if (g_FileManager_kinda->GetExpandedPath(copyOfFileString.c_str()) != NULL) {
		
		resFileString = copyOfFileString;

		redirectPath->_modEntry = *modEntry;
		redirectPath->_filePath = resFileString;

		std::cout << "[REPENTOGON] Patched " << resFileString << std::endl;

		return true;
	}


	return false;
}

void ASMPatchRedirectToLocalizedResources() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS, true);
	ASMPatch patch;

	SigScan signature("6a0668????????8d4d??c645??01");
	signature.Scan();

	void* addr = signature.GetAddress();
	printf("[REPENTOGON] Patching ModManager::TryRedirectPath for testing at %p\n", addr);

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

void ASMPatchRedirectToLocalizationFolders() {
	ASMPatchRedirectToLocalizedResources();
}