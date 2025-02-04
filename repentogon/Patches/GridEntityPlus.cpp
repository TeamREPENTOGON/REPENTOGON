#include "GridEntityPlus.h"

#include "HookSystem.h"
#include "IsaacRepentance.h"

// GridEntity class has no room for any "writing pointers in unused bytes" shenanigans.
// Thankfully, GridEntities are relatively limited, so we just store one GridEntityPlus struct per valid grid index.
// This can be reworked in the future if any issues are found.

namespace {

constexpr int GRID_SIZE = 448;

GridEntityPlus _gridEntityPlus[GRID_SIZE];

bool IsValidGridIndex(const int idx) {
	return idx >= 0 && idx < GRID_SIZE;
}

void ResetGridEntityPlus(GridEntity* gridEntity) {
	if (gridEntity && IsValidGridIndex(gridEntity->_gridIndex)) {
		_gridEntityPlus[gridEntity->_gridIndex] = GridEntityPlus();
	}
}

GridEntityPlus* GetGridEntityPlus(const uint32_t gridIndex) {
	if (IsValidGridIndex(gridIndex)) {
		return &_gridEntityPlus[gridIndex];
	}
	return nullptr;
}

}  // namespace

GridEntityPlus* GetGridEntityPlus(GridEntity* gridEntity) {
	if (gridEntity) {
		return GetGridEntityPlus(gridEntity->_gridIndex);
	}
	return nullptr;
}

HOOK_STATIC(Isaac, CreateGridEntity, (int gridEntityType, unsigned int gridIndex) -> GridEntity*, __cdecl) {
	GridEntity* gridEntity = super(gridEntityType, gridIndex);
	ResetGridEntityPlus(gridEntity);
	return gridEntity;
}

HOOK_METHOD(GridEntity, Init, (unsigned int seed) -> void) {
	ResetGridEntityPlus(this);
	super(seed);
}


// ----------------------------------------------------------------------------------------------------
// -- WaterClipInfo hooks

void HandleGridEntityWaterClipInfo(GridEntity* gridEntity, WaterClipInfo* out) {
	GridEntityPlus* gridEntityPlus = GetGridEntityPlus(gridEntity);
	if (gridEntityPlus && gridEntityPlus->waterClipInfoFlagsOverride) {
		out->bitFlags = *gridEntityPlus->waterClipInfoFlagsOverride;
	}
}

#define HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(_type) \
HOOK_METHOD(_type, GetWaterClipInfo, (WaterClipInfo* out) -> WaterClipInfo*) { \
	super(out); \
	HandleGridEntityWaterClipInfo(this, out); \
	return out; \
}

HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity);  // Also used for Statue, Wall, Fire, Spikes, TNT, Gravity
HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity_Pit);  // Also used for Teleporter, TrapDoor, Web, Stairs, Decoration
HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity_Door);
HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity_Poop);
HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity_PressurePlate);
HOOK_GRIDENTITY_GET_WATER_CLIP_INFO(GridEntity_Rock);
