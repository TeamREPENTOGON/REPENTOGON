#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_CreateBeamDummy) {
	int top = lua_gettop(L);
	if (top < 4) {
		luaL_error(L, "Expected 4 arguments, got %d", top);
	}
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const int layerID = (const int)luaL_checkinteger(L, 2);
	const unsigned int layerCount = sprite->GetLayerCount();
	if (layerID < 0 || (const unsigned int)layerID + 1 > layerCount) {
		luaL_argerror(L, 2, "Invalid layer id");
	}

	bool useOverlay = lua_toboolean(L, 3);
	bool unk = lua_toboolean(L, 4);

	if ((unk && sprite->_overlayAnimState._animData == nullptr) || sprite->_animState._animData == nullptr) {
		luaL_error(L, "%sAnimState is NULL!", useOverlay ? "Overlay " : "");
	}

	BeamRenderer* toLua = lua::place<BeamRenderer>(L, lua::metatables::BeamRendererMT, sprite, layerID, useOverlay, unk);
	luaL_setmetatable(L, lua::metatables::BeamRendererMT);
	return 1;
}

LUA_FUNCTION(Lua_BeamBegin) {
	int top = lua_gettop(L);
	if (top < 4) {
		luaL_error(L, "Expected 4 arguments, got %d", top);
	}
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const int layerID = (const int)luaL_checkinteger(L, 2);
	const unsigned int layerCount = sprite->GetLayerCount();
	if (layerID < 0 || (const unsigned int)layerID + 1 > layerCount) {
		luaL_argerror(L, 2, "Invalid layer id");
	}

	bool unk1 = lua_toboolean(L, 3);
	bool unk2 = lua_toboolean(L, 4);

	g_BeamRenderer->Begin(sprite, layerID, unk1, unk2);

	return 0;
}

LUA_FUNCTION(Lua_BeamAdd) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	float heightMod = (float)luaL_checknumber(L, 3);
	float widthMod = (float)luaL_checknumber(L, 4);
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");

	Point point(pos, Vector(0, 0), heightMod, widthMod, color);
	beam->_points.push_back(point);

	return 0;
}

LUA_FUNCTION(Lua_BeamRender) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);

	g_BeamRenderer->Begin(beam->_anm2, beam->_layer, beam->_useOverlayData, beam->_unkBool);

	for (auto it = beam->_points.begin(); it != beam->_points.end(); ++it) {
		g_BeamRenderer->Add(it->_pos, it->_color, it->_heightMod, it->_widthMod);
	}

	g_BeamRenderer->End();

	return 0;
}

LUA_FUNCTION(Lua_BeamRenderer__gc) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	beam->~BeamRenderer();

	return 0;
}

static void RegisterBeamRenderer(lua_State* L) {
	luaL_Reg functions[] = {
		{ "Add", Lua_BeamAdd},
		{ "Render", Lua_BeamRender},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::NullFrameMT, lua::metatables::BeamRendererMT, functions, Lua_BeamRenderer__gc);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBeamRenderer(_state);
}