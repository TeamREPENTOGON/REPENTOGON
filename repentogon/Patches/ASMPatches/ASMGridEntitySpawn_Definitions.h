#pragma once
#include "IsaacRepentance.h"
#include "ASMPatcher.hpp"

struct GridEntitySpawnPatchInfo {
	std::string signature;
	std::string comment; // for log
	int sigOffset; // offset from address found by scanner
	GridEntityType type;
	std::optional<ASMPatch::Registers> variantReg; // register containing variant. NO_VARIANT_REG if not used
	int variantOffset; // if no register is set, contains variant, otherwise contains offset to variant
	ASMPatch::Registers idxReg; // register containing grid idx
	int idxOffset; // offset from idxReg to grid idx
	ASMPatch::Registers seedReg; // register containing seed
	int seedOffset; // offset from seedReg to seed
	int jumpOffset; // offset from start of patch to jumped address
	std::optional<int> jumpCondOffset;
};

const std::array<GridEntitySpawnPatchInfo, 23>& GetGridEntitySpawnPatches();