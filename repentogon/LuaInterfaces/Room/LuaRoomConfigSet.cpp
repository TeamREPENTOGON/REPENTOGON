#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/VirtualRoomSets.h"
#include "LuaRoomConfigSet.h"

LuaRoomConfigSet::Userdata::Userdata(RoomConfig_Stage& stage, int mode)
	: vanillaSet(&stage._rooms[mode]),
	  virtualSet(VirtualRoomSetManager::GetVanillaSet(stage._id, mode))
{}

LUA_FUNCTION(Lua_RoomConfigSetGetRoom)
{
	LuaRoomConfigSet::Userdata* ud = LuaRoomConfigSet::GetUserdata(L, 1);
	int idx = (int)lua_tointeger(L, 2);

	if (idx < 0)
	{
		lua_pushnil(L);
		return 1;
	}

	if (ud->IsVanilla())
	{
		size_t vanillaCount = ud->vanillaSet->_count;
		if ((size_t)idx < vanillaCount)
		{
			lua::luabridge::UserdataPtr::push(L, &ud->vanillaSet->_configs[idx], lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
			return 1;
		}

		idx -= vanillaCount;
	}
	
	if ((size_t)idx < ud->virtualSet.size())
	{
		lua::luabridge::UserdataPtr::push(L, ud->virtualSet[idx], lua::GetMetatableKey(lua::Metatables::ROOM_CONFIG_ROOM));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSetGetSize)
{
	LuaRoomConfigSet::Userdata* ud = LuaRoomConfigSet::GetUserdata(L, 1);
	size_t vanillaSize = ud->IsVanilla() ? ud->vanillaSet->_count : 0;
	lua_pushinteger(L, ud->virtualSet.size() + vanillaSize);
	return 1;
}

LUA_FUNCTION(Lua_RoomConfigSetAddRooms)
{
	LuaRoomConfigSet::Userdata* ud = LuaRoomConfigSet::GetUserdata(L, 1);
	if (!lua_istable(L, 2))
	{
		return luaL_argerror(L, 2, REPENTOGON::Lua::GenerateInvalidTypeMessage(L, 2, "table").c_str());
	}

	int returnParameters = VirtualRoomSetManager::detail::Lua_AddLuaRooms(L, ud->virtualSet, 2);
	return returnParameters;
}

LUA_FUNCTION(Lua_RoomConfigSetLoadStb) {
	LuaRoomConfigSet::Userdata* ud = LuaRoomConfigSet::GetUserdata(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	int returnParameters = VirtualRoomSetManager::detail::Lua_AddStbRooms(L, ud->virtualSet, filename);
	return returnParameters;
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
		{ "LoadStb", Lua_RoomConfigSetLoadStb },
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