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

	__declspec(dllexport) const char* L_GridEntityDoor_GetCloseAnimation(GridEntity_Door* door) {
		return door->_closeAnimation.c_str();
	}

	__declspec(dllexport) const char* L_GridEntityDoor_GetLockedAnimation(GridEntity_Door* door) {
		return door->_lockedAnimation.c_str();
	}

	__declspec(dllexport) const char* L_GridEntityDoor_GetOpenAnimation(GridEntity_Door* door) {
		return door->_openAnimation.c_str();
	}

	__declspec(dllexport) const char* L_GridEntityDoor_GetOpenLockedAnimation(GridEntity_Door* door) {
		return door->_openLockedAnimation.c_str();
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

	__declspec(dllexport) void L_GridEntityDoor_SetLocked(GridEntity_Door* door, bool locked) {
		door->SetLocked(locked);
	}

	__declspec(dllexport) void L_GridEntityDoor_SetRoomTypes(GridEntity_Door* door, int currentRoomType, int targetRoomType) {
		door->SetRoomTypes(currentRoomType, targetRoomType);
	}

	__declspec(dllexport) void L_GridEntityDoor_Update(GridEntity_Door* door) {
		door->Update();
	}
}

LUA_FUNCTION(Lua_GridEntityDoorTryBlowOpen) {
	GridEntity_Door* door = LuaGridEntityDoor::Get(L, 1);
	bool fromExplosion = lua::luaL_checkboolean(L, 2);
	Entity* source = LuaEntity::GetOpt(L, 3);

	lua_pushboolean(L, door->TryBlowOpen(fromExplosion, source));
	return 1;
}

LUA_FUNCTION(Lua_GridEntityDoorTryUnlock) {
	GridEntity_Door* door = LuaGridEntityDoor::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::Get(L, 2);
	bool force = lua::luaL_checkboolean(L, 3);

	lua_pushboolean(L, door->TryUnlock(player, force));
	return 1;
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_GridEntityDoor_TryBlowOpen", Lua_GridEntityDoorTryBlowOpen);
	lua_register(_state, "__Lua_GridEntityDoor_TryUnlock", Lua_GridEntityDoorTryUnlock);

	super();
}