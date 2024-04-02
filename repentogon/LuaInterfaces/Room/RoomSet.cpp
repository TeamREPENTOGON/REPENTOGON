#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomSetGetName)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomSetMT);
	lua_pushstring(L, set->_filepath.c_str());

	return 1;
}

LUA_FUNCTION(Lua_RoomSetSetName)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomSetMT);
	set->_filepath = lua_tostring(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_RoomSetGetRooms)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomSetMT);
	lua_newtable(L);
	for (size_t i = 0; i < set->_count; ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, &set->_configs[i], lua::GetMetatableKey(lua::Metatables::CONST_ROOM_CONFIG_ROOM));
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomSetIsLoaded)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomSetMT);
	lua_pushboolean(L, set->_loaded);

	return 1;
}

static void RegisterRoomSet(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetXMLName", Lua_RoomSetGetName },
		{ "SetXMLName", Lua_RoomSetSetName },
		{ "GetRooms", Lua_RoomSetGetRooms },
		{ "IsLoaded", Lua_RoomSetIsLoaded },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::RoomSetMT, lua::metatables::RoomSetMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomSet(_state);
}