#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MainMenuMT = "MainMenu";

static int Lua_MainMenuGameGetGameMenuSprite(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuMT);
	ANM2* anm2 = menuGame->GetGameMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetContinueWidgetSprite(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuMT);
	ANM2* anm2 = menuGame->GetContinueWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetAnimationState(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuMT);
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
	luaL_newmetatable(L, MainMenuMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetGameMenuSprite", Lua_MainMenuGameGetGameMenuSprite},
		{ "GetContinueWidgetSprite", Lua_MainMenuGameGetContinueWidgetSprite},
		{ "GetContinueWidgetAnimationState", Lua_MainMenuGameGetAnimationState},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMainMenuGame(state);
}