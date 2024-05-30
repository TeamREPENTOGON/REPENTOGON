#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "Level.h"

LevelASM levelASM;

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

LUA_FUNCTION(lua_LevelSetGreedWavesClearedWithoutRedHeartDamage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	level->_greedwavesclearedwithoutredheartdamage = luaL_checkinteger(L, 2);
	return 0;
}
LUA_FUNCTION(lua_LevelGetGreedWavesClearedWithoutRedHeartDamage) {
	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->_greedwavesclearedwithoutredheartdamage);
	return 1;
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

LUA_FUNCTION(Lua_GetForceSpecialQuest) {
	lua_pushinteger(L, (int)levelASM.ForceSpecialQuest);
	return 1;
}

LUA_FUNCTION(Lua_SetForceSpecialQuest) {
	levelASM.ForceSpecialQuest = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_LevelIsAltPath) {
	bool ret = false;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
	}
	lua_pushboolean(L, ret);
	return 1;
}

LUA_FUNCTION(Lua_GetStageId) {
	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
	lua_pushinteger(L, level->GetStageID(false));
	return 1;
}

/*
HOOK_METHOD(Level, IsAltPath, () -> bool) {
	bool ret = false;
	// If ForceSpecialQuest is -1, quest doors are disabled
	if (levelASM.ForceSpecialQuest > -1) {
		// ForceSpecialQuest 0 defaults to vanilla behavior
		ret = levelASM.ForceSpecialQuest > 0 || (g_Game->_stageType == 4 || g_Game->_stageType == 5);
	}
	return ret;
}
*/

bool CheckQuest(Level* level, const int levelStage, const int expected, const int quest) {
	unsigned int stage;
	if (g_Game->_difficulty < 2 && levelASM.ForceSpecialQuest > -1) {
		if (stage = g_Game->_stage, 5 < stage - 1 || (g_Game->_stateFlags & 0x10000) == 0 && level->IsAltPath()) {
			return (levelASM.ForceSpecialQuest == quest || levelStage == expected || (levelStage == expected - 1 && (g_Game->_curses & 2) != 0));
		}
	}
	return false;
}

HOOK_METHOD(Level, HasMirrorDimension, () -> bool) {
	return CheckQuest(this, g_Game->_stage, 2, 1);
}

HOOK_METHOD(Level, HasAbandonedMineshaft, () -> bool) {
	return CheckQuest(this, g_Game->_stage, 4, 2);
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
		{ "GetForceSpecialQuest", Lua_GetForceSpecialQuest },
		{ "SetForceSpecialQuest", Lua_SetForceSpecialQuest },
		{ "GetStageId", Lua_GetStageId },

		{ "SetGreedWavesClearedWithoutRedHeartDamage", lua_LevelSetGreedWavesClearedWithoutRedHeartDamage },
		{ "GetGreedWavesClearedWithoutRedHeartDamage", lua_LevelGetGreedWavesClearedWithoutRedHeartDamage },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::LEVEL, functions);
}