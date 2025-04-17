#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetFXParams) {
	Room* room = lua::GetLuabridgeUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	FXParams** ud = (FXParams**)lua_newuserdata(L, sizeof(FXParams*));
	*ud = room->GetFXParams();
	luaL_setmetatable(L, lua::metatables::FXParamsMT);
	return 1;
}

LUA_FUNCTION(Lua_GetColorModifier)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	// It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
	// This logic provides backwards compatability.
	ColorModState color(params->roomColor._red, params->roomColor._green, params->roomColor._blue, params->roomColor._alpha, params->brightness, params->contrast);
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, &color, sizeof(ColorModState));

	return 1;
}

LUA_FUNCTION(Lua_SetColorModifier)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	// It was discovered in rep+ that FXParams does not actually contain a ColorModState, its KColor+floats, and KColor gained a new field.
	// This logic provides backwards compatability.
	ColorModState luacolor = *lua::GetRawUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);
	params->roomColor._red = luacolor.r;
	params->roomColor._green = luacolor.g;
	params->roomColor._blue = luacolor.b;
	params->roomColor._alpha = luacolor.a;
	params->brightness = luacolor.brightness;
	params->contrast = luacolor.contrast;

	return 0;
}

LUA_FUNCTION(Lua_GetWaterV2)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushboolean(L, *params->GetWaterV2());

	return 1;
}

LUA_FUNCTION(Lua_SetWaterV2)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterV2() = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetWaterColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetWaterColor();

	return 1;
}

LUA_FUNCTION(Lua_SetWaterColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterColor() = *lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

LUA_FUNCTION(Lua_GetWaterColorMultiplier)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetWaterColorMultiplier();

	return 1;
}

LUA_FUNCTION(Lua_SetWaterColorMultiplier)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterColorMultiplier() = *lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

LUA_FUNCTION(Lua_GetShadowAlpha)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushnumber(L, *params->GetShadowAlpha());

	return 1;
}

LUA_FUNCTION(Lua_SetShadowAlpha)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetShadowAlpha() = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetShadowColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetShadowColor();

	return 1;
}

LUA_FUNCTION(Lua_SetShadowColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetShadowColor() = *lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

LUA_FUNCTION(Lua_GetLightColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetLightColor();

	return 1;
}

LUA_FUNCTION(Lua_SetLightColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetLightColor() = *lua::GetLuabridgeUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}


LUA_FUNCTION(Lua_GetWaterEffectColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *params->GetWaterEffectColor();

	return 1;
}

LUA_FUNCTION(Lua_SetWaterEffectColor)
{
	FXParams* params = *lua::GetRawUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterEffectColor() = *lua::GetLuabridgeUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

static void RegisterFXParams(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetFXParams", Lua_GetFXParams);

	// manual creation of FXParams class to ensure __newindex definition and variable additions work correctly
	luaL_newmetatable(L, lua::metatables::FXParamsMT);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua::luabridge::indexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua::luabridge::newIndexMetaMethod);
	lua_rawset(L, -3);

	lua_pushstring(L, "__propget");
	lua_newtable(L);

	lua_pushstring(L, "ColorModifier");
	lua_pushcfunction(L, Lua_GetColorModifier);
	lua_rawset(L, -3);

	lua_pushstring(L, "UseWaterV2");
	lua_pushcfunction(L, Lua_GetWaterV2);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterColor");
	lua_pushcfunction(L, Lua_GetWaterColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterColorMultiplier");
	lua_pushcfunction(L, Lua_GetWaterColorMultiplier);
	lua_rawset(L, -3);

	lua_pushstring(L, "ShadowAlpha");
	lua_pushcfunction(L, Lua_GetShadowAlpha);
	lua_rawset(L, -3);

	lua_pushstring(L, "ShadowColor");
	lua_pushcfunction(L, Lua_GetShadowColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "LightColor");
	lua_pushcfunction(L, Lua_GetLightColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterEffectColor");
	lua_pushcfunction(L, Lua_GetWaterEffectColor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	// and repeat!
	lua_pushstring(L, "__propset");
	lua_newtable(L);

	lua_pushstring(L, "ColorModifier");
	lua_pushcfunction(L, Lua_SetColorModifier);
	lua_rawset(L, -3);

	lua_pushstring(L, "UseWaterV2");
	lua_pushcfunction(L, Lua_SetWaterV2);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterColor");
	lua_pushcfunction(L, Lua_SetWaterColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterColorMultiplier");
	lua_pushcfunction(L, Lua_SetWaterColorMultiplier);
	lua_rawset(L, -3);

	lua_pushstring(L, "ShadowAlpha");
	lua_pushcfunction(L, Lua_SetShadowAlpha);
	lua_rawset(L, -3);

	lua_pushstring(L, "ShadowColor");
	lua_pushcfunction(L, Lua_SetShadowColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "LightColor");
	lua_pushcfunction(L, Lua_SetLightColor);
	lua_rawset(L, -3);

	lua_pushstring(L, "WaterEffectColor");
	lua_pushcfunction(L, Lua_SetWaterEffectColor);
	lua_rawset(L, -3);

	lua_rawset(L, -3);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterFXParams(_state);
}