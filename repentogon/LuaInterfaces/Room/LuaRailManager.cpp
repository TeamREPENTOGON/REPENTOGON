#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomGetRailManager)
{
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	RailManager** ud = (RailManager**)lua_newuserdata(L, sizeof(RailManager*));
	*ud = &room->_railManager;
	luaL_setmetatable(L, lua::metatables::RailManagerMT);
	return 1;
}

LUA_FUNCTION(Lua_RailManagerGetRailsSprite)
{
	RailManager* railManager = *lua::GetRawUserdata<RailManager**>(L, 1, lua::metatables::RailManagerMT);
	ANM2* anm2 = &railManager->_sprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static void RegisterRailManager(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetRailManager", Lua_RoomGetRailManager);

	luaL_Reg functions[] = {
		{ "GetRailsSprite", Lua_RailManagerGetRailsSprite },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::RailManagerMT, lua::metatables::RailManagerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRailManager(_state);
}