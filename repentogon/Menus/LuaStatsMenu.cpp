#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* StatsMenuMT = "StatsMenu";


static int Lua_StatsMenu_GetStatsMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_StatsMenuSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementsSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite1(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite1;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite2(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite2;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite3(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite3;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite4(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite4;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite5(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite5;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite6(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite6;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite7(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite7;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite8(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite8;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuMiniSprite9(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_achievementMiniSprite9;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuCursorLeftSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_cursorLeftSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_GetSecretsMenuCursorRightSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	ANM2* anm2 = &menuStats->_cursorRightSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_StatsMenu_IsSecretsMenuVisible(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "StatsMenu functions can only be used in the main menu"); }
	Menu_Stats* menuStats = g_MenuManager->GetMenuStats();
	lua_pushboolean(L, menuStats->_isAchievementScreenVisible == 1);

	return 1;
}

static void RegisterStatsMenuGame(lua_State* L)
{	
	lua_newtable(L);
	lua::TableAssoc(L, "GetStatsMenuSprite", Lua_StatsMenu_GetStatsMenuSprite);
	lua::TableAssoc(L, "GetSecretsMenuSprite", Lua_StatsMenu_GetSecretsMenuSprite);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite1", Lua_StatsMenu_GetSecretsMenuMiniSprite1);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite2", Lua_StatsMenu_GetSecretsMenuMiniSprite2);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite3", Lua_StatsMenu_GetSecretsMenuMiniSprite3);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite4", Lua_StatsMenu_GetSecretsMenuMiniSprite4);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite5", Lua_StatsMenu_GetSecretsMenuMiniSprite5);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite6", Lua_StatsMenu_GetSecretsMenuMiniSprite6);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite7", Lua_StatsMenu_GetSecretsMenuMiniSprite7);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite8", Lua_StatsMenu_GetSecretsMenuMiniSprite8);
	lua::TableAssoc(L, "GetSecretsMenuMiniSprite9", Lua_StatsMenu_GetSecretsMenuMiniSprite9);
	lua::TableAssoc(L, "GetSecretsMenuCursorLeftSprite", Lua_StatsMenu_GetSecretsMenuCursorLeftSprite);
	lua::TableAssoc(L, "GetSecretsMenuCursorRightSprite", Lua_StatsMenu_GetSecretsMenuCursorRightSprite);
	lua::TableAssoc(L, "IsSecretsMenuVisible", Lua_StatsMenu_IsSecretsMenuVisible);
	lua_setglobal(L, "StatsMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterStatsMenuGame(state);
}