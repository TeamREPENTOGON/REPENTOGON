#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/ASMPatches/ASMSplitTears.h"

LUA_FUNCTION(Lua_TearGetDeadEyeIntensity)
{
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua_pushnumber(L, tear->_deadEyeIntensity);
	return 1;
}

LUA_FUNCTION(Lua_TearMakeMultidimensionalCopy)
{
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	Entity* spawner = nullptr;
	lua::luabridge::UserdataPtr::push(L, tear->MakeMultidimensionalCopy(spawner), lua::Metatables::ENTITY_TEAR);
	return 1;
}

LUA_FUNCTION(Lua_GetTearHaloSprite) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_tearHaloANM2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_GetTearEffectSprite) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_tearEffectANM2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_GetDeadEyeSprite) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_deadEyeANM2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_TearResetSpriteScale)
{
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	bool force = lua::luaL_optboolean(L, 2, false);
	if (force) {
		tear->_scaleAnimNum = -1;
	}
	tear->ResetSpriteScale();
	return 0;
}

LUA_FUNCTION(Lua_IsMultidimensionalTouched) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua_pushboolean(L, tear->_multidimensionalTouched);
	return 1;
}

LUA_FUNCTION(Lua_SetMultidimensionalTouched) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	tear->_multidimensionalTouched = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_IsPrismTouched) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua_pushboolean(L, tear->_prismTouched);
	return 1;
}

LUA_FUNCTION(Lua_SetPrismTouched) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	tear->_prismTouched = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_GetHitList) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");

	lua_newtable(L);
	int idx = 1;
	for (int index : tear->_hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_ClearHitList) {
	Entity_Tear* tear = lua::GetLuabridgeUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	tear->_hitList.clear();

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetDeadEyeIntensity", Lua_TearGetDeadEyeIntensity },
		{ "MakeMultidimensionalCopy", Lua_TearMakeMultidimensionalCopy },
		{ "GetTearHaloSprite", Lua_GetTearHaloSprite },
		{ "GetTearEffectSprite", Lua_GetTearEffectSprite },
		{ "GetDeadEyeSprite", Lua_GetDeadEyeSprite },
		{ "ResetSpriteScale", Lua_TearResetSpriteScale },
		{ "IsMultidimensionalTouched", Lua_IsMultidimensionalTouched },
		{ "SetMultidimensionalTouched", Lua_SetMultidimensionalTouched },
		{ "IsPrismTouched", Lua_IsPrismTouched },
		{ "SetPrismTouched", Lua_SetPrismTouched },
		{ "GetHitList", Lua_GetHitList },
		{ "ClearHitList", Lua_ClearHitList },
		{ "FireSplitTear", SplitTears::Lua_FireSplitTear },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_TEAR, functions);
}