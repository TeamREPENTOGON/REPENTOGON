#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/VirtualRoomSets.h"

LUA_FUNCTION(Lua_RoomConfigSetGetRoom)
{
	VirtualRoomSetManager::RoomSet* set = *lua::GetRawUserdata<VirtualRoomSetManager::RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	int idx = (int)lua_tointeger(L, 2);
	if (idx >= 0 && (uint32_t)idx < set->absolute_size()) {
		lua::luabridge::UserdataPtr::push(L, &set->at_absolute_index(idx), lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSetGetSize)
{
	VirtualRoomSetManager::RoomSet* set = *lua::GetRawUserdata<VirtualRoomSetManager::RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);
	lua_pushinteger(L, set->absolute_size());

	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSetAddRooms)
{
	VirtualRoomSetManager::RoomSet* set = *lua::GetRawUserdata<VirtualRoomSetManager::RoomSet**>(L, 1, lua::metatables::RoomConfigSetMT);

	if (!lua_istable(L, 2))
	{
		return luaL_argerror(L, 2, REPENTOGON::Lua::GenerateInvalidTypeMessage(L, 2, "table").c_str());
	}

	VirtualRoomSetManager::__AddLuaRooms(L, set->GetStageId(), set->GetMode(), 2);
	return 1;
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
		{ "AddRooms", Lua_RoomConfigSetAddRooms },
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