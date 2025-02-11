#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_MinimapGetConfig)
{
	Minimap* minimap = g_Game->GetMinimap();
	MinimapConfig** toLua = (MinimapConfig**)lua_newuserdata(L, sizeof(MinimapConfig*));
	const int index = (int)luaL_checkinteger(L, 1);
	*toLua = &minimap->_config[index];
	luaL_setmetatable(L, lua::metatables::MinimapConfigMT);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigGetSprite) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<ANM2>::push(L, lua::GetMetatableKey(lua::Metatables::SPRITE), config->_mapSprite);
	return 1;
}

LUA_FUNCTION(Lua_MininmapConfigGetVec1) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), config->_vec1);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetVec1) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_vec1 = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetBorderPadding) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), config->_borderPadding);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetBorderPadding) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_borderPadding = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetPosOffsetX) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua_pushnumber(L, config->_posOffsetX);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetPosOffsetX) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_posOffsetX = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetBorderScale) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua_pushnumber(L, config->_borderScale);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetBorderScale) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_borderScale = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetI1) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua_pushinteger(L, config->_i1);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetI1) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_i1 = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetI2) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua_pushinteger(L, config->_i2);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetI2) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_i2 = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MinimapConfigGetIconNum) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua_pushinteger(L, config->_minimapIconNum);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetIconNum) {
	MinimapConfig* config = *lua::GetRawUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_minimapIconNum = (int)luaL_checkinteger(L, 2);
	return 0;
}

static void RegisterMinimapConfig(lua_State* L) {
	//lua::RegisterGlobalClassFunction(L, "Minimap", "GetConfig", Lua_MinimapGetConfig);

	luaL_Reg functions[] = {
		{ "GetSprite", Lua_MinimapConfigGetSprite },
		{ "GetVec1", Lua_MininmapConfigGetVec1 },
		{ "SetVec1", Lua_MinimapConfigSetVec1 },
		{ "GetBorderPadding", Lua_MinimapConfigGetBorderPadding },
		{ "SetBorderPadding", Lua_MinimapConfigSetBorderPadding },
		{ "GetPosOffsetX", Lua_MinimapConfigGetPosOffsetX },
		{ "SetPosOffsetX", Lua_MinimapConfigSetPosOffsetX },
		{ "GetBorderScale", Lua_MinimapConfigGetBorderScale },
		{ "SetBorderScale", Lua_MinimapConfigSetBorderScale },
		{ "GetI1", Lua_MinimapConfigGetI1 },
		{ "SetI1", Lua_MinimapConfigSetI1 },
		{ "GetI2", Lua_MinimapConfigGetI2 },
		{ "SetI2", Lua_MinimapConfigSetI2 },
		{ "GetIconNum", Lua_MinimapConfigGetIconNum },
		{ "SetIconNum", Lua_MinimapConfigSetIconNum },
		//{ "GetPlayer", Lua_PlayerHUDGetPlayer },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::MinimapConfigMT, lua::metatables::MinimapConfigMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterMinimapConfig(_state);
}