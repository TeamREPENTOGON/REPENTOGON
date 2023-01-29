#include "IsaacRepentance.h"
#include "HookSystem.h"

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	printf("[REPENTOGON WAS HERE] (flame everywhere woah gif modding of isaac sticker)\n");
	lua_State *state = g_LuaEngine->GetState(); //Soon.
};
