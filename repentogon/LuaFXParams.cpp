#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GetFXParams(lua_State* L) {
	Room* room = lua::GetUserdata<Room*>(L, 1, lua::Metatables::ROOM, "Room");
	FXParams** ud = (FXParams**)lua_newuserdata(L, sizeof(FXParams*));
	*ud = room->GetFXParams();
	luaL_setmetatable(L, lua::metatables::FXParamsMT);
	return 1;
}

int Lua_GetColorModifier(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorModState* color = params->GetColorModifier();
	ColorModState* toLua = (ColorModState*)lua_newuserdata(L, sizeof(ColorModState));
	luaL_setmetatable(L, lua::metatables::ColorModifierMT);
	memcpy(toLua, color, sizeof(ColorModState));

	return 1;
}

int Lua_SetColorModifier(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetColorModifier() = *lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorModifierMT);

	return 0;
}

int Lua_GetWaterV2(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushboolean(L, *params->GetWaterV2());

	return 1;
}

int Lua_SetWaterV2(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterV2() = lua_toboolean(L, 2);
	return 0;
}

int Lua_GetWaterColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetWaterColor();

	return 1;
}

int Lua_SetWaterColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

int Lua_GetWaterColorMultiplier(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetWaterColorMultiplier();

	return 1;
}

int Lua_SetWaterColorMultiplier(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterColorMultiplier() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

int Lua_GetShadowAlpha(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushnumber(L, *params->GetShadowAlpha());

	return 1;
}

int Lua_SetShadowAlpha(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetShadowAlpha() = lua_tonumber(L, 2);
	return 0;
}

int Lua_GetShadowColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetShadowColor();

	return 1;
}

int Lua_SetShadowColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetShadowColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

int Lua_GetLightColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetLightColor();

	return 1;
}

int Lua_SetLightColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetLightColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}


int Lua_GetWaterEffectColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *params->GetWaterEffectColor();

	return 1;
}

int Lua_SetWaterEffectColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetWaterEffectColor() = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

static void RegisterFXParams(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ROOM, "GetFXParams", Lua_GetFXParams);

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
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterFXParams(state);
}