#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*
* While internally, this is the ANM2 class, it is exposed to Lua as "Sprite".
* I've named this file "LuaSprite" for consistency with the existing API metatable.
*/

LUA_FUNCTION(Lua_SpriteReplaceSpritesheet)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	int layerId = (int)luaL_checkinteger(L, 2);

	std::string filename = luaL_checkstring(L, 3);

	bool loadGraphics = lua::luaL_optboolean(L, 4, false);

	anm2->ReplaceSpritesheet(layerId, filename);

	if (loadGraphics) {
		anm2->LoadGraphics(false);
	}

	return 0;
}

LUA_FUNCTION(Lua_SpriteIsOverlayEventTriggered)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* eventName = luaL_checkstring(L, 2);
	lua_pushboolean(L, anm2->GetOverlayAnimationState()->IsEventTriggered(eventName));

	return 1;
}

LUA_FUNCTION(Lua_SpriteWasOverlayEventTriggered)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* eventName = luaL_checkstring(L, 2);
	lua_pushboolean(L, anm2->GetOverlayAnimationState()->WasEventTriggered(eventName));

	return 1;
}

LUA_FUNCTION(Lua_SpriteGetLayer)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	LayerState* layerState = nullptr;
	if (lua_type(L, 2) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 2);
		layerState = anm2->GetLayer(layerName);
	}
	else {
		const int layerID = (const int)luaL_checkinteger(L, 2);
		const unsigned int layerCount = anm2->GetLayerCount();
		if (layerID >= 0 && (const unsigned int)layerID < layerCount) {
			layerState = anm2->GetLayer(layerID);
		}
	}
	if (layerState == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	LayerState** luaLayer = (LayerState**)lua_newuserdata(L, sizeof(LayerState*));
	*luaLayer = layerState;
	luaL_setmetatable(L, lua::metatables::LayerStateMT);
	return 1;
}

LUA_FUNCTION(Lua_SpriteGetAllLayers)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	lua_newtable(L);
	LayerState* layerState = nullptr;
	for (size_t i = 0; i < anm2->GetLayerCount(); ++i) {
		lua_pushinteger(L, i + 1);
		layerState = anm2->GetLayer(i);
		LayerState** luaLayer = (LayerState**)lua_newuserdata(L, sizeof(LayerState*));
		*luaLayer = layerState;
		luaL_setmetatable(L, lua::metatables::LayerStateMT);
		lua_rawset(L, -3);
	}

	return 1;
}


LUA_FUNCTION(Lua_SpriteGetRenderFlags)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	lua_pushinteger(L, *anm2->GetRenderFlags());
	return 1;
}

LUA_FUNCTION(Lua_SpriteSetRenderFlags)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	*anm2->GetRenderFlags() = (unsigned int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_SpriteSetOverlayFrame)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	if (lua_type(L, 2) == LUA_TSTRING) {
		const char* animName = luaL_checkstring(L, 2);
		anm2->SetOverlayFrame(animName, (int)luaL_checkinteger(L, 3));
	}
	else {
		anm2->SetOverlayFrame((int)luaL_checkinteger(L, 2));
	}
	return 0;
}

LUA_FUNCTION(Lua_SpriteStop)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	bool stopOverlay = lua::luaL_optboolean(L, 2, true);
	anm2->GetAnimationState()->Stop();
	if (stopOverlay) {
		anm2->GetOverlayAnimationState()->Stop();
	}
	return 0;
}

LUA_FUNCTION(Lua_SpriteStopOverlay)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	anm2->GetOverlayAnimationState()->Stop();
	return 0;
}

LUA_FUNCTION(Lua_SpriteContinue)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	bool continueOverlay = lua::luaL_optboolean(L, 2, true);
	anm2->GetAnimationState()->Play();
	if (continueOverlay) {
		anm2->GetOverlayAnimationState()->Play();
	}
	return 0;
}

LUA_FUNCTION(Lua_SpriteContinueOverlay)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	anm2->GetOverlayAnimationState()->Play();
	return 0;
}

// LayerState from here on out

LUA_FUNCTION(Lua_LayerStateGetLayerID)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushinteger(L, layerState->GetLayerID());
	return 1;
}

LUA_FUNCTION(Lua_LayerStateGetName)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushstring(L, layerState->GetName().c_str());
	return 1;
}

LUA_FUNCTION(Lua_LayerStateGetSpritesheetPath)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushstring(L, layerState->GetSpritesheetPath().c_str());
	return 1;
}

LUA_FUNCTION(Lua_LayerStateGetDefaultSpritesheetPath)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushstring(L, layerState->GetDefaultSpritesheetPath().c_str());
	return 1;
}

LUA_FUNCTION(Lua_LayerStateIsVisible)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushboolean(L, *layerState->IsVisible());

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetVisible)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->IsVisible() = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetSize)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetSize();

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetSize)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->GetSize() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetRotation)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushnumber(L, *layerState->GetRotation());

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetRotation)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->GetRotation() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetPos)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetPos();

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetPos)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->GetPos() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetColor)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *layerState->GetColor();

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetColor)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	*layerState->GetColor() = *color;
	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetRenderFlags)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushinteger(L, *layerState->GetRenderFlags());
	return 1;
}
LUA_FUNCTION(Lua_LayerStateSetRenderFlags)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->GetRenderFlags() = (unsigned int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetCropOffset)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetCropOffset();

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetCropOffset)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	*layerState->GetCropOffset() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetBlendMode) {
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	BlendMode** ud = (BlendMode**)lua_newuserdata(L, sizeof(BlendMode*));
	*ud = layerState->GetBlendMode();
	luaL_setmetatable(L, lua::metatables::BlendModeMT);
	return 1;
}

LUA_FUNCTION(Lua_LayerStateGetWrapSMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushinteger(L, layerState->_wrapSMode);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateGetWrapTMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushinteger(L, layerState->_wrapTMode);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetWrapSMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_wrapSMode = luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateSetWrapTMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_wrapTMode = luaL_checkinteger(L, 2);

	return 0;
}

static void RegisterSpriteFuncs(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetLayer", Lua_SpriteGetLayer},
		{ "GetAllLayers", Lua_SpriteGetAllLayers},
		{ "ReplaceSpritesheet", Lua_SpriteReplaceSpritesheet},
		{ "IsOverlayEventTriggered", Lua_SpriteIsOverlayEventTriggered},
		{ "WasOverlayEventTriggered", Lua_SpriteWasOverlayEventTriggered},
		{ "SetOverlayFrame", Lua_SpriteSetOverlayFrame},
		{ "Stop", Lua_SpriteStop},
		{ "StopOverlay", Lua_SpriteStopOverlay},
		{ "Continue", Lua_SpriteContinue},
		{ "ContinueOverlay", Lua_SpriteContinueOverlay},
		{ "GetRenderFlags", Lua_SpriteGetRenderFlags},
		{ "SetRenderFlags", Lua_SpriteSetRenderFlags},
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::SPRITE, functions);
}

static void RegisterLayerState(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetLayerID", Lua_LayerStateGetLayerID },
		{ "GetName", Lua_LayerStateGetName },
		{ "GetSpritesheetPath", Lua_LayerStateGetSpritesheetPath },
		{ "GetDefaultSpritesheetPath", Lua_LayerStateGetDefaultSpritesheetPath },
		{ "IsVisible", Lua_LayerStateIsVisible },
		{ "SetVisible", Lua_LayerStateSetVisible},
		{ "GetSize", Lua_LayerStateGetSize},
		{ "SetSize", Lua_LayerStateSetSize},
		{ "GetRotation", Lua_LayerStateGetRotation},
		{ "SetRotation", Lua_LayerStateSetRotation},
		{ "GetPos", Lua_LayerStateGetPos},
		{ "SetPos", Lua_LayerStateSetPos},
		{ "GetColor", Lua_LayerStateGetColor},
		{ "SetColor", Lua_LayerStateSetColor},
		{ "GetCropOffset", Lua_LayerStateGetCropOffset},
		{ "SetCropOffset", Lua_LayerStateSetCropOffset},
		{ "GetRenderFlags", Lua_LayerStateGetRenderFlags},
		{ "SetRenderFlags", Lua_LayerStateSetRenderFlags},
		{ "GetBlendMode", Lua_LayerStateGetBlendMode},
		{ "GetWrapSMode", Lua_LayerStateGetWrapSMode},
		{ "SetWrapSMode", Lua_LayerStateSetWrapSMode},
		{ "GetWrapTMode", Lua_LayerStateGetWrapTMode},
		{ "SetWrapTMode", Lua_LayerStateSetWrapTMode},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::LayerStateMT, lua::metatables::LayerStateMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterSpriteFuncs(_state);
	RegisterLayerState(_state);
}

