#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


int Lua_WorldToMenuPosition(lua_State* L)
{
	if (g_MenuManager != NULL){
		int n = lua_gettop(L);
		if (n != 2) {
			return luaL_error(L, "Expected two parameters(MenuId,WorldPosition) got %d\n", n);
		}
		int menuid = luaL_checkinteger(L, 1);
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
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterWorldToMenuPos(state);
}