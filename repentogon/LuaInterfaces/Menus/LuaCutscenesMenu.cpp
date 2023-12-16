#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CutscenesMenu_GetCutscenesMenuSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CutscenesMenuMT);
	Menu_Cutscenes* menu = g_MenuManager->GetMenuCutscenes();
	ANM2* anm2 = menu->GetCutscenesMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_CutscenesMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CutscenesMenuMT);
	Menu_Cutscenes* menu = g_MenuManager->GetMenuCutscenes();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_CutscenesMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CutscenesMenuMT);
	Menu_Cutscenes* menu = g_MenuManager->GetMenuCutscenes();
	menu->SelectedElement = (int)luaL_checkinteger(L, 1);

	return 0;
}

static void RegisterCutscenesMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_CutscenesMenu_GetCutscenesMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_CutscenesMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_CutscenesMenu_SetSelectedElement);
	lua_setglobal(L, lua::metatables::CutscenesMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCutscenesMenuGame(state);
}