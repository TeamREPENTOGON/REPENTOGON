#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* SaveMenuMT = "SaveMenu";

static int Lua_SaveMenu_SaveSelectMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSaveSelectMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_DeleteButtonSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeleteButtonSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_DeletePopupSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetDeletePopupSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_Save1DrawingSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave1DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_Save2DrawingSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave2DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_Save3DrawingSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	ANM2* anm2 = menu->GetSave3DrawingSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_SaveMenu_GetSelectedSave(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushinteger(L, menu->SelectedSave);
	
	return 1;
}

static int Lua_SaveMenu_IsDeleteActive(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "SaveMenu functions can only be used in the main menu"); }
	Menu_Save* menu = g_MenuManager->GetMenuSave();
	lua_pushboolean(L, menu->State == 1);

	return 1;
}

static void RegisterSaveMenuGame(lua_State* L)
{	
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetSaveSelectMenuSprite", Lua_SaveMenu_SaveSelectMenuSprite);
	lua::TableAssoc(L, "GetDeleteButtonSprite", Lua_SaveMenu_DeleteButtonSprite);
	lua::TableAssoc(L, "GetDeletePopupSprite", Lua_SaveMenu_DeletePopupSprite);
	lua::TableAssoc(L, "GetSave1DrawingSprite", Lua_SaveMenu_Save1DrawingSprite);
	lua::TableAssoc(L, "GetSave2DrawingSprite", Lua_SaveMenu_Save2DrawingSprite);
	lua::TableAssoc(L, "GetSave3DrawingSprite", Lua_SaveMenu_Save3DrawingSprite);
	lua::TableAssoc(L, "GetSelectedSave", Lua_SaveMenu_GetSelectedSave);
	lua::TableAssoc(L, "IsDeleteActive", Lua_SaveMenu_IsDeleteActive);
	lua_setglobal(L, "SaveMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterSaveMenuGame(state);
}