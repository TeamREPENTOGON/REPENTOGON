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

static std::string CustomStageName;

LUA_FUNCTION(lua_LevelSetName) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	const char* name = luaL_checkstring(L, 2);
	CustomStageName = name;
	return 0;
}

/* HOOK_METHOD(Level, GetName, () -> std::string) {
	std::string name = super();
	if (!CustomStageName.empty()) {
		name = CustomStageName;
	}

	return name;
} */

HOOK_GLOBAL(GetLevelName, (std_string* result, uint32_t levelStage, uint32_t stageType, uint32_t curseMask, uint32_t unk1, uint32_t unk2, bool unk3) -> void, __cdecl) {
	super(result, levelStage, stageType, curseMask, unk1, unk2, unk3);
	if (!CustomStageName.empty()) {
		*result = CustomStageName;
	}
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
	lua::RegisterFunction(state, mt, "SetName", lua_LevelSetName);
}