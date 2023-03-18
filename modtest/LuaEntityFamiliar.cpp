#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*int Lua_FamiliarGetFollowerPriority(lua_State* L)
{
	Entity_Familiar* familiar = *(Entity_Familiar**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar") + 4);
	int priority = familiar->GetFollowerPriority();
	lua_pushinteger(L, priority);
	return 1;
}

static void RegisterFamiliarGetFollowerPriority(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_FAMILIAR);
	lua_pushstring(L, "GetFollowerPriority");
	lua_pushcfunction(L, Lua_FamiliarGetFollowerPriority);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}
*/

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	//RegisterFamiliarGetFollowerPriority(state);
	//RegisterEntityCopyStatusEffects(state);
}