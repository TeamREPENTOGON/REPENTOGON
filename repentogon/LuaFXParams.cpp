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

// Color correction
int Lua_GetColorCorrection(lua_State* L)
{	
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorModState* color = params->GetColorCorrection();
	ColorModState** toLua = (ColorModState**)lua_newuserdata(L, sizeof(ColorModState*));
	*toLua = color;
	luaL_setmetatable(L, lua::metatables::ColorCorrectionMT);

	return 1;
}

int Lua_SetColorCorrection(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetColorCorrection() = *lua::GetUserdata<ColorModState*>(L, 2, lua::metatables::ColorCorrectionMT);

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
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetColorCorrection", Lua_GetColorCorrection },
		{ "SetColorCorrection", Lua_SetColorCorrection },
		{ "GetWaterV2", Lua_GetWaterV2 },
		{ "SetWaterV2", Lua_SetWaterV2 },
		{ "GetWaterColor", Lua_GetWaterColor },
		{ "SetWaterColor", Lua_SetWaterColor },
		{ "GetWaterColorMultiplier", Lua_GetWaterColorMultiplier },
		{ "SetWaterColorMultiplier", Lua_SetWaterColorMultiplier },
		{ "GetShadowAlpha", Lua_GetShadowAlpha },
		{ "SetShadowAlpha", Lua_SetShadowAlpha },
		{ "GetShadowColor", Lua_GetShadowColor },
		{ "SetShadowColor", Lua_SetShadowColor },
		{ "GetLightColor", Lua_GetLightColor },
		{ "SetLightColor", Lua_SetLightColor },
		{ "GetWaterEffectColor", Lua_GetWaterEffectColor },
		{ "SetWaterEffectColor", Lua_SetWaterEffectColor },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterFXParams(state);
}