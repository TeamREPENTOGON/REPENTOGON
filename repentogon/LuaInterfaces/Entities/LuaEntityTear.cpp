#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_TearGetDeadEyeIntensity)
{
	Entity_Tear* tear = lua::GetUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua_pushnumber(L, tear->_deadEyeIntensity);
	return 1;
}

LUA_FUNCTION(Lua_TearMakeMultidimensionalCopy)
{
	Entity_Tear* tear = lua::GetUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	Entity* spawner = nullptr;
	lua::luabridge::UserdataPtr::push(L, tear->MakeMultidimensionalCopy(spawner), lua::Metatables::ENTITY_TEAR);
	return 1;
}

LUA_FUNCTION(Lua_GetTearHaloSprite) {
	Entity_Tear* tear = lua::GetUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_tearHaloANM2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_GetTearEffectSprite) {
	Entity_Tear* tear = lua::GetUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_tearEffectANM2, lua::Metatables::SPRITE);
	return 1;
}

LUA_FUNCTION(Lua_GetDeadEyeSprite) {
	Entity_Tear* tear = lua::GetUserdata<Entity_Tear*>(L, 1, lua::Metatables::ENTITY_TEAR, "EntityTear");
	lua::luabridge::UserdataPtr::push(L, &tear->_deadEyeANM2, lua::Metatables::SPRITE);
	return 1;
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
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_TEAR, functions);
}