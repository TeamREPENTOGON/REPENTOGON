#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_ProjectileDeflect)
{
	Entity_Projectile* projectile = lua::GetUserdata<Entity_Projectile*>(L, 1, lua::Metatables::ENTITY_PROJECTILE, "EntityProjectile");
	Vector* vel = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	projectile->Reflect(nullptr, vel);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "Deflect", Lua_ProjectileDeflect },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PROJECTILE, functions);
}