#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

constexpr auto POINT_VECTOR_SIZE = 8;

LUA_FUNCTION(Lua_CreateBeamDummy) {
	int top = lua_gettop(L);
	if (top < 4) {
		luaL_error(L, "Expected at least 4 arguments, got %d", top);
	}
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const int layerID = (const int)luaL_checkinteger(L, 2);
	bool useOverlay = lua_toboolean(L, 3);
	bool unk = lua_toboolean(L, 4);
	const int vectorSize = (const int)luaL_optinteger(L, 5, POINT_VECTOR_SIZE);

	BeamRenderer* toLua = lua::place<BeamRenderer>(L, lua::metatables::BeamRendererMT, layerID, useOverlay, unk);
	toLua->_anm2.construct_from_copy(sprite);
	toLua->_points.reserve(vectorSize);
	luaL_setmetatable(L, lua::metatables::BeamRendererMT);
	return 1;
}

LUA_FUNCTION(Lua_BeamAdd) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	float heightMod = (float)luaL_checknumber(L, 3);
	float widthMod = (float)luaL_checknumber(L, 4);
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");

	Point point(*pos, heightMod, widthMod, *color);
	beam->_points.push_back(point);

	return 0;
}

const char* errors[3] = {
	"AnimState is NULL!",
	"Overlay AnimState is NULL!",
	"Invalid layer id"
};

LUA_FUNCTION(Lua_BeamRender) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	bool clearPoints = true;
	byte error = 0;

	if (lua_isboolean(L, 2)) {
		clearPoints = lua_toboolean(L, 2);
	}

	if (beam->_useOverlayData) {
		if (beam->_anm2._overlayAnimState._animData == nullptr) {
			error = 1;
			goto funcEnd;
		}
	}
	else if (beam->_anm2._animState._animData == nullptr) {
		error = 2;
		goto funcEnd;
	}
	
	if (beam->_layer < 0 || (const unsigned int)beam->_layer + 1 > beam->_anm2.GetLayerCount()) {
		error = 3;
		goto funcEnd;
	}

	g_BeamRenderer->Begin(beam->GetANM2(), beam->_layer, beam->_useOverlayData, beam->_unkBool);

	for (auto it = beam->_points.begin(); it != beam->_points.end(); ++it) {
		g_BeamRenderer->Add(&it->_pos, &it->_color, it->_heightMod, it->_widthMod);
	}

	g_BeamRenderer->End();

	// i'm doing this specifically so the points vector gets cleared if needed
	funcEnd:

	if (clearPoints) {
		beam->_points.clear();
	}

	if (error != 0) {
		return luaL_error(L, errors[error-1]);
	}

	return 0;
}

LUA_FUNCTION(Lua_BeamGetSprite) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	ANM2* anm2 = beam->GetANM2();
	if (anm2 == nullptr) {
		return luaL_error(L, "Beam Sprite is NULL!");
	}
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_BeamSetSprite) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 2, lua::Metatables::SPRITE, "Sprite");
	beam->_anm2.destructor();
	beam->_anm2.construct_from_copy(anm2);
	return 0;

}

LUA_FUNCTION(Lua_BeamGetLayer)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	lua_pushnumber(L, *beam->GetLayer());

	return 1;
}

LUA_FUNCTION(Lua_BeamSetLayer)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	*beam->GetLayer() = luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BeamGetUseOverlay)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	lua_pushboolean(L, *beam->GetUseOverlay());

	return 1;
}

LUA_FUNCTION(Lua_BeamSetUseOverlay)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	*beam->GetUseOverlay() = lua_toboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BeamGetUnkBool)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	lua_pushboolean(L, *beam->GetUnkBool());

	return 1;
}

LUA_FUNCTION(Lua_BeamSetUnkBool)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	*beam->GetUnkBool() = lua_toboolean(L, 2);
	return 0;
}


LUA_FUNCTION(Lua_BeamRenderer__gc) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamRendererMT);
	beam->_anm2.destructor();
	beam->~BeamRenderer();

	return 0;
}

static void RegisterBeamRenderer(lua_State* L) {
	luaL_Reg functions[] = {
		{ "Add", Lua_BeamAdd},
		{ "Render", Lua_BeamRender},
		{ "GetSprite", Lua_BeamGetSprite},
		{ "SetSprite", Lua_BeamSetSprite},
		{ "GetLayer", Lua_BeamGetLayer},
		{ "SetLayer", Lua_BeamSetLayer},
		{ "GetUseOverlay", Lua_BeamGetUseOverlay},
		{ "SetUseOverlay", Lua_BeamSetUseOverlay},
		{ "GetUnkBool", Lua_BeamGetUnkBool},
		{ "SetUnkBool", Lua_BeamSetUnkBool},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::BeamRendererMT, lua::metatables::BeamRendererMT, functions, Lua_BeamRenderer__gc);
	lua_register(L, lua::metatables::BeamRendererMT, Lua_CreateBeamDummy);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBeamRenderer(_state);
}