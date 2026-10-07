#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../../LuaClasses.h"

extern "C" {
	__declspec(dllexport) void L_GridEntityDoor_Bar(GridEntity_Door* door) {
		door->Bar();
	}

	__declspec(dllexport) bool L_GridEntityDoor_CanBlowOpen(GridEntity_Door* door) {
		return door->CanBlowOpen();
	}

	__declspec(dllexport) void L_GridEntityDoor_Close(GridEntity_Door* door, bool force) {
		door->Close(force);
	}

	__declspec(dllexport) bool L_GridEntityDoor_IsLocked(GridEntity_Door* door) {
		return door->IsLocked();
	}

	__declspec(dllexport) bool L_GridEntityDoor_IsTargetRoomArcade(GridEntity_Door* door) {
		return door->IsTargetRoomArcade();
	}

	__declspec(dllexport) void L_GridEntityDoor_Open(GridEntity_Door* door) {
		door->Open();
	}

	__declspec(dllexport) void L_GridEntityDoor_PlayAnimation(GridEntity_Door* door) {
		door->play_animation();
	}

	__declspec(dllexport) void L_GridEntityDoor_Render(GridEntity_Door* door, Vector offset) {
		door->Render(offset);
	}

	__declspec(dllexport) void L_GridEntityDoor_SetExtraSprite(GridEntity_Door* door, ANM2* sprite) {
		if (&door->_extraSprite == sprite)
			return;
		door->_extraSprite.destructor();
		door->_extraSprite.construct_from_copy(sprite);
	}
	
	__declspec(dllexport) void L_GridEntityDoor_SetLocked(GridEntity_Door* door, bool locked) {
		door->SetLocked(locked);
	}

	__declspec(dllexport) void L_GridEntityDoor_SetRoomTypes(GridEntity_Door* door, int currentRoomType, int targetRoomType) {
		door->SetRoomTypes(currentRoomType, targetRoomType);
	}

		__declspec(dllexport) void L_GridEntityDoor_SpawnDust(GridEntity_Door* door) {
		door->SpawnDust();
	}

	__declspec(dllexport) void L_GridEntityDoor_Update(GridEntity_Door* door) {
		door->Update();
	}
}

MOD_EXPORT bool L_GridEntityDoor_TryBlowOpen(GridEntity_Door* door, bool fromExplosion, Entity* source) {
	return door->TryBlowOpen(fromExplosion, source);
}

MOD_EXPORT bool L_GridEntityDoor_TryUnlock(GridEntity_Door* door, Entity_Player* player, bool force) {
	return door->TryUnlock(player, force);
}
