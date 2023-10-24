#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* BeamRendererMT = "BeamRenderer";

LUA_FUNCTION(Lua_BeamRendererBegin) {
	int top = lua_gettop(L);
	if (top < 4) {
		luaL_error(L, "Expected 4 arguments, got %d", top);
	}
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const int layerID = (const int)luaL_checkinteger(L, 2);
	const unsigned int layerCount = sprite->GetLayerCount();
	if (layerID < 0 || (const unsigned int)layerID > layerCount) {
		luaL_argerror(L, 2, "Invalid layer id");
	}

	bool unk1 = lua_toboolean(L, 3);
	bool unk2 = lua_toboolean(L, 4);

	g_BeamRenderer->Begin(sprite, layerID, unk1, unk2);
	
	return 0;
}

LUA_FUNCTION(Lua_BeamRendererAdd) {
	Vector* point = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float unk1 = (float)luaL_checknumber(L, 2);
	float unk2 = (float)luaL_checknumber(L, 3);
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 4, lua::Metatables::COLOR, "Color");

	// args are shuffled around for ease of xmm register accomodation
	g_BeamRenderer->Add(point, color, unk1, unk2);

	return 0;
}

LUA_FUNCTION(Lua_BeamRendererEnd) {
	g_BeamRenderer->End();

	return 0;
}

static void RegisterBeamRenderer(lua_State* L) {
	lua_newtable(L);
	lua::TableAssoc(L, "Begin", Lua_BeamRendererBegin);
	lua::TableAssoc(L, "Add", Lua_BeamRendererAdd);
	lua::TableAssoc(L, "End", Lua_BeamRendererEnd);
	lua_setglobal(L, BeamRendererMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterBeamRenderer(state);
}

