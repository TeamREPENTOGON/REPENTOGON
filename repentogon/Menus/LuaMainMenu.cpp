#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MainMenuMT = "MainMenu";

static int Lua_MainMenuGameGetGameMenuSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetGameMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetContinueWidgetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	ANM2* anm2 = menuGame->GetContinueWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetAnimationState(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MainMenu functions can only be used in the main menu"); }
	Menu_Game* menuGame = g_MenuManager->GetMenuGame();
	AnimationState* toLua = menuGame->GetContinueWidgetAnimationState();
	if (toLua == nullptr) {
		printf("ALERT, ANIMSTATE IS NULL");
		lua_pushnil(L);
		return 1;
	}
	AnimationState** luaAnimationState = (AnimationState**)lua_newuserdata(L, sizeof(AnimationState*));
	*luaAnimationState = toLua;
	luaL_setmetatable(L, lua::metatables::AnimationStateMT);
	return 1;
}

static void RegisterMainMenuGame(lua_State* L)
{	
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetGameMenuSprite", Lua_MainMenuGameGetGameMenuSprite);
	lua::TableAssoc(L, "GetContinueWidgetSprite", Lua_MainMenuGameGetContinueWidgetSprite);
	lua::TableAssoc(L, "GetContinueWidgetAnimationState", Lua_MainMenuGameGetAnimationState);
	lua_setglobal(L, "MainMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMainMenuGame(state);
}