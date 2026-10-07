#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../../Patches/GridEntityPlus.h"
#include "../../LuaClasses.h"

extern "C" {
	__declspec(dllexport) bool L_GridEntity_Destroy(GridEntity* grid, bool immediate, EntityRef* source) {
		return grid->Destroy(immediate, source);
	}

	__declspec(dllexport) void L_GridEntity_GetPosition(GridEntity* grid, Vector* buffer) {
		grid->GetPosition(buffer);
	}

	__declspec(dllexport) void L_GridEntity_GetRenderPosition(GridEntity* grid, Vector* buffer) {
		grid->GetRenderPosition(buffer);
	}

	__declspec(dllexport) unsigned int L_GridEntity_GetWaterClipFlags(GridEntity* grid) {
		WaterClipInfo info;
		grid->GetWaterClipInfo(&info);
		return info.bitFlags;
	}
		
	__declspec(dllexport) bool L_GridEntity_Hurt(GridEntity* grid, int damage, EntityRef* source) {
		return grid->Hurt(damage, source);
	}


	__declspec(dllexport) void L_GridEntity_HurtSurroundings(GridEntity* grid, float enemyDistance, float playerDistance, float enemyDamage, int playerDamage, uint64_t damageFlags, bool ignoreGridCol) {
		grid->hurt_surroundings(enemyDistance, playerDistance, enemyDamage, playerDamage, damageFlags, ignoreGridCol);
	}

	__declspec(dllexport) void L_GridEntity_Init(GridEntity* grid, unsigned int seed) {
		grid->Init(seed);
	}

	__declspec(dllexport) void L_GridEntity_Render(GridEntity* grid, Vector offset) {
		grid->Render(offset);
	}

	__declspec(dllexport) void L_GridEntity_ResetWaterClipFlags(GridEntity* grid) {
		GridEntityPlus* gridEntPlus = GetGridEntityPlus(grid);
		if (gridEntPlus) {
			gridEntPlus->waterClipInfoFlagsOverride = std::nullopt;
		}
	}

	__declspec(dllexport) void L_GridEntity_SetWaterClipFlags(GridEntity* grid, unsigned int flags) {
		GridEntityPlus* gridEntPlus = GetGridEntityPlus(grid);
		if (gridEntPlus) {
			gridEntPlus->waterClipInfoFlagsOverride = flags;
		}
	}
}

MOD_EXPORT void L_GridEntity_HurtDamage(GridEntity* gridEnt, Entity* entity, int playerDamage, uint64_t damageFlags, float enemyDamage, bool ignoreGridCol) {
	gridEnt->hurt_func(entity, enemyDamage, playerDamage, damageFlags, ignoreGridCol);
}
