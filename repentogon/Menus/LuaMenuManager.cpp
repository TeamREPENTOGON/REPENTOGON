#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "Log.h"

static constexpr const char* MenuManagerMT = "MenuManager";
static constexpr const char* StatsMenuMT = "StatsMenu";
static constexpr const char* MainMenuMT = "MainMenu";
static constexpr const char* CharacterMenuMT = "CharacterMenu";

int Lua_WorldToMenuPosition(lua_State* L)
{
	if (g_MenuManager != NULL) {
		int n = lua_gettop(L);
		if (n != 2) {
			return luaL_error(L, "Expected two parameters(MenuId,WorldPosition) got %d\n", n);
		}
		int menuid = (int)luaL_checkinteger(L, 1);
		Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
		Vector* ref = &g_MenuManager->_ViewPosition; //-49~ 72~ worldpos of ref // 10 95 is 0,0 on title // 59 23 offset on title
		ref = new Vector(ref->x + 39, ref->y + 15);
		Vector* offset;
		switch (menuid) {
		case 1: //menu title
			offset = new Vector(ref->x + 0, ref->y + 0); //this is 0,0 at main title  -20,-8
			break;
		case 2: //menu save
			offset = new Vector(ref->x + 0, ref->y + 270);
			break;
		case 3: //menu game
			offset = new Vector(ref->x + 0, ref->y + 540);
			break;
		case 4: //menu daily run
			offset = new Vector(ref->x + 479, ref->y + 540);
			break;
		case 5: //menu character
			offset = new Vector(ref->x + 0, ref->y + 810);
			break;
		case 6: //menu eastereggs
			offset = new Vector(ref->x + 959, ref->y + 810);
			break;
		case 7: //menu challenge
			offset = new Vector(ref->x - 480, ref->y + 216);
			break;
		case 8: //menu collection
			offset = new Vector(ref->x + 0, ref->y + 1080);
			break;
		case 9: //menu stats
			offset = new Vector(ref->x - 480, ref->y + 1350);
			break;
		case 10: //menu options
			offset = new Vector(ref->x + 479, ref->y + 1080);
			break;
		case 11: //menu controls
			offset = new Vector(ref->x + 479, ref->y + 1620);
			break;
		case 12: //menu keyconfig
			offset = new Vector(ref->x + 479, ref->y + 1890);
			break;
		case 13: //menu endings
			offset = new Vector(ref->x + 479, ref->y + 270);
			break;
		case 14: //menu bestiary
			offset = new Vector(ref->x - 480, ref->y + 1670);
			break;
		case 15: //menu customchallenges
			offset = new Vector(ref->x - 950, ref->y + 216);
			break;
		case 16: //menu mods
			offset = new Vector(ref->x - 1440, ref->y + 216);
			break;
		case 17: //menu seed
			offset = new Vector(ref->x + 479, ref->y + 810);
			break;
		default:
			return luaL_error(L, "Invalid Menu Id");
			break;
		}
		lua::LuaCaller(L).pushUserdataValue(*new Vector(offset->x + pos->x, offset->y + pos->y), lua::Metatables::VECTOR);
		return 1;
	}
	else {
		return luaL_error(L, "WorldToMenu can only be used in the main menu");
	}
}

static void RegisterWorldToMenuPos(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "WorldToMenuPosition");
	lua_pushcfunction(L, Lua_WorldToMenuPosition);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_MenuManager_GetShadowSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_MenuShadowSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MenuManager_GetBackWidgetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_BackWidgetSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MenuManager_GetSelectWidgetSprite(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ANM2* anm2 = &menuManager->_SelectWidgetSprite;
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_GetSelectedMenuID(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	lua_pushinteger(L, menuManager->_selectedMenuID);
	return 1;
}

static int Lua_SetSelectedMenuID(lua_State* L)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	menuManager->_selectedMenuID = (int)luaL_checkinteger(L, 1);
	return 1;
}

LUA_FUNCTION(Lua_MenuGetCurrentColorModifier)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetCurrentColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}
LUA_FUNCTION(Lua_MenuGetTargetColorModifier)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetTargetColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_MenuGetLerpColorModifier)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ColorModState* color = menuManager->GetLerpColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}


static void MenuSetColorModifier(MenuManager* menu, ColorModState* newMod, bool lerp, float rate)
{
	ZHL::Logger logger;
	logger.Log("newMod: %f %f %f %f %f %f\n", newMod->r, newMod->g, newMod->b, newMod->a, newMod->brightness, newMod->contrast);
	menu->_targetColorModState = newMod;
	if (lerp) {
		ColorModState lerp = (*newMod - menu->_currentColorModState);
		ColorModState* oldMod = &menu->_currentColorModState;
		lerp *= (rate / 2);
		lerp.r = abs(lerp.r);
		lerp.g = abs(lerp.g);
		lerp.b = abs(lerp.b);
		lerp.a = abs(lerp.a);
		lerp.brightness = abs(lerp.brightness);
		lerp.contrast = abs(lerp.contrast);
		logger.Log("oldMod: %f %f %f %f %f %f\n", oldMod->r, oldMod->g, oldMod->b, oldMod->a, oldMod->brightness, oldMod->contrast);
		menu->_lerpColorModState = lerp;
		logger.Log("lerpColorModState: %f %f %f %f %f %f\n", menu->_lerpColorModState.r, menu->_lerpColorModState.g, menu->_lerpColorModState.b, menu->_lerpColorModState.a, menu->_lerpColorModState.brightness, menu->_lerpColorModState.contrast);
		menu->_shouldLerpColorModState = true;
	}
	else {
		menu->_currentColorModState = newMod;
		logger.Log("currentColorModState: %f %f %f %f %f %f\n", menu->_currentColorModState.r, menu->_currentColorModState.g, menu->_currentColorModState.b, menu->_currentColorModState.a, menu->_currentColorModState.brightness, menu->_currentColorModState.contrast);
		menu->_shouldLerpColorModState = false;
	}
}

LUA_FUNCTION(Lua_MenuSetColorModifier)
{
	if (g_MenuManager == NULL) { return luaL_error(L, "MenuManager functions can only be used in the main menu"); }
	MenuManager* menuManager = g_MenuManager;
	ColorModState* pColor = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	bool lerp = true;
	if lua_isboolean(L, 3)
		lerp = lua_toboolean(L, 3);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	// game func is inlined, gotta do it ourselves
	MenuSetColorModifier(menuManager, pColor, lerp, rate);

	return 0;
}


static void RegisterMenuManager(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetShadowSprite", Lua_MenuManager_GetShadowSprite);
	lua::TableAssoc(L, "GetBackWidgetSprite", Lua_MenuManager_GetBackWidgetSprite);
	lua::TableAssoc(L, "GetSelectWidgetSprite", Lua_MenuManager_GetSelectWidgetSprite);
	lua::TableAssoc(L, "SetActiveMenu", Lua_SetSelectedMenuID);
	lua::TableAssoc(L, "GetActiveMenu", Lua_GetSelectedMenuID);
	
	lua::TableAssoc(L, "GetCurrentColorModifier", Lua_MenuGetCurrentColorModifier);
	lua::TableAssoc(L, "GetTargetColorModifier", Lua_MenuGetTargetColorModifier);
	lua::TableAssoc(L, "GetColorModifierLerpAmount", Lua_MenuGetLerpColorModifier);
	lua::TableAssoc(L, "SetColorModifier", Lua_MenuSetColorModifier);
	
	lua_setglobal(L, "MenuManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMenuManager(state);
	RegisterWorldToMenuPos(state);
}
