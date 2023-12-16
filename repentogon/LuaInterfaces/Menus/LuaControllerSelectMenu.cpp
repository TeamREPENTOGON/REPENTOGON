#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ControllerSelectMenu_GetControllerSelectSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ControllerSelectMenuMT);
	Menu_ControllerSelect* menu = g_MenuManager->GetMenuControllerSelect();
	ANM2* anm2 = menu->GetControllerSelectSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_ControllerSelectMenu_GetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ControllerSelectMenuMT);
	Menu_ControllerSelect* menu = g_MenuManager->GetMenuControllerSelect();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

LUA_FUNCTION(Lua_ControllerSelectMenu_SetSelectedElement)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::ControllerSelectMenuMT);
	Menu_ControllerSelect* menu = g_MenuManager->GetMenuControllerSelect();
	menu->SelectedElement = (int)luaL_checkinteger(L, 1);

	return 0;
}

static void RegisterControllerSelectMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_ControllerSelectMenu_GetControllerSelectSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_ControllerSelectMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_ControllerSelectMenu_SetSelectedElement);
	lua_setglobal(L, lua::metatables::ControllerSelectMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterControllerSelectMenuGame(state);
}