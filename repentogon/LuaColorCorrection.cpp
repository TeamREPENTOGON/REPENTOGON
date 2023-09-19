#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//#include "Log.h"

// R
int Lua_GetColor(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *color->GetColor();

	return 1;
}

int Lua_SetColor(lua_State* L)
{
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	*color->GetColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
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

int Lua_ColorCorrectionUpdate(lua_State* L)
{
	//ZHL::Logger logger;
	//logger.Log("doing ColorCorrection():Update");
	ColorModState* color = *lua::GetUserdata<ColorModState**>(L, 1, lua::metatables::ColorCorrectionMT);
	bool process = true;
	if lua_isboolean(L, 2)
		process = lua_toboolean(L, 2);
	bool lerp = false;
	if lua_isboolean(L, 3)
		lerp = lua_toboolean(L, 3);
	float rate = (float)luaL_optnumber(L, 4, 0.015);

	//logger.Log("process is %s, lerp is %s, rate is %f\n", process ? "TRUE" : "FALSE", lerp ? "TRUE" : "FALSE", rate);

	ColorModState current;
	if (process) {
		//logger.Log("trying Room():ComputeColorModifier()");
		Room* room = *g_Game->GetCurrentRoom();
		current = room->ComputeColorModifier();
		color = &current;
	}
	//logger.Log("trying Game():SeteColorModifier(color, %s, %f)", lerp ? "true" : "false", rate);
	g_Game->SetColorModifier(color, lerp, rate);
	return 0;
}

static void RegisterColorCorrection(lua_State* L) {

	luaL_newmetatable(L, lua::metatables::ColorCorrectionMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
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

	lua_pushstring(L, "Color");
	lua_pushcfunction(L, Lua_GetColor);
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

	lua_pushstring(L, "Color");
	lua_pushcfunction(L, Lua_SetColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Brightness");
	lua_pushcfunction(L, Lua_SetBrightness);
	lua_rawset(L, -3);

	lua_pushstring(L, "Contrast");
	lua_pushcfunction(L, Lua_SetContrast);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	luaL_Reg functions[] = {
	{ "Update", Lua_ColorCorrectionUpdate },
	{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterColorCorrection(state);
}