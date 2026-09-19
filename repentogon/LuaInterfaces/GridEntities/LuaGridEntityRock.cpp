#include "IsaacRepentance.h"

extern "C" {
	__declspec(dllexport) bool L_GridEntityRock_Destroy(GridEntity_Rock* rock, bool immediate, EntityRef* source) {
		return rock->Destroy(immediate, source);
	}

	//TODO: we can probably reimplement this one in Lua pretty easily
	__declspec(dllexport) int L_GridEntityRock_GetAltRockType(int backdrop) {
		return GridEntity_Rock::GetAltRockType(backdrop);
	}

	__declspec(dllexport) const char* L_GridEntityRock_GetAnim(GridEntity_Rock* rock) {
		return rock->_anim.c_str();
	}

	__declspec(dllexport) const char* L_GridEntityRock_GetRubbleAnim(GridEntity_Rock* rock) {
		return rock->_rubbleAnim.c_str();
	}

	// ditto
	__declspec(dllexport) void L_GridEntityRock_PlayBreakSound(GridEntity_Rock* rock, int gridType, int backdrop) {
		rock->PlayBreakSound(gridType, backdrop);
	}

	__declspec(dllexport) void L_GridEntityRock_PostInit(GridEntity_Rock* rock) {
		rock->PostInit();
	}


	// ditto
	__declspec(dllexport) void L_GridEntityRock_RegisterRocksDestroyed(GridEntity_Rock* rock, int gridType) {
		rock->RegisterRockDestroyed(gridType);
	}

	__declspec(dllexport) void L_GridEntityRock_Render(GridEntity_Rock* rock, Vector offset) {
		rock->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityRock_RenderTop(GridEntity_Rock* rock, Vector offset) {
		rock->RenderTop(offset);
	}

	__declspec(dllexport) void L_GridEntityRock_SpawnDrops(Vector position, int gridType, int gridVariant, unsigned int seed, bool unk, int backdropType) {
		GridEntity_Rock::SpawnDrops(position, gridType, gridVariant, seed, unk, backdropType);
	}

	__declspec(dllexport) void L_GridEntityRock_TrySpawnLadder(GridEntity_Rock* rock) {
		rock->TrySpawnLadder();
	}

	__declspec(dllexport) void L_GridEntityRock_TrySpawnWorms(GridEntity_Rock* rock) {
		rock->TrySpawnWorms();
	}

	__declspec(dllexport) void L_GridEntityRock_Update(GridEntity_Rock* rock) {
		rock->Update();
	}
	
	// ditto
	__declspec(dllexport) void L_GridEntityRock_UpdateCollision(GridEntity_Rock* rock) {
		rock->update_collision();
	}

	__declspec(dllexport) void L_GridEntityRock_UpdateNeighbors(GridEntity_Rock* rock) {
		rock->UpdateNeighbors();
	}
}
