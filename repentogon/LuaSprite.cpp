#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*
* While internally, this is the ANM2 class, it is exposed to Lua as "Sprite".
* I've named this file "LuaSprite" for consistency with the existing API metatable.
*/

static constexpr const char* LayerStateMT = "LayerState";

int Lua_SpriteReplaceSpritesheet(lua_State* L)
{
	bool loadGraphics = false;
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	int layerId = (int)luaL_checkinteger(L, 2);

	std::string filename  = luaL_checkstring(L, 3);

	if (lua_isboolean(L, 4))
		loadGraphics = lua_toboolean(L, 4);

	anm2->ReplaceSpritesheet(layerId, filename);

	if (loadGraphics) {
		anm2->LoadGraphics(false); 
	}

	return 0;
}

int Lua_SpriteIsOverlayEventTriggered(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* eventName = luaL_checkstring(L, 2);
	lua_pushboolean(L, anm2->GetOverlayAnimationState()->IsEventTriggered(eventName));

	return 1;
}

int Lua_SpriteWasOverlayEventTriggered(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* eventName = luaL_checkstring(L, 2);
	lua_pushboolean(L, anm2->GetOverlayAnimationState()->WasEventTriggered(eventName));

	return 1;
}

/*int Lua_SpriteGetLayer(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* layerName = luaL_checkstring(L, 2);
	LayerState* toLua = (LayerState*)lua_newuserdata(L, sizeof(LayerState));
	toLua = anm2->GetLayer(layerName);
	if (toLua == nullptr) {
		printf("ABORT THIS");
	}
	luaL_setmetatable(L, LayerStateMT);
	return 1;
}
*/

static int Lua_SpriteGetLayer(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	unsigned int layerID = (unsigned int)luaL_checkinteger(L, 2);
	LayerState* toLua = anm2->GetLayer(layerID);
	unsigned int layerCount = anm2->GetLayerCount();
	if ((layerID < 0) || (layerCount <= layerID)) {
		return luaL_error(L, "Invalid layer ID %d ", layerID);
	}
	LayerState** luaLayer = (LayerState**)lua_newuserdata(L, sizeof(LayerState*));
	*luaLayer = toLua;
	luaL_setmetatable(L, LayerStateMT);
	return 1;
}

static int Lua_SpriteGetLayer_Text(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* layerName = luaL_checkstring(L, 2);
	LayerState* toLua = anm2->GetLayer(layerName);
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	LayerState** luaLayer = (LayerState**)lua_newuserdata(L, sizeof(LayerState*));
	*luaLayer = toLua;
	luaL_setmetatable(L, LayerStateMT);
	return 1;
}

static int Lua_LayerStateGetName(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushstring(L, layerState->GetName().c_str());
	return 1;
}

static int Lua_SpriteGetSpritesheetPath(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushstring(L, layerState->GetSpritesheetPath().c_str());
	return 1;
}

static int Lua_LayerStateIsVisible(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushboolean(L, *layerState->IsVisible());

	return 1;
}

static int Lua_LayerStateSetVisible(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->IsVisible() = lua_toboolean(L, 2);

	return 0;
}


static int Lua_LayerStateGetSize(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetSize();

	return 1;
}

static int Lua_LayerStateSetSize(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->GetSize() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

static int Lua_LayerStateGetRotation(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushnumber(L, *layerState->GetRotation());

	return 1;
}

static int Lua_LayerStateSetRotation(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->GetRotation() = (float)lua_tonumber(L, 2);

	return 0;
}

static int Lua_LayerStateGetPos(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetPos();

	return 1;
}

static int Lua_LayerStateSetPos(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->GetPos() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}

static int Lua_LayerStateGetColor(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *layerState->GetColor();

	return 1;
}

static int Lua_LayerStateSetColor(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	ColorMod* color = lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	*layerState->GetColor() = *color;
	return 0;
}

static int Lua_LayerStateGetCropOffset(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *layerState->GetCropOffset();

	return 1;
}

static int Lua_LayerStateSetCropOffset(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->GetCropOffset() = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");

	return 0;
}


static void RegisterLayerState(lua_State* L) {
	luaL_newmetatable(L, LayerStateMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetName", Lua_LayerStateGetName },
		{ "GetSpritesheetPath", Lua_SpriteGetSpritesheetPath },
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
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::SPRITE;
	lua::RegisterFunction(state, mt, "GetLayer", Lua_SpriteGetLayer);
	lua::RegisterFunction(state, mt, "GetLayerByName", Lua_SpriteGetLayer_Text);
	lua::RegisterFunction(state, mt, "ReplaceSpritesheet", Lua_SpriteReplaceSpritesheet);
	lua::RegisterFunction(state, mt, "IsOverlayEventTriggered", Lua_SpriteIsOverlayEventTriggered);
	lua::RegisterFunction(state, mt, "WasOverlayEventTriggered", Lua_SpriteWasOverlayEventTriggered);
	RegisterLayerState(state);
}

