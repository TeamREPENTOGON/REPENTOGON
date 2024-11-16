#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "../ModReloading.h"

// Skip Render Names entirely 
void PatchModMenu_Font(const char* sig) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();
	ZHL::Log("[REPENTOGON] Patching Menu_Mods::Render() to skip font rendering at %p \n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\xc4\x2c"); // add esp, 0x2C
	sASMPatcher.FlatPatch(addr, &patch, true);
}

void ASMPatchModsMenu() {
	PatchModMenu_Font("e8????????393b");
	PatchModMenu_Font("e8????????8b95????????8bfb");
	PatchModMenu_Font("e8????????8d43??3907");
}
void ASMPatchMenuOptionsLanguageChange() {
	// before lua reset in MenuOptions::Update
	// this only happens when switching the language in the Options menu.
	SigScan scanner("743e8b35????????68????????6a00e8????????83c408");
	scanner.Scan();
	void* addr1 = scanner.GetAddress();
	ZHL::Log("[REPENTOGON] Patching options menu language change operate at %p.\n", addr1);
	ASMPatch patch;
	patch.AddInternalCall(GameRestart);
	sASMPatcher.PatchAt(addr1, &patch);
}