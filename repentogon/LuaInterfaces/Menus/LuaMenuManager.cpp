#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/MainMenuBlock.h"


LUA_FUNCTION(Lua_WorldToMenuPosition)
{
	if (g_MenuManager != NULL) {
		int n = lua_gettop(L);
		if (n != 2) {
			return luaL_error(L, "Expected two parameters(MenuId,WorldPosition) got %d\n", n);
		}
		eMainMenuType menuid = (eMainMenuType)luaL_checkinteger(L, 1);
		Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
		Vector* ref = &g_MenuManager->_ViewPosition; //-49~ 72~ worldpos of ref // 10 95 is 0,0 on title // 59 23 offset on title
		Vector posbase = *ref + Vector(39, 15);
		ref = &posbase;
;		Vector offset;
		offset = Vector(ref->x + 39, ref->y + 15);

		if (menuid >= TITLE && menuid <= ONLINEAWARDS) {
			offset = Vector(ref->x + (-g_MenuManager->_viewPositionSet[menuid].x), ref->y + (-g_MenuManager->_viewPositionSet[menuid].y));
		}
		else {
			return luaL_error(L, "Invalid Menu Id %d\n", menuid);
		}
		
		lua::LuaCaller(L).pushUserdataValue(Vector(offset.x + pos->x, offset.y + pos->y), lua::Metatables::VECTOR);
		return 1;
	}
	else {
		return luaL_error(L, "WorldToMenu can only be used in the main menu");
	}
}

LUA_FUNCTION(Lua_MenuManager_GetShadowSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_MenuShadowSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

// REP+: Disabled because we lost track of these sprite objects.
/*
LUA_FUNCTION(Lua_MenuManager_GetBackWidgetSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_BackWidgetSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}
LUA_FUNCTION(Lua_MenuManager_GetSelectWidgetSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_SelectWidgetSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}
*/

LUA_FUNCTION(Lua_GetSelectedMenuID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	lua_pushinteger(L, menuManager->_selectedMenuID);
	return 1;
}

LUA_FUNCTION(Lua_SetSelectedMenuID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	menuManager->_selectedMenuID = (int)luaL_checkinteger(L, 1);
	return 1;
}

LUA_FUNCTION(Lua_MenuGetCurrentColorModifier)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetCurrentColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}
LUA_FUNCTION(Lua_MenuGetTargetColorModifier)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetTargetColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_MenuGetLerpColorModifier)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetLerpColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

static void MenuSetColorModifier(MenuManager* menu, ColorModState* newMod, bool lerp, float rate)
{
	menu->_targetColorModState = *newMod;
	if (lerp) {
		ColorModState lerp = (*newMod - menu->_currentColorModState);
		lerp *= (rate / 2);
		lerp.r = abs(lerp.r);
		lerp.g = abs(lerp.g);
		lerp.b = abs(lerp.b);
		lerp.a = abs(lerp.a);
		lerp.brightness = abs(lerp.brightness);
		lerp.contrast = abs(lerp.contrast);

		menu->_lerpColorModState = lerp;
		menu->_shouldLerpColorModState = true;
	}
	else {
		menu->_currentColorModState = *newMod;
		menu->_shouldLerpColorModState = false;
	}
}

LUA_FUNCTION(Lua_MenuSetColorModifier)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ColorModState* pColor = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	bool lerp = lua::luaL_optboolean(L, 2, true);
	float rate = (float)luaL_optnumber(L, 3, 0.015);

	// game func is inlined, gotta do it ourselves
	MenuSetColorModifier(menuManager, pColor, lerp, rate);

	return 0;
}

LUA_FUNCTION(Lua_MenuGetViewPosition)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = menuManager->_ViewPosition;

	return 1;
}

LUA_FUNCTION(Lua_MenuSetViewPosition)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	MenuManager* menuManager = g_MenuManager;
	menuManager->_ViewPosition = *lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");

	return 0;
}

HOOK_METHOD(InputManager, IsActionTriggered, (int btn, int controllerid, int unk)->bool) {
	if (MainMenuInputBlock::_enabled==true) {
		return (MainMenuInputBlock::_inputmask & (1 << btn)) && super(btn, controllerid, unk);
	}
	else {
		return super(btn, controllerid, unk);
	}
};
HOOK_METHOD(InputManager, IsActionPressed, (int btn, int controllerid, int unk)->bool) {
	if (MainMenuInputBlock::_enabled==true) {
		return (MainMenuInputBlock::_inputmask & (1 << btn)) && super(btn, controllerid, unk);
	}
	else{
		return super(btn, controllerid, unk);
	}
};

HOOK_STATIC(LuaEngine, PostGameStart, (unsigned int state)->void,__stdcall) {
	MainMenuInputBlock::ClearInputMask();
	super(state);
};

LUA_FUNCTION(Lua_SetInputMask) {
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	unsigned int mask = (int)luaL_checkinteger(L, 1);
	MainMenuInputBlock::SetInputMask(mask);
	return 1;
}

LUA_FUNCTION(Lua_GetInputMask) {
	lua::LuaCheckMainMenuExists(L, lua::metatables::MenuManagerMT);
	lua_pushinteger(L,MainMenuInputBlock::GetInputMask());
	return 1;
}


static void RegisterMenuManager(lua_State* L)
{
	lua::RegisterGlobalClassFunction(L, lua::GlobalClasses::Isaac, "WorldToMenuPosition", Lua_WorldToMenuPosition);

	lua_newtable(L);
	lua::TableAssoc(L, "GetShadowSprite", Lua_MenuManager_GetShadowSprite);
	// REP+: Disabled because we lost track of these sprite objects.
	// lua::TableAssoc(L, "GetBackWidgetSprite", Lua_MenuManager_GetBackWidgetSprite);
	// lua::TableAssoc(L, "GetSelectWidgetSprite", Lua_MenuManager_GetSelectWidgetSprite);
	lua::TableAssoc(L, "SetActiveMenu", Lua_SetSelectedMenuID);
	lua::TableAssoc(L, "GetActiveMenu", Lua_GetSelectedMenuID);

	lua::TableAssoc(L, "GetCurrentColorModifier", Lua_MenuGetCurrentColorModifier);
	lua::TableAssoc(L, "GetTargetColorModifier", Lua_MenuGetTargetColorModifier);
	lua::TableAssoc(L, "GetColorModifierLerpAmount", Lua_MenuGetLerpColorModifier);
	lua::TableAssoc(L, "SetColorModifier", Lua_MenuSetColorModifier);

	lua::TableAssoc(L, "GetViewPosition", Lua_MenuGetViewPosition);
	lua::TableAssoc(L, "SetViewPosition", Lua_MenuSetViewPosition);
	lua::TableAssoc(L, "GetInputMask", Lua_GetInputMask);
	lua::TableAssoc(L, "SetInputMask", Lua_SetInputMask);

	lua_setglobal(L, lua::metatables::MenuManagerMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	
	lua::LuaStackProtector protector(_state);
	RegisterMenuManager(_state);
}
