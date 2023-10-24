#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityLaserGetScale)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushnumber(L, *laser->GetScale());

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserSetScale)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetScale() = (float)luaL_checknumber(L, 2);
	laser->ResetSpriteScale();

	return 0;
}

LUA_FUNCTION(Lua_EntityLaserResetSpriteScale)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	laser->ResetSpriteScale();

	return 0;
}

LUA_FUNCTION(Lua_EntityLaserGetHomingType)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushinteger(L, *laser->GetHomingType());

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserSetHomingType)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetHomingType() = (uint32_t)luaL_checkinteger(L, 2);

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetScale", Lua_EntityLaserGetScale },
		{ "SetScale", Lua_EntityLaserSetScale },
		{ "ResetSpriteScale", Lua_EntityLaserResetSpriteScale },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_LASER, functions);

	// fix HomingType
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_LASER, "HomingType", Lua_EntityLaserGetHomingType, Lua_EntityLaserSetHomingType);
}