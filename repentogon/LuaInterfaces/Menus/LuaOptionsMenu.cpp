#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* OptionsMenuMT = "OptionsMenu";

static int Lua_OptionsMenu_GetOptionsMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "OptionsMenu functions can only be used in the main menu"); }
	Menu_Options* menu = g_MenuManager->GetMenuOptions();
	ANM2* anm2 = menu->GetOptionsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_OptionsMenu_GetGammaWidgetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "OptionsMenu functions can only be used in the main menu"); }
	Menu_Options* menu = g_MenuManager->GetMenuOptions();
	ANM2* anm2 = menu->GetGammaMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_OptionsMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "OptionsMenu functions can only be used in the main menu"); }
	Menu_Options* menu = g_MenuManager->GetMenuOptions();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static int Lua_OptionsMenu_SetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "OptionsMenu functions can only be used in the main menu"); }
	Menu_Options* menu = g_MenuManager->GetMenuOptions();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterOptionsMenu(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetOptionsMenuSprite", Lua_OptionsMenu_GetOptionsMenuSprite);
	lua::TableAssoc(L, "GetGammaWidgetSprite", Lua_OptionsMenu_GetGammaWidgetSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_OptionsMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_OptionsMenu_SetSelectedElement);
	lua_setglobal(L, "OptionsMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterOptionsMenu(state);
}