#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* CutscenesMenuMT = "CutscenesMenu";

static int Lua_CutscenesMenu_GetCutscenesMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "CutscenesMenu functions can only be used in the main menu"); }
	Menu_Cutscenes* menu = g_MenuManager->GetMenuCutscenes();
	ANM2* anm2 = menu->GetCutscenesMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_CutscenesMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "CutscenesMenu functions can only be used in the main menu"); }
	Menu_Cutscenes* menu = g_MenuManager->GetMenuCutscenes();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static void RegisterCutscenesMenuGame(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetCutscenesMenuSprite", Lua_CutscenesMenu_GetCutscenesMenuSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_CutscenesMenu_GetSelectedElement);
	lua_setglobal(L, "CutscenesMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCutscenesMenuGame(state);
}