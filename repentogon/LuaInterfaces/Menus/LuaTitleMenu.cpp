#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_TitleMenuTitleScreenSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::TitleMenuMT);
	Menu_Title* menu = g_MenuManager->GetMenuTitle();
	ANM2* anm2 = menu->GetTitleScreenSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}
static void RegisterTitleMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_TitleMenuTitleScreenSprite);
	lua_setglobal(L, lua::metatables::TitleMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterTitleMenuGame(state);
}