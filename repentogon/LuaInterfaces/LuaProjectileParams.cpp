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
	float damage = (float)luaL_checknumber(L, 2);

	if (damage < 0) {
		// This used to be here and crashed, for whatever reason.
		// return luaL_error(L, "Invalid damage value %f", damage);
		// But also, -1 is actually the default value for this field, so erroring when trying to set that value doesn't really seem right.
		damage = -1;
	}

	params->Damage = damage;
	return 0;
}

static void RegisterProjectileParamsDamage(lua_State* L) {
	lua::RegisterVariable(L, lua::Metatables::PROJECTILE_PARAMS, "Damage", lua_ProjectileParams_GetDamage, lua_ProjectileParams_SetDamage);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterProjectileParamsDamage(_state);
}