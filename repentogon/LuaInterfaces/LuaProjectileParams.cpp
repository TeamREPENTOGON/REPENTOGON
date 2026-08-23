#include "LuaCore.h"
#include "HookSystem.h"
#include "IsaacRepentance.h"

LUA_FUNCTION(lua_ProjectileParams_GetDamage) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	lua_pushnumber(L, params->Damage);
	return 1;
}

LUA_FUNCTION(lua_ProjectileParams_SetDamage) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
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

LUA_FUNCTION(lua_ProjectileParams_GetFireDirectionLimit) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");

	lua::ffi::pushCdata(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR], params->FireDirectionLimit);
	return 1;
}

LUA_FUNCTION(lua_ProjectileParams_SetFireDirectionLimit) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	Vector* limit = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");

	params->FireDirectionLimit = *limit;
	return 0;
}

LUA_FUNCTION(lua_ProjectileParams_GetPositionOffset) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");

	lua::ffi::pushCdata(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR], params->PositionOffset);
	return 1;
}

LUA_FUNCTION(lua_ProjectileParams_SetPositionOffset) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	Vector* offset = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");

	params->PositionOffset = *offset;
	return 0;
}

LUA_FUNCTION(lua_ProjectileParams_GetTargetPosition) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");

	lua::ffi::pushCdata(L, lua::ffi::CData[lua::ffi::CDataID::VECTOR], params->TargetPosition);
	return 1;
}

LUA_FUNCTION(lua_ProjectileParams_SetTargetPosition) {
	ProjectileParams* params = lua::GetLuabridgeUserdata<ProjectileParams*>(L, 1, lua::Metatables::PROJECTILE_PARAMS, "ProjectileParams");
	Vector* pos = lua::GetCData<Vector*>(L, 2, lua::ffi::CData[lua::ffi::CDataID::VECTOR], "Vector");

	params->TargetPosition = *pos;
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	
	lua::RegisterVariable(_state, lua::Metatables::PROJECTILE_PARAMS, "FireDirectionLimit", lua_ProjectileParams_GetFireDirectionLimit, lua_ProjectileParams_SetFireDirectionLimit);
	lua::RegisterVariable(_state, lua::Metatables::PROJECTILE_PARAMS, "PositionOffset", lua_ProjectileParams_GetPositionOffset, lua_ProjectileParams_SetPositionOffset);
	lua::RegisterVariable(_state, lua::Metatables::PROJECTILE_PARAMS, "TargetPosition", lua_ProjectileParams_GetTargetPosition, lua_ProjectileParams_SetTargetPosition);
	lua::RegisterVariable(_state, lua::Metatables::PROJECTILE_PARAMS, "Damage", lua_ProjectileParams_GetDamage, lua_ProjectileParams_SetDamage);

}