#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_LevelCanSpawnDoorOutline(lua_State* L) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	int roomIDX = luaL_checkinteger(L, 2);
	unsigned int doorSlot = luaL_checkinteger(L, 3);
	lua_pushboolean(L, level->CanSpawnDoorOutline(roomIDX, doorSlot));
	return 1;
}

int Lua_LevelHasAbandonedMineshaft(lua_State* L)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasAbandonedMineshaft());

	return 1;
}

int Lua_LevelHasMirrorDimension(lua_State* L)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasMirrorDimension());

	return 1;
}

int Lua_LevelHasPhotoDoor(lua_State* L)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasPhotoDoor());

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::LEVEL;

	lua::RegisterFunction(state, mt, "CanSpawnDoorOutline", Lua_LevelCanSpawnDoorOutline);
	lua::RegisterFunction(state, mt, "HasAbandonedMineshaft", Lua_LevelHasAbandonedMineshaft);
	lua::RegisterFunction(state, mt, "HasMirrorDimension", Lua_LevelHasMirrorDimension);
	lua::RegisterFunction(state, mt, "HasPhotoDoor", Lua_LevelHasPhotoDoor);
}