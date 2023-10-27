#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CreateColorModifier) {
	ColorModState color;
	color.r = (float)luaL_optnumber(L, 1, 1.0f);
	color.g = (float)luaL_optnumber(L, 2, 1.0f);
	color.b = (float)luaL_optnumber(L, 3, 1.0f);
	color.a = (float)luaL_optnumber(L, 4, 0.0f);
	color.brightness = (float)luaL_optnumber(L, 5, 0.0f);
	color.contrast = (float)luaL_optnumber(L, 6, 1.0f);
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, &color, sizeof(ColorModState));
	return 1;
}

// R
LUA_FUNCTION(Lua_GetR)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetR());

	return 1;
}

LUA_FUNCTION(Lua_SetR)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetR() = (float)luaL_checknumber(L, 2);
	return 0;
}

// G
LUA_FUNCTION(Lua_GetG)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetG());

	return 1;
}

LUA_FUNCTION(Lua_SetG)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetG() = (float)luaL_checknumber(L, 2);
	return 0;
}

// B
LUA_FUNCTION(Lua_GetB)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetB());

	return 1;
}

LUA_FUNCTION(Lua_SetB)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetB() = (float)luaL_checknumber(L, 2);
	return 0;
}

// A
LUA_FUNCTION(Lua_GetA)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetA());

	return 1;
}

LUA_FUNCTION(Lua_SetA)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetA() = (float)luaL_checknumber(L, 2);
	return 0;
}

// Brightness
LUA_FUNCTION(Lua_GetBrightness)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetBrightness());

	return 1;
}

LUA_FUNCTION(Lua_SetBrightness)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetBrightness() = (float)luaL_checknumber(L, 2);
	return 0;
}

// Contrast
LUA_FUNCTION(Lua_GetContrast)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	lua_pushnumber(L, *color->GetContrast());

	return 1;
}

LUA_FUNCTION(Lua_SetContrast)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	*color->GetContrast() = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_ColorModState__eq)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	ColorModState* other = lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);

	lua_pushboolean(L, (*color == *other));

	return 1;
}

LUA_FUNCTION(Lua_ColorModState__add)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	ColorModState* other = lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);
	ColorModState result = *color + *other;
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, &result, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_ColorModState__sub)
{
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	ColorModState* other = lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);
	ColorModState result = *color - *other;
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, &result, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_ColorModState__mul) {
	int t1 = lua_type(L, 1);
	int t2 = lua_type(L, 2);

	if (t1 == LUA_TNUMBER) {
		return luaL_error(L, "Cannot left multiply a ColorModifier with a number");
	}
	else if (t2 == LUA_TNUMBER) {
		ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
		ColorModState result = *color * luaL_checknumber(L, 2);
		ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
		luaL_setmetatable(L, lua::metatables::ColorModifierMT);
		memcpy(toLua, &result, sizeof(ColorModState));
	}

	return 1;
}

LUA_FUNCTION(Lua_ColorModState__div) {
	int t1 = lua_type(L, 1);
	int t2 = lua_type(L, 2);

	if (t1 == LUA_TNUMBER) {
		return luaL_error(L, "Cannot left divide a ColorModifier with a number");
	}
	else if (t2 == LUA_TNUMBER) {
		float div = (float)luaL_checknumber(L, 2);
		if (div == 0) {
			return luaL_error(L, "Divide by zero");
		}
		ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
		ColorModState result = *color / div;
		ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
		luaL_setmetatable(L, lua::metatables::ColorModifierMT);
		memcpy(toLua, &result, sizeof(ColorModState));
	}

	return 1;
}

LUA_FUNCTION(Lua_ColorModState__gc) {
	ColorModState* color = lua::GetUserdata<ColorModState*>(L, 1, lua::metatables::ColorModifierMT);
	color->~ColorModState();

	return 0;
}

static void RegisterColorModifier(lua_State* L) {

	luaL_newmetatable(L, lua::metatables::ColorModifierMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__name");
	lua_pushstring(L, "ColorModifier");
	lua_rawset(L, -3);

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

	lua_register(L, lua::metatables::ColorModifierMT, Lua_CreateColorModifier);

	luaL_Reg functions[] = {
	{ "__eq",  Lua_ColorModState__eq  },
	{ "__add", Lua_ColorModState__add },
	{ "__sub", Lua_ColorModState__sub },
	{ "__mul", Lua_ColorModState__mul },
	{ "__div", Lua_ColorModState__div },
	{ "__gc",  Lua_ColorModState__gc  },
	{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterColorModifier(state);
}