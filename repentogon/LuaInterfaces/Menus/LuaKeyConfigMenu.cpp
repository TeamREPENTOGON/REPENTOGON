#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* KeyConfigMenuMT = "KeyConfigMenu";

LUA_FUNCTION(Lua_KeyConfigMenu_KeyConfigSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	ANM2* anm2 = menu->GetKeyConfigSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_KeyConfigMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_KeyConfigMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_KeyConfigMenu_GetSelectedColumn)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushinteger(L, menu->SelectedColumn);

	return 1;
}

LUA_FUNCTION(Lua_KeyConfigMenu_SetSelectedColumn)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	menu->SelectedColumn = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_KeyConfigMenu_IsEditActive)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::KeyConfigMenuMT);
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static void RegisterKeyConfigMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_KeyConfigMenu_KeyConfigSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_KeyConfigMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_KeyConfigMenu_SetSelectedElement);
	lua::TableAssoc(L, "GetSelectedColumn", Lua_KeyConfigMenu_GetSelectedColumn);
	lua::TableAssoc(L, "SetSelectedColumn", Lua_KeyConfigMenu_SetSelectedColumn);
	lua::TableAssoc(L, "IsEditActive", Lua_KeyConfigMenu_IsEditActive);
	lua_setglobal(L, lua::metatables::KeyConfigMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterKeyConfigMenuGame(state);
}