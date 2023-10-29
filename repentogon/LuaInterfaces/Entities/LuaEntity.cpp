#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_EntityAddBleeding)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBleeding(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddMagnetized)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddMagnetized(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddBaited)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBaited(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddWeakness)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddWeakness(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddBrimstoneMark)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBrimstoneMark(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddIce)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddIce(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddKnockback)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector"); //replace with const?
	int duration = (int)luaL_checkinteger(L, 4);
	bool TakeImpactDamage = lua_toboolean(L, 5);
	ent->AddKnockback(*ref, *pos, duration, TakeImpactDamage);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetShadowSize)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, *entity->GetShadowSize());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetShadowSize)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	float shadowSize = (float)luaL_checknumber(L, 2);
	*entity->GetShadowSize() = shadowSize;
	return 0;
}

LUA_FUNCTION(Lua_EntityCopyStatusEffects)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");

	ent->CopyStatusEffects();

	return 0;
}


LUA_FUNCTION(Lua_EntityGetNullOffset)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const char* nullLayerName = luaL_checkstring(L, 2);

	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), entity->GetNullOffset(nullLayerName));

	return 1;
}

LUA_FUNCTION(lua_Entity_GetType) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetType());
	return 1;
}

LUA_FUNCTION(lua_Entity_GetPosVel) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* pos = (Vector*)((char*)entity + 0x294);
	Vector* vel = (Vector*)((char*)entity + 0x2B8);

	PosVel posVel = PosVel(*pos, *vel);

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), posVel);

	return 1;
}

LUA_FUNCTION(Lua_EntityGetBossStatusEffectCooldown)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetBossStatusEffectCooldown());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBossStatusEffectCooldown)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int bossStatusEffectCooldown = (int)luaL_checkinteger(L, 2);
	*entity->GetBossStatusEffectCooldown() = bossStatusEffectCooldown;
	return 0;
}

LUA_FUNCTION(Lua_EntityForceCollide)
{
	Entity* first = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* second = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	bool low = lua_toboolean(L, 3);
	lua_pushboolean(L, first->ForceCollide(first, second, low));
	return 1;
}

LUA_FUNCTION(Lua_EntitySetDead)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isDead = lua_toboolean(L, 2);
	*entity->IsDead() = isDead;
	return 0;
}

LUA_FUNCTION(Lua_EntitySetInvincible)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isInvincible = lua_toboolean(L, 2);
	*entity->IsInvincible() = isInvincible;
	return 0;
}

LUA_FUNCTION(lua_EntityGiveMinecart) {
	if (int n = lua_gettop(L); n != 3) {
		return luaL_error(L, "Expected two parameters, got %d\n", n - 1);
	}

	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* velocity = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	Entity* minecart = entity->GiveMinecart(position, velocity);
	lua::luabridge::UserdataPtr::push(L, minecart, lua::GetMetatableKey(lua::Metatables::ENTITY_NPC));
	return 1;
}

LUA_FUNCTION(lua_EntityGetMinecart) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity_NPC* minecart = entity->GetMinecart();

	if (!minecart) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, minecart, lua::GetMetatableKey(lua::Metatables::ENTITY_NPC));
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityGetHitListIndex) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, entity->GetHitListIndex());
	return 1;
}

LUA_FUNCTION(Lua_EntityGetPauseTime) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetPauseTime());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetPauseTime) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	*entity->GetPauseTime() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_Entity_GetSpeedMultiplier) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, entity->_speedMultiplier);
	return 1;
}

LUA_FUNCTION(Lua_Entity_SetSpeedMultiplier) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	entity->_speedMultiplier = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityTryThrow) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* dir = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	const float force = luaL_checknumber(L, 4);
	lua_pushboolean(L, entity->TryThrow(*ref, dir, force));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "AddBleeding", Lua_EntityAddBleeding },
		{ "AddMagnetized", Lua_EntityAddMagnetized },
		{ "AddBaited", Lua_EntityAddBaited },
		{ "AddWeakness", Lua_EntityAddWeakness },
		{ "AddBrimstoneMark", Lua_EntityAddBrimstoneMark },
		{ "AddIce", Lua_EntityAddIce },
		{ "AddKnockback", Lua_EntityAddKnockback },
		{ "GetShadowSize", Lua_EntityGetShadowSize },
		{ "SetShadowSize", Lua_EntitySetShadowSize },
		{ "AddKnockback", Lua_EntityAddKnockback },
		{ "CopyStatusEffects", Lua_EntityCopyStatusEffects },
		{ "GetNullOffset", Lua_EntityGetNullOffset },
		{ "GetType", lua_Entity_GetType },
		{ "GetPosVel", lua_Entity_GetPosVel },
		{ "GetBossStatusEffectCooldown", Lua_EntityGetBossStatusEffectCooldown },
		{ "SetBossStatusEffectCooldown", Lua_EntitySetBossStatusEffectCooldown },
		{ "ForceCollide", Lua_EntityForceCollide },
		{ "SetDead", Lua_EntitySetDead },
		{ "SetInvincible", Lua_EntitySetInvincible },
		{ "GiveMinecart", lua_EntityGiveMinecart },
		{ "GetMinecart", lua_EntityGetMinecart },
		{ "GetHitListIndex", Lua_EntityGetHitListIndex },
		{ "GetPauseTime", Lua_EntityGetPauseTime },
		{ "SetPauseTime", Lua_EntitySetPauseTime },
		{ "GetSpeedMultiplier", Lua_Entity_GetSpeedMultiplier },
		{ "SetSpeedMultiplier", Lua_Entity_SetSpeedMultiplier },
		{ "TryThrow", Lua_EntityTryThrow },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PLAYER, functions);

}