#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ModsMenu_GetModsMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ModsMenuMT);
	Menu_Mods* menu = g_MenuManager->GetMenuMods();
	ANM2* anm2 = menu->GetModsMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_ModsMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ModsMenuMT);
	Menu_Mods* menu = g_MenuManager->GetMenuMods();
	lua_pushinteger(L, menu->SelectedElement + 1);

	return 1;
}

LUA_FUNCTION(Lua_ModsMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ModsMenuMT);
	Menu_Mods* menu = g_MenuManager->GetMenuMods();
	int newPosition = (int)luaL_checkinteger(L, 1) - 1;


	if (newPosition >= (int)g_Manager->GetModManager()->_mods.size())
		newPosition = g_Manager->GetModManager()->_mods.size() - 1;

	if (newPosition < 0)
		newPosition = 0;

	printf("%d %d\n", menu->SelectedElement, newPosition);

	menu->_pointerToSelectedMod = *(ModEntry**)g_Manager->GetModManager() + (newPosition * 4);
	menu->SelectedElement = newPosition;

	return 0;
}

LUA_FUNCTION(Lua_ModsMenu_WasListEdited)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ModsMenuMT);
	Menu_Mods* menu = g_MenuManager->GetMenuMods();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static void RegisterModsMenu(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_ModsMenu_GetModsMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_ModsMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_ModsMenu_SetSelectedElement);
	lua::TableAssoc(L, "WasListEdited", Lua_ModsMenu_WasListEdited);
	lua_setglobal(L, lua::metatables::ModsMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterModsMenu(state);
}