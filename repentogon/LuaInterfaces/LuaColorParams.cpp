#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ColorParamsConstructor) {
	ColorParams color;
	color._color = *lua::GetUserdata<ColorMod*>(L, 1, lua::Metatables::COLOR, "Color");
	color._priority = (int)luaL_checkinteger(L, 2);
	color._duration1 = (int)luaL_checkinteger(L, 3);
	color._duration2 = (int)luaL_checkinteger(L, 3);
	color._fadeout = lua::luaL_checkboolean(L, 4);
	color._shared = lua::luaL_checkboolean(L, 5);
	ColorParams* toLua = (ColorParams*)lua_newuserdata(L, sizeof(ColorParams));
	luaL_setmetatable(L, lua::metatables::ColorParamsMT);
	*toLua = color;
	return 1;
}

LUA_FUNCTION(Lua_ColorParamsGetColorMod)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = color->_color;

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetColorMod)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_color = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

LUA_FUNCTION(Lua_ColorParamsGetPriority)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	lua_pushinteger(L, color->_priority);

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetPriority)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_priority = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_ColorParamsGetDuration)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	lua_pushinteger(L, color->_duration1);

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetDuration)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_duration1 = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_ColorParamsGetLifespan)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	lua_pushinteger(L, color->_duration2);

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetLifespan)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_duration2 = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_ColorParamsGetFadeout)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	lua_pushboolean(L, color->_fadeout);

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetFadeout)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_fadeout = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_ColorParamsGetShared)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	lua_pushboolean(L, color->_shared);

	return 1;
}

LUA_FUNCTION(Lua_ColorParamsSetShared)
{
	ColorParams* color = lua::GetRawUserdata<ColorParams*>(L, 1, lua::metatables::ColorParamsMT);
	color->_shared = lua::luaL_checkboolean(L, 2);

	return 0;
}

static void RegisterColorParams(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetColor", Lua_ColorParamsGetColorMod },
		{ "SetColor", Lua_ColorParamsSetColorMod },
		{ "GetPriority", Lua_ColorParamsGetPriority },
		{ "SetPriority", Lua_ColorParamsSetPriority },
		{ "GetDuration", Lua_ColorParamsGetDuration },
		{ "SetDuration", Lua_ColorParamsSetDuration },
		{ "GetLifespan", Lua_ColorParamsGetLifespan },
		{ "SetLifespan", Lua_ColorParamsSetLifespan },
		{ "GetFadeout", Lua_ColorParamsGetFadeout },
		{ "SetFadeout", Lua_ColorParamsSetFadeout },
		{ "GetShared", Lua_ColorParamsGetShared },
		{ "SetShared", Lua_ColorParamsSetShared },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::ColorParamsMT, lua::metatables::ColorParamsMT, functions);
	lua_register(L, lua::metatables::ColorParamsMT, Lua_ColorParamsConstructor);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterColorParams(state);
}