#include <sstream>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static int Lua_ColorGetTint(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	lua_newtable(L);

	const char* keys[4] = {"R", "G", "B", "A"};

	for (int i = 0; i < 4; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_tint[i]);
		lua_settable(L, -3);
	}
	return 1;
}

static int Lua_ConstColorGetTint(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::CONST_COLOR, "Color");
	lua_newtable(L);

	const char* keys[4] = { "R", "G", "B", "A" };

	for (int i = 0; i < 4; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_tint[i]);
		lua_settable(L, -3);
	}
	return 1;
}

static int Lua_ColorGetColorize(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	lua_newtable(L);

	const char* keys[4] = { "R", "G", "B", "A" };

	for (int i = 0; i < 4; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_colorize[i]);
		lua_settable(L, -3);
	}

	return 1;
}

static int Lua_ConstColorGetColorize(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::CONST_COLOR, "Color");
	lua_newtable(L);

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
	lua_newtable(L);

	const char* keys[3] = { "R", "G", "B" };

	for (int i = 0; i < 3; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_offset[i]);
		lua_settable(L, -3);
	}

	return 1;
}

static int Lua_ConstColorGetOffset(lua_State* L) {
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::CONST_COLOR, "Color");
	lua_newtable(L);

	const char* keys[3] = { "R", "G", "B" };

	for (int i = 0; i < 3; ++i) {
		lua_pushstring(L, keys[i]);
		lua_pushnumber(L, color->_offset[i]);
		lua_settable(L, -3);
	}

	return 1;
}

static void Lua_Color_ToString(lua_State* L, ColorMod* color) {
	std::ostringstream s;
	s << "[Color " << color->_tint[0] << " " << color->_tint[1] << " " << color->_tint[2] << " " << color->_tint[3] <<
		" | Colorize " << color->_colorize[0] << " " << color->_colorize[1] << " " << color->_colorize[2] << " " << color->_colorize[3] <<
		" | Offset " << color->_offset[0] << " " << color->_offset[1] << " " << color->_offset[2] << "]";
	lua_pushstring(L, s.str().c_str());
}

LUA_FUNCTION(Lua_Color_ToString) {
	ColorMod* mod = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	Lua_Color_ToString(L, mod);
	return 1;
}

LUA_FUNCTION(Lua_ConstColor_ToString) {
	ColorMod* mod = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::CONST_COLOR, "Color");
	Lua_Color_ToString(L, mod);
	return 1;
}

static void Lua_Color_Print(lua_State* L, ColorMod* color) {
	std::ostringstream s;
	s << "Color: " << color->_tint[0] << " " << color->_tint[1] << " " << color->_tint[2] << " " << color->_tint[3] << std::endl << 
		"Colorize: " << color->_colorize[0] << " " << color->_colorize[1] << " " << color->_colorize[2] << " " << color->_colorize[3] << std::endl <<
		"Offset: " << color->_offset[0] << " " << color->_offset[1] << " " << color->_offset[2];
	lua_getglobal(L, "print");
	lua_pushstring(L, s.str().c_str());
	lua_pcall(L, 1, 0, NULL);
}

LUA_FUNCTION(Lua_Color_Print) {
	ColorMod* mod = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	Lua_Color_Print(L, mod);
	return 0;
}

LUA_FUNCTION(Lua_ConstColor_Print) {
	ColorMod* mod = lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::CONST_COLOR, "Color");
	Lua_Color_Print(L, mod);
	return 0;
}

LUA_FUNCTION(Lua_CreateColor) {
	float array[11] = {
		// colorize
		(float)luaL_optnumber(L, 1, 1.0f), 
		(float)luaL_optnumber(L, 2, 1.0f),
		(float)luaL_optnumber(L, 3, 1.0f),
		(float)luaL_optnumber(L, 4, 1.0f),

		//offset
		(float)luaL_optnumber(L, 5, 0.0f),
		(float)luaL_optnumber(L, 6, 0.0f),
		(float)luaL_optnumber(L, 7, 0.0f),

		//tint
		(float)luaL_optnumber(L, 8, 1.0f),
		(float)luaL_optnumber(L, 9, 1.0f),
		(float)luaL_optnumber(L, 10, 1.0f),
		(float)luaL_optnumber(L, 11, 1.0f),
	};
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	memcpy(toLua, &array, sizeof(ColorMod));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::COLOR;
	lua::Metatables const_mt = lua::Metatables::CONST_COLOR;

	lua::RegisterFunction(state, mt, "GetTint", Lua_ColorGetTint);
	lua::RegisterFunction(state, mt, "GetColorize", Lua_ColorGetColorize);
	lua::RegisterFunction(state, mt, "GetOffset", Lua_ColorGetOffset);
	lua::RegisterFunction(state, mt, "__tostring", Lua_Color_ToString);
	lua::RegisterFunction(state, mt, "Print", Lua_Color_Print);

	lua::RegisterFunction(state, const_mt, "GetTint", Lua_ConstColorGetTint);
	lua::RegisterFunction(state, const_mt, "GetColorize", Lua_ConstColorGetColorize);
	lua::RegisterFunction(state, const_mt, "GetOffset", Lua_ConstColorGetOffset);
	lua::RegisterFunction(state, const_mt, "__tostring", Lua_ConstColor_ToString);
	lua::RegisterFunction(state, const_mt, "Print", Lua_ConstColor_Print);

	//lua_register(state, "Color", Lua_CreateColor); //this fucks up Game (memory corruption?)
}