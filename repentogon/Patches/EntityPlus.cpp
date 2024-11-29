#include <memory>

#include "EntityPlus.h"

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "ASMPatcher.hpp"

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
	if (type == 1) {
		holder->data = std::make_unique<EntityPlayerPlus>();
	} else if (type == 3) {
		holder->data = std::make_unique<EntityFamiliarPlus>();
	} else if (type == 7) {
		holder->data = std::make_unique<EntityLaserPlus>();
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

EntityLaserPlus* GetEntityLaserPlus(Entity_Laser* laser) {
	return dynamic_cast<EntityLaserPlus*>(GetEntityPlusHolder(laser, true)->data.get());
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

}  // namespace

void ASMPatchesForEntityPlus() {
	PatchRecalculateLaserSamples("f30f108f????????0f2e8f", 8);  // update_laser
	PatchRecalculateLaserSamples("8b97????????8d8f????????52", 6);  // update_circle_laser
}
