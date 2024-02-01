#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_FamiliarGetFollowerPriority)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	int priority = fam->GetFollowerPriority();
	lua_pushinteger(L, priority);
	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetPathFinder)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	NPCAI_Pathfinder* pathFinder = fam->GetPathFinder();
	lua::luabridge::UserdataPtr::push(L, pathFinder, lua::GetMetatableKey(lua::Metatables::PATHFINDER));

	return 1;
}

LUA_FUNCTION(Lua_FamiliarTryAimAtMarkedTarget)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Vector* aimDirection = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	int direction = (int)luaL_checkinteger(L, 3);
	Vector buffer;
	if (fam->TryAimAtMarkedTarget(aimDirection, direction, &buffer)) {
		lua::luabridge::UserdataPtr::push(L, &buffer, lua::GetMetatableKey(lua::Metatables::VECTOR));
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_FamiliarTriggerRoomClear)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->TriggerRoomClear();

	return 0;
}

LUA_FUNCTION(Lua_FamiliarUpdateDirtColor)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->UpdateDirtColor(true);
	return 0;
}

LUA_FUNCTION(Lua_FamiliarGetDirtColor)
{
	Entity_Familiar* fam = lua::GetUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = fam->_dirtColor;

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetFollowerPriority", Lua_FamiliarGetFollowerPriority },
		{ "GetPathFinder", Lua_FamiliarGetPathFinder },
		{ "TryAimAtMarkedTarget", Lua_FamiliarTryAimAtMarkedTarget },
		{ "TriggerRoomClear", Lua_FamiliarTriggerRoomClear },
		{ "UpdateDirtColor", Lua_FamiliarUpdateDirtColor },
		{ "GetDirtColor", Lua_FamiliarGetDirtColor },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_FAMILIAR, functions);
}