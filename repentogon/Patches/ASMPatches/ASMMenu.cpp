#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "../ModReloading.h"

// Skip Render Names entirely 
void PatchModMenu_Font(const char* sig) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Menu_Mods::Render() to skip font rendering at %p \n", addr);

	ASMPatch patch;
	patch.AddBytes("\x83\xc4\x24"); // add esp, 0x24
	sASMPatcher.FlatPatch(addr, &patch, true);
}

void ASMPatchModsMenu() {
	PatchModMenu_Font("e8????????8d8d????????e8????????8d8d????????e8????????8b8d");
	PatchModMenu_Font("e8????????8d8d????????e8????????8d8d????????e8????????8bbd");
	PatchModMenu_Font("e8????????8d8d????????e8????????8d8d????????e8????????8b45??83c02d");  // Actually Menu_CustomChallenge::Render()
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

// We shouldn't allow REPENTOGON online, even unmodded, as our patches can cause desyncs.
// However, we still want to allow singleplayer dailies.
// Forbid selecting any options except for "Daily Run". On the daily menu, forbid "Multiplayer".

// The easiest way to patch this is to force the cursor on state 3 at all times, which is "Daily Run".
// Then, jmp over the section that handles accepting up/down inputs.
void ASMPatchOnlineSelection() {
	SigScan scanner("6a006aff6a16b9????????e8????????84c074??8b87");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	SigScan jumpSig("8b87????????85c075??5068????????eb");
	jumpSig.Scan();
	void* jumpAddr = jumpSig.GetAddress();

	ASMPatch patch;
	patch.AddBytes("\xC7\x87\x2C\x02").AddZeroes(2).AddBytes("\x03").AddZeroes(3) // mov dword ptr ds:[edi+22C], 3
		.AddBytes("\xC7\x87\x30\x02").AddZeroes(2).AddBytes("\x03").AddZeroes(3)  // mov dword ptr ds:[edi+22C], 3
		.AddRelativeJump(jumpAddr);

	sASMPatcher.PatchAt(addr, &patch);
}

// Daily menu has two different sections for whether the menu is incrementing or decrementing.
// Handle both cases. We need to skip over state 1 which is the multiplayer option.
void ASMPatchDailySelection() {
	SigScan incScanner("83f8047c??c747??00000000");
	incScanner.Scan();
	void* incAddr = incScanner.GetAddress();

	SigScan incJmp("8b0f85c074??83f801");
	incJmp.Scan();
	void* incJmpAddr = incJmp.GetAddress();


	ASMPatch incPatch;
	incPatch.AddBytes("\x83\xF8\x01") // cmp dword ptr ds:[edi+0x8], 1
		.AddBytes("\x75\x04") // jnz 0x4
		.AddBytes("\x40") // inc eax
		.AddBytes("\x89\x47\x08") // mov dword ptr ds:[edi+0x8], eax
		.AddBytes("\x83\xF8\x04") // cmp eax, 4
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JL, incJmpAddr)
		.AddRelativeJump((char*)incAddr + 0x5);

	sASMPatcher.PatchAt(incAddr, &incPatch);

	SigScan decScanner("85c074??83f80175??833f02");
	decScanner.Scan();
	void* decAddr = decScanner.GetAddress();

	SigScan decJmp1("833f0274");
	decJmp1.Scan();
	void* decJmpAddr1 = decJmp1.GetAddress();

	SigScan decJmp2("85c079??c747??03000000");
	decJmp2.Scan();
	void* decJmpAddr2 = decJmp2.GetAddress();


	ASMPatch decPatch;
	decPatch.AddBytes("\x83\xF8\x01") // cmp eax, 1
		.AddBytes("\x75\x04") // jnz 0x4
		.AddBytes("\x48") // dec eax
		.AddBytes("\x89\x47\x08") // mov dword ptr ds:[edi+0x8], eax
		.AddBytes("\x85\xC0") // test eax,eax
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JZ, decJmpAddr1)
		.AddBytes("\x83\xF8\x01") // cmp eax, 1
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, decJmpAddr2)
		.AddRelativeJump(decJmpAddr1);

	sASMPatcher.PatchAt(decAddr, &decPatch);
}

void ASMPatchOnlineMenu() {
	ASMPatchOnlineSelection();
	ASMPatchDailySelection();
}