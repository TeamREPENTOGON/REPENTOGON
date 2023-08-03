#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MainMenuGameMT = "MainMenuGame";

static int Lua_GetMainMenuGame(lua_State* L) {
	MenuManager* menuManager = g_MenuManager;
	Menu_Game** ud = (Menu_Game**)lua_newuserdata(L, sizeof(Menu_Game*));
	*ud = menuManager->GetMenuGame();
	luaL_setmetatable(L, MainMenuGameMT);
	return 1;
}


int Lua_WorldToMenuPosition(lua_State* L)
{
	if (g_MenuManager != NULL){
		int n = lua_gettop(L);
		if (n != 2) {
			return luaL_error(L, "Expected two parameters(MenuId,WorldPosition) got %d\n", n);
		}
		int menuid = (int)luaL_checkinteger(L, 1);
		Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
		Vector* ref = (Vector *)(g_MenuManager + 60); //-49~ 72~ worldpos of ref // 10 95 is 0,0 on title // 59 23 offset on title
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

static int Lua_MainMenuGameGetGameMenuSprite(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuGameMT);
	ANM2* anm2 = menuGame->GetGameMenuSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetContinueWidgetSprite(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuGameMT);
	ANM2* anm2 = menuGame->GetContinueWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

static int Lua_MainMenuGameGetAnimationState(lua_State* L)
{
	Menu_Game* menuGame = *lua::GetUserdata<Menu_Game**>(L, 1, MainMenuGameMT);
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
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "GetMainMenu");
	lua_pushcfunction(L, Lua_GetMainMenuGame);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, MainMenuGameMT);
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
	RegisterWorldToMenuPos(state);
	RegisterMainMenuGame(state);
}