#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_AnimationFrameGetCrop(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->crop);
	return 1;
}

static int Lua_AnimationFrameGetWidth(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->width);
	return 1;
}

static int Lua_AnimationFrameGetHeight(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->height);
	return 1;
}

static int Lua_AnimationFrameGetPos(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->pos);
	return 1;
}

static int Lua_AnimationFrameGetScale(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->scale);
	return 1;
}

static int Lua_AnimationFrameGetPivot(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->pivot);
	return 1;
}

static int Lua_AnimationFrameIsVisible(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushboolean(L, animationFrame->visible);
	return 1;
}

static int Lua_AnimationFrameGetRotation(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->rotation);
	return 1;
}

static int Lua_AnimationFrameIsInterpolated(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushboolean(L, animationFrame->interpolated);
	return 1;
}

static int Lua_AnimationFrameGetColor(lua_State* L)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<ColorMod>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_COLOR), animationFrame->color);
	return 1;
}

static void RegisterAnimationFrame(lua_State* L) {
	luaL_newmetatable(L, lua::metatables::AnimationFrameMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetCrop", Lua_AnimationFrameGetCrop },
		{ "GetWidth", Lua_AnimationFrameGetWidth },
		{ "GetHeight", Lua_AnimationFrameGetHeight },
		{ "GetPos", Lua_AnimationFrameGetPos },
		{ "GetScale", Lua_AnimationFrameGetScale },
		{ "GetPivot", Lua_AnimationFrameGetPivot },
		{ "IsVisible", Lua_AnimationFrameIsVisible },
		{ "GetRotation", Lua_AnimationFrameGetRotation },
		{ "IsInterpolated", Lua_AnimationFrameIsInterpolated },
		{ "GetColor", Lua_AnimationFrameGetColor },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterAnimationFrame(state);
}
