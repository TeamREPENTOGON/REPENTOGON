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

MOD_EXPORT void L_GridEntityLock_TryUnlock(GridEntity_Lock* lock, Entity_Player* player, bool force) {
	lock->TryUnlock(player, force);
}
