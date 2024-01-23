#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "../ModReloading.h"

// Render mod names with a smaller font
void PatchModMenu_Font(const char* sig) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Menu_Mods::Render() for a smaller font at %p \n", addr);

	ASMPatch patch;
	patch.AddBytes("\x8D\x89\x14\xA5\x10").AddZeroes(1);
	sASMPatcher.FlatPatch(addr, &patch);
}

void ASMPatchModsMenu() {
	PatchModMenu_Font("8d89????????f30f1140");
	PatchModMenu_Font("8d89????????f30f1100");
}
void ASMPatchMenuOptionsLanguageChange() {
	// before lua reset in MenuOptions::Update
	// this only happens when switching the language in the Options menu.
	SigScan scanner("743e8b35????????68????????6a00e8????????83c408");
	scanner.Scan();
	void* addr1 = scanner.GetAddress();
	printf("[REPENTOGON] Patching options menu language change operate at %p.\n", addr1);
	ASMPatch patch;
	patch.AddInternalCall(GameRestart);
	sASMPatcher.PatchAt(addr1, &patch);
}