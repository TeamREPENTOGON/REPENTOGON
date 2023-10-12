#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SaveMenu_SaveSelectMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSaveSelectMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_DeleteButtonSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeleteButtonSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_DeletePopupSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeletePopupSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save1DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave1DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save2DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave2DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_Save3DrawingSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave3DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_GetSelectedSave)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushinteger(L, menu->SelectedSave);

	return 1;
}

LUA_FUNCTION(Lua_SaveMenu_SetSelectedSave)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	menu->SelectedSave = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_SaveMenu_IsDeleteActive)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SaveMenuMT);
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static void RegisterSaveMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSaveSelectMenuSprite", Lua_SaveMenu_SaveSelectMenuSprite);
	lua::TableAssoc(L, "GetDeleteButtonSprite", Lua_SaveMenu_DeleteButtonSprite);
	lua::TableAssoc(L, "GetDeletePopupSprite", Lua_SaveMenu_DeletePopupSprite);
	lua::TableAssoc(L, "GetSave1DrawingSprite", Lua_SaveMenu_Save1DrawingSprite);
	lua::TableAssoc(L, "GetSave2DrawingSprite", Lua_SaveMenu_Save2DrawingSprite);
	lua::TableAssoc(L, "GetSave3DrawingSprite", Lua_SaveMenu_Save3DrawingSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_SaveMenu_GetSelectedSave);
	lua::TableAssoc(L, "SetSelectedElement", Lua_SaveMenu_SetSelectedSave);
	lua::TableAssoc(L, "IsDeleteActive", Lua_SaveMenu_IsDeleteActive);
	lua_setglobal(L, lua::metatables::SaveMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterSaveMenuGame(state);
}