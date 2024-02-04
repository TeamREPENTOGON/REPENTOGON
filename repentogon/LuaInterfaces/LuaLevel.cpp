#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "Level.h"

LevelASM levelASM;

extern "C" {
	void L_Level_AddAngelRoomChance(float chance) {
		g_Game->_level.AddAngelRoomChance(chance);
	}

	void L_Level_AddCurse(int curse, bool showName) {
		g_Game->_level.AddCurse(curse, showName);
	}

	void L_Level_ApplyBlueMapEffect() {
		g_Game->_level.ApplyBlueMapEffect();
	}

	void L_Level_ApplyCompassEffect(bool persistent) {
		g_Game->_level.ApplyCompassEffect(persistent);
	}

	void L_Level_ApplyMapEffect() {
		g_Game->_level.ApplyMapEffect();
	}

	bool L_Level_CanOpenChallengeRoom(int roomIndex) {
		return g_Game->_level.CanOpenChallengeRoom(roomIndex);
	}

	bool L_Level_CanSpawnDevilRoom() {
		return g_Game->_level.CanSpawnDevilRoom();
	}

	bool L_Level_CanStageHaveCurseOfLabyrinth(LevelStage stage) {
		return g_Game->_level.CanStageHaveCurseOfLabyrinth(stage);
	}

	void L_Level_ChangeRoom(int roomIndex, int dimension) {
		g_Game->_level.ChangeRoom(roomIndex, dimension);
	}

	void L_Level_DisableDevilRoom() {
		g_Game->_level._devilRoomDisabled = true;
	}

	bool L_Level_ForceHorsemanBoss(unsigned int seed) {
		return g_Game->_level.ForceHorsemanBoss(seed);
	}

	// LevelStage
	int L_Level_GetAbsoluteStage() {
		return g_Game->_level.GetAbsoluteStage();
	}

	float L_Level_GetAngelRoomChance() {
		return g_Game->_level.GetAngelRoomChance();
	}

	bool L_Level_GetCanSeeEverything() {
		return g_Game->_level._canSeeEverything;
	}

	// Room
	Room* L_Level_GetCurrentRoom() {
		return g_Game->_level._room;
	}

	// RoomDescriptor
	RoomDescriptor* L_Level_GetCurrentRoomDesc() {
		return g_Game->GetRoomByIdx(g_Game->_level._currentRoomIdx, g_Game->_level._currentDimensionIdx);
	}

	int L_Level_GetCurrentRoomIndex() {
		return g_Game->_level._currentRoomIdx;
	}

	const char* L_Level_GetCurseName() {
		// leaky!
		return strdup(g_Game->_level.GetCurseName().c_str());
	}

	int L_Level_GetCurses() {
		return g_Game->_level.GetCurses();
	}

	RNG* L_Level_GetDevilAngelRoomRNG() {
		return &g_Game->_level._devilAngelRoomRNG;
	}

	int L_Level_GetDungeonPlacementSeed() {
		return g_Game->_level._dungeonPlacementSeed;
	}

	Vector* L_Level_GetEnterPosition() {
		return g_Game->_level.GetEnterPosition();
	}

	bool L_Level_GetHeartPicked() {
		return g_Game->_level._heartPicked;
	}
	int L_Level_GetLastBossRoomListIndex() {
		return g_Game->_level._lastBossRoomListIdx;
	}

	// RoomDescriptor
	RoomDescriptor* L_Level_GetLastRoomDesc() {
		return g_Game->GetRoomByIdx(g_Game->_level._lastRoomIdx, g_Game->_level._lastRoomDimensionIdx);
	}

	const char* L_Level_GetName() {
		// leaky!
		return strdup(g_Game->_level.GetName().c_str());
	}

	int L_Level_GetNonCompleteRoomIndex() {
		return g_Game->_level.GetNonCompleteRoomIndex();
	}

	float L_Level_GetPlanetariumChance() {
		return g_Game->GetPlanetariumChance();
	}

	int L_Level_GetPreviousRoomIndex() {
		return g_Game->_level._lastRoomIdx;
	}

	int L_Level_GetRandomRoomIndex(bool iAmErrorRoom, unsigned int seed) {
		return g_Game->_level.GetRandomRoomIndex(iAmErrorRoom, seed);
	}

	// RoomDescriptor
	RoomDescriptor* L_Level_GetRoomByIdx(int idx, int dimension) {
		return g_Game->GetRoomByIdx(idx, dimension);
	}

	int L_Level_GetRoomCount() {
		return g_Game->_level._nbRooms;
	}

	// RoomDescriptor[]: { u32 roomCount : 0x0; RoomDescriptor* rooms : 0x4; }
	RoomDescriptor_List* L_Level_GetRooms() {
		return new auto(g_Game->_level.GetRooms());
	}

	// LevelStage
	int L_Level_GetStage() {
		return g_Game->_level._stage;
	}

	// StageType
	int L_Level_GetStageType() {
		return g_Game->_level._stageType;
	}

	int L_Level_GetStartingRoomIndex() {
		return g_Game->_level._startingRoomIdx;
	}

	// LevelStateFlag
	bool L_Level_GetStateFlag(int levelStateFlag) {
		return g_Game->_level.GetStateFlag(levelStateFlag);
	}

	bool L_Level_HasBossChallenge() {
		return g_Game->_level._bossChallenge;
	}

	void L_Level_InitializeDevilAngelRoom(bool forceAngel, bool forceDevil) {
		g_Game->_level.InitializeDevilAngelRoom(forceAngel, forceDevil);
	}

	bool L_Level_IsAltStage() {
		if (g_Game->_level._stageType == 4 || g_Game->_level._stageType == 5) return true;

		return false;
	}

	bool L_Level_IsAscent() {
		return g_Game->_level.IsAscent();
	}

	bool L_Level_IsDevilRoomDisabled() {
		return g_Game->_level._devilRoomDisabled;
	}

	bool L_Level_IsNextStageAvailable() {
		return g_Game->_level.IsNextStageAvailable();
	}

	bool L_Level_IsPreAscent() {
		return g_Game->_level.IsPreAscent();
	}

	// arg2: DoorSlot
	bool L_Level_MakeRedRoomDoor(int roomIdx, unsigned int doorSlot) {
		return g_Game->_level.MakeRedRoomDoor(roomIdx, doorSlot);
	}

	// RoomType
	int L_Level_QueryRoomTypeIndex(unsigned int roomType, unsigned int visited, RNG* rng, bool ignoreGroup) {
		return g_Game->_level.QueryRoomTypeIndex(roomType, visited, rng, ignoreGroup);
	}

	void L_Level_RemoveCompassEffect() {
		g_Game->_level.RemoveCompassEffect();
	}

	// LevelCurses
	void L_Level_RemoveCurses(int curses) {
		g_Game->_level.RemoveCurses(curses);
	}

	void L_Level_SetCanSeeEverything(bool canSeeEverything) {
		g_Game->_level._canSeeEverything = canSeeEverything;
	}

	void L_Level_SetHeartPicked() {
		g_Game->_level._heartPicked = true;

	}

	void L_Level_SetNextStage() {
		g_Game->_level.SetNextStage();
	}

	void L_Level_SetRedHeartDamage() {
		g_Game->_level.SetRedHeartDamage();
	}

	void L_Level_SetStage(int stageOffset, int stageTypeOffset) {
		g_Game->_level.SetStage(stageOffset, stageTypeOffset);
	}

	// LevelStateFlag
	void L_Level_SetStateFlag(int levelStateFlag, bool value) {
		g_Game->_level.SetStateFlag(levelStateFlag, value);
	}

	void L_Level_ShowMap() {
		g_Game->_level.ShowMap();
	}

	void L_Level_ShowName(bool sticky) {
		g_Game->_level.ShowName(sticky);
	}

	// arg2: DoorSlot
	void L_Level_UncoverHiddenDoor(int roomIdx, int doorSlot) {
		g_Game->_level.UncoverHiddenDoor(roomIdx, doorSlot);
	}

	void L_Level_Update() {
		g_Game->_level.Update();
	}

	void L_Level_UpdateVisibility() {
		g_Game->_level.UpdateVisibility();
	}

	Vector* L_Level_GetDungeonReturnPosition() {
		return &g_Game->_level._dungeonReturnPos;
	}

	int L_Level_GetDungeonReturnRoomIndex() {
		return g_Game->_level._dungeonRoomIndex;
	}

	int L_Level_GetEnterDoor() {
		return g_Game->_level._enterDoor;
	}

	int L_Level_GetGreedModeWave() {
		return g_Game->_level._greedModeWave;
	}

	int L_Level_GetLeaveDoor() {
		return g_Game->_level._leaveDoor;
	}

	void L_Level_SetDungeonReturnPosition(Vector* pos) {
		g_Game->_level._dungeonReturnPos = *pos;
	}

	void L_Level_SetDungeonReturnRoomIndex(int index) {
		g_Game->_level._dungeonRoomIndex = index;
	}

	void L_Level_SetEnterDoor(int enterDoor) {
		g_Game->_level._enterDoor = enterDoor;
	}

	void L_Level_SetGreedModeWave(int wave) {
		g_Game->_level._greedModeWave = wave;
	}

	void L_Level_SetLeaveDoor(int leaveDoor) {
		g_Game->_level._leaveDoor = leaveDoor;
	}
}

//LUA_FUNCTION(Lua_LevelCanSpawnDoorOutline) {
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	int roomIDX = (int)luaL_checkinteger(L, 2);
//	unsigned int doorSlot = (unsigned int)luaL_checkinteger(L, 3);
//	lua_pushboolean(L, level->CanSpawnDoorOutline(roomIDX, doorSlot));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_LevelHasAbandonedMineshaft)
//{
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	lua_pushboolean(L, level->HasAbandonedMineshaft());
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_LevelHasMirrorDimension)
//{
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	lua_pushboolean(L, level->HasMirrorDimension());
//
//	return 1;
//}
//
//LUA_FUNCTION(Lua_LevelHasPhotoDoor)
//{
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	lua_pushboolean(L, level->HasPhotoDoor());
//
//	return 1;
//}
//
//static std::string CustomStageName;
//
//LUA_FUNCTION(lua_LevelSetName) {
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	const char* name = luaL_checkstring(L, 2);
//	CustomStageName = name;
//	return 0;
//}
//
//LUA_FUNCTION(lua_LevelIsStageAvailable) {
//	Level* level = lua::GetUserdata<Level*>(L, 1, lua::Metatables::LEVEL, "Level");
//	int levelStage = (int)luaL_checkinteger(L, 2);
//	int stageType = (int)luaL_checkinteger(L, 3);
//	lua_pushboolean(L, level->IsStageAvailable(levelStage, stageType));
//	return 1;
//}
//
//LUA_FUNCTION(Lua_GetDimension) {
//	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
//	lua_pushinteger(L, level->GetDimension());
//	return 1;
//}
//
//LUA_FUNCTION(Lua_GetForceSpecialQuest) {
//	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
//	lua_pushinteger(L, (int)levelASM.ForceSpecialQuest);
//	return 1;
//}
//
//LUA_FUNCTION(Lua_SetForceSpecialQuest) {
//	Game* level = lua::GetUserdata<Game*>(L, 1, lua::Metatables::LEVEL, "Level");
//	levelASM.ForceSpecialQuest = (int)luaL_checkinteger(L, 2);
//	return 0;
//}
//
///* HOOK_METHOD(Level, GetName, () -> std::string) {
//	std::string name = super();
//	if (!CustomStageName.empty()) {
//		name = CustomStageName;
//	}
//
//	return name;
//} */
//
//HOOK_GLOBAL(GetLevelName, (std_string* result, uint32_t levelStage, uint32_t stageType, uint32_t curseMask, uint32_t unk1, uint32_t unk2, bool unk3) -> void, __cdecl) {
//	super(result, levelStage, stageType, curseMask, unk1, unk2, unk3);
//	if (!CustomStageName.empty()) {
//		*result = CustomStageName;
//	}
//}
//
//HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
//	super();
//
//	lua::LuaStackProtector protector(_state);
//
//	luaL_Reg functions[] = {
//		{ "CanSpawnDoorOutline", Lua_LevelCanSpawnDoorOutline },
//		{ "HasAbandonedMineshaft", Lua_LevelHasAbandonedMineshaft },
//		{ "HasMirrorDimension", Lua_LevelHasMirrorDimension },
//		{ "HasPhotoDoor", Lua_LevelHasPhotoDoor },
//		{ "SetName", lua_LevelSetName },
//		{ "IsStageAvailable", lua_LevelIsStageAvailable },
//		{ "GetDimension", Lua_GetDimension},
//		{ "GetForceSpecialQuest", Lua_GetForceSpecialQuest },
//		{ "SetForceSpecialQuest", Lua_SetForceSpecialQuest },
//		{ NULL, NULL }
//	};
//
//	lua::RegisterFunctions(_state, lua::Metatables::LEVEL, functions);
//}