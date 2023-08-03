#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* StatsMenuGameMT = "StatsMenuGame";

static int Lua_GetStatsMenuGame(lua_State* L) {
	MenuManager* menuManager = g_MenuManager;
	Menu_Stats** ud = (Menu_Stats**)lua_newuserdata(L, sizeof(Menu_Stats*));
	*ud = menuManager->GetMenuStats();
	luaL_setmetatable(L, StatsMenuGameMT);
	return 1;
}

static int Lua_StatsMenu_GetStatsMenuSprite(lua_State* L)
{
	Menu_Stats* menuStats = *lua::GetUserdata<Menu_Stats**>(L, 1, StatsMenuGameMT);
	ANM2* anm2 = menuStats->GetStatsMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetAchievementsSprite(lua_State* L)
{
	Menu_Stats* menuStats = *lua::GetUserdata<Menu_Stats**>(L, 1, StatsMenuGameMT);
	ANM2* anm2 = menuStats->GetAchievementsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}


static int Lua_StatsMenu_GetSecretsMenuComponentsSprite(lua_State* L)
{
	Menu_Stats* menuStats = *lua::GetUserdata<Menu_Stats**>(L, 1, StatsMenuGameMT);
	ANM2* anm2 = menuStats->GetSecretsMenuComponentsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetPosition(lua_State* L)
{
	Menu_Stats* menuStats = *lua::GetUserdata<Menu_Stats**>(L, 1, StatsMenuGameMT);
	lua::LuaCaller(L).pushUserdataValue(*new Vector(menuStats->GetPosX(), menuStats->GetPosY()), lua::Metatables::VECTOR);

	return 1;
}

static int Lua_StatsMenu_IsAchievementsScreenVisible(lua_State* L)
{
	Menu_Stats* menuStats = *lua::GetUserdata<Menu_Stats**>(L, 1, StatsMenuGameMT);
	lua_pushboolean(L, menuStats->IsAchievementsScreenVisible());

	return 1;
}


static void RegisterStatsMenuGame(lua_State* L)
{
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetStatsMenu");
	lua_pushcfunction(L, Lua_GetStatsMenuGame);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, StatsMenuGameMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetStatsMenuSprite", Lua_StatsMenu_GetStatsMenuSprite},
		{ "GetAchievementsSprite", Lua_StatsMenu_GetAchievementsSprite},
		{ "GetSecretsMenuComponentsSprite", Lua_StatsMenu_GetSecretsMenuComponentsSprite},
		{ "GetPosition", Lua_StatsMenu_GetPosition},
		{ "IsAchievementsScreenVisible", Lua_StatsMenu_IsAchievementsScreenVisible},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterStatsMenuGame(state);
}