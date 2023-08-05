#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* StatsMenuMT = "StatsMenu";


static int Lua_StatsMenu_GetStatsMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = menuStats->GetStatsMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetAchievementsSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = menuStats->GetAchievementsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuComponentsSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = menuStats->GetSecretsMenuComponentsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_IsAchievementsScreenVisible(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	lua_pushboolean(L, menuStats->IsAchievementsScreenVisible());

	return 1;
}


static void RegisterStatsMenuGame(lua_State* L)
{	
	lua_newtable(L);
	lua::TableAssoc(L, "GetStatsMenuSprite", Lua_StatsMenu_GetStatsMenuSprite);
	lua::TableAssoc(L, "GetAchievementsSprite", Lua_StatsMenu_GetAchievementsSprite);
	lua::TableAssoc(L, "GetSecretsMenuComponentsSprite", Lua_StatsMenu_GetSecretsMenuComponentsSprite);
	lua::TableAssoc(L, "IsAchievementsScreenVisible", Lua_StatsMenu_IsAchievementsScreenVisible);
	lua_setglobal(L, "StatsMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterStatsMenuGame(state);
}