#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_PauseMenu_GetPauseMenuSprite)
{
	
	PauseScreen* menu =g_Game->GetPauseMenu();
	ANM2* anm2 = &menu->mainsprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_GetMyStuffSprite)
{

	PauseScreen* menu = g_Game->GetPauseMenu();
	ANM2* anm2 = &menu->mystuffminispritebase;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_GetCompletionMarksSprite)
{

	CompletionWidget* widgt = g_Game->GetPauseMenu()->GetCompletionWidget();
	ANM2* anm2 = &widgt->anm2;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_GetStatsSprite)
{
	
	PauseScreen* menu =g_Game->GetPauseMenu();
	ANM2* anm2 = &menu->statssprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_GetState)
{
	PauseScreen* menu = g_Game->GetPauseMenu();
	lua_pushinteger(L, menu->status);

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_SetState)
{
	PauseScreen* menu = g_Game->GetPauseMenu();
	menu->status = (int)luaL_checkinteger(L, 1);

	return 0;
}

LUA_FUNCTION(Lua_PauseMenu_GetSelectedElement)
{
	PauseScreen* menu = g_Game->GetPauseMenu();
	lua_pushinteger(L, menu->selectedelement);

	return 1;
}

LUA_FUNCTION(Lua_PauseMenu_SetSelectedElement)
{	
	PauseScreen* menu =g_Game->GetPauseMenu();
	menu->selectedelement = (int)luaL_checkinteger(L, 1);

	return 0;
}

static void RegisterOptionsMenu(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetSprite", Lua_PauseMenu_GetPauseMenuSprite);
	lua::TableAssoc(L, "GetStatsSprite", Lua_PauseMenu_GetStatsSprite);
	lua::TableAssoc(L, "GetMyStuffSprite", Lua_PauseMenu_GetMyStuffSprite);
	lua::TableAssoc(L, "GetCompletionMarksSprite", Lua_PauseMenu_GetCompletionMarksSprite);
	lua::TableAssoc(L, "GetSelectedElement", Lua_PauseMenu_GetSelectedElement);
	lua::TableAssoc(L, "SetSelectedElement", Lua_PauseMenu_SetSelectedElement);
	lua::TableAssoc(L, "GetState", Lua_PauseMenu_GetState);
	lua::TableAssoc(L, "SetState", Lua_PauseMenu_SetState);
	lua_setglobal(L, lua::metatables::PauseMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterOptionsMenu(state);
}