#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../MiscFunctions.h"
#include "../Patches/Anm2Extras.h"

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

LUA_FUNCTION(Lua_SpriteLoadRgonSprite)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* path = luaL_checkstring(L, 2);
	bool loadg = lua::luaL_checkboolean(L, 3);
	anm2->Load(REPENTOGON::GetRGONGfxAbsolutePath(path), loadg);

	return 0;
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

LUA_FUNCTION(Lua_SpriteSetCustomShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const std::string path = luaL_checkstring(L, 2);
	const bool success = SetCustomShader(anm2, path, /*champion=*/false);
	if (!success) {
		return luaL_error(L, ("Failed to load shader: " + path).c_str());
	}
	return 0;
}

LUA_FUNCTION(Lua_SpriteClearCustomShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	ClearCustomShader(anm2, /*champion=*/false);
	return 0;
}

LUA_FUNCTION(Lua_SpriteHasCustomShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const std::string path = luaL_optstring(L, 2, "");
	bool result = false;
	if (path.empty()) {
		result = HasCustomShader(anm2, /*champion=*/false);
	}
	else {
		result = HasCustomShader(anm2, path, /*champion=*/false);
	}
	lua_pushboolean(L, result);

	return 1;
}

LUA_FUNCTION(Lua_SpriteSetCustomChampionShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const std::string path = luaL_checkstring(L, 2);
	const bool success = SetCustomShader(anm2, path, /*champion=*/true);
	if (!success) {
		return luaL_error(L, ("Failed to load shader: " + path).c_str());
	}
	return 0;
}

LUA_FUNCTION(Lua_SpriteClearCustomChampionShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	ClearCustomShader(anm2, /*champion=*/true);
	return 0;
}

LUA_FUNCTION(Lua_SpriteHasCustomChampionShader)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	const std::string path = luaL_optstring(L, 2, "");
	bool result = false;
	if (path.empty()) {
		result = HasCustomShader(anm2, /*champion=*/true);
	}
	else {
		result = HasCustomShader(anm2, path, /*champion=*/true);
	}
	lua_pushboolean(L, result);

	return 1;
}


// LayerState from here on out

LUA_FUNCTION(Lua_LayerStateSetCustomShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	const std::string path = luaL_checkstring(L, 2);
	const bool success = SetCustomShader(layerState, path, /*champion=*/false);
	if (!success) {
		return luaL_error(L, ("Failed to load shader: " + path).c_str());
	}
	return 0;
}

LUA_FUNCTION(Lua_LayerStateClearCustomShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	ClearCustomShader(layerState, /*champion=*/false);
	return 0;
}

LUA_FUNCTION(Lua_LayerStateHasCustomShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);

	const std::string path = luaL_optstring(L, 2, "");
	bool result = false;
	if (path.empty()) {
		result = HasCustomShader(layerState, /*champion=*/false);
	}
	else {
		result = HasCustomShader(layerState, path, /*champion=*/false);
	}
	lua_pushboolean(L, result);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetCustomChampionShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	const std::string path = luaL_checkstring(L, 2);
	const bool success = SetCustomShader(layerState, path, /*champion=*/true);
	if (!success) {
		return luaL_error(L, ("Failed to load shader: " + path).c_str());
	}
	return 0;
}

LUA_FUNCTION(Lua_LayerStateClearCustomChampionShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	ClearCustomShader(layerState, /*champion=*/true);
	return 0;
}

LUA_FUNCTION(Lua_LayerStateHasCustomChampionShader)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);

	const std::string path = luaL_optstring(L, 2, "");
	bool result = false;
	if (path.empty()) {
		result = HasCustomShader(layerState, /*champion=*/true);
	}
	else {
		result = HasCustomShader(layerState, path, /*champion=*/true);
	}
	lua_pushboolean(L, result);

	return 1;
}

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

LUA_FUNCTION(Lua_LayerStateSetWrapSMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_wrapSMode = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetWrapTMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushinteger(L, layerState->_wrapTMode);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetWrapTMode)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_wrapTMode = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetFlipX)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushboolean(L, layerState->_flipX);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetFlipX)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_flipX = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_LayerStateGetFlipY)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	lua_pushboolean(L, layerState->_flipY);

	return 1;
}

LUA_FUNCTION(Lua_LayerStateSetFlipY)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, lua::metatables::LayerStateMT);
	layerState->_flipY = lua::luaL_checkboolean(L, 2);

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
		{ "LoadRGON", Lua_SpriteLoadRgonSprite},
		{ "SetCustomShader", Lua_SpriteSetCustomShader},
		{ "ClearCustomShader", Lua_SpriteClearCustomShader},
		{ "HasCustomShader", Lua_SpriteHasCustomShader},
		{ "SetCustomChampionShader", Lua_SpriteSetCustomChampionShader},
		{ "ClearCustomChampionShader", Lua_SpriteClearCustomChampionShader},
		{ "HasCustomChampionShader", Lua_SpriteHasCustomChampionShader},
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
		{ "GetFlipX", Lua_LayerStateGetFlipX},
		{ "SetFlipX", Lua_LayerStateSetFlipX},
		{ "GetFlipY", Lua_LayerStateGetFlipY},
		{ "SetFlipY", Lua_LayerStateSetFlipY},
		{ "SetCustomShader", Lua_LayerStateSetCustomShader},
		{ "ClearCustomShader", Lua_LayerStateClearCustomShader},
		{ "HasCustomShader", Lua_LayerStateHasCustomShader},
		{ "SetCustomChampionShader", Lua_LayerStateSetCustomChampionShader},
		{ "ClearCustomChampionShader", Lua_LayerStateClearCustomChampionShader},
		{ "HasCustomChampionShader", Lua_LayerStateHasCustomChampionShader},
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

