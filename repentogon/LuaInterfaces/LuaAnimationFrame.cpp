#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_AnimationFrameGetCrop)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->crop);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetWidth)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->width);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetHeight)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->height);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetPos)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->pos);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetScale)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->scale);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetPivot)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), animationFrame->pivot);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameIsVisible)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushboolean(L, animationFrame->visible);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetRotation)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushnumber(L, animationFrame->rotation);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameIsInterpolated)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushboolean(L, animationFrame->interpolated);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetColor)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua::luabridge::UserdataValue<ColorMod>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_COLOR), animationFrame->color);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetStartFrame)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushinteger(L, animationFrame->startFrame);
	return 1;
}

LUA_FUNCTION(Lua_AnimationFrameGetEndFrame)
{
	AnimationFrame* animationFrame = *lua::GetUserdata<AnimationFrame**>(L, 1, lua::metatables::AnimationFrameMT);
	lua_pushinteger(L, animationFrame->endFrame);
	return 1;
}

static void RegisterAnimationFrame(lua_State* L) {
	luaL_Reg functions[] = {
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
		{ "GetStartFrame", Lua_AnimationFrameGetStartFrame },
		{ "GetEndFrame", Lua_AnimationFrameGetEndFrame },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::AnimationFrameMT, lua::metatables::AnimationFrameMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterAnimationFrame(_state);
}
