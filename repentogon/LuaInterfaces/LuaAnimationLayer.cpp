#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

// Given that some AnimationFrames have a duration >1, need to crawl through them to
// find which AnimationFrame is used for the Nth frame of the whole animation.
// I'm sure there is something in the game that could allow me to find this faster,
// but I haven't found that yet. Could replace this with a binary search later...
static AnimationFrame* FindAnimationFrame(AnimationLayer* animationLayer, const int frame) {
	for (int i = 0; i < animationLayer->GetFrameCount(); ++i) {
		AnimationFrame* animationFrame = animationLayer->GetFrame(i);
		if (animationFrame == nullptr) {
			// Uh oh! This should never happen! I think!
			return nullptr;
		}
		if (frame >= animationFrame->startFrame && frame < animationFrame->endFrame) {
			return animationFrame;
		}
	}
	// Return the last frame.
	return animationLayer->GetFrame(animationLayer->GetFrameCount() - 1);
}

LUA_FUNCTION(Lua_AnimationLayerGetFrame)
{
	AnimationLayer* animationLayer = *lua::GetUserdata<AnimationLayer**>(L, 1, lua::metatables::AnimationLayerMT);

	if (animationLayer->GetFrameCount() == 0) {
		lua_pushnil(L);
		return 1;
	}

	int frame = (int)luaL_checkinteger(L, 2);

	if (frame < 0) {
		frame = 0;
	}

	AnimationFrame* animationFrame = FindAnimationFrame(animationLayer, frame);

	if (animationFrame == nullptr) {
		// We clamp the requested frame if needed, so reaching this error
		// probably indicates a bug or something I didn't expect.
		return luaL_error(L, "Failed to read an AnimationFrame while looking for frame %d", frame);
	}

	AnimationFrame** toLua = (AnimationFrame**)lua_newuserdata(L, sizeof(AnimationFrame*));
	*toLua = animationFrame;
	luaL_setmetatable(L, lua::metatables::AnimationFrameMT);

	return 1;
}

LUA_FUNCTION(Lua_AnimationLayerGetLayerId)
{
	AnimationLayer* animationLayer = *lua::GetUserdata<AnimationLayer**>(L, 1, lua::metatables::AnimationLayerMT);
	lua_pushinteger(L, animationLayer->GetLayerID());
	return 1;
}

LUA_FUNCTION(Lua_AnimationLayerIsVisible)
{
	AnimationLayer* animationLayer = *lua::GetUserdata<AnimationLayer**>(L, 1, lua::metatables::AnimationLayerMT);
	lua_pushboolean(L, animationLayer->IsVisible());
	return 1;
}

static void RegisterAnimationLayer(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetFrame", Lua_AnimationLayerGetFrame },
		{ "GetLayerID", Lua_AnimationLayerGetLayerId },
		{ "IsVisible", Lua_AnimationLayerIsVisible },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::AnimationLayerMT, lua::metatables::AnimationLayerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterAnimationLayer(_state);
}
