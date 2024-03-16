#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


//1
LUA_FUNCTION(Lua_GetRGBSourceFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_rgbSourceFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetRGBSourceFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	mode->_rgbSourceFactor = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//2
LUA_FUNCTION(Lua_GetRGBDestinationFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_rgbDestFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetRGBDestinationFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	mode->_rgbDestFactor = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//3
LUA_FUNCTION(Lua_GetAlphaSourceFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_alphaSourceFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetAlphaSourceFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	mode->_alphaSourceFactor = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//4
LUA_FUNCTION(Lua_GetAlphaDestinationFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, mode->_alphaDestFactor);

	return 1;
}

LUA_FUNCTION(Lua_SetAlphaDestinationFactor)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	mode->_alphaDestFactor = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

const BlendMode modes[5] = {
	BlendMode(1, 0, 1, 0),
	BlendMode(1, 7, 1, 7),
	BlendMode(1, 1, 1, 1),
	BlendMode(0, 2, 0, 2),
	BlendMode(4, 7, 4, 7)
};

LUA_FUNCTION(Lua_SetMode)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	if (lua_gettop(L) > 2) {
		mode->_rgbSourceFactor = (int)luaL_optnumber(L, 2, mode->_rgbSourceFactor);
		mode->_rgbDestFactor = (int)luaL_optnumber(L, 3, mode->_rgbDestFactor);
		mode->_alphaSourceFactor = (int)luaL_optnumber(L, 4, mode->_alphaSourceFactor);
		mode->_alphaDestFactor = (int)luaL_optnumber(L, 5, mode->_alphaDestFactor);
	}
	else
	{
		int newMode = (int)luaL_checknumber(L, 2);

		//invalid modes are simply ignored
		if (newMode >= 0 && newMode <= 5) {
			*mode = modes[newMode];
		}
	}

	return 0;
}

static void RegisterBlendMode(lua_State* L) {

	// manual creation of BlendMode class to ensure __newindex definition and variable additions work correctly
	luaL_newmetatable(L, lua::metatables::BlendModeMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__name");
	lua_pushstring(L, "BlendMode");
	lua_rawset(L, -3);

	lua_pushstring(L, "__propget");
	lua_newtable(L);

	lua_pushstring(L, "RGBSourceFactor");
	lua_pushcfunction(L, Lua_GetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "RGBDestinationFactor");
	lua_pushcfunction(L, Lua_GetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaSourceFactor");
	lua_pushcfunction(L, Lua_GetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaDestinationFactor");
	lua_pushcfunction(L, Lua_GetAlphaDestinationFactor);
	lua_rawset(L, -3);

	// depreciated
	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_GetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_GetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_GetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_GetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	// and repeat!
	lua_pushstring(L, "__propset");
	lua_newtable(L);

	lua_pushstring(L, "RGBSourceFactor");
	lua_pushcfunction(L, Lua_SetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "RGBDestinationFactor");
	lua_pushcfunction(L, Lua_SetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaSourceFactor");
	lua_pushcfunction(L, Lua_SetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "AlphaDestinationFactor");
	lua_pushcfunction(L, Lua_SetAlphaDestinationFactor);
	lua_rawset(L, -3);

	// depreciated
	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_SetRGBSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_SetRGBDestinationFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_SetAlphaSourceFactor);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_SetAlphaDestinationFactor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	luaL_Reg functions[] = {
	{ "SetMode", Lua_SetMode },
	{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBlendMode(_state);
}