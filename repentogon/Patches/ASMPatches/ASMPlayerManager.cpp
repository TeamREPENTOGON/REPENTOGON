#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../XMLPlayerExtras.h"

#include "ASMEntityNPC.h"

void ASMPatchSpawnSelectedBaby() {
	SigScan scanner("8b4d??6affe8????????8bf8");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.MoveFromMemory(ASMPatch::Registers::EBP, -0x38, ASMPatch::Registers::ECX)
		.Push(ASMPatch::Registers::EBX, 0x8) //death awaits
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
}

// Patches into `PlayerManager::init_special_baby_selection` to hide modded characters from the co-op menu if they are still locked behind an achievement.
bool __stdcall ShouldHideModdedCharacterInCoopWheel(EntityConfig_Player* conf) {
	return conf->_hidden || (conf->_achievement > 0 && !g_Manager->GetPersistentGameData()->Unlocked(conf->_achievement)) || !IsCharacterUnlockedRgon(conf->_id);
}
void ASMPatchCoopWheelRespectModdedAchievements() {
	SigScan scanner("80b8????????0075??8b47??895d");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching PlayerManager::init_special_baby_selection (to respect modded achievements) at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX)  // EntityConfig_Player
		.AddInternalCall(ShouldHideModdedCharacterInCoopWheel)
		.AddBytes("\x84\xC0")  // test al, al
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}
