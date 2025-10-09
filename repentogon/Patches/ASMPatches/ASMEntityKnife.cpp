#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "../ASMPatches.h"
#include "SigScan.h"
#include "LuaCore.h"
#include "../EntityPlus.h"

void __stdcall RunMeleeHitboxCallback(Entity_Knife* hitboxKnife, Entity_Knife* mainKnife) {
	// Give the hitbox a reference to the main knife
	EntityKnifePlus* entityPlus = GetEntityKnifePlus(hitboxKnife);
	if (entityPlus) {
		entityPlus->hitboxSource.SetReference(mainKnife);
	}
	hitboxKnife->Update();  // Patch overwrites an update call.
}
void PatchMeleeHitboxInit() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::PostMeleeHitboxInit);

	printf("[REPENTOGON] Patching Entity_Knife::update_bone_swing for melee hitbox init at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::ESI)  // "Weapon" Knife
		.Push(ASMPatch::Registers::EDI)  // "Hitbox" Knife
		.AddInternalCall(RunMeleeHitboxCallback)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump((char*)addr + 0x7);
	sASMPatcher.PatchAt(addr, &patch);
}
