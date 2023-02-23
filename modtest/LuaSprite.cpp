#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*
* While internally, this is the ANM2 class, it is exposed to Lua as "Sprite".
* I've named this file "LuaSprite" for consistency with the existing API metatable.
*/

int Lua_SpriteReplaceSpritesheet(lua_State* L)
{
	bool loadGraphics = false;
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	int layerId = luaL_checkinteger(L, 2);

	IsaacString str;
	const char* filename = luaL_checkstring(L, 3);
	if (strlen(filename) < 16) {
		strcpy(str.text, filename);
	}
	else {
		*(char**)str.text = (char*)filename;
	}
	str.unk = str.size = strlen(filename);

	if (lua_isboolean(L, 4))
		loadGraphics = lua_toboolean(L, 4);

	anm2->ReplaceSpritesheet(layerId, str);

	if (loadGraphics) {
		anm2->LoadGraphics(false); 
	}

	return 0;
}

static void RegisterNewReplaceSpriteSheet(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::SPRITE);
	lua_pushstring(L, "ReplaceSpritesheet");
	lua_pushcfunction(L, Lua_SpriteReplaceSpritesheet);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterNewReplaceSpriteSheet(state);
}

