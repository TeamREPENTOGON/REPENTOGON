#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* ControllerSelectMenuMT = "ControllerSelectMenu";

static int Lua_ControllerSelectMenu_GetControllerSelectSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "ControllerSelectMenu functions can only be used in the main menu"); }
	Menu_ControllerSelect* menu = g_MenuManager->GetMenuControllerSelect();
	ANM2* anm2 = menu->GetControllerSelectSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_ControllerSelectMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "ControllerSelectMenu functions can only be used in the main menu"); }
	Menu_ControllerSelect* menu = g_MenuManager->GetMenuControllerSelect();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static void RegisterControllerSelectMenuGame(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetControllerSelectMenuSprite", Lua_ControllerSelectMenu_GetControllerSelectSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_ControllerSelectMenu_GetSelectedElement);
	lua_setglobal(L, "ControllerSelectMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterControllerSelectMenuGame(state);
}