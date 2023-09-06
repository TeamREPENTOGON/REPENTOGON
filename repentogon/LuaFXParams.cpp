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

// Unknown bool
int Lua_GetUnkBool(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushboolean(L, *params->GetUnknownBool());

	return 1;
}

int Lua_SetUnkBool(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetUnknownBool() = lua_toboolean(L, 2);
	return 0;
}

// Unknown KColor 1
int Lua_GetUnknownKColor1(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetUnknownKColor1();

	return 1;
}

int Lua_SetUnknownKColor1(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetUnknownKColor1() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

// Unknown KColor 2
int Lua_GetUnknownKColor2(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetUnknownKColor2();

	return 1;
}

int Lua_SetUnknownKColor2(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetUnknownKColor2() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

// Room lighting alpha
int Lua_GetRoomLightingAlpha(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	lua_pushnumber(L, *params->GetRoomLightingAlpha());

	return 1;
}

int Lua_SetRoomLightingAlpha(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetRoomLightingAlpha() = lua_tonumber(L, 2);
	return 0;
}

// Room lighting KColor
int Lua_GetRoomLightingKColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetRoomLightingKColor();

	return 1;
}

int Lua_SetRoomLightingKColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetRoomLightingKColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

// FX lighting KColor
int Lua_GetFXLightingKColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	KColor* toLua = lua::luabridge::UserdataValue<KColor>::place(L, lua::GetMetatableKey(lua::Metatables::KCOLOR));
	*toLua = *params->GetFXLightingKColor();

	return 1;
}

int Lua_SetFXLightingKColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetFXLightingKColor() = *lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");

	return 0;
}

// Unknown Color
int Lua_GetUnknownColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *params->GetUnknownColor();

	return 1;
}

int Lua_SetUnknownColor(lua_State* L)
{
	FXParams* params = *lua::GetUserdata<FXParams**>(L, 1, lua::metatables::FXParamsMT);
	*params->GetUnknownColor() = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

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
		{ "GetUnknownBool", Lua_GetUnkBool },
		{ "SetUnknownBool", Lua_SetUnkBool },
		{ "GetUnknownKColor1", Lua_GetUnknownKColor1 },
		{ "SetUnknownKColor1", Lua_SetUnknownKColor1 },
		{ "GetUnknownKColor2", Lua_GetUnknownKColor2 },
		{ "SetUnknownKColor2", Lua_SetUnknownKColor2 },
		{ "GetRoomLightingAlpha", Lua_GetRoomLightingAlpha },
		{ "SetRoomLightingAlpha", Lua_SetRoomLightingAlpha },
		{ "GetRoomLightingKColor", Lua_GetRoomLightingKColor },
		{ "SetRoomLightingKColor", Lua_SetRoomLightingKColor },
		{ "GetFXLightingKColor", Lua_GetFXLightingKColor },
		{ "SetFXLightingKColor", Lua_SetFXLightingKColor },
		{ "GetUnknownColor", Lua_GetUnknownColor },
		{ "SetUnknownColor", Lua_SetUnknownColor },
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