#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_SpriteGetCurrentAnimationData)
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

LUA_FUNCTION(Lua_SpriteGetOverlayAnimationData)
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

LUA_FUNCTION(Lua_SpriteGetAnimationData)
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

LUA_FUNCTION(Lua_SpriteGetAllAnimationData)
{
	ANM2* anm2 = lua::GetUserdata<ANM2*>(L, 1, lua::Metatables::SPRITE, "Sprite");

	lua_newtable(L);
	for (unsigned int i = 0; i < anm2->GetAnimationCount(); ++i) {
		lua_pushinteger(L, i + 1);
		AnimationData** toLua = (AnimationData**)lua_newuserdata(L, sizeof(AnimationData*));
		*toLua = anm2->GetAnimationData(i);
		luaL_setmetatable(L, lua::metatables::AnimationDataMT);
		lua_settable(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_AnimationDataGetName)
{
	AnimationData* animationData = *lua::GetRawUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushstring(L, animationData->GetName().c_str());
	return 1;
}

LUA_FUNCTION(Lua_AnimationDataGetLength)
{
	AnimationData* animationData = *lua::GetRawUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushinteger(L, animationData->GetLength());
	return 1;
}

LUA_FUNCTION(Lua_AnimationDataGetLayerById)
{
	AnimationData* animationData = *lua::GetRawUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
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

LUA_FUNCTION(Lua_AnimationDataGetAllLayers)
{
	AnimationData* animationData = *lua::GetRawUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);

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

LUA_FUNCTION(Lua_AnimationDataIsLoopingAnimation)
{
	AnimationData* animationData = *lua::GetRawUserdata<AnimationData**>(L, 1, lua::metatables::AnimationDataMT);
	lua_pushboolean(L, animationData->IsLoopingAnimation());
	return 1;
}

static void RegisterAnimationData(lua_State* L) {
	luaL_Reg spriteFunctions[] = {
		{ "GetCurrentAnimationData", Lua_SpriteGetCurrentAnimationData },
		{ "GetOverlayAnimationData", Lua_SpriteGetOverlayAnimationData },
		{ "GetAnimationData", Lua_SpriteGetAnimationData },
		{ "GetAllAnimationData", Lua_SpriteGetAllAnimationData },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(L, lua::Metatables::SPRITE, spriteFunctions);

	luaL_Reg functions[] = {
		{ "GetAllLayers", Lua_AnimationDataGetAllLayers },
		{ "GetLayer", Lua_AnimationDataGetLayerById },
		{ "GetName", Lua_AnimationDataGetName },
		{ "GetLength", Lua_AnimationDataGetLength },
		{ "IsLoopingAnimation", Lua_AnimationDataIsLoopingAnimation },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::AnimationDataMT, lua::metatables::AnimationDataMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterAnimationData(_state);
}
