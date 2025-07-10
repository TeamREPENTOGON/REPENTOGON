#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

#include "ASMEntityNPC.h"


thread_local FireProjectilesStorage projectilesStorage;

static bool IsHushLaserSpeedFixEnabled() {
	return repentogonOptions.hushLaserSpeedFix && g_Game->GetDailyChallenge()._id == 0;
}

bool __stdcall IsRoomSlow() {
	Room* room = g_Game->_room;
	return IsHushLaserSpeedFixEnabled() && (room->_slowdownDuration > 0 || room->GetBrokenWatchState() == 1);
}

const float hushLaserAdjust = 0.513f * 0.75; // a base value i got empirically a while back + a slight extra bit of wiggle room
void PatchHushLaserSpeed() {
	const char* signature = "e8????????f30f1185????????f30f1005????????f30f1185????????8d85";
	SigScan scanner(signature);
	scanner.Scan();
	void* addr = scanner.GetAddress();
	printf("[REPENTOGON] Patching Hush laser attack speed bug at %p\n", addr);

	const size_t playerMoveSpeedOffset = offsetof(Entity_Player, _movespeed);

	const float* floatPtr = &hushLaserAdjust;
	ASMPatch patch;
	patch.Push(ASMPatch::Registers::ECX); // preserve player
	patch.AddInternalCall(IsRoomSlow);
	patch.Pop(ASMPatch::Registers::ECX); // restore player
	patch.AddBytes("\xF3\x0F\x10\x81"); // movss xmm0,dword ptr ds:[ecx+_movespeed]
	patch.AddBytes(ByteBuffer().AddAny((char*)&playerMoveSpeedOffset, sizeof(playerMoveSpeedOffset))).AddZeroes(2); //add bytes for playerMoveSpeedOffset
	patch.AddBytes("\x84\xC0"); // test al, al
	patch.AddBytes("\x74\x08"); // je, eip+0x8
	patch.AddBytes("\xF3\x0F\x59\x05").AddBytes(ByteBuffer().AddAny((char*)&floatPtr, 4)); // mulss xmm0, dword ptr [0xXXXXXXXX]
	patch.AddRelativeJump((char*)addr + 0x5); // jmp isaac-ng.XXXXXXXX
	sASMPatcher.PatchAt(addr, &patch);
}

void ASMPatchHushBug() {
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
	const char* signature = "f30f104424??8bd8";
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

void __stdcall ProcessApplyFrozenEnemyDeathEffects(Entity_NPC* npc) {
	printf("[REPENTOGON] Entity: %p type %d\n", npc, npc->_type);

	g_Game->_scoreSheet.AddKilledEnemy(npc);
	for (int i = 0; i < g_Game->GetNumPlayers(); i++) {
		g_Game->GetPlayer(i)->TriggerEnemyDeath(npc);
	}
}

void ASMPatchApplyFrozenEnemyDeathEffects() {
	SigScan scanner("8d85????????508d8f????????e8????????8d85????????508d8f????????e8????????508d8d????????e8????????84c074??8d8d");
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