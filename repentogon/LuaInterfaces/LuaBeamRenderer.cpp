#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

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
	const int top = lua_gettop(L);
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
	
	bool useOverlay = lua::luaL_checkboolean(L, 3);
	bool unk = lua::luaL_checkboolean(L, 4);

	BeamRenderer* toLua = lua::place<BeamRenderer>(L, lua::metatables::BeamMT, layerID, useOverlay, unk);
	toLua->_anm2.construct_from_copy(sprite);
	toLua->_anm2.GetLayer(layerID)->_wrapSMode = 0;
	toLua->_anm2.GetLayer(layerID)->_wrapTMode = 1;

	luaL_setmetatable(L, lua::metatables::BeamMT);
	return 1;
}

LUA_FUNCTION(Lua_BeamAdd) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	Point point;

	if (lua_gettop(L) == 2) {
		point = *lua::GetUserdata<Point*>(L, 2, lua::metatables::PointMT);
	}
	else
	{
		point._pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
		point._spritesheetCoordinate = (float)luaL_optnumber(L, 3, 0.0f);
		point._width = (float)luaL_optnumber(L, 4, 1.f);
		if (lua_type(L,5) == LUA_TUSERDATA) {
			point._color = *lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
		}
		point._worldSpace = lua::luaL_optboolean(L, 6, false);
	}

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
		Vector posBuffer;
		if (it->_worldSpace)
			LuaEngine::Isaac_WorldToScreen(&posBuffer, &it->_pos);
		else
			posBuffer = it->_pos;
		g_BeamRenderer->Add(&it->_pos, &it->_color, it->_width, it->_spritesheetCoordinate);
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
	
	if (lua_gettop(L) > 2) {
		int layerID = beam->_layer;
		if (lua_type(L, 3) == LUA_TSTRING) {
			const char* layerName = luaL_checkstring(L, 3);
			LayerState* layerState = anm2->GetLayer(layerName);
			if (layerState != nullptr) {
				layerID = layerState->GetLayerID();
			}
			else
			{
				return luaL_error(L, "Invalid layer name %s", layerName);
			}
		}
		else if (lua_isinteger(L, 3)) {
			layerID = (int)luaL_checkinteger(L, 3);
			if (!IsValidLayerID(anm2, layerID)) {
				return luaL_error(L, "Invalid layer ID %d", layerID);
			}
		}
		
		// hiding the layer set under a stack check to prevent the layer from being changed
		// if the call would ultimately have errored
		beam->_useOverlayData = lua::luaL_checkboolean(L, 4);
		beam->_layer = layerID;
	}
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
	beam->GetANM2()->GetLayer(layerID)->_wrapSMode = 0;
	beam->GetANM2()->GetLayer(layerID)->_wrapTMode = 1;
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
	*beam->GetUseOverlay() = lua::luaL_checkboolean(L, 2);
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
	*beam->GetUnkBool() = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BeamRenderer__gc) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	beam->_anm2.destructor();
	beam->~BeamRenderer();
	return 0;
}

// Point
LUA_FUNCTION(Lua_CreatePointDummy) {
	Vector* pos = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	float spritesheetCoord = (float)luaL_checknumber(L, 2);
	float widthMod = (float)luaL_optnumber(L, 3, 1.0f);

	ColorMod color;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 4, lua::Metatables::COLOR, "Color");
	}

	bool worldSpace = lua::luaL_optboolean(L, 5, false);

	Point* toLua = lua::place<Point>(L, lua::metatables::PointMT, *pos, spritesheetCoord, widthMod, color, worldSpace);
	luaL_setmetatable(L, lua::metatables::PointMT);

	return 1;
}

LUA_FUNCTION(Lua_PointGetPos) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = point->_pos;

	return 1;
}

LUA_FUNCTION(Lua_PointSetPos) {
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

LUA_FUNCTION(Lua_PointGetColor)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = point->_color;

	return 1;
}

LUA_FUNCTION(Lua_PointSetColor)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_color = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

LUA_FUNCTION(Lua_PointGetWidth)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushnumber(L, point->_width);

	return 1;
}

LUA_FUNCTION(Lua_PointSetWidth)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_width = (float)lua_tonumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PointGetSpritesheetCoordinate)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushnumber(L, point->_spritesheetCoordinate);

	return 1;
}

LUA_FUNCTION(Lua_PointSetSpritesheetCoordinate)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_spritesheetCoordinate = (float)lua_tonumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PointGetIsWorldSpace)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	lua_pushboolean(L, point->_worldSpace);

	return 1;
}

LUA_FUNCTION(Lua_PointSetIsWorldSpace)
{
	Point* point = lua::GetUserdata<Point*>(L, 1, lua::metatables::PointMT);
	point->_worldSpace = lua::luaL_checkboolean(L, 2);

	return 0;
}

static void RegisterBeamRenderer(lua_State* L) {
	// Beam
	luaL_Reg beamFunctions[] = {
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
	lua::RegisterNewClass(L, lua::metatables::BeamMT, lua::metatables::BeamMT, beamFunctions, Lua_BeamRenderer__gc);
	lua_register(L, lua::metatables::BeamMT, Lua_CreateBeamDummy);

	luaL_Reg pointFunctions[] = {
		{ "GetSpritesheetCoordinate", Lua_PointGetSpritesheetCoordinate},
		{ "SetSpritesheetCoordinate", Lua_PointSetSpritesheetCoordinate},
		{ "GetHeight", Lua_PointGetSpritesheetCoordinate}, // deprecated
		{ "SetHeight", Lua_PointSetSpritesheetCoordinate}, // deprecated
		{ "GetWidth", Lua_PointGetWidth},
		{ "SetWidth", Lua_PointSetWidth},
		{ "GetPosition", Lua_PointGetPos},
		{ "SetPosition", Lua_PointSetPos},
		{ "GetColor", Lua_PointGetColor},
		{ "SetColor", Lua_PointSetColor},
		{ "GetIsWorldSpace", Lua_PointGetIsWorldSpace},
		{ "SetIsWorldSpace", Lua_PointSetIsWorldSpace},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::PointMT, lua::metatables::PointMT, pointFunctions);
	lua_register(L, lua::metatables::PointMT, Lua_CreatePointDummy);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterBeamRenderer(_state);
}