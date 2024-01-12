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

LUA_FUNCTION(Lua_GridEntityDoor_GetPreviousState) {
	GridEntity_Door* gridEnt = lua::GetUserdata<GridEntity_Door*>(L, 1, lua::Metatables::GRID_ENTITY_DOOR, "GridEntityDoor");
	lua_pushinteger(L, gridEnt->_previousState);
	return 1;
}

LUA_FUNCTION(Lua_GridEntityDoor_SetPreviousState) {
	GridEntity_Door* gridEnt = lua::GetUserdata<GridEntity_Door*>(L, 1, lua::Metatables::GRID_ENTITY_DOOR, "GridEntityDoor");
	gridEnt->_previousState = (unsigned int)luaL_checkinteger(L, 2);

	return 1;
}

LUA_FUNCTION(Lua_GridEntityDoor_GetPreviousVarinat) {
	GridEntity_Door* gridEnt = lua::GetUserdata<GridEntity_Door*>(L, 1, lua::Metatables::GRID_ENTITY_DOOR, "GridEntityDoor");
	lua_pushinteger(L, gridEnt->_previousVariant);

	return 1;
}

LUA_FUNCTION(Lua_GridEntityDoor_SetPreviousVariant) {
	GridEntity_Door* gridEnt = lua::GetUserdata<GridEntity_Door*>(L, 1, lua::Metatables::GRID_ENTITY_DOOR, "GridEntityDoor");
	gridEnt->_previousVariant = (unsigned int)luaL_checkinteger(L, 2);
	
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
	
	//fix PreviousState/PreviousVariant
	lua::RegisterVariable(_state, lua::Metatables::GRID_ENTITY_DOOR, "PreviousState", Lua_GridEntityDoor_GetPreviousState, Lua_GridEntityDoor_SetPreviousState);
	lua::RegisterVariable(_state, lua::Metatables::GRID_ENTITY_DOOR, "PreviousVariant", Lua_GridEntityDoor_GetPreviousVarinat, Lua_GridEntityDoor_SetPreviousVariant);
}