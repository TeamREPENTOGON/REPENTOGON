#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/EntityPlus.h"

LUA_FUNCTION(Lua_EntityLaserGetDisableFollowParent)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushboolean(L, *laser->GetDisableFollowParent());

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserSetDisableFollowParent)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetDisableFollowParent() = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityLaserGetOneHit)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushboolean(L, *laser->GetOneHit());

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserGetShrink)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushboolean(L, *laser->GetShrink());

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserSetShrink)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	*laser->GetShrink() = lua::luaL_checkboolean(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_EntityLaserGetTimeout)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushinteger(L, *laser->GetTimeout());

	return 1;
}

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

LUA_FUNCTION(Lua_EntityLaserGetDamageMultiplier)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	lua_pushnumber(L, laser->_damageMultiplier);

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserSetDamageMultiplier)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	laser->_damageMultiplier = (float)luaL_checknumber(L, 2);

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

LUA_FUNCTION(Lua_EntityLaserGetHitList) {
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	std::vector<unsigned int> hitList = *laser->GetHitList();

	lua_newtable(L);
	int idx = 1;
	for (int index : hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityLaserRotateToAngle)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	float angle = (float)luaL_checknumber(L, 2);
	float speed = (float)luaL_optnumber(L, 3, 8.0f);
	laser->RotateToAngle(angle, speed);

	return 0;
}

LUA_FUNCTION(Lua_EntityLaserRecalculateSamplesNextUpdate)
{
	Entity_Laser* laser = lua::GetUserdata<Entity_Laser*>(L, 1, lua::Metatables::ENTITY, "EntityLaser");
	EntityLaserPlus* laserPlus = GetEntityLaserPlus(laser);
	if (laserPlus) {
		laserPlus->recalculateSamplesNextUpdate = true;
	}
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	
	luaL_Reg functions[] = {
		{ "GetDisableFollowParent", Lua_EntityLaserGetDisableFollowParent },
		{ "SetDisableFollowParent", Lua_EntityLaserSetDisableFollowParent },
		{ "GetHitList", Lua_EntityLaserGetHitList },
		{ "GetOneHit", Lua_EntityLaserGetOneHit },
		{ "GetScale", Lua_EntityLaserGetScale },
		{ "SetScale", Lua_EntityLaserSetScale },
		{ "GetDamageMultiplier", Lua_EntityLaserGetDamageMultiplier },
		{ "SetDamageMultiplier", Lua_EntityLaserSetDamageMultiplier },
		{ "GetShrink", Lua_EntityLaserGetShrink },
		{ "SetShrink", Lua_EntityLaserSetShrink },
		{ "GetTimeout", Lua_EntityLaserGetTimeout },
		{ "ResetSpriteScale", Lua_EntityLaserResetSpriteScale },
		{ "RotateToAngle", Lua_EntityLaserRotateToAngle },
		{ "RecalculateSamplesNextUpdate", Lua_EntityLaserRecalculateSamplesNextUpdate },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_LASER, functions);

	// fix HomingType
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_LASER, "HomingType", Lua_EntityLaserGetHomingType, Lua_EntityLaserSetHomingType);
}