#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* KeyConfigMenuMT = "KeyConfigMenu";

static int Lua_KeyConfigMenu_KeyConfigSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "KeyConfigMenu functions can only be used in the main menu"); }
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	ANM2* anm2 = menu->GetKeyConfigSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_KeyConfigMenu_GetSelectedElement(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "KeyConfigMenu functions can only be used in the main menu"); }
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushinteger(L, menu->SelectedElement);
	
	return 1;
}

static int Lua_KeyConfigMenu_GetSelectedColumn(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "KeyConfigMenu functions can only be used in the main menu"); }
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushinteger(L, menu->SelectedColumn);
	
	return 1;
}
static int Lua_KeyConfigMenu_IsEditActive(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "KeyConfigMenu functions can only be used in the main menu"); }
	Menu_KeyConfig* menu = g_MenuManager->GetMenuKeyConfig();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static void RegisterKeyConfigMenuGame(lua_State* L)
{	
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetKeyConfigSprite", Lua_KeyConfigMenu_KeyConfigSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_KeyConfigMenu_GetSelectedElement);
	lua::TableAssoc(L, "GetSelectedColumn", Lua_KeyConfigMenu_GetSelectedColumn);
	lua::TableAssoc(L, "IsEditActive", Lua_KeyConfigMenu_IsEditActive);
	lua_setglobal(L, "KeyConfigMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterKeyConfigMenuGame(state);
}