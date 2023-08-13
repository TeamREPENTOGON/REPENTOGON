#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* TitleMenuMT = "TitleMenu";

static int Lua_TitleMenuTitleScreenSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "TitleMenu functions can only be used in the main menu"); }
	Menu_Title* menu = g_MenuManager->GetMenuTitle();
	ANM2* anm2 = menu->GetTitleScreenSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}
static void RegisterTitleMenuGame(lua_State* L)
{	
	lua::LuaStackProtector protector(L);
	lua_newtable(L);
	lua::TableAssoc(L, "GetTitleScreenSprite", Lua_TitleMenuTitleScreenSprite);
	lua_setglobal(L, "TitleMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterTitleMenuGame(state);
}