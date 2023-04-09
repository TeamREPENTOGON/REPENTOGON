#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

int Lua_FamiliarGetFollowerPriority(lua_State* L)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	int priority = fam->GetFollowerPriority();
	lua_pushinteger(L, priority);
	return 1;
}

int Lua_FamiliarGetPathFinder(lua_State* L)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	NPCAI_Pathfinder* pathFinder = fam->GetPathFinder();
	lua::luabridge::UserdataPtr::push(L, pathFinder, lua::GetMetatableKey(lua::Metatables::PATHFINDER));

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY_FAMILIAR;
	lua::RegisterFunction(state, mt, "GetFollowerPriority", Lua_FamiliarGetFollowerPriority);
	lua::RegisterFunction(state, mt, "GetPathFinder", Lua_FamiliarGetPathFinder);
}