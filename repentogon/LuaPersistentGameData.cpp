#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* PersistentGameDataMT = "PersistentGameData";

static const unsigned int PGD_COUNTER_MAX = 495;
static const unsigned int COLLECITBLE_MAX = 732;

static int Lua_GetPersistentGameData(lua_State* L) {
	Manager* manager = g_Manager;
	PersistentGameData** ud = (PersistentGameData**)lua_newuserdata(L, sizeof(PersistentGameData*));
	*ud = manager->GetPersistentGameData();
	luaL_setmetatable(L, PersistentGameDataMT);
	return 1;
}

int Lua_PGDTryUnlock(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int unlock = (int)luaL_checkinteger(L, 2);

	bool success = pgd->TryUnlock(unlock);
	lua_pushboolean(L, success);
	return 1;
}

int Lua_PGDUnlocked(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int unlock = (int)luaL_checkinteger(L, 2);

	bool unlocked = pgd->Unlocked(unlock);
	lua_pushboolean(L, unlocked);
	return 1;
}

int Lua_PGDIncreaseEventCounter(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int eventCounter = (int)luaL_checkinteger(L, 2);

	if (eventCounter > PGD_COUNTER_MAX)
		luaL_error(L, "bad argument #2 to 'IncreaseEventCounter' (EventCounter cannot be higher than %d)", PGD_COUNTER_MAX);

	int num = (int)luaL_checkinteger(L, 3);

	pgd->IncreaseEventCounter(eventCounter, num);

	return 0;
}

int Lua_PGDGetEventCounter(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int eventCounter = (int)luaL_checkinteger(L, 2);

	if (eventCounter > PGD_COUNTER_MAX)
		luaL_error(L, "bad argument #2 to 'GetEventCounter' (EventCounter cannot be higher than %d)", PGD_COUNTER_MAX);

	lua_pushinteger(L, pgd->GetEventCounter(eventCounter));


	return 1;
}

int Lua_PGDIsItemInCollection(lua_State* L) {
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int collectID = (int)luaL_checkinteger(L, 2);

	if (collectID > COLLECITBLE_MAX)
		luaL_error(L, "bad argument #2 to 'IsItemInCollection' (CollectibleType cannot be higher than %d)", COLLECITBLE_MAX);

	lua_pushboolean(L, pgd->IsItemInCollection(collectID));

	return 1;
}

int Lua_PGDGetBestiaryKillCount(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int entType = (int)luaL_checkinteger(L, 2);
	int entVariant = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pgd->GetBestiaryKillCount(entType, entVariant));
	return 1;
}

int Lua_PGDGetBestiaryDeathCount(lua_State* L)
{
	PersistentGameData* pgd = *lua::GetUserdata<PersistentGameData**>(L, 1, PersistentGameDataMT);
	int entType = (int)luaL_checkinteger(L, 2);
	int entVariant = (int)luaL_checkinteger(L, 3);
	lua_pushinteger(L, pgd->GetBestiaryDeathCount(entType, entVariant));
	return 1;
}

static void RegisterPersistentGameData(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetPersistentGameData");
	lua_pushcfunction(L, Lua_GetPersistentGameData);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PersistentGameDataMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "TryUnlock", Lua_PGDTryUnlock },
		{ "Unlocked", Lua_PGDUnlocked },
		{ "IncreaseEventCounter", Lua_PGDIncreaseEventCounter},
		{ "GetEventCounter", Lua_PGDGetEventCounter},
		{ "IsItemInCollection", Lua_PGDIsItemInCollection},
		{ "GetBestiaryKillCount", Lua_PGDGetBestiaryKillCount},
		{ "GetBestiaryDeathCount", Lua_PGDGetBestiaryDeathCount},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterPersistentGameData(state);
}

