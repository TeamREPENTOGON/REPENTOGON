#include "LuaCore.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"

LUA_FUNCTION(lua_ProjectileParams_GetDamage) {
	ProjectileParams* params = lua::GetUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	lua_pushnumber(L, params->Damage);
	return 1;
}

LUA_FUNCTION(lua_ProjectileParams_SetDamage) {
	ProjectileParams* params = lua::GetUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	float damage = luaL_checknumber(L, 2);

	if (damage < 0) {
		return luaL_error(L, "Invalid damage value %f\n", damage);
	}

	params->Damage = damage;
	return 0;
}

static void RegisterProjectileParamsDamage(lua_State* L) {
	lua::LuaStackProtector protector(L);

	lua::PushMetatable(L, lua::Metatables::PROJECTILE_PARAMS);
	lua_pushstring(L, "__propget");
	lua_rawget(L, -2);

	lua_pushstring(L, "Damage");
	lua_pushcfunction(L, lua_ProjectileParams_GetDamage);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	lua_pushstring(L, "__propset");
	lua_rawget(L, -2);

	lua_pushstring(L, "Damage");
	lua_pushcfunction(L, lua_ProjectileParams_SetDamage);
	lua_rawset(L, -3);
	lua_pop(L, 2);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	RegisterProjectileParamsDamage(_state);
}