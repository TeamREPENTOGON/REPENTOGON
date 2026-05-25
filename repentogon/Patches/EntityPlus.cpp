#include <memory>

#include "EntityPlus.h"

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"
#include "ASMDefinition.h"
#include "Log.h"

// ----------------------------------------------------------------------------------------------------
// -- EntityPlusHolder

namespace {

// Wrapper struct to hold EntityPlus. A pointer to one of these is what actually gets written into the entity.
struct EntityPlusHolder {
	std::unique_ptr<EntityPlus> data;
};

void InitEntityPlusHolder(Entity* entity) {
	// Create an EntityPlusHolder.
	EntityPlusHolder* holder = new EntityPlusHolder();

	// Cut the pointer to it in half.
	uintptr_t val = (uintptr_t)holder;
	unsigned short up = (val & 0xFFFF0000) >> 16;
	unsigned short low = val & 0x0000FFFF;

	// Store the half-pointers in unused boolean padding.
	entity->_entityExtraDataPtrUpperBytes = up;
	entity->_entityExtraDataPtrLowerBytes = low;
}

EntityPlusHolder* GetEntityPlusHolder(Entity* entity, const bool initIfMissing) {
	if (entity->_entityExtraDataPtrUpperBytes == 0 && entity->_entityExtraDataPtrLowerBytes == 0) {
		if (initIfMissing) {
			g_Game->GetConsole()->PrintError("[REPENTOGON] EntityPlusHolder was not initialized!\n");
			// Since the EntityPlusHolder is initialized in the Entity constructor,
			// this should never happen, but leaving this here for now just in case.
			InitEntityPlusHolder(entity);
		}
		else {
			return nullptr;
		}
	}

	// Get the two pointer halves.
	const unsigned short up = entity->_entityExtraDataPtrUpperBytes;
	const unsigned short low = entity->_entityExtraDataPtrLowerBytes;

	// Re-combine them.
	const uintptr_t val = ((uintptr_t)up << 16) + low;

	// Return the reconstructed pointer.
	return (EntityPlusHolder*)val;
}

}  // namespace


// ----------------------------------------------------------------------------------------------------
// -- Memory management/initialization/cleanup hooks.

// Create the EntityPlusHolder in the constructor.
// This should enforce that a valid pointer is always populated in the padding bytes.
HOOK_METHOD(Entity, constructor, () -> void) {
	super();
	InitEntityPlusHolder(this);
}

HOOK_METHOD(Entity, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int seed) -> void) {
	EntityPlusHolder* holder = GetEntityPlusHolder(this, true);
	holder->data.reset();  // Delete any pre-existing EntityPlus.

	// Initialize the EntityPlus as the correct subclass for this entity.
	if (type == ENTITY_PLAYER) {
		holder->data = std::make_unique<EntityPlayerPlus>();
	} else if (type == ENTITY_FAMILIAR) {
		holder->data = std::make_unique<EntityFamiliarPlus>();
	} else if (type == ENTITY_TEAR) {
		holder->data = std::make_unique<EntityTearPlus>();
	} else if (type == ENTITY_LASER) {
		holder->data = std::make_unique<EntityLaserPlus>();
	} else if (type == ENTITY_KNIFE) {
		holder->data = std::make_unique<EntityKnifePlus>();
	} else {
		holder->data = std::make_unique<EntityPlus>();
	}

	super(type, variant, subtype, seed);
}

// Delete the EntityPlus after the entity is removed.
// The only thing the game actually deletes on Remove is the lua GetData, so maybe this isn't necessary.
/*HOOK_METHOD(Entity, Remove, ()->void) {
	super();
	GetEntityPlusHolder(this, true)->data.reset();
}*/

// Destroy the EntityPlusHolder.
// This will also destroy the EntityPlus itself if one exists.
HOOK_METHOD(Entity, destructor, () -> void) {
	delete GetEntityPlusHolder(this, false);
	super();
}


// ----------------------------------------------------------------------------------------------------
// -- EntityPlus Accessors

EntityPlus* GetEntityPlus(Entity* entity) {
	return GetEntityPlusHolder(entity, true)->data.get();
}

// The dynamic_cast returns nullptr if the entity was not initialized with the correct subclass.
EntityPlayerPlus* GetEntityPlayerPlus(Entity_Player* player) {
	return dynamic_cast<EntityPlayerPlus*>(GetEntityPlusHolder(player, true)->data.get());
}

EntityFamiliarPlus* GetEntityFamiliarPlus(Entity_Familiar* familiar) {
	return dynamic_cast<EntityFamiliarPlus*>(GetEntityPlusHolder(familiar, true)->data.get());
}

EntityTearPlus* GetEntityTearPlus(Entity_Tear* tear)
{
    return dynamic_cast<EntityTearPlus*>(GetEntityPlusHolder(tear, true)->data.get());
}

EntityLaserPlus* GetEntityLaserPlus(Entity_Laser* laser) {
	return dynamic_cast<EntityLaserPlus*>(GetEntityPlusHolder(laser, true)->data.get());
}

EntityKnifePlus* GetEntityKnifePlus(Entity_Knife* knife) {
	return dynamic_cast<EntityKnifePlus*>(GetEntityPlusHolder(knife, true)->data.get());
}


// ----------------------------------------------------------------------------------------------------
// -- Make entities obey EntityPlus.isFlyingOverride

HOOK_METHOD(Entity, IsFlying, ()->bool) {
	EntityPlus* entityPlus = GetEntityPlus(this);
	if (entityPlus && entityPlus->isFlyingOverride) {
		return *entityPlus->isFlyingOverride;
	}
	return super();
}


// ----------------------------------------------------------------------------------------------------
// -- WaterClipInfo

HOOK_METHOD(Entity, GetWaterClipInfo, (WaterClipInfo* out) -> WaterClipInfo*) {
	super(out);
	EntityPlus* entityPlus = GetEntityPlus(this);
	if (entityPlus && entityPlus->waterClipInfoFlagsOverride) {
		out->bitFlags = *entityPlus->waterClipInfoFlagsOverride;
	}
	return out;
}


// ----------------------------------------------------------------------------------------------------
// -- ASM Patches

namespace {

// EntityLaserPlus.recalculateSamplesNextUpdate
void __stdcall CheckRecalculateSamples(Entity_Laser* laser) {
	EntityLaserPlus* laserPlus = GetEntityLaserPlus(laser);
	if (laserPlus && laserPlus->recalculateSamplesNextUpdate) {
		laser->_homingLaser._bezier.clear();
		laser->_homingLaser._samples.clear();
		laser->_homingLaser._unkVec.clear();  // The game does clear this when the others are reset.
		laser->_homingLaser._nonOptimizedSamples.clear();
		laserPlus->recalculateSamplesNextUpdate = false;
	}
}
void PatchRecalculateLaserSamples(const char* sig, const int numOverriddenBytes) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

	printf("[REPENTOGON] Patching for triggering laser sample recalculation at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // Entity_Laser*
		.AddInternalCall(CheckRecalculateSamples)
		.RestoreRegisters(savedRegisters)
		.AddBytes(ByteBuffer().AddAny((char*)addr, numOverriddenBytes))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + numOverriddenBytes);
	sASMPatcher.PatchAt(addr, &patch);
}

bool __stdcall PlayerHasCamoOverride(Entity_Player* player) {
	EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(player);

	return playerPlus && playerPlus->camoOverride;
}

void PatchPlayerForceCamo() {
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityPlus_PlayerForceCamo);
	const int jumpOffset = 0x15 + *(int*)((char*)addr + 0x11);

	printf("[REPENTOGON] Patching Player::UpdateEffect for camo override at %p\n", addr);

	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EDI) // push player
		.AddInternalCall(PlayerHasCamoOverride)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (char*)addr + jumpOffset) //skipping vanilla curse icons rendering
		.AddBytes(ByteBuffer().AddAny((char*)addr, 0x6))  // Restore the bytes we overwrote
		.AddRelativeJump((char*)addr + 0x6);
	sASMPatcher.PatchAt(addr, &patch);
}

constexpr uint32_t SOUND_NULL = 0;

HOOK_METHOD(Entity_Laser, PlayInitSound, () -> void) {
	EntityLaserPlus* laserPlus = GetEntityLaserPlus(this);
	assert(laserPlus);
	if (laserPlus && laserPlus->initSound) {
		const uint32_t customSound = laserPlus->initSound.value();
		if (customSound != SOUND_NULL) {
			g_Manager->_sfxManager.Play(customSound, 1.0, 2, false, 1.0, 0);
		}
	} else {
		super();
	}
}

static bool __fastcall asm_try_play_custom_init_sound(Entity_Tear* tear)
{
	EntityTearPlus* tearPlus = GetEntityTearPlus(tear);
	assert(tearPlus);
	
	std::optional<uint32_t> initSound = tearPlus->initSound;
	if (!initSound)
	{
		return false;
	}

	int customSound = initSound.value();
	if (customSound != SOUND_NULL)
	{
		g_Manager->_sfxManager.Play(customSound, 1.0, 2, false, 1.0, 0);
	}

	return true;
}

static void Patch_InitTear_PlaySound()
{
	intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityTear_InitTear_PrePlaySound);
	intptr_t skipAddr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::EntityTear_InitTear_PostPlaySound);
	ZHL::Log("[REPENTOGON] Patching Entity_Tear::init_tear for play custom init sound at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;

	intptr_t resumeAddr = addr + 7;
	constexpr size_t RESTORED_BYTES = 7;

	patch.AddBytes(ByteBuffer().AddAny((void*)addr, RESTORED_BYTES))
		.PreserveRegisters(savedRegisters)
		.AddBytes("\x89\xF9") // MOV ECX, EDI
		.AddInternalCall(asm_try_play_custom_init_sound)
		.AddBytes("\x84\xC0") // TEST AL, AL
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNZ, (void*)skipAddr)
		.AddRelativeJump((void*)resumeAddr);

	sASMPatcher.PatchAt((void*)addr, &patch);
}

}  // namespace

void ASMPatchesForEntityPlus() {
	PatchRecalculateLaserSamples("f30f108f????????0f2e8f", 8);  // update_laser
	PatchRecalculateLaserSamples("8b97????????8d8f????????52", 6);  // update_circle_laser
	PatchPlayerForceCamo(); // update_effects
	Patch_InitTear_PlaySound();
}
