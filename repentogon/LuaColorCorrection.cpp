#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_FXParamsGetColorCorrection(lua_State* L) {
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorModState** ud = (ColorModState**)lua_newuserdata(L, sizeof(ColorModState*));
	*ud = params->GetColorCorrection();
	luaL_setmetatable(L, lua::metatables::ColorCorrectionMT);
	return 1;
}

// R
int Lua_GetR(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetR());

	return 1;
}

int Lua_SetR(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetR() = lua_tonumber(L, 2);
	return 0;
}

// G
int Lua_GetG(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetG());

	return 1;
}

int Lua_SetG(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetG() = lua_tonumber(L, 2);
	return 0;
}

// B
int Lua_GetB(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetB());

	return 1;
}

int Lua_SetB(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetB() = lua_tonumber(L, 2);
	return 0;
}

// A
int Lua_GetA(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetA());

	return 1;
}

int Lua_SetA(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetA() = lua_tonumber(L, 2);
	return 0;
}

// Brightness
int Lua_GetBrightness(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetBrightness());

	return 1;
}

int Lua_SetBrightness(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetBrightness() = lua_tonumber(L, 2);
	return 0;
}

// Contrast
int Lua_GetContrast(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	lua_pushnumber(L, *color->GetContrast());

	return 1;
}

int Lua_SetContrast(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetContrast() = lua_tonumber(L, 2);
	return 0;
}

static void RegisterColorCorrection(lua_State* L) {

	luaL_newmetatable(L, lua::metatables::ColorCorrectionMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	/*
		from http://lua-users.org/wiki/SimpleLuaApiExample, to a future nami

		* To put values into the table, we first push the index, then the
		* value, and then call lua_rawset() with the index of the table in the
		* stack. Let's see why it's -3: In Lua, the value -1 always refers to
		* the top of the stack. When you create the table with lua_newtable(),
		* the table gets pushed into the top of the stack. When you push the
		* index and then the cell value, the stack looks like:
		*
		* <- [stack bottom] -- table, index, value [top]
		*
		* So the -1 will refer to the cell value, thus -3 is used to refer to
		* the table itself. Note that lua_rawset() pops the two last elements
		* of the stack, so that after it has been called, the table is at the
		* top of the stack.
	*/
	lua_pushstring(L, "__propget");
	lua_newtable(L);

	lua_pushstring(L, "R");
	lua_pushcfunction(L, Lua_GetR);
	lua_rawset(L, -3);

	lua_pushstring(L, "G");
	lua_pushcfunction(L, Lua_GetG);
	lua_rawset(L, -3);

	lua_pushstring(L, "B");
	lua_pushcfunction(L, Lua_GetB);
	lua_rawset(L, -3);

	lua_pushstring(L, "A");
	lua_pushcfunction(L, Lua_GetA);
	lua_rawset(L, -3);

	lua_pushstring(L, "Brightness");
	lua_pushcfunction(L, Lua_GetBrightness);
	lua_rawset(L, -3);

	lua_pushstring(L, "Contrast");
	lua_pushcfunction(L, Lua_GetContrast);
	lua_rawset(L, -3);

	lua_rawset(L, -3);
	lua_rawset(L, -3);

	// and repeat!
	lua_pushstring(L, "__propset");
	lua_newtable(L);

	lua_pushstring(L, "R");
	lua_pushcfunction(L, Lua_SetR);
	lua_rawset(L, -3);

	lua_pushstring(L, "G");
	lua_pushcfunction(L, Lua_SetG);
	lua_rawset(L, -3);

	lua_pushstring(L, "B");
	lua_pushcfunction(L, Lua_SetB);
	lua_rawset(L, -3);

	lua_pushstring(L, "A");
	lua_pushcfunction(L, Lua_SetA);
	lua_rawset(L, -3);

	lua_pushstring(L, "Brightness");
	lua_pushcfunction(L, Lua_SetBrightness);
	lua_rawset(L, -3);

	lua_pushstring(L, "Contrast");
	lua_pushcfunction(L, Lua_SetContrast);
	lua_rawset(L, -3);

	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterColorCorrection(state);
}