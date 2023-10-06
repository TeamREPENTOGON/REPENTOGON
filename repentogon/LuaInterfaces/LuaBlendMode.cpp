#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

//1
int Lua_GetFlag1(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag1());

	return 1;
}

int Lua_SetFlag1(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag1() = (unsigned int) lua_tointeger(L, 2);
	return 0;
}

//2
int Lua_GetFlag2(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag2());

	return 1;
}

int Lua_SetFlag2(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag2() = (unsigned int) lua_tointeger(L, 2);
	return 0;
}

//3
int Lua_GetFlag3(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag3());

	return 1;
}

int Lua_SetFlag3(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag3() = (unsigned int) lua_tointeger(L, 2);
	return 0;
}

//4
int Lua_GetFlag4(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	lua_pushinteger(L, *mode->GetFlag4());

	return 1;
}

int Lua_SetFlag4(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	*mode->GetFlag4() = (unsigned int) lua_tointeger(L, 2);
	return 0;
}

int Lua_SetMode(lua_State* L)
{
	BlendMode* mode = *lua::GetUserdata<BlendMode**>(L, 1, lua::metatables::BlendModeMT);
	unsigned int newMode = (unsigned int) lua_tonumber(L, 2);

	//invalid modes are simply ignored
	mode->SetMode(newMode);
	return 0;
}

static void RegisterBlendMode(lua_State* L) {
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

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterBlendMode(state);
}