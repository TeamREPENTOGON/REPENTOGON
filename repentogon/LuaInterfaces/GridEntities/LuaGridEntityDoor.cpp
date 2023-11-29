#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GridEntityDoor_GetExtraSprite)
{
	GridEntity_Door* gridEnt = lua::GetUserdata<GridEntity_Door*>(L, 1, lua::Metatables::GRID_ENTITY_DOOR, "GridEntityDoor");
	ANM2* anm2 = &gridEnt->_extraSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	luaL_Reg functions[] = {
		{ "GetExtraSprite", Lua_GridEntityDoor_GetExtraSprite },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GRID_ENTITY_DOOR, functions);
}