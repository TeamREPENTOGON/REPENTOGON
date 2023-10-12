#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_MainMenu_GetGameMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MainMenuMT);
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetGameMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_GetContinueWidgetSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MainMenuMT);
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetContinueWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MainMenuMT);
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	lua_pushinteger(L, menuGame->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MainMenuMT);
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	menuGame->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterMainMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetGameMenuSprite", Lua_MainMenu_GetGameMenuSprite);
	lua::TableAssoc(L, "GetContinueWidgetSprite", Lua_MainMenu_GetContinueWidgetSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_MainMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_MainMenu_SetSelectedElement);
	lua_setglobal(L, lua::metatables::MainMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMainMenuGame(state);
}