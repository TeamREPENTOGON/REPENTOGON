#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_SpriteGetNullFrame(lua_State* L)
{
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* nullLayerName = luaL_checkstring(L, 2);
	NullFrame* toLua = sprite->GetNullFrame(nullLayerName);
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	NullFrame** luaNullFrame = (NullFrame**)lua_newuserdata(L, sizeof(NullFrame*));
	*luaNullFrame = toLua;
	luaL_setmetatable(L, lua::metatables::NullFrameMT);
	return 1;
}

static int Lua_SpriteGetOverlayNullFrame(lua_State* L)
{
	ANM2* sprite = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	const char* nullLayerName = luaL_checkstring(L, 2);
	NullFrame* toLua = sprite->GetOverlayNullFrame(nullLayerName);
	if (toLua == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	NullFrame** luaNullFrame = (NullFrame**)lua_newuserdata(L, sizeof(NullFrame*));
	*luaNullFrame = toLua;
	luaL_setmetatable(L, lua::metatables::NullFrameMT);
	return 1;
}

static int Lua_NullFrameGetScale(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetScale();

	return 1;
}

static int Lua_NullFrameIsVisible(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	lua_pushboolean(L, *nullFrame->IsVisible());

	return 1;
}

static int Lua_NullFrameGetPos(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetPos();

	return 1;
}

static int Lua_NullFrameGetColor(lua_State* L)
{
	NullFrame* layerState = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *layerState->GetColor();

	return 1;
}

static int Lua_NullFrameGetRotation(lua_State* L)
{
	NullFrame* nullFrame = *lua::GetUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	lua_pushnumber(L, *nullFrame->GetRotation());

	return 1;
}

static void RegisterNullFrame(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetNullFrame", Lua_SpriteGetNullFrame);
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetOverlayNullFrame", Lua_SpriteGetOverlayNullFrame);

	luaL_newmetatable(L, lua::metatables::NullFrameMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetPos", Lua_NullFrameGetPos},
		{ "GetRotation", Lua_NullFrameGetRotation},
		{ "IsVisible", Lua_NullFrameIsVisible},
		{ "GetScale", Lua_NullFrameGetScale},
		{ "GetColor", Lua_NullFrameGetColor},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterNullFrame(state);
}