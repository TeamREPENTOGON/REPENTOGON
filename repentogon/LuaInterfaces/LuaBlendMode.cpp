#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//1
LUA_FUNCTION(Lua_GetFlag1)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag1());

	return 1;
}

LUA_FUNCTION(Lua_SetFlag1)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag1() = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//2
LUA_FUNCTION(Lua_GetFlag2)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag2());

	return 1;
}

LUA_FUNCTION(Lua_SetFlag2)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag2() = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//3
LUA_FUNCTION(Lua_GetFlag3)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag3());

	return 1;
}

LUA_FUNCTION(Lua_SetFlag3)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag3() = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

//4
LUA_FUNCTION(Lua_GetFlag4)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag4());

	return 1;
}

LUA_FUNCTION(Lua_SetFlag4)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag4() = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_SetMode)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int newMode = (unsigned int)luaL_checknumber(L, 2);

	//invalid modes are simply ignored
	mode->SetMode(newMode);
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

	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_GetFlag1);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_GetFlag2);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_GetFlag3);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_GetFlag4);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	// and repeat!
	lua_pushstring(L, "__propset");
	lua_newtable(L);

	lua_pushstring(L, "Flag1");
	lua_pushcfunction(L, Lua_SetFlag1);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag2");
	lua_pushcfunction(L, Lua_SetFlag2);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag3");
	lua_pushcfunction(L, Lua_SetFlag3);
	lua_rawset(L, -3);

	lua_pushstring(L, "Flag4");
	lua_pushcfunction(L, Lua_SetFlag4);
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