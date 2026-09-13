#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "Log.h"
#include "SigScan.h"
#include "XMLData.h"
#include "EntityConfigEx.h"

// Contains the code for some tag strings that can be added to the new "customtags" XML attribute in entities2.xml to support custom familiars.

// "familiarblockprojectiles" tag allows a familiar to automatically destroy any projectiles on contact.
HOOK_METHOD(Entity_Familiar, CanBlockProjectiles, ()->bool) {
	if (auto* ex = EntityConfigEx::GetEntityEx(ENTITY_FAMILIAR, this->_variant, this->_subtype); ex && ex->HasCustomTag(CustomTags::FAMILIAR_BLOCK_PROJECTILES)) {
		return true;
	}
	return super();
}

// "familiarcantakedamage" tag allows a familiar to take damage from enemy contact & lasers.
// The familiar still ignores projectiles unless it has the "familiarblockprojectiles" tag as well.
HOOK_METHOD(Entity_Familiar, CanBeDamagedByEnemy, ()->bool) {
	if (auto* ex = EntityConfigEx::GetEntityEx(ENTITY_FAMILIAR, this->_variant, this->_subtype); ex && ex->HasCustomTag(CustomTags::FAMILIAR_CAN_TAKE_DAMAGE)) {
		return true;
	}
	return super();
}

// Re-implementation of the inlined Entity_Familiar::CanBeDamagedByProjectiles() that allows for overriding via customtags.
bool __stdcall FamiliarCanBeDamagedByProjectilesReimplementation(Entity_Familiar* fam) {
	const unsigned int var = fam->_variant;
	const unsigned int subt = fam->_subtype;

	// Familiar can be damaged by projectiles if it has both the "familiarcantakedamage" AND "familiarblockprojectiles" customtags.
	if (auto* ex = EntityConfigEx::GetEntityEx(ENTITY_FAMILIAR, var, subt)) {
		if (ex->HasCustomTag(CustomTags::FAMILIAR_CAN_TAKE_DAMAGE) && ex->HasCustomTag(CustomTags::FAMILIAR_BLOCK_PROJECTILES)) {
			return true;
		}
	}

	// Re-implementation of Entity_Familiar::CanBeDamagedByProjectiles()
	return var == 62 || var == 67 || var == 100 || var == 211 || var == 237 || (var == 206 && subt != 702);
}

// Entity_Familiar::CanBeDamagedByProjectiles() is inlined into Entity_Projectile::HandleCollision().
// This patch injects a re-implementation of that function.
void ASMPatchFamiliarCanBeDamagedByProjectiles() {
	SigScan scanner("8b43??83f83e74??83f84374");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching inlined Entity_Familiar::CanBeDamagedByProjectiles in Entity_Projectile::HandleCollision at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EBX) // Push EBX (the familiar) for function input
		.AddInternalCall(FamiliarCanBeDamagedByProjectilesReimplementation) // call function
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x1D) // Jump for TRUE (can be hit)
		.AddRelativeJump((char*)addr + 0x94); // Jump for FALSE (can't be hit)
	sASMPatcher.PatchAt(addr, &patch);
}

// Re-implementation of the inlined Entity_Familiar::CanBeDamagedByLaser() that allows for overriding via a customtag.
bool __stdcall FamiliarCanBeDamagedByLaserReimplementation(Entity_Familiar* fam) {
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();

	// Allow laser collision if familiar has the "familiarcantakedamage" customtag.
	if (auto* ex = EntityConfigEx::GetEntityEx(ENTITY_FAMILIAR, var, subt); ex && ex->HasCustomTag(CustomTags::FAMILIAR_CAN_TAKE_DAMAGE)) {
		return true;
	}

	// (Deprecated) Whole XML attribute that enabled just collision with lasers.
	// Replaced by the customtag above.
	const std::string laserCollisionsTag = XMLStuff.EntityData->GetAttributeByTypeVarSub(ENTITY_FAMILIAR, var, subt, false, "familiarallowlasercollision");
	if (laserCollisionsTag == "true") {
		return true;
	} else if (laserCollisionsTag == "false") {
		return false;
	}

	// Re-implementation of Entity_Familiar::CanBeDamagedByLaser()
	return var == 62 || var == 67 || var == 100 || var == 211 || (var == 206 && subt == 427);
}

// Entity_Familiar::CanBeDamagedByLaser() is inlined into Entity_Laser::CanDamageEntity().
// This patch injects a re-implementation of that function.
void ASMPatchFamiliarCanBeDamagedByLaser() {
	SigScan scanner("8b46??83f83e74??83f84374??3dd300000074??83f86474");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching inlined Entity_Familiar::CanBeDamagedByLaser in Entity_Laser::CanDamageEntity at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.AddBytes("\x56") // Push ESI (the familiar) for function input
		.AddInternalCall(FamiliarCanBeDamagedByLaserReimplementation) // call function
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x29) // Jump for TRUE (can be hit)
		.AddRelativeJump((char*)addr + 0x2D); // Jump for FALSE (can't be hit)
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall FamiliarGetMultiplierTrampoline(Entity_Familiar* fam) {
	// Check if the familiar has the "familiarignorebffs" customtag.
	if (auto* ex = EntityConfigEx::GetEntityEx(fam->_type, fam->_variant, fam->_subtype); ex && ex->HasCustomTag(CustomTags::FAMILIAR_IGNORE_BFFS)) {
		return false;
	}

	// Default behaviour: Return true if the familiar's player has BFFs.
	Entity_Player* plr = fam->GetPlayer();
	return plr && plr->HasCollectible(COLLECTIBLE_BFFS, false);
}

// This patch allows for disabling the check for BFFS in Entity_Familiar::GetMultiplier() with a customtag.
void ASMPatchFamiliarGetMultiplier() {
	SigScan scanner("e8????????84c074??8b46??83f82b");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ZHL::Log("[REPENTOGON] Patching Entity_Familiar::GetMultiplier at %p\n", addr);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.AddBytes("\x83\xC4\x08") // add esp, 4
		.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ESI)
		.AddInternalCall(FamiliarGetMultiplierTrampoline) // call FamiliarGetMultiplierTrampoline()
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)addr + 0x16) // jump for false
		.AddRelativeJump((char*)addr + 0x9); // jump for true
	sASMPatcher.PatchAt(addr, &patch);
}

HOOK_METHOD_PRIORITY(Entity_Familiar, CanCharm, -1000, () -> bool) {
	if (auto* ex = EntityConfigEx::GetEntityEx(ENTITY_FAMILIAR, this->_variant, this->_subtype); ex && ex->HasCustomTag(CustomTags::FAMILIAR_NO_CHARM)) {
		return false;
	}
	return super();
}

// Function called in ASMPatches.cpp to run patches at the appropriate time.
void ASMPatchesForFamiliarCustomTags() {
	ASMPatchFamiliarCanBeDamagedByLaser();
	ASMPatchFamiliarCanBeDamagedByProjectiles();
	// ASMPatchFamiliarCanCollideWithEnemies();
	ASMPatchFamiliarGetMultiplier();
}
