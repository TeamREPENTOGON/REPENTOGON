#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_SpriteGetCurrentAnimationData(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	AnimationData* animData = anm2->GetAnimationState()->GetAnimationData();

	if (animData == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	AnimationData** toLua = (AnimationData**)lua_newuserdata(L, sizeof(AnimationData*));
	*toLua = animData;
	luaL_setmetatable(L, lua::metatables::AnimationDataMT);

	return 1;
}

static int Lua_SpriteGetOverlayAnimationData(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	AnimationData* animData = anm2->GetOverlayAnimationState()->GetAnimationData();

	if (animData == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	AnimationData** toLua = (AnimationData**)lua_newuserdata(L, sizeof(AnimationData*));
	*toLua = animData;
	luaL_setmetatable(L, lua::metatables::AnimationDataMT);

	return 1;
}

static AnimationData* GetAnimationDataByName(ANM2* anm2, const std_string layerName) {
	for (unsigned int i = 0; i < anm2->GetAnimationCount(); ++i) {
		AnimationData* animData = anm2->GetAnimationData(i);
		if (animData != nullptr && animData->GetName() == layerName) {
			return animData;
		}
	}
	return nullptr;
}

static int Lua_SpriteGetAnimationData(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");
	std_string layerName = luaL_checkstring(L, 2);
	AnimationData* animData = GetAnimationDataByName(anm2, layerName);

	if (animData == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	AnimationData** toLua = (AnimationData**)lua_newuserdata(L, sizeof(AnimationData*));
	*toLua = animData;
	luaL_setmetatable(L, lua::metatables::AnimationDataMT);

	return 1;
}

static int Lua_AnimationDataGetName(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushstring(L, animationData->GetName().c_str());
	return 1;
}

static int Lua_AnimationDataGetLayerCount(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushinteger(L, animationData->GetLayerCount());
	return 1;
}

static int Lua_AnimationDataGetLength(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushinteger(L, animationData->GetLength());
	return 1;
}

static int Lua_AnimationDataGetLayerById(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	int layerID = (int)luaL_checkinteger(L, 2);

	if (layerID < 0 || layerID >= animationData->GetLayerCount()) {
		lua_pushnil(L);
		return 1;
	}

	AnimationLayer* animationLayer = animationData->GetLayerById(layerID);

	if (animationLayer == nullptr) {
		// Since we just push nil if the requested layer is not within the expected range,
		// reaching this error would probably mean a bug or something I didn't expect.
		return luaL_error(L, "Failed to read AnimationLayer with LayerID %d, despite there being %d layers", layerID, animationData->GetLayerCount());
	}

	AnimationLayer** toLua = (AnimationLayer**)lua_newuserdata(L, sizeof(AnimationLayer*));
	*toLua = animationLayer;
	luaL_setmetatable(L, lua::metatables::AnimationLayerMT);

	return 1;
}

static int Lua_AnimationDataIsLoopingAnimation(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushinteger(L, animationData->IsLoopingAnimation());
	return 1;
}

static void RegisterAnimationData(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetCurrentAnimationData", Lua_SpriteGetCurrentAnimationData);
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetOverlayAnimationData", Lua_SpriteGetOverlayAnimationData);
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetAnimationData", Lua_SpriteGetAnimationData);

	luaL_newmetatable(L, lua::metatables::AnimationDataMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetLayer", Lua_AnimationDataGetLayerById },
		{ "GetName", Lua_AnimationDataGetName },
		{ "GetLayerCount", Lua_AnimationDataGetLayerCount },
		{ "GetLength", Lua_AnimationDataGetLength },
		{ "IsLoopingAnimation", Lua_AnimationDataIsLoopingAnimation },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, funcs, 0);

	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterAnimationData(state);
}
