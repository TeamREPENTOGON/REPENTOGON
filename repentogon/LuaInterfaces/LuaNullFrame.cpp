#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SpriteGetNullFrame)
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

LUA_FUNCTION(Lua_SpriteGetOverlayNullFrame)
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

LUA_FUNCTION(Lua_NullFrameGetScale)
{
	NullFrame* nullFrame = *lua::GetRawUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetScale();

	return 1;
}

LUA_FUNCTION(Lua_NullFrameIsVisible)
{
	NullFrame* nullFrame = *lua::GetRawUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	lua_pushboolean(L, *nullFrame->IsVisible());

	return 1;
}

LUA_FUNCTION(Lua_NullFrameGetPos)
{
	NullFrame* nullFrame = *lua::GetRawUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = *nullFrame->GetPos();

	return 1;
}

LUA_FUNCTION(Lua_NullFrameGetColor)
{
	NullFrame* layerState = *lua::GetRawUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = *layerState->GetColor();

	return 1;
}

LUA_FUNCTION(Lua_NullFrameGetRotation)
{
	NullFrame* nullFrame = *lua::GetRawUserdata<NullFrame**>(L, 1, lua::metatables::NullFrameMT);
	lua_pushnumber(L, *nullFrame->GetRotation());

	return 1;
}

static void RegisterNullFrame(lua_State* L)
{
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetNullFrame", Lua_SpriteGetNullFrame);
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetOverlayNullFrame", Lua_SpriteGetOverlayNullFrame);

	luaL_Reg functions[] = {
		{ "GetPos", Lua_NullFrameGetPos},
		{ "GetRotation", Lua_NullFrameGetRotation},
		{ "IsVisible", Lua_NullFrameIsVisible},
		{ "GetScale", Lua_NullFrameGetScale},
		{ "GetColor", Lua_NullFrameGetColor},
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::NullFrameMT, lua::metatables::NullFrameMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterNullFrame(_state);
}