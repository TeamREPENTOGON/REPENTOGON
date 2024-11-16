#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

// Planetarium chance was implemented, but disabled! Oops!
bool renderStatHUDPlanetarium = false;
void __stdcall SetStatHUDPlanetarium() {
	renderStatHUDPlanetarium = g_Manager->GetPersistentGameData()->Unlocked(406) && repentogonOptions.statHUDPlanetarium; //TODO enum
}

void PatchStatHudPlanetariumChanceIcon() {
	SigScan signature("83fe080f84????????83fe090f84????????83fe06");
	signature.Scan();

	SigScan jmpSig("4683fe0a0f82????????33c9");
	jmpSig.Scan(); 

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSig.GetAddress();
	void* statHUDPlanetariumAddr = &renderStatHUDPlanetarium;
	ZHL::Log("[REPENTOGON] Patching planetarium chance icon visibility at %p\n", addr);
	ASMPatch patch;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddBytes("\x75\x1E") // jne (current addr + 0x1E)
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(SetStatHUDPlanetarium)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&statHUDPlanetariumAddr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchStatHudPlanetariumChanceText() {
	SigScan signature("83fe080f84????????83fe090f84????????83fe07");
	signature.Scan();

	SigScan jmpSig("4683fe0a0f82????????8b9d");
	jmpSig.Scan();

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSig.GetAddress();
	void* statHUDPlanetariumAddr = &renderStatHUDPlanetarium;

	ZHL::Log("[REPENTOGON] Patching planetarium chance text visibility at %p\n", addr);
	ASMPatch patch;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddBytes("\x75\x19") // jne (current addr + 0x19)
		.PreserveRegisters(savedRegisters)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&statHUDPlanetariumAddr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchStatHudPlanetariumChanceIf(const char* sig, const char* jmpSig) {
	SigScan signature(sig);
	signature.Scan();
	
	SigScan jmpSignature(jmpSig);
	jmpSignature.Scan();

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSignature.GetAddress();

	ZHL::Log("[REPENTOGON] Patching if statement for planetarium chance visibility at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\x83\xFE\x07") // cmp index, 0x7
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

float statHudPlanetariumChance = 0.0f;
void __stdcall SetStatHUDPlanetariumChance() {
	statHudPlanetariumChance = g_Game->GetPlanetariumChance() * 100;
}

void PatchStatHudPlanetariumChanceCalc() {
	SigScan signature("8d8e????????0f57c903cf");
	signature.Scan();

	void* addr = signature.GetAddress();
	void* planetariumStat = &statHudPlanetariumChance;

	ZHL::Log("[REPENTOGON] Patching planetarium chance calculation at %p\n", addr);
	ASMPatch patch;
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x8D\x8E\xB8\x01").AddZeroes(2) // lea this, [esi + 0x1b8]
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(SetStatHUDPlanetariumChance)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\xF3\x0F\x10\x0D").AddBytes(ByteBuffer().AddAny((char*)&planetariumStat, 4)) // movss xmm1, dword ptr ds:[0xXXXXXXXX])
		.AddRelativeJump((char*)addr + 0x9);

	sASMPatcher.PatchAt(addr, &patch);
}

void PatchStatHudPlanetariumChance() {
	PatchStatHudPlanetariumChanceIcon();
	PatchStatHudPlanetariumChanceText();
	PatchStatHudPlanetariumChanceIf("83fe0774??f30f1044", "f30f1044????8d45??0f5ac083ec08f20f11042468????????6a0c50e8????????b8????????b9????????f30f1044");
	PatchStatHudPlanetariumChanceIf("83fe0774??85f675", "85c90f85????????a1????????8d3c");
	PatchStatHudPlanetariumChanceCalc();
}