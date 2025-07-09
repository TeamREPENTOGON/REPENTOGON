#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/ASMPatches/ASMCallbacks.h"
#include "../../Patches/EntityPlus.h"

LUA_FUNCTION(Lua_EntityAddBleeding)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBleeding(*ref, duration, false);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddMagnetized)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddMagnetized(*ref, duration, false);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddBaited)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBaited(*ref, duration, false);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddWeakness)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddWeakness(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddBrimstoneMark)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddBrimstoneMark(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddIce)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = (int)luaL_checkinteger(L, 3);
	ent->AddIce(*ref, duration);
	return 0;
}

LUA_FUNCTION(Lua_EntityAddKnockback)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector"); //replace with const?
	int duration = (int)luaL_checkinteger(L, 4);
	bool TakeImpactDamage = lua::luaL_checkboolean(L, 5);
	ent->AddKnockback(*ref, *pos, duration, TakeImpactDamage);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetShadowSize)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, *entity->GetShadowSize());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetShadowSize)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	float shadowSize = (float)luaL_checknumber(L, 2);
	*entity->GetShadowSize() = shadowSize;
	return 0;
}

LUA_FUNCTION(Lua_EntityComputeStatusEffectDuration)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	unsigned int initial = std::max((int)luaL_checkinteger(L, 2), 0);
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 3, lua::Metatables::ENTITY_REF, "EntityRef");
	lua_pushinteger(L, ent->ComputeStatusEffectDuration(initial, ref));

	return 1;
}

LUA_FUNCTION(Lua_EntityGetNullOffset)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const char* nullLayerName = luaL_checkstring(L, 2);

	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), entity->GetNullOffset(nullLayerName));

	return 1;
}

LUA_FUNCTION(lua_Entity_GetType) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetType());
	return 1;
}

LUA_FUNCTION(lua_Entity_GetPosVel) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* pos = &entity->_pos;
	Vector* vel = &entity->_velocity;

	PosVel posVel = PosVel(*pos, *vel);

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), posVel);

	return 1;
}

LUA_FUNCTION(Lua_EntityGetBossStatusEffectCooldown)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetBossStatusEffectCooldown());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBossStatusEffectCooldown)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int bossStatusEffectCooldown = (int)luaL_checkinteger(L, 2);
	*entity->GetBossStatusEffectCooldown() = bossStatusEffectCooldown;
	return 0;
}

void ForceCollideLaser(Entity_Laser* laser, Entity* entity) {
	if (laser->CanDamageEntity(entity) && !RunPreLaserCollisionCallback(laser, entity)) {
		laser->DoDamage(entity, laser->_collisionDamage);
	}
}

LUA_FUNCTION(Lua_EntityForceCollide)
{
	Entity* first = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* second = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	bool low = lua::luaL_checkboolean(L, 3);
	if (*first->GetType() == 7) {
		ForceCollideLaser((Entity_Laser*)first, second);
	}
	if (*second->GetType() == 7) {
		ForceCollideLaser((Entity_Laser*)second, first);
	}
	lua_pushboolean(L, first->ForceCollide(first, second, low));
	return 1;
}

LUA_FUNCTION(Lua_EntitySetDead)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isDead = lua::luaL_checkboolean(L, 2);
	*entity->IsDead() = isDead;
	return 0;
}

LUA_FUNCTION(Lua_EntitySetInvincible)
{
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isInvincible = lua::luaL_checkboolean(L, 2);
	*entity->IsInvincible() = isInvincible;
	return 0;
}

LUA_FUNCTION(lua_EntityGiveMinecart) {
	if (int n = lua_gettop(L); n != 3) {
		return luaL_error(L, "Expected two parameters, got %d\n", n - 1);
	}

	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* position = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* velocity = lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");

	Entity* minecart = entity->GiveMinecart(position, velocity);
	lua::luabridge::UserdataPtr::push(L, minecart, lua::GetMetatableKey(lua::Metatables::ENTITY_NPC));
	return 1;
}

LUA_FUNCTION(lua_EntityGetMinecart) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* minecart = entity->GetMinecart();

	if (!minecart) {
		lua_pushnil(L);
	}
	else if (minecart->ToNPC()) {
		lua::luabridge::UserdataPtr::push(L, minecart, lua::GetMetatableKey(lua::Metatables::ENTITY_NPC));
	}
	else {
		// bwuh?
		lua::luabridge::UserdataPtr::push(L, minecart, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityGetHitListIndex) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, entity->GetHitListIndex());
	return 1;
}

LUA_FUNCTION(Lua_EntityGetPauseTime) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetPauseTime());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetPauseTime) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	*entity->GetPauseTime() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_Entity_GetSpeedMultiplier) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, entity->_speedMultiplier);
	return 1;
}

LUA_FUNCTION(Lua_Entity_SetSpeedMultiplier) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	entity->_speedMultiplier = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityTryThrow) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* dir = lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	const float force = (float)luaL_checknumber(L, 4);
	lua_pushboolean(L, entity->TryThrow(*ref, dir, force));
	return 1;
}

// will need an asm patch to return a table of effects
LUA_FUNCTION(Lua_EntitySpawnWaterImpactEffects) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* pos = lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector vel;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		vel = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	}
	const float scale = (float)luaL_checknumber(L, 4);

	Entity::DoGroundImpactEffects(pos, &vel, scale);

	return 0;
}

LUA_FUNCTION(Lua_EntityGetPredictedTargetPosition) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* target = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	const float delay = (float)luaL_checknumber(L, 3);
	Vector res = entity->GetPredictedTargetPosition(target, delay);

	lua::luabridge::UserdataPtr::push(L, &res, lua::GetMetatableKey(lua::Metatables::VECTOR));

	return 1;
}

LUA_FUNCTION(Lua_Entity_MakeBloodEffect) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int subtype = (int)luaL_optinteger(L, 2, 0);
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pos = *lua::GetLuabridgeUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	}
	Vector offset;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		offset = *lua::GetLuabridgeUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	}
	ColorMod color;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		color = *lua::GetLuabridgeUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}
	Vector velocity;
	if (lua_type(L, 6) == LUA_TUSERDATA) {
		velocity = *lua::GetLuabridgeUserdata<Vector*>(L, 6, lua::Metatables::VECTOR, "Vector");
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 2, pos, velocity, nullptr, subtype, Isaac::genrand_int32(), 0);
	effect->SetColor(&color, -1, 255, false, true);
	effect->_sprite._offset = offset;
	effect->_depthOffset = -10.0f;
	lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

// TODO: asm patch to return effect
LUA_FUNCTION(Lua_EntityMakeBloodPoof) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}
	
	ColorMod color;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		color = *lua::GetLuabridgeUserdata<ColorMod*>(L, 3, lua::Metatables::COLOR, "Color");
	}
	
	float scale = (float)luaL_optnumber(L, 4, 1.0f);

	Entity_Effect* poof = entity->MakeBloodPoof(&pos, &color, scale);
	if (!poof) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, poof, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityMakeGroundPoof) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}

	ColorMod color;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		color = *lua::GetLuabridgeUserdata<ColorMod*>(L, 3, lua::Metatables::COLOR, "Color");
	}

	float scale = (float)luaL_optnumber(L, 4, 1.0f);

	Entity_Effect* poof = entity->MakeGroundPoof(&pos, &color, scale);
	if (!poof) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, poof, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityIgnoreEffectFromFriendly) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetLuabridgeUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	lua_pushboolean(L, entity->IgnoreEffectFromFriendly(ref));
	return 0;
}

LUA_FUNCTION(Lua_EntityTeleportToRandomPosition) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	entity->TeleportToRandomPosition(true);
	return 0;
}

#define nonzero(a,b)	((a == 0) ? (b) : (a))

inline void SlowTrackCopyStatusEffects(Entity* ent1, Entity* ent2) {
	UINT64 statusFlags[2] = { ent1->_flags ^ EntityFlag::FLAG_NON_STATUS_EFFECTS, ent2->_flags ^ EntityFlag::FLAG_NON_STATUS_EFFECTS };
	UINT64 resFlags = statusFlags[0] ^ statusFlags[1]; // get difference in status flags between ent1 and ent2
	ent2->_flags |= resFlags; // apply difference in flags to ent2

	ent2->_freezeCountdown = nonzero(ent1->_freezeCountdown, ent2->_freezeCountdown);
	ent2->_poisonCountdown = nonzero(ent1->_poisonCountdown, ent2->_poisonCountdown);
	ent2->_slowingCountdown = nonzero(ent1->_slowingCountdown, ent2->_slowingCountdown);
	ent2->_charmedCountdown = nonzero(ent1->_charmedCountdown, ent2->_charmedCountdown);
	ent2->_confusionCountdown = nonzero(ent1->_confusionCountdown, ent2->_confusionCountdown);
	ent2->_midasFreezeCountdown = nonzero(ent1->_midasFreezeCountdown, ent2->_midasFreezeCountdown);
	ent2->_fearCountdown = nonzero(ent1->_fearCountdown, ent2->_fearCountdown);
	ent2->_burnCountdown = nonzero(ent1->_burnCountdown, ent2->_burnCountdown);
	ent2->_bleedingCountdown = nonzero(ent1->_bleedingCountdown, ent2->_bleedingCountdown);
	ent2->_shrinkCountdown = nonzero(ent1->_shrinkCountdown, ent2->_shrinkCountdown);
	ent2->_poisonDamage = nonzero(ent1->_poisonDamage, ent2->_poisonDamage);
	ent2->_burnDamage = nonzero(ent1->_burnDamage, ent2->_burnDamage);
	ent2->_magnetizedCountdown = nonzero(ent1->_magnetizedCountdown, ent2->_magnetizedCountdown);
	ent2->_baitedCountdown = nonzero(ent1->_baitedCountdown, ent2->_baitedCountdown);
	ent2->_knockbackCountdown = nonzero(ent1->_knockbackCountdown, ent2->_knockbackCountdown);
	ent2->_knockbackDirection = (ent1->_knockbackDirection.x != 0 || ent1->_knockbackDirection.y != 0) ? ent1->_knockbackDirection : ent2->_knockbackDirection;
	ent2->_iceCountdown = nonzero(ent1->_iceCountdown, ent2->_iceCountdown);
	ent2->_weaknessCountdown = nonzero(ent1->_weaknessCountdown, ent2->_weaknessCountdown);
	ent2->_brimstoneMarkCountdown = nonzero(ent1->_brimstoneMarkCountdown, ent2->_brimstoneMarkCountdown);
	ent2->_shrinkStatus1 = nonzero(ent1->_shrinkStatus1, ent2->_shrinkStatus1);
	ent2->_shrinkStatus2 = nonzero(ent1->_shrinkStatus2, ent2->_shrinkStatus2);

	if (ent1->_type >= 10 && ent1->_type < 1000) {
		Entity_NPC* npc = static_cast<Entity_NPC*>(ent2);
		if (npc->_isBoss) {
			ent2->_bossStatusEffectCooldown = std::max(ent1->_bossStatusEffectCooldown, ent2->_bossStatusEffectCooldown);
		}
	}

	for (ColorParams& p : ent1->_colorParams) {
		// try to automatically determine what colors should be shared
		if (p._priority == 255 && !p._fadeout && p._shared) {
			ent2->_colorParams.push_back(p);
		}
	}
}

#undef nonzero

inline void FastTrackCopyStatusEffects(Entity* ent1, Entity* ent2) {
	UINT64 statusFlags = ent1->_flags & EntityFlag::FLAG_STATUS_EFFECTS;
	ent2->_flags &= EntityFlag::FLAG_NON_STATUS_EFFECTS; // remove ent2 status effect flags
	ent2->_flags |= statusFlags; // add ent1 status effect flags

	ent2->_freezeCountdown = ent1->_freezeCountdown;
	ent2->_poisonCountdown = ent1->_poisonCountdown;
	ent2->_slowingCountdown = ent1->_slowingCountdown;
	ent2->_charmedCountdown = ent1->_charmedCountdown;
	ent2->_confusionCountdown = ent1->_confusionCountdown;
	ent2->_midasFreezeCountdown = ent1->_midasFreezeCountdown;
	ent2->_fearCountdown = ent1->_fearCountdown;
	ent2->_burnCountdown = ent1->_burnCountdown;
	ent2->_bleedingCountdown = ent1->_bleedingCountdown;
	ent2->_shrinkCountdown = ent1->_shrinkCountdown;
	ent2->_poisonDamage = ent1->_poisonDamage;
	ent2->_burnDamage = ent1->_burnDamage;
	ent2->_magnetizedCountdown = ent1->_magnetizedCountdown;
	ent2->_baitedCountdown = ent1->_baitedCountdown;
	ent2->_knockbackCountdown = ent1->_knockbackCountdown;
	ent2->_knockbackDirection = ent1->_knockbackDirection;
	ent2->_iceCountdown = ent1->_iceCountdown;
	ent2->_weaknessCountdown = ent1->_weaknessCountdown;
	ent2->_brimstoneMarkCountdown = ent1->_brimstoneMarkCountdown;
	ent2->_shrinkStatus1 = ent1->_shrinkStatus1;
	ent2->_shrinkStatus2 = ent1->_shrinkStatus2;

	if (ent1->_type >= 10 && ent1->_type < 1000) {
		Entity_NPC* npc = static_cast<Entity_NPC*>(ent2);
		if (npc->_isBoss) {
			ent2->_bossStatusEffectCooldown = ent1->_bossStatusEffectCooldown;
		}
	}

	for (ColorParams& p : ent2->_colorParams) {
		// try to automatically determine what colors should be removed
		if (p._priority == 255 && !p._fadeout && p._shared) {
			p._duration2 = 1; // make it go away while still properly handling other colors
		}
	}
	for (ColorParams& p : ent1->_colorParams) {
		// try to automatically determine what colors should be shared
		if (p._priority == 255 && !p._fadeout && p._shared) {
			ent2->_colorParams.push_back(p);
		}
	}
}

void CopyStatusEffects(Entity* ent1, Entity* ent2, bool overwrite) {
	if (overwrite) {
		FastTrackCopyStatusEffects(ent1, ent2);
	}
	else
	{
		SlowTrackCopyStatusEffects(ent1, ent2);
	}
}

LUA_FUNCTION(Lua_EntityCopyStatusEffects) {
	Entity* ent1 = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool overwrite = lua::luaL_optboolean(L, 3, false);
	if (lua_isnil(L, 2)) {
		for (Entity* child = ent1->GetChild(); child != (Entity*)0x0; child = child->GetChild()) {
			CopyStatusEffects(ent1, child, overwrite);
		}
	}
	else
	{
		Entity* ent2 = lua::GetLuabridgeUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
		CopyStatusEffects(ent1, ent2, overwrite);
	}

	return 0;
}

LUA_FUNCTION(Lua_EntityGetColorParams) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_newtable(L);
	for (size_t i = 0; i < entity->_colorParams.size(); ++i) {
		lua_pushinteger(L, i + 1);
		ColorParams* ud = (ColorParams*)lua_newuserdata(L, sizeof(ColorParams));
		*ud = entity->_colorParams[i];
		luaL_setmetatable(L, lua::metatables::ColorParamsMT);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntitySetColorParams) {
	Entity* entity = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (!lua_istable(L, 2))
	{
		return luaL_argerror(L, 2, "Expected a table as second argument");
	}

	size_t length = (size_t)lua_rawlen(L, 2);

	if (length == 0)
	{
		entity->_colorParams.clear();
	}
	else
	{
		vector_ColorParams list;
		list.reserve(length);
		for (size_t i = 0; i < length; i++)
		{
			lua_rawgeti(L, 2, i + 1);
			list.push_back(*lua::GetRawUserdata<ColorParams*>(L, -1, lua::metatables::ColorParamsMT));
			lua_pop(L, 1);
		}
		entity->_colorParams = list;
	}

	return 0;
}

LUA_FUNCTION(Lua_EntityGetDamageCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *ent->GetDamageCountdown());
	return 1;
}

LUA_FUNCTION(Lua_EntitySetDamageCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int countdown = (int)luaL_checkinteger(L, 2);
	if (countdown < 0) countdown = 0;
	*ent->GetDamageCountdown() = countdown;
	return 0;
}

LUA_FUNCTION(Lua_EntityGetFireDamageCooldown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_fireDamageCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetFireDamageCooldown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_fireDamageCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetFreezeCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_freezeCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetFreezeCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_freezeCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetPoisonCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_poisonCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetPoisonCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_poisonCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetSlowingCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_slowingCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetSlowingCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_slowingCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetCharmedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_charmedCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetCharmedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_charmedCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetConfusionCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_confusionCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetConfusionCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_confusionCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetMidasFreezeCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_midasFreezeCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetMidasFreezeCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_midasFreezeCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetFearCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_fearCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetFearCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_fearCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBurnCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_burnCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBurnCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_burnCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetShrinkCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_shrinkCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetShrinkCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_shrinkCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBleedingCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_bleedingCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBleedingCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_bleedingCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetMagnetizedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_magnetizedCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetMagnetizedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_magnetizedCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBaitedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_baitedCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBaitedCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_baitedCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetKnockbackCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_knockbackCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetKnockbackCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_knockbackCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetWeaknessCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_weaknessCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetWeaknessCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_weaknessCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetIceCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_iceCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetIceCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_iceCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBrimstoneMarkCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_brimstoneMarkCountdown);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBrimstoneMarkCountdown) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_brimstoneMarkCountdown = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetPoisonDamageTimer) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_poisonDamageTimer);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetPoisonDamageTimer) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_poisonDamageTimer = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBurnDamageTimer) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, ent->_burnDamageTimer);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBurnDamageTimer) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_burnDamageTimer = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetPoisonDamage) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, ent->_poisonDamage);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetPoisonDamage) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_poisonDamage = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetBurnDamage) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, ent->_burnDamage);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetBurnDamage) {
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_burnDamage = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_EntityGetKnockbackDirection)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = ent->_knockbackDirection;
	return 1;
}

LUA_FUNCTION(Lua_EntitySetKnockbackDirection)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	ent->_knockbackDirection = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	return 0;
}

LUA_FUNCTION(Lua_EntityGetWaterClipFlags)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	WaterClipInfo info;
	ent->GetWaterClipInfo(&info);
	lua_pushinteger(L, info.bitFlags);
	return 1;
}

LUA_FUNCTION(Lua_EntitySetWaterClipFlags)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const uint32_t flags = (uint32_t)luaL_checkinteger(L, 2);
	EntityPlus* entityPlus = GetEntityPlus(ent);
	if (entityPlus) {
		entityPlus->waterClipInfoFlagsOverride = flags;
	}
	return 0;
}

LUA_FUNCTION(Lua_EntityResetWaterClipFlags)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityPlus* entityPlus = GetEntityPlus(ent);
	if (entityPlus) {
		entityPlus->waterClipInfoFlagsOverride = std::nullopt;
	}
	return 0;
}

LUA_FUNCTION(Lua_EntityCanDevolve)
{
	Entity* ent = lua::GetLuabridgeUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushboolean(L, ent->CanDevolve());

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
		{ "ComputeStatusEffectDuration", Lua_EntityComputeStatusEffectDuration },
		{ "CopyStatusEffects", Lua_EntityCopyStatusEffects },
		{ "IgnoreEffectFromFriendly", Lua_EntityIgnoreEffectFromFriendly },
		{ "GetNullOffset", Lua_EntityGetNullOffset },
		{ "GetPredictedTargetPosition", Lua_EntityGetPredictedTargetPosition },
		{ "GetType", lua_Entity_GetType },
		{ "GetPosVel", lua_Entity_GetPosVel },
		{ "GetBossStatusEffectCooldown", Lua_EntityGetBossStatusEffectCooldown },
		{ "SetBossStatusEffectCooldown", Lua_EntitySetBossStatusEffectCooldown },
		{ "ForceCollide", Lua_EntityForceCollide },
		{ "SetDead", Lua_EntitySetDead },
		{ "SetInvincible", Lua_EntitySetInvincible },
		{ "GiveMinecart", lua_EntityGiveMinecart },
		{ "GetMinecart", lua_EntityGetMinecart },
		{ "MakeBloodPoof", Lua_EntityMakeBloodPoof },
		{ "MakeGroundPoof", Lua_EntityMakeGroundPoof },
		{ "GetHitListIndex", Lua_EntityGetHitListIndex },
		{ "GetPauseTime", Lua_EntityGetPauseTime },
		{ "SetPauseTime", Lua_EntitySetPauseTime },
		{ "GetSpeedMultiplier", Lua_Entity_GetSpeedMultiplier },
		{ "SetSpeedMultiplier", Lua_Entity_SetSpeedMultiplier },
		{ "SpawnBloodEffect", Lua_Entity_MakeBloodEffect },
		{ "SpawnWaterImpactEffects", Lua_EntitySpawnWaterImpactEffects },
		{ "TeleportToRandomPosition", Lua_EntityTeleportToRandomPosition },
		{ "TryThrow", Lua_EntityTryThrow },
		{ "GetColorParams", Lua_EntityGetColorParams },
		{ "SetColorParams", Lua_EntitySetColorParams },
		{ "GetDamageCountdown", Lua_EntityGetDamageCountdown },
		{ "SetDamageCountdown", Lua_EntitySetDamageCountdown },
		{ "GetFireDamageCooldown", Lua_EntityGetFireDamageCooldown },
		{ "SetFireDamageCooldown", Lua_EntitySetFireDamageCooldown },
		{ "GetFreezeCountdown", Lua_EntityGetFreezeCountdown },
		{ "SetFreezeCountdown", Lua_EntitySetFreezeCountdown },
		{ "GetPoisonCountdown", Lua_EntityGetPoisonCountdown },
		{ "SetPoisonCountdown", Lua_EntitySetPoisonCountdown },
		{ "GetSlowingCountdown", Lua_EntityGetSlowingCountdown },
		{ "SetSlowingCountdown", Lua_EntitySetSlowingCountdown },
		{ "GetCharmedCountdown", Lua_EntityGetCharmedCountdown },
		{ "SetCharmedCountdown", Lua_EntitySetCharmedCountdown },
		{ "GetConfusionCountdown", Lua_EntityGetConfusionCountdown },
		{ "SetConfusionCountdown", Lua_EntitySetConfusionCountdown },
		{ "GetMidasFreezeCountdown", Lua_EntityGetMidasFreezeCountdown },
		{ "SetMidasFreezeCountdown", Lua_EntitySetMidasFreezeCountdown },
		{ "GetFearCountdown", Lua_EntityGetFearCountdown },
		{ "SetFearCountdown", Lua_EntitySetFearCountdown },
		{ "GetBurnCountdown", Lua_EntityGetBurnCountdown },
		{ "SetBurnCountdown", Lua_EntitySetBurnCountdown },
		{ "GetShrinkCountdown", Lua_EntityGetShrinkCountdown },
		{ "SetShrinkCountdown", Lua_EntitySetShrinkCountdown },
		{ "GetBleedingCountdown", Lua_EntityGetBleedingCountdown },
		{ "SetBleedingCountdown", Lua_EntitySetBleedingCountdown },
		{ "GetMagnetizedCountdown", Lua_EntityGetMagnetizedCountdown },
		{ "SetMagnetizedCountdown", Lua_EntitySetMagnetizedCountdown },
		{ "GetBaitedCountdown", Lua_EntityGetBaitedCountdown },
		{ "SetBaitedCountdown", Lua_EntitySetBaitedCountdown },
		{ "GetKnockbackCountdown", Lua_EntityGetKnockbackCountdown },
		{ "SetKnockbackCountdown", Lua_EntitySetKnockbackCountdown },
		{ "GetWeaknessCountdown", Lua_EntityGetWeaknessCountdown },
		{ "SetWeaknessCountdown", Lua_EntitySetWeaknessCountdown },
		{ "GetIceCountdown", Lua_EntityGetIceCountdown },
		{ "SetIceCountdown", Lua_EntitySetIceCountdown },
		{ "GetBrimstoneMarkCountdown", Lua_EntityGetBrimstoneMarkCountdown },
		{ "SetBrimstoneMarkCountdown", Lua_EntitySetBrimstoneMarkCountdown },
		{ "GetPoisonDamageTimer", Lua_EntityGetPoisonDamageTimer },
		{ "SetPoisonDamageTimer", Lua_EntitySetPoisonDamageTimer },
		{ "GetBurnDamageTimer", Lua_EntityGetBurnDamageTimer },
		{ "SetBurnDamageTimer", Lua_EntitySetBurnDamageTimer },
		{ "GetPoisonDamage", Lua_EntityGetPoisonDamage },
		{ "SetPoisonDamage", Lua_EntitySetPoisonDamage },
		{ "GetBurnDamage", Lua_EntityGetBurnDamage },
		{ "SetBurnDamage", Lua_EntitySetBurnDamage },
		{ "GetKnockbackDirection", Lua_EntityGetKnockbackDirection },
		{ "SetKnockbackDirection", Lua_EntitySetKnockbackDirection },
		{ "GetWaterClipFlags", Lua_EntityGetWaterClipFlags },
		{ "SetWaterClipFlags", Lua_EntitySetWaterClipFlags },
		{ "ResetWaterClipFlags", Lua_EntityResetWaterClipFlags },
		{ "CanDevolve", Lua_EntityCanDevolve },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY, functions);
}
