#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SpecialSeedsMenu_GetSpecialSeedsMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SpecialSeedsMenuMT);
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	ANM2* anm2 = menu->GetSpecialSeedsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_SpecialSeedsMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SpecialSeedsMenuMT);
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_SpecialSeedsMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::SpecialSeedsMenuMT);
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterSpecialSeedsMenu(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_SpecialSeedsMenu_GetSpecialSeedsMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_SpecialSeedsMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_SpecialSeedsMenu_SetSelectedElement);
	lua_setglobal(L, lua::metatables::SpecialSeedsMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterSpecialSeedsMenu(state);
}