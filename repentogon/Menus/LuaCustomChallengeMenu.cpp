#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* CustomChallengeMenuMT = "CustomChallengeMenu";

static int Lua_CustomChallengeMenu_GetCustomChallengeMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "CustomChallengeMenu functions can only be used in the main menu"); }
	Menu_CustomChallenge* menu = g_MenuManager->GetMenuCustomChallenge();
	ANM2* anm2 = menu->GetCustomChallengeSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_CustomChallengeMenu_GetSelectedChallengeID(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "CustomChallengeMenu functions can only be used in the main menu"); }
	Menu_CustomChallenge* menu = g_MenuManager->GetMenuCustomChallenge();
	lua_pushinteger(L, menu->SelectedElement);

	return 1;
}

static int Lua_CustomChallengeMenu_SetSelectedChallengeID(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "CustomChallengeMenu functions can only be used in the main menu"); }
	Menu_CustomChallenge* menu = g_MenuManager->GetMenuCustomChallenge();
	menu->SelectedElement = (int)luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterCustomChallengeMenuGame(lua_State* L)
{
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_CustomChallengeMenu_GetCustomChallengeMenuSprite);
	lua::TableAssoc(L, "GetSelectedChallengeID", Lua_CustomChallengeMenu_GetSelectedChallengeID);
	lua::TableAssoc(L, "SetSelectedChallengeID", Lua_CustomChallengeMenu_SetSelectedChallengeID);
	lua_setglobal(L, "CustomChallengeMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterCustomChallengeMenuGame(state);
}