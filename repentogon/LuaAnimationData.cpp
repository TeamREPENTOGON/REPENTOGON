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

static int Lua_SpriteGetAllAnimationData(lua_State* L)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	lua_newtable(L);
	for (int i = 0; i < anm2->GetAnimationCount(); ++i) {
		lua_pushinteger(L, i + 1);
		AnimationData** toLua = (AnimationData**)lua_newuserdata(L, sizeof(AnimationData*));
		*toLua = anm2->GetAnimationData(i);
		luaL_setmetatable(L, lua::metatables::AnimationDataMT);
		lua_settable(L, -3);
	}

	return 1;
}

static int Lua_AnimationDataGetName(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushstring(L, animationData->GetName().c_str());
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

	if (layerID < 0) {
		lua_pushnil(L);
		return 1;
	}

	AnimationLayer* animationLayer = animationData->GetLayerById(layerID);

	if (animationLayer == nullptr) {
		// No layer with this ID found. It IS possible for an anm2 file/animation to just not have a layer with a given ID even if it has layers with surrounding IDs.
		lua_pushnil(L);
		return 1;
	}

	AnimationLayer** toLua = (AnimationLayer**)lua_newuserdata(L, sizeof(AnimationLayer*));
	*toLua = animationLayer;
	luaL_setmetatable(L, lua::metatables::AnimationLayerMT);

	return 1;
}

static int Lua_AnimationDataGetAllLayers(lua_State* L)
{
	AnimationData* animationData = *lua::GetUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	
	lua_newtable(L);
	for (int i = 0; i < animationData->GetLayerCount(); ++i) {
		lua_pushinteger(L, i + 1);
		AnimationLayer** toLua = (AnimationLayer**)lua_newuserdata(L, sizeof(AnimationLayer*));
		*toLua = animationData->GetLayerByOrder(i);
		luaL_setmetatable(L, lua::metatables::AnimationLayerMT);
		lua_settable(L, -3);
	}

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
	lua::RegisterFunction(L, lua::Metatables::SPRITE, "GetAllAnimationData", Lua_SpriteGetAllAnimationData);

	luaL_newmetatable(L, lua::metatables::AnimationDataMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg funcs[] = {
		{ "GetAllLayers", Lua_AnimationDataGetAllLayers },
		{ "GetLayer", Lua_AnimationDataGetLayerById },
		{ "GetName", Lua_AnimationDataGetName },
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
