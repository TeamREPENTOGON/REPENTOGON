#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/XMLData.h"
#include "../Patches/ChallengesStuff.h"
#include "../Patches/AchievementsStuff.h"

static const unsigned int PGD_COUNTER_MAX = 495;
static const unsigned int COLLECTIBLE_MAX = 732;
static const unsigned int CHALLENGE_MAX = 45;

LUA_FUNCTION(Lua_GetPersistentGameData) {
	Manager* manager = g_Manager;
	PersistentGameData** ud = (PersistentGameData**)lua_newuserdata(L, sizeof(PersistentGameData*));
	*ud = manager->GetPersistentGameData();
	luaL_setmetatable(L, lua::metatables::PersistentGameDataMT);
	return 1;
}

LUA_FUNCTION(Lua_PGDTryUnlock)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int unlock = (int)luaL_checkinteger(L, 2);
	if (lua_isboolean(L, 3) && lua_toboolean(L, 3)) {
		nextSkipAchiev = unlock;
	}


	bool success = pgd->TryUnlock(unlock);
	if (!success) {
		// It failed, so reset state manually
		nextSkipAchiev = -1;
;	}
	lua_pushboolean(L, success);
	return 1;
}

LUA_FUNCTION(Lua_PGDUnlocked)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int unlock = (int)luaL_checkinteger(L, 2);

	bool unlocked = pgd->Unlocked(unlock);
	lua_pushboolean(L, unlocked);
	return 1;
}

LUA_FUNCTION(Lua_PGDIncreaseEventCounter)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int eventCounter = (int)luaL_checkinteger(L, 2);

	if (eventCounter > PGD_COUNTER_MAX)
		luaL_error(L, "bad argument #2 to 'IncreaseEventCounter' (EventCounter cannot be higher than %d)", PGD_COUNTER_MAX);

	int num = (int)luaL_checkinteger(L, 3);

	pgd->IncreaseEventCounter(eventCounter, num);

	return 0;
}

LUA_FUNCTION(Lua_PGDGetEventCounter)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int eventCounter = (int)luaL_checkinteger(L, 2);

	if (eventCounter > PGD_COUNTER_MAX)
		luaL_error(L, "bad argument #2 to 'GetEventCounter' (EventCounter cannot be higher than %d)", PGD_COUNTER_MAX);

	lua_pushinteger(L, pgd->GetEventCounter(eventCounter));


	return 1;
}

LUA_FUNCTION(Lua_PGDIsItemInCollection) {
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int collectID = (int)luaL_checkinteger(L, 2);

	if (collectID > COLLECTIBLE_MAX)
		luaL_error(L, "bad argument #2 to 'IsItemInCollection' (CollectibleType cannot be higher than %d)", COLLECTIBLE_MAX);

	lua_pushboolean(L, pgd->IsItemInCollection(collectID));

	return 1;
}

LUA_FUNCTION(Lua_PGDIsChallengeCompleted) {
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int challengeID = (int)luaL_checkinteger(L, 2);

	if (challengeID <= CHALLENGE_MAX) {
		lua_pushboolean(L, g_Manager->GetPersistentGameData()->challenges[challengeID]);
	}
	else {
		XMLAttributes node = XMLStuff.ChallengeData->GetNodeById(challengeID);
		lua_pushboolean(L, Challenges[node["name"] + node["sourceid"]] > 0);
	}

	return 1;
}

LUA_FUNCTION(Lua_PGDGetBestiaryKillCount)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int entType = (int)luaL_checkinteger(L, 2);
	int entVariant = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pgd->GetBestiaryKillCount(entType, entVariant));
	return 1;
}

LUA_FUNCTION(Lua_PGDGetBestiaryDeathCount)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int entType = (int)luaL_checkinteger(L, 2);
	int entVariant = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pgd->GetBestiaryDeathCount(entType, entVariant));
	return 1;
}

LUA_FUNCTION(Lua_PGDGetBestiaryEncounterCount)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, lua::metatables::PersistentGameDataMT);
	int entType = (int)luaL_checkinteger(L, 2);
	int entVariant = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pgd->GetBestiaryEncounterCount(entType, entVariant));
	return 1;
}

static void RegisterPersistentGameData(lua_State* L)
{
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "GetPersistentGameData", Lua_GetPersistentGameData);

	luaL_Reg functions[] = {
		{ "TryUnlock", Lua_PGDTryUnlock },
		{ "Unlocked", Lua_PGDUnlocked },
		{ "IncreaseEventCounter", Lua_PGDIncreaseEventCounter},
		{ "GetEventCounter", Lua_PGDGetEventCounter},
		{ "IsItemInCollection", Lua_PGDIsItemInCollection},
		{ "IsChallengeCompleted", Lua_PGDIsChallengeCompleted},
		{ "GetBestiaryKillCount", Lua_PGDGetBestiaryKillCount},
		{ "GetBestiaryDeathCount", Lua_PGDGetBestiaryDeathCount},
		{ "GetBestiaryEncounterCount", Lua_PGDGetBestiaryEncounterCount},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::PersistentGameDataMT, lua::metatables::PersistentGameDataMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPersistentGameData(_state);
}

