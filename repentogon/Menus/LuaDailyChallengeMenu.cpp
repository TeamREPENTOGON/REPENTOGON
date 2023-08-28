#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* DailyChallengeMenuMT = "DailyChallengeMenu";

static int Lua_DailyChallengeMenu_GetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	ANM2* anm2 = &menu->_DailyRunSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_DailyChallengeMenu_GetLeaderboardSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	ANM2* anm2 = &menu->_leaderboard._leaderboardMenuSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_DailyChallengeMenu_GetLeaderboardScoreMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	ANM2* anm2 = &menu->_leaderboard._scoreMenuSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_DailyChallengeMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static int Lua_DailyChallengeMenu_SetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static int Lua_DailyChallengeMenu_IsLeaderboardVisible(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	lua_pushboolean(L, menu->_leaderboard._displayState > 0);

	return 1;
}

static int Lua_DailyChallengeMenu_GetTimeLeftHours(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	lua_pushinteger(L, menu->_timeHoursLeft);

	return 1;
}

static int Lua_DailyChallengeMenu_GetTimeLeftMinutes(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	lua_pushinteger(L, menu->_timeMinutesLeft);

	return 1;
}

static int Lua_DailyChallengeMenu_GetTimeLeftSeconds(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "DailyChallengeMenu functions can only be used in the main menu"); }
	Menu_DailyChallenge* menu = g_MenuManager->GetMenuDailyChallenge();
	lua_pushinteger(L, menu->_timeSecondsLeft);

	return 1;
}

static void RegisterDailyChallengeMenuGame(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_DailyChallengeMenu_GetSprite);
	lua::TableAssoc(L, "GetLeaderboardSprite", Lua_DailyChallengeMenu_GetLeaderboardSprite);
	lua::TableAssoc(L, "GetLeaderboardScoreMenuSprite", Lua_DailyChallengeMenu_GetLeaderboardScoreMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_DailyChallengeMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_DailyChallengeMenu_SetSelectedElement);
	lua::TableAssoc(L, "IsLeaderboardVisible", Lua_DailyChallengeMenu_IsLeaderboardVisible);
	lua::TableAssoc(L, "GetTimeLeftHours", Lua_DailyChallengeMenu_GetTimeLeftHours);
	lua::TableAssoc(L, "GetTimeLeftMinutes", Lua_DailyChallengeMenu_GetTimeLeftMinutes);
	lua::TableAssoc(L, "GetTimeLeftSeconds", Lua_DailyChallengeMenu_GetTimeLeftSeconds);
	lua_setglobal(L, "DailyChallengeMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterDailyChallengeMenuGame(state);
}