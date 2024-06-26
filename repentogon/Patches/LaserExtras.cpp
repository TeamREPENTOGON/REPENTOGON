#include "LaserExtras.h"

#include <unordered_set>

#include "ASMPatches.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "SigScan.h"
#include "XMLData.h"
#include "../LuaInit.h"
#include "ASMPatcher.hpp"

std::unordered_set<int> laserIndexesToRecalculateSamples;

void RecalculateLaserSamplesNextUpdate(Entity_Laser* laser) {
	laserIndexesToRecalculateSamples.insert(laser->_index);
}

HOOK_METHOD(Entity_Laser, Init, (uint32_t type, uint32_t variant, uint32_t subtype, uint32_t seed)->void) {
	laserIndexesToRecalculateSamples.erase(this->_index);
	super(type, variant, subtype, seed);
}

HOOK_METHOD(Entity_Laser, Remove, ()->void) {
	laserIndexesToRecalculateSamples.erase(this->_index);
	super();
}

HOOK_METHOD(Game, Exit, (bool ShouldSave)->void) {
	laserIndexesToRecalculateSamples.clear();
	super(ShouldSave);
}

void __stdcall CheckRecalculateSamples(Entity_Laser* laser) {
	if (laserIndexesToRecalculateSamples.find(laser->_index) != laserIndexesToRecalculateSamples.end()) {
		laser->_homingLaser._bezier.clear();
		laser->_homingLaser._samples.clear();
		laser->_homingLaser._unkVec.clear();
		laser->_homingLaser._nonOptimizedSamples.clear();
		laserIndexesToRecalculateSamples.erase(laser->_index);
	}
}

void PatchRecalculateLaserSamples(const char* sig, const int numOverriddenBytes) {
	SigScan scanner(sig);
	scanner.Scan();
	void* addr = scanner.GetAddress();

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

void ASMPatchesForLaserExtras() {
	PatchRecalculateLaserSamples("f30f108f????????0f2e8f", 8);  // update_laser
	PatchRecalculateLaserSamples("8b97????????8d8f????????f30f1055", 6);  // update_circle_laser
}
