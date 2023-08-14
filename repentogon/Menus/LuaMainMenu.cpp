#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MainMenuMT = "MainMenu";

static int Lua_MainMenu_GetGameMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetGameMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenu_GetContinueWidgetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetContinueWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenu_GetKeyDownTimer(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	lua_pushinteger(L, menuGame->KeyDownTimer);

	return 1;
}

static int Lua_MainMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	lua_pushinteger(L, menuGame->SelectedElement);

	return 1;
}

static int Lua_MainMenu_SetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	menuGame->SelectedElement = luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterMainMenuGame(lua_State* L)
{	
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetGameMenuSprite", Lua_MainMenu_GetGameMenuSprite);
	lua::TableAssoc(L, "GetContinueWidgetSprite", Lua_MainMenu_GetContinueWidgetSprite);
	lua::TableAssoc(L, "GetKeyDownTimer", Lua_MainMenu_GetKeyDownTimer);
	lua::TableAssoc(L, "GetSelectedElement", Lua_MainMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_MainMenu_SetSelectedElement);
	lua_setglobal(L, "MainMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMainMenuGame(state);
}