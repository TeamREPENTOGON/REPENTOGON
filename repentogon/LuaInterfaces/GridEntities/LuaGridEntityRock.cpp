#include "IsaacRepentance.h"

extern "C" {
	MOD_EXPORT bool L_GridEntityRock_Destroy(GridEntity_Rock* rock, bool immediate, EntityRef* source) {
		return rock->Destroy(immediate, source);
	}

	//TODO: we can probably reimplement this one in Lua pretty easily
	MOD_EXPORT int L_GridEntityRock_GetAltRockType(int backdrop) {
		return GridEntity_Rock::GetAltRockType(backdrop);
	}

	// ditto
	MOD_EXPORT void L_GridEntityRock_PlayBreakSound(GridEntity_Rock* rock, int gridType, int backdrop) {
		rock->PlayBreakSound(gridType, backdrop);
	}

	MOD_EXPORT void L_GridEntityRock_PostInit(GridEntity_Rock* rock) {
		rock->PostInit();
	}


	// ditto
	MOD_EXPORT void L_GridEntityRock_RegisterRocksDestroyed(GridEntity_Rock* rock, int gridType) {
		rock->RegisterRockDestroyed(gridType);
	}

	MOD_EXPORT void L_GridEntityRock_Render(GridEntity_Rock* rock, Vector offset) {
		rock->Render(offset);
	}

	MOD_EXPORT void L_GridEntityRock_RenderTop(GridEntity_Rock* rock, Vector offset) {
		rock->RenderTop(offset);
	}

	MOD_EXPORT void L_GridEntityRock_SetBigRockFrame(GridEntity_Rock* rock, int frame) {
		rock->SetBigRockFrame(frame);
	}

	MOD_EXPORT void L_GridEntityRock_SpawnDrops(Vector position, int gridType, int gridVariant, unsigned int seed, bool unk, int backdropType) {
		GridEntity_Rock::SpawnDrops(position, gridType, gridVariant, seed, unk, backdropType);
	}

	MOD_EXPORT void L_GridEntityRock_TrySpawnLadder(GridEntity_Rock* rock) {
		rock->TrySpawnLadder();
	}

	MOD_EXPORT void L_GridEntityRock_TrySpawnWorms(GridEntity_Rock* rock) {
		rock->TrySpawnWorms();
	}

	MOD_EXPORT void L_GridEntityRock_Update(GridEntity_Rock* rock) {
		rock->Update();
	}
	
	// ditto
	MOD_EXPORT void L_GridEntityRock_UpdateCollision(GridEntity_Rock* rock) {
		rock->update_collision();
	}

	MOD_EXPORT void L_GridEntityRock_UpdateNeighbors(GridEntity_Rock* rock) {
		rock->UpdateNeighbors();
	}
}
