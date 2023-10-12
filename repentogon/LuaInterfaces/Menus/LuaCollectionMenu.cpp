#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CollectionMenu_GetCollectionMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	ANM2* anm2 = menu->GetCollectionMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_CollectionMenu_GetDeathScreenSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	ANM2* anm2 = menu->GetDeathScreenSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_CollectionMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_CollectionMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_CollectionMenu_GetSelectedPage)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	lua_pushinteger(L, menu->SelectedPage);

	return 1;
}

LUA_FUNCTION(Lua_CollectionMenu_SetSelectedPage)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CollectionMenuMT);
	Menu_Collection* menu = g_MenuManager->GetMenuCollection();
	menu->SelectedPage = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterCollectionMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetCollectionMenuSprite", Lua_CollectionMenu_GetCollectionMenuSprite);
	lua::TableAssoc(L, "GetDeathScreenSprite", Lua_CollectionMenu_GetDeathScreenSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_CollectionMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_CollectionMenu_SetSelectedElement);
	lua::TableAssoc(L, "GetSelectedPage", Lua_CollectionMenu_GetSelectedPage);
	lua::TableAssoc(L, "SetSelectedPage", Lua_CollectionMenu_SetSelectedPage);
	lua_setglobal(L, lua::metatables::CollectionMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCollectionMenuGame(state);
}