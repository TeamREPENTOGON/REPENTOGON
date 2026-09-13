#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../../LuaClasses.h"
extern "C" {
	__declspec(dllexport) void L_GridEntityLock_Update(GridEntity_Lock* lock) {
		lock->Update();
	}

	__declspec(dllexport) void L_GridEntityLock_Render(GridEntity_Lock* lock, Vector offset) {
		lock->Render(offset);
	}

}	

LUA_FUNCTION(Lua_GridEntityLockTryUnlock) {
	GridEntity_Lock* lock = LuaGridEntityLock::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::Get(L, 2);
	bool force = lua::luaL_checkboolean(L, 3);
	lock->TryUnlock(player, force);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_GridEntitylock_TryUnlock", Lua_GridEntityLockTryUnlock);

	super();
}