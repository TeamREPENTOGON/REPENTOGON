#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_RoomConfigSetGetRoom)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	int idx = (int)lua_tointeger(L, 2);
	if (idx >= 0 && idx < (int)set->_count) {
		lua::luabridge::UserdataPtr::push(L, &set->_configs[idx], lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSetGetSize)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	lua_pushinteger(L, set->_count);

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSet__gc)
{
	RoomSet* set = *lua::GetUserdata<RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	set->game_destructor();

	return 0;
}

static void RegisterRoomConfigSet(lua_State* L) {
	luaL_newmetatable(L, lua::metatables::RoomConfigSetMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__name");
	lua_pushstring(L, "RoomConfigSet");
	lua_rawset(L, -3);

	lua_pushstring(L, "__propget");
	lua_newtable(L);

	lua_pushstring(L, "Size");
	lua_pushcfunction(L, Lua_RoomConfigSetGetSize);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	luaL_Reg functions[] = {
		{ "Get", Lua_RoomConfigSetGetRoom },
		{ "__len", Lua_RoomConfigSetGetSize },
		{ "__gc", Lua_RoomConfigSet__gc },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterRoomConfigSet(_state);
}