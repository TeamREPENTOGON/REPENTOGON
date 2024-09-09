#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMEntityNPC.h"


thread_local FireProjectilesStorage projectilesStorage;

/*
* It's Over.
* Hush's AI freaks out since Repentance due to an internal restructure of the code.
* The values Hush uses to track HP percentage internally was reduced by 100, but HP checks weren't.
* This makes Hush enter "panic" state at 50% HP and not 0.5%. Oops!
*/
float hushPanicLevel = 0.005f;

static bool IsHushPanicStateFixEnabled() {
	return repentogonOptions.hushPanicStateFix && g_Game->GetDailyChallenge()._id == 0;
}
void __stdcall SetHushPanicLevel() {
	// This goes out to the masochists that want to deliberately play bugged Hush (hereby dubbed VINH MODE)
	hushPanicLevel = IsHushPanicStateFixEnabled() ? 0.005f : 0.5f;
}

void PerformHushPanicPatch(void* addr) {

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

bool __stdcall IsRoomSlow() {
	Room* room = g_Game->_room;
	return IsHushPanicStateFixEnabled() && (room->_slowdownDuration > 0 || room->GetBrokenWatchState() == 1);
}

const float hushLaserAdjust = 0.513f * 0.75; // a base value i got empirically a while back + a slight extra bit of wiggle room
void PatchHushLaserSpeed() {
	const char* signature = "e8????????f30f1185????????f30f1005????????f30f1185????????8b95";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Hush laser attack speed bug at %p\n", addr);

	const float* floatPtr = &hushLaserAdjust;
	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ECX); // preserve player
	patch.AddInternalCall(IsRoomSlow);
	patch.Pop(ASMPatch::Registers::ECX); // restore player
	patch.AddBytes("\xF3\x0F\x10\x81\xAC\x14").AddZeroes(2); // movss xmm0,dword ptr ds:[ecx+14AC]
	patch.AddBytes("\x84\xC0"); // test al, al
	patch.AddBytes("\x74\x08"); // je, eip+0x8
	patch.AddBytes("\xF3\x0F\x59\x05").AddBytes(ByteBuffer().AddAny((char*)&floatPtr, 4)); // mulss xmm0, dword ptr [0xXXXXXXXX]
	patch.AddRelativeJump((char*)addr + 0x5); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchHushBug() {
	SigScan scanner1("f30f1005????????0f2f85????????0f86????????33c9");
	SigScan scanner2("f30f1005????????0f2f85????????0f86????????837f??00");
	scanner1.Scan();
	scanner2.Scan();
	void* addrs[2] = { scanner1.GetAddress(), scanner2.GetAddress() };
	printf("[REPENTOGON] Patching the Hush panic state bug at %p, %p\n", addrs[0], addrs[1]);
	PerformHushPanicPatch(addrs[0]);
	PerformHushPanicPatch(addrs[1]);
	PatchHushLaserSpeed();
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

void __stdcall ProcessApplyFrozenEnemyDeathEffects(Entity_NPC* npc) {
	printf("[REPENTOGON] Entity: %p type %d\n", npc, npc->_type);

	g_Game->_scoreSheet.AddKilledEnemy(npc);
	for (int i = 0; i < g_Game->GetNumPlayers(); i++) {
		g_Game->GetPlayer(i)->TriggerEnemyDeath(npc);
	}
}

void ASMPatchApplyFrozenEnemyDeathEffects() {
	SigScan scanner("8d85????????508d8f????????e8????????8d85????????508d8f????????e8????????50");
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Frozen Enemy Death Effects at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI)
		.AddInternalCall(ProcessApplyFrozenEnemyDeathEffects)
		.RestoreRegisters(reg)
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}