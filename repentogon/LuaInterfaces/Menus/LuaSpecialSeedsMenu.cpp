#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* SpecialSeedsMenuMT = "SpecialSeedsMenu";

static int Lua_SpecialSeedsMenu_GetSpecialSeedsMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SpecialSeedsMenu functions can only be used in the main menu"); }
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	ANM2* anm2 = menu->GetSpecialSeedsSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SpecialSeedsMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SpecialSeedsMenu functions can only be used in the main menu"); }
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static int Lua_SpecialSeedsMenu_SetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SpecialSeedsMenu functions can only be used in the main menu"); }
	Menu_SpecialSeeds* menu = g_MenuManager->GetMenuSpecialSeeds();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterSpecialSeedsMenu(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_SpecialSeedsMenu_GetSpecialSeedsMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_SpecialSeedsMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_SpecialSeedsMenu_SetSelectedElement);
	lua_setglobal(L, "SpecialSeedsMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterSpecialSeedsMenu(state);
}