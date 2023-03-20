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
	int layerId = luaL_checkinteger(L, 2);

	IsaacString str;
	const char* filename = luaL_checkstring(L, 3);
	if (strlen(filename) < 16) {
		strcpy(str.text, filename);
	}
	else {
		*(char**)str.text = (char*)filename;
	}
	str.unk = str.size = strlen(filename);

	if (lua_isboolean(L, 4))
		loadGraphics = lua_toboolean(L, 4);

	anm2->ReplaceSpritesheet(layerId, str);

	if (loadGraphics) {
		anm2->LoadGraphics(false); 
	}

	return 0;
}

static void RegisterNewReplaceSpriteSheet(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::SPRITE);
	lua_pushstring(L, "ReplaceSpritesheet");
	lua_pushcfunction(L, Lua_SpriteReplaceSpritesheet);
	lua_rawset(L, -3);
	lua_pop(L, 1);
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
	int layerName = luaL_checkinteger(L, 2);
	LayerState** luaLayer = (LayerState**)lua_newuserdata(L, sizeof(LayerState*));
	*luaLayer = anm2->GetLayer(layerName);
	luaL_setmetatable(L, LayerStateMT);
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

/*static int Lua_LayerStateGetUnk1(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushnumber(L, *layerState->GetUnk1());

	return 1;
}

static int Lua_LayerStateGetUnk2(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushnumber(L, *layerState->GetUnk2());

	return 1;
}
*/

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

static int Lua_LayerStateGetCropYOffset(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	lua_pushnumber(L, *layerState->GetCropYOffset());

	return 1;
}

static int Lua_LayerStateSetCropYOffset(lua_State* L)
{
	LayerState* layerState = *lua::GetUserdata<LayerState**>(L, 1, LayerStateMT);
	*layerState->GetCropYOffset() = luaL_checknumber(L, 2);

	return 0;
}


static void RegisterLayerState(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::SPRITE);
	lua_pushstring(L, "GetLayer");
	lua_pushcfunction(L, Lua_SpriteGetLayer);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, LayerStateMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "IsVisible", Lua_LayerStateIsVisible },
		{ "SetVisible", Lua_LayerStateSetVisible},
		{ "GetCropYOffset", Lua_LayerStateGetCropYOffset},
		{ "SetCropYOffset", Lua_LayerStateSetCropYOffset},
		//{ "GetUnk1", Lua_LayerStateGetUnk1},
		//{ "GetUnk2", Lua_LayerStateGetUnk2},
		{ "GetSize", Lua_LayerStateGetSize},
		{ "SetSize", Lua_LayerStateSetSize},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterNewReplaceSpriteSheet(state);
	RegisterLayerState(state);
}

