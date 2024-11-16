#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "Log.h"
#include "SigScan.h"
#include "XMLData.h"

// Contains the code for some tag strings that can be added to the new "customtags" XML attribute in entities2.xml to support custom familiars.

// TODO: Maybe make the "familiarcantakedamage" tag enable damage from spikes & creep automatically?
constexpr char FAMILIAR_CAN_TAKE_DAMAGE_TAG[] = "familiarcantakedamage";  // Allows familiars to take damage from enemies/hazards by default.
constexpr char FAMILIAR_BLOCK_PROJECTILES_TAG[] = "familiarblockprojectiles";  // Allows familiars to block projectiles automatically.
constexpr char FAMILIAR_IGNORE_BFFS_TAG[] = "familiarignorebffs";  // Makes the familiar ignore the default effects of BFFs.
constexpr char FAMILIAR_NO_CHARM[] = "nocharm";  // Makes the familiar not be charmed by Siren.

// (Disabled for now, causes projectiles to collide with the enemy but not break so they bounce off weirdly.)
// (This tag isn't as necessary since this can be accomplished with REPENTOGON's improved pre-collision callbacks.)
/* constexpr char FAMILIAR_COLLIDE_WITH_ENEMIES_TAG[] = "familiarcollidewithenemies";  // Allows familiars to physically collide with enemies by default.

// Re-implementation of the inlined Entity_Familiar::CanCollideWithEnemies() that allows for overriding via customtag.
bool __stdcall FamiliarCanCollideWithEnemiesReimplementation(Entity_Familiar* fam) {
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();

	// Allow physical collision if familiar has the "familiarcollidewithenemies" customtag.
	if (XMLStuff.EntityData->HasCustomTag(3, var, subt, FAMILIAR_COLLIDE_WITH_ENEMIES_TAG)) {
		return true;
	}

	// Re-implementation of Entity_Familiar::CanCollideWithEnemies()
	return var == 62 || var == 67 || var == 228 || var == 238 || (var == 212 && fam->_state == 1);
}

// Entity_Familiar::CanCollideWithEnemies() is inlined into Entity_Familiar::HandleCollision().
// This patch injects a re-implementation of that function.
void ASMPatchFamiliarCanCollideWithEnemies() {
	SigScan scanner("8b47??83f83e74??83f84374??3de4000000");
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDI) // Push EDI (the familiar) for function input
		.AddInternalCall(FamiliarCanCollideWithEnemiesReimplementation) // call function
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(reg)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + 0x2B) // Jump for TRUE (allow collision)
		.AddRelativeJump((char*)addr + 0x3E); // Jump for FALSE (no collision)
	sASMPatcher.PatchAt(addr, &patch);
}*/

// "familiarblockprojectiles" tag allows a familiar to automatically destroy any projectiles on contact.
HOOK_METHOD(Entity_Familiar, CanBlockProjectiles, ()->bool) {
	if (XMLStuff.EntityData->HasCustomTag(3, *this->GetVariant(), *this->GetSubType(), FAMILIAR_BLOCK_PROJECTILES_TAG)) {
		return true;
	}
	return super();
}

// "familiarcantakedamage" tag allows a familiar to take damage from enemy contact & lasers.
// The familiar still ignores projectiles unless it has the "familiarblockprojectiles" tag as well.
HOOK_METHOD(Entity_Familiar, CanBeDamagedByEnemy, ()->bool) {
	if (XMLStuff.EntityData->HasCustomTag(3, *this->GetVariant(), *this->GetSubType(), FAMILIAR_CAN_TAKE_DAMAGE_TAG)) {
		return true;
	}
	return super();
}

// Re-implementation of the inlined Entity_Familiar::CanBeDamagedByProjectiles() that allows for overriding via customtags.
bool __stdcall FamiliarCanBeDamagedByProjectilesReimplementation(Entity_Familiar* fam) {
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();

	// Familiar can be damaged by projectiles if it has both the "familiarcantakedamage" AND "familiarblockprojectiles" customtags.
	if (XMLStuff.EntityData->HasCustomTag(3, var, subt, FAMILIAR_CAN_TAKE_DAMAGE_TAG) && XMLStuff.EntityData->HasCustomTag(3, var, subt, FAMILIAR_BLOCK_PROJECTILES_TAG)) {
		return true;
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
	if (XMLStuff.EntityData->HasCustomTag(3, var, subt, FAMILIAR_CAN_TAKE_DAMAGE_TAG)) {
		return true;
	}

	// (Deprecated) Whole XML attribute that enabled just collision with lasers.
	// Replaced by the customtag above.
	const std::string laserCollisionsTag = XMLStuff.EntityData->GetNodesByTypeVarSub(3, var, subt, false)["familiarallowlasercollision"];
	if (laserCollisionsTag == "true") {
		return true;
	}
	else if (laserCollisionsTag == "false") {
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
	const unsigned int var = *fam->GetVariant();
	const unsigned int subt = *fam->GetSubType();

	// Check if the familiar has the "familiarignorebffs" customtag.
	if (XMLStuff.EntityData->HasCustomTag(3, var, subt, FAMILIAR_IGNORE_BFFS_TAG)) {
		return false;
	}

	// [Deprecated] Whole XML attribute just for ingoring BFFs.
	// Replaced by the customtag above.
	if (XMLStuff.EntityData->GetNodesByTypeVarSub(3, var, subt, false)["familiarignorebffs"] == "true") {
		return false;
	}

	// Default behaviour: Return true if the familiar's player has BFFs.
	Entity_Player* plr = *fam->GetPlayer();
	return plr && plr->HasCollectible(0xF7, false);
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
	if (XMLStuff.EntityData->HasCustomTag(3, *this->GetVariant(), *this->GetSubType(), FAMILIAR_NO_CHARM)) {
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
