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

	bool useOverlay = lua::luaL_checkboolean(L, 3);
	bool unk = lua::luaL_checkboolean(L, 4);

	BeamRenderer* toLua = lua::place<BeamRenderer>(L, lua::metatables::BeamMT, *sprite, layerID, useOverlay, unk);
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
		point._spritesheetCoordinate = (float)luaL_checknumber(L, 3);
		point._width = (float)luaL_optnumber(L, 4, 1.0f);
	}
	beam->_points.deque.push_back(point);

	return 0;
}

LUA_FUNCTION(Lua_BeamRender) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	int8_t error = -1;
	bool clearPoints = lua::luaL_optboolean(L, 2, true);

	if (beam->_points.deque.size() < 2) {
		error = 0;
		goto funcEnd;
	}

	if (beam->_useOverlayData) {
		if (beam->GetANM2()->_overlayAnimState._animData == nullptr) {
			error = 1;
			goto funcEnd;
		}
	}
	else if (beam->GetANM2()->_animState._animData == nullptr) {
		error = 2;
		goto funcEnd;
	}
	
	if (!IsValidLayerID(beam->GetANM2(), beam->_layer)) {
		error = 3;
		goto funcEnd;
	}

	g_BeamRenderer->Begin(beam->GetANM2(), beam->_layer, beam->_useOverlayData, beam->_unkBool);

	#pragma warning(suppress:4533) 
	ColorMod color;
	for (auto it = beam->_points.deque.begin(); it != beam->_points.deque.end(); ++it) {
		g_BeamRenderer->Add(&it->_pos, &color, it->_width, it->_spritesheetCoordinate);
	}

	g_BeamRenderer->End();

	// i'm doing this specifically so the points vector gets cleared if needed
	funcEnd:

	if (clearPoints) {
		beam->_points.deque.clear();
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
	*beam->GetANM2() = *anm2;

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
		LayerState* layerState = beam->GetANM2()->GetLayer(layerName);
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
	beam->~BeamRenderer();
	return 0;
}

// Points
LUA_FUNCTION(Lua_BeamGetPoints) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	lua_newtable(L);
	for (size_t i = 0; i < beam->_points.deque.size(); ++i) {
		lua_pushinteger(L, i + 1);
		Point* ud = (Point*)lua_newuserdata(L, sizeof(Point));
		*ud = beam->_points.deque[i];
		luaL_setmetatable(L, lua::metatables::PointMT);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_BeamSetPoints) {
	BeamRenderer* beam = lua::GetUserdata<BeamRenderer*>(L, 1, lua::metatables::BeamMT);
	if (!lua_istable(L, 2))
	{
		return luaL_argerror(L, 2, "Expected a table as second argument");
	}

	size_t length = (size_t)lua_rawlen(L, 2);
	if (length < 2)
	{
		return luaL_argerror(L, 2, "Must have at least two points");
	}
	else
	{
		deque_Point list;
		for (size_t i = 0; i < length; i++)
		{
			lua_rawgeti(L, 2, i + 1);
			list.push_back(*lua::GetUserdata<Point*>(L, -1, lua::metatables::PointMT));
			lua_pop(L, 1);
		}
		beam->_points.deque = list;
	}

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
		{ "GetPoints", Lua_BeamGetPoints},
		{ "SetPoints", Lua_BeamSetPoints},
		{ "GetFixed", Lua_BeamGetPoints},
		{ "SetFixed", Lua_BeamSetPoints},
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