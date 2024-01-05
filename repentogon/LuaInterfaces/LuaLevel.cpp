#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_LevelCanSpawnDoorOutline) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	int roomIDX = (int)luaL_checkinteger(L, 2);
	unsigned int doorSlot = (unsigned int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, level->CanSpawnDoorOutline(roomIDX, doorSlot));
	return 1;
}

LUA_FUNCTION(Lua_LevelHasAbandonedMineshaft)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasAbandonedMineshaft());

	return 1;
}

LUA_FUNCTION(Lua_LevelHasMirrorDimension)
{
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushboolean(L, level->HasMirrorDimension());

	return 1;
}

LUA_FUNCTION(Lua_LevelHasPhotoDoor)
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

LUA_FUNCTION(lua_LevelIsStageAvailable) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	int levelStage = (int)luaL_checkinteger(L, 2);
	int stageType = (int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, level->IsStageAvailable(levelStage, stageType));
	return 1;
}

LUA_FUNCTION(Lua_GetDimension) {
	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->GetDimension());
	return 1;
}

LUA_FUNCTION(Lua_GetForcedStage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->REPENTOGON_ForcedStage);
	return 1;
}

LUA_FUNCTION(Lua_SetForcedStage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	level->REPENTOGON_ForcedStage = (byte)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetForcedStageType) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->REPENTOGON_ForcedStageType);
	return 1;
}

LUA_FUNCTION(Lua_SetForcedStageType) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	level->REPENTOGON_ForcedStageType = (byte)luaL_checkinteger(L, 2);
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

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "CanSpawnDoorOutline", Lua_LevelCanSpawnDoorOutline },
		{ "HasAbandonedMineshaft", Lua_LevelHasAbandonedMineshaft },
		{ "HasMirrorDimension", Lua_LevelHasMirrorDimension },
		{ "HasPhotoDoor", Lua_LevelHasPhotoDoor },
		{ "SetName", lua_LevelSetName },
		{ "IsStageAvailable", lua_LevelIsStageAvailable },
		{ "GetDimension", Lua_GetDimension},
		{ "GetForcedStage", Lua_GetForcedStage},
		{ "SetForcedStage", Lua_SetForcedStage},
		{ "GetForcedStageType", Lua_GetForcedStageType},
		{ "SetForcedStageType", Lua_SetForcedStageType},
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::LEVEL, functions);
}