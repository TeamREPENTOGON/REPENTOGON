#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* BestiaryMenuMT = "BestiaryMenu";


static int Lua_BestiaryMenu_GetBestiaryMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetBestiaryMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_BestiaryMenu_GetDeathScreenSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetDeathScreenSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_BestiaryMenu_GetEnemySprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	ANM2* anm2 = menu->GetEnemySprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenu_GetCurrentPage(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->CurrentPage);

	return 1;
}

static int Lua_MainMenu_SetCurrentPage(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	menu->CurrentPage = (int)luaL_checkinteger(L, 2);

	return 0;
}

static int Lua_MainMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static int Lua_MainMenu_SetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static int Lua_MainMenu_GetLastEnemyPageID(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "BestiaryMenu functions can only be used in the main menu"); }
	Menu_Bestiary* menu = g_MenuManager->GetMenuBestiary();
	lua_pushinteger(L, menu->LastEnemyPageID);

	return 1;
}

static void RegisterBestiaryMenu(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetBestiaryMenuSprite", Lua_BestiaryMenu_GetBestiaryMenuSprite);
	lua::TableAssoc(L, "GetDeathScreenSprite", Lua_BestiaryMenu_GetDeathScreenSprite);
	lua::TableAssoc(L, "GetEnemySprite", Lua_BestiaryMenu_GetEnemySprite);
	lua::TableAssoc(L, "GetCurrentPage", Lua_MainMenu_GetCurrentPage);
	lua::TableAssoc(L, "SetCurrentPage", Lua_MainMenu_SetCurrentPage);
	lua::TableAssoc(L, "GetSelectedElement", Lua_MainMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_MainMenu_SetSelectedElement);
	lua::TableAssoc(L, "GetLastEnemyPageID", Lua_MainMenu_GetLastEnemyPageID);
	lua_setglobal(L, "BestiaryMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterBestiaryMenu(state);
}
