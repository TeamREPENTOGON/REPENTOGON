#include "IsaacRepentance.h"
#include "SigScan.h"
#include "LuaCore.h"

#include "../ASMPatches.h"
#include "ASMEntityNPC.h"
#include "ASMPatcher.hpp"

thread_local FireProjectilesStorage projectilesStorage;

/*
* It's Over.
* Hush's AI freaks out since Repentance due to an internal restructure of the code.
* The values Hush uses to track HP percentage internally was reduced by 100, but HP checks weren't.
* This makes Hush enter "panic" state at 50% HP and not 0.5%. Oops!
*/
float hushPanicLevel = 0.005f;
void __stdcall SetHushPanicLevel() {
	// This goes out to the masochists that want to deliberately play bugged Hush (hereby dubbed VINH MODE)
	hushPanicLevel = repentogonOptions.hushPanicStateFix ? 0.005f : 0.5f;
}

void PerformHushPatch(void* addr) {

	void* panicPtr = &hushPanicLevel;
	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddInternalCall(SetHushPanicLevel) // call SetHushPanicLevel()
		.RestoreRegisters(reg)
		.AddBytes("\xF3\x0F\x10\x05").AddBytes(ByteBuffer().AddAny((char*)&panicPtr, 4))  // movss xmm0, dword ptr ds:[0xXXXXXXXX]
		.AddRelativeJump((char*)addr + 0x8); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchHushBug() {
	SigScan scanner1("f30f1005????????0f2f85????????0f86????????33c9");
	SigScan scanner2("f30f1005????????0f2f85????????0f86????????837f??00");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };
	printf("[REPENTOGON] Patching the Hush panic state bug at %p, %p\n", addrs[0], addrs[1]);
	PerformHushPatch(addrs[0]);
	PerformHushPatch(addrs[1]);
}

void __stdcall FireProjectilesEx_Internal(std::vector<Entity_Projectile*> const& projectiles) {
	if (!projectilesStorage.inUse) {
		return;
	}

	for (Entity_Projectile* projectile : projectiles) {
		projectilesStorage.projectiles.push_back(projectile);
	}
}

void __stdcall FireBossProjectilesEx_Internal(Entity_Projectile* projectile) {
	if (!projectilesStorage.inUse) {
		return;
	}

	projectilesStorage.projectiles.push_back(projectile);
}

void ASMPatchFireProjectiles() {
	const char* signature = "33c92b55b4c1fa02894ddc8955e4";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	using Reg = ASMPatch::SavedRegisters::Registers;
	using GPReg = ASMPatch::Registers;

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(Reg::EAX | Reg::EBX | Reg::ECX | Reg::EDX | Reg::EDI | Reg::ESI |
		Reg::XMM0 | Reg::XMM1 | Reg::XMM2 | Reg::XMM3 | Reg::XMM4 | Reg::XMM5, true);
	patch.PreserveRegisters(registers);
	// patch.MoveFromMemory(GPReg::EBP, -0x4C, GPReg::ESI, true);
	patch.LoadEffectiveAddress(GPReg::EBP, -0x4C, GPReg::ESI);
	patch.Push(GPReg::ESI);
	patch.AddInternalCall(FireProjectilesEx_Internal);
	patch.RestoreRegisters(registers);
	patch.AddBytes("\x33\xc9\x2b\x55\xb4");
	patch.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchFireBossProjectiles() {
	const char* signature = "f30f104424388bf883c414";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	using Reg = ASMPatch::SavedRegisters::Registers;
	using GPReg = ASMPatch::Registers;

	ASMPatch patch;
	ASMPatch::SavedRegisters registers(Reg::GP_REGISTERS_STACKLESS | Reg::XMM0 | Reg::XMM1 | Reg::XMM2 | Reg::XMM3, true);
	patch.PreserveRegisters(registers);
	patch.Push(GPReg::EAX);
	patch.AddInternalCall(FireBossProjectilesEx_Internal);
	patch.RestoreRegisters(registers);
	patch.AddBytes("\xF3\x0f\x10\x44\x24\x38");
	patch.AddRelativeJump((char*)addr + 0x6);

	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchAddWeakness() {
	SigScan scanner("576afd"); // this is the first push of args for ComputeStausEffectDuration
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EDI)  // Push entity
		.Push(ASMPatch::Registers::EBP, 0xc) // Push duration
		.AddBytes("\x8b\xce") // MOV ECX, ESI
		// this fits exactly in the 5 bytes uses to push arguments
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}