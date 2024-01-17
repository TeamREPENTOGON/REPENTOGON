#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_MinimapGetConfig)
{
	Minimap* minimap = g_Game->GetMinimap();
	MinimapConfig** toLua = (MinimapConfig**)lua_newuserdata(L, sizeof(MinimapConfig*));
	const int index = luaL_checkinteger(L, 1);
	*toLua = &minimap->_config[index];
	luaL_setmetatable(L, lua::metatables::MinimapConfigMT);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigGetSprite) {
	MinimapConfig* config = *lua::GetUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<ANM2>::push(L, lua::GetMetatableKey(lua::Metatables::SPRITE), config->_anm2);
	return 1;
}

LUA_FUNCTION(Lua_MininmapConfigGetVec1) {
	MinimapConfig* config = *lua::GetUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), config->_vec1);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetVec1) {
	MinimapConfig* config = *lua::GetUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_vec1 = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_MininmapConfigGetVec2) {
	MinimapConfig* config = *lua::GetUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), config->_vec2);
	return 1;
}

LUA_FUNCTION(Lua_MinimapConfigSetVec2) {
	MinimapConfig* config = *lua::GetUserdata<MinimapConfig**>(L, 1, lua::metatables::MinimapConfigMT);
	config->_vec2 = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

static void RegisterMinimapConfig(lua_State* L) {
	lua::RegisterGlobalClassFunction(L, "Minimap", "GetConfig", Lua_MinimapGetConfig);

	luaL_Reg functions[] = {
		{ "GetSprite", Lua_MinimapConfigGetSprite },
		{ "GetVec1", Lua_MininmapConfigGetVec1 },
		{ "SetVec1", Lua_MinimapConfigSetVec1 },
		{ "GetVec2", Lua_MininmapConfigGetVec2 },
		{ "SetVec2", Lua_MinimapConfigSetVec2 },
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