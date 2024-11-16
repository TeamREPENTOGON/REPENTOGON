#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "../XMLPlayerExtras.h"

#include "ASMEntityNPC.h"

void ASMPatchSpawnSelectedBaby() {
	SigScan scanner("8b4d??83c4086aff"); // this is the first push of args for ComputeStausEffectDuration
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.MoveFromMemory(ASMPatch::Registers::EBP, -0x38, ASMPatch::Registers::ECX)
		.AddBytes("\x83\xc4\x08")
		.Push(ASMPatch::Registers::EBX, 0x8) //death awaits
		.AddRelativeJump((char*)addr + 0x8);
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

	ZHL::Log("[REPENTOGON] Patching PlayerManager::init_special_baby_selection (to respect modded achievements) at %p\n", addr);

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
