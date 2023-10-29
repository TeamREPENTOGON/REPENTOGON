#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

constexpr auto POINT_VECTOR_SIZE = 8;

const char* errors[4] = {
	"Must have at least two points",
	"Overlay AnimState is NULL!",
	"AnimState is NULL!",
	"Invalid layer id"
};

bool IsValidLayerID(ANM2* anm2, int id) {
	return (id >= 0 && (const unsigned int)id + 1 <= anm2->GetLayerCount());
}

LUA_FUNCTION(Lua_CreateBeamDummy) {
	int top = lua_gettop(L);
	if (top < 4) {
		luaL_error(L, "Expected at least 4 arguments, got %d", top);
	}

	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	int layerID = 0;
	if (lua_type(L, 2) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 2);
		LayerState* layerState = sprite->GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 2);
		if (!IsValidLayerID(sprite, layerID)) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}
	
	bool useOverlay = lua_toboolean(L, 3);
	bool unk = lua_toboolean(L, 4);
	int vectorSize = (int)luaL_optinteger(L, 5, POINT_VECTOR_SIZE);
	if (vectorSize < 2) {
		return luaL_error(L, "Must allocate at least two points");
	}

	BeamRenderer* toLua = lua::place<BeamRenderer>(L, lua::metatables::BeamMT, layerID, useOverlay, unk);

	toLua->_anm2.construct_from_copy(sprite);
	// fuck you, be loaded
	if (!sprite->_loaded) {
		toLua->_anm2.Load(sprite->_filename, true);
	}

	toLua->_points.reserve(vectorSize);
	luaL_setmetatable(L, lua::metatables::BeamMT);
	return 1;
}

LUA_FUNCTION(Lua_BeamAdd) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	float heightMod = (float)luaL_optnumber(L, 3, 1.0f);
	float widthMod = (float)luaL_optnumber(L, 4, 1.0f);
	ColorMod color;
	if (lua_gettop(L) > 4) {
		color = *lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}

	Point point(*pos, heightMod, widthMod, color);
	beam->_points.push_back(point);

	return 0;
}

LUA_FUNCTION(Lua_BeamRender) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	int8_t error = -1;
	bool clearPoints = lua::luaL_optboolean(L, 2, true);

	if (beam->_points.size() < 2) {
		error = 0;
		goto funcEnd;
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
	
	if (!IsValidLayerID(beam->GetANM2(), beam->_layer)) {
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

	if (error != -1) {
		return luaL_error(L, errors[error]);
	}

	return 0;
}

LUA_FUNCTION(Lua_BeamGetSprite) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	ANM2* anm2 = beam->GetANM2();
	if (anm2 == nullptr) {
		return luaL_error(L, "Beam Sprite is NULL!");
	}
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_BeamSetSprite) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 2, lua::Metatables::SPRITE, "Sprite");
	beam->_anm2.destructor();
	beam->_anm2.construct_from_copy(anm2);
	return 0;

}

LUA_FUNCTION(Lua_BeamGetLayer)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	lua_pushinteger(L, *beam->GetLayer());
	return 1;
}

LUA_FUNCTION(Lua_BeamSetLayer)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	int layerID = -1;
	if (lua_type(L, 2) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 2);
		LayerState* layerState = beam->_anm2.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else 
	{
		layerID = (int)luaL_checkinteger(L, 2);
		if (!IsValidLayerID(beam->GetANM2(), layerID)) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}
	beam->_layer = layerID;
	return 0;
}

LUA_FUNCTION(Lua_BeamGetUseOverlay)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	lua_pushboolean(L, *beam->GetUseOverlay());
	return 1;
}

LUA_FUNCTION(Lua_BeamSetUseOverlay)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	*beam->GetUseOverlay() = lua_toboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BeamGetUnkBool)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	lua_pushboolean(L, *beam->GetUnkBool());
	return 1;
}

LUA_FUNCTION(Lua_BeamSetUnkBool)
{
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	*beam->GetUnkBool() = lua_toboolean(L, 2);
	return 0;
}


LUA_FUNCTION(Lua_BeamRenderer__gc) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
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
	lua::RegisterNewClass(L, lua::metatables::BeamMT, lua::metatables::BeamMT, functions, Lua_BeamRenderer__gc);
	lua_register(L, lua::metatables::BeamMT, Lua_CreateBeamDummy);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBeamRenderer(_state);
}