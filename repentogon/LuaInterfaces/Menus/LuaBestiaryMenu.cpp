#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


LUA_FUNCTION(Lua_BestiaryMenu_GetBestiaryMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetBestiaryMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_BestiaryMenu_GetDeathScreenSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetDeathScreenSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_BestiaryMenu_GetEnemySprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetEnemySprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_GetSelectedPage)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->CurrentPage);

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_SetSelectedPage)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	menu->CurrentPage = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_MainMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_MainMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_MainMenu_GetLastEnemyPageID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::BestiaryMenuMT);
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->LastEnemyPageID);

	return 1;
}

static void RegisterBestiaryMenu(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetBestiaryMenuSprite", Lua_BestiaryMenu_GetBestiaryMenuSprite);
	lua::TableAssoc(L, "GetDeathScreenSprite", Lua_BestiaryMenu_GetDeathScreenSprite);
	lua::TableAssoc(L, "GetEnemySprite", Lua_BestiaryMenu_GetEnemySprite);
	lua::TableAssoc(L, "GetSelectedPage", Lua_MainMenu_GetSelectedPage);
	lua::TableAssoc(L, "SetSelectedPage", Lua_MainMenu_SetSelectedPage);
	lua::TableAssoc(L, "GetSelectedElement", Lua_MainMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_MainMenu_SetSelectedElement);
	lua::TableAssoc(L, "GetLastEnemyPageID", Lua_MainMenu_GetLastEnemyPageID);
	lua_setglobal(L, lua::metatables::BestiaryMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterBestiaryMenu(state);
}
