#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static int Lua_ColorGetTint(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	lua_newtable(L, 0);

	const char* keys[4] = {"R", "G", "B", "A"};

	for (int i = 0; i < 4; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_tint[i]);
		lua_settable(L, -3);
	}
	return 1;
}

static int Lua_ColorGetColorize(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	lua_newtable(L, 0);

	const char* keys[4] = { "R", "G", "B", "A" };

	for (int i = 0; i < 4; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_colorize[i]);
		lua_settable(L, -3);
	}

	return 1;
}


static int Lua_ColorGetOffset(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	lua_newtable(L, 0);

	const char* keys[3] = { "R", "G", "B" };

	for (int i = 0; i < 3; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_offset[i]);
		lua_settable(L, -3);
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::COLOR;
	lua::RegisterFunction(state, mt, "GetTint", Lua_ColorGetTint);
	lua::RegisterFunction(state, mt, "GetColorize", Lua_ColorGetColorize);
	lua::RegisterFunction(state, mt, "GetOffset", Lua_ColorGetOffset);
}