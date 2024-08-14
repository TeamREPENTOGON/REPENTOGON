#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ChallengeMenu_GetChallengeMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ChallengeMenuMT);
	Menu_Challenge* menu = g_MenuManager->GetMenuChallenge();
	ANM2* anm2 = menu->GetChallengeMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_ChallengeMenu_GetSelectedChallengeID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ChallengeMenuMT);
	Menu_Challenge* menu = g_MenuManager->GetMenuChallenge();
	lua_pushinteger(L, menu->SelectedChallengeID);

	return 1;
}

LUA_FUNCTION(Lua_ChallengeMenu_SetSelectedChallengeID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ChallengeMenuMT);
	Menu_Challenge* menu = g_MenuManager->GetMenuChallenge();
	menu->SelectedChallengeID = (int)luaL_checkinteger(L, 1);

	return 0;
}

static void RegisterChallengeMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_ChallengeMenu_GetChallengeMenuSprite);
	lua::TableAssoc(L, "GetSelectedChallengeID", Lua_ChallengeMenu_GetSelectedChallengeID);
	lua::TableAssoc(L, "SetSelectedChallengeID", Lua_ChallengeMenu_SetSelectedChallengeID);
	lua_setglobal(L, lua::metatables::ChallengeMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterChallengeMenuGame(state);
}