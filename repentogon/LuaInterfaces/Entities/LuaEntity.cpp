#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "ASMPatcher.hpp"
#include "SigScan.h"
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
	bool TakeImpactDamage = lua::luaL_checkboolean(L, 5);
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

LUA_FUNCTION(Lua_EntityComputeStatusEffectDuration)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	unsigned int initial = max((int)luaL_checkinteger(L, 2), 0);
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	lua_pushinteger(L, ent->ComputeStatusEffectDuration(initial, ref));

	return 1;
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
	bool low = lua::luaL_checkboolean(L, 3);
	lua_pushboolean(L, first->ForceCollide(first, second, low));
	return 1;
}

LUA_FUNCTION(Lua_EntitySetDead)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isDead = lua::luaL_checkboolean(L, 2);
	*entity->IsDead() = isDead;
	return 0;
}

LUA_FUNCTION(Lua_EntitySetInvincible)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isInvincible = lua::luaL_checkboolean(L, 2);
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
	const float force = (float)luaL_checknumber(L, 4);
	lua_pushboolean(L, entity->TryThrow(*ref, dir, force));
	return 1;
}

// will need an asm patch to return a table of effects
LUA_FUNCTION(Lua_EntitySpawnWaterImpactEffects) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector vel;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		vel = *lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	}
	const float scale = (float)luaL_checknumber(L, 4);

	Entity::DoGroundImpactEffects(pos, &vel, scale);

	return 0;
}

LUA_FUNCTION(Lua_EntityGetPredictedTargetPosition) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* target = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	const float delay = (float)luaL_checknumber(L, 3);
	Vector res;
	entity->GetPredictedTargetPosition(&res, target, delay);

	lua::luabridge::UserdataPtr::push(L, &res, lua::GetMetatableKey(lua::Metatables::VECTOR));

	return 1;
}

LUA_FUNCTION(Lua_Entity_MakeBloodEffect) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int subtype = (int)luaL_optinteger(L, 2, 0);
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pos = *lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	}
	Vector offset;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		offset = *lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	}
	ColorMod color;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 5, lua::Metatables::COLOR, "Color");
	}
	Vector velocity;
	if (lua_type(L, 6) == LUA_TUSERDATA) {
		velocity = *lua::GetUserdata<Vector*>(L, 6, lua::Metatables::VECTOR, "Vector");
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
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}
	
	ColorMod color;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");
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
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector pos = *entity->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}

	ColorMod color;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		color = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");
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
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	lua_pushboolean(L, entity->IgnoreEffectFromFriendly(ref));
	return 0;
}

LUA_FUNCTION(Lua_EntityTeleportToRandomPosition) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	entity->TeleportToRandomPosition(true);
	return 0;
}

void CopyStatusEffects(Entity* ent1, Entity* ent2) {
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
	// don't ask me what this does
	ent2->_flags[0] = ent2->_flags[0] & 0xdeffe01f | ent1->_flags[0] & 0x21001fe0;
	ent2->_flags[1] = ent2->_flags[1] & 0xfea27fff | ent1->_flags[1] & 0x15d8000;

	ent2->_entityColors = ent1->_entityColors;
	ent2->_sprite._color = ent1->_sprite._color;
}

LUA_FUNCTION(Lua_EntityCopyStatusEffects) {
	Entity* ent1 = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity* ent2 = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
		CopyStatusEffects(ent1, ent2);
	}
	else
	{
		for (Entity* child = ent1->_child; child != (Entity*)0x0; child = child->_child) {
			CopyStatusEffects(ent1, child);
		}
	}

	return 0;
}

LUA_FUNCTION(Lua_EntityGetEntityColors) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_newtable(L);
	for (size_t i = 0; i < entity->_entityColors.size(); ++i) {
		lua_pushinteger(L, i + 1);
		EntityColor* ud = (EntityColor*)lua_newuserdata(L, sizeof(EntityColor));
		*ud = entity->_entityColors[i];
		luaL_setmetatable(L, lua::metatables::EntityColorMT);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntitySetEntityColors) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	if (!lua_istable(L, 2))
	{
		return luaL_argerror(L, 2, "Expected a table as second argument");
	}

	size_t length = (size_t)lua_rawlen(L, 2);

	if (length == 0)
	{
		entity->_entityColors.clear();
		entity->ResetColor();
	}
	else
	{
		vector_EntityColor list;
		list.reserve(length);
		for (size_t i = 0; i < length; i++)
		{
			lua_rawgeti(L, 2, i + 1);
			list.push_back(*lua::GetUserdata<EntityColor*>(L, -1, lua::metatables::EntityColorMT));
			lua_pop(L, 1);
		}
		entity->_entityColors = list;
	}

	return 0;
}

void PatchAddWeakness() {
	SigScan scanner("576afd"); // this is the first push of args for ComputeStausEffectDuration
	scanner.Scan();
	void* addr = scanner.GetAddress();

	ASMPatch patch;
	patch.Push(ASMPatch::Registers::EDI)  // Push entity
		.Push(ASMPatch::Registers::EBP, 0x8) // Push duration
		.AddBytes("\x8b\xce") // MOV ECX, ESI
		// this fits exactly in the 5 bytes uses to push arguments
		.AddRelativeJump((char*)addr + 0x5);
	sASMPatcher.PatchAt(addr, &patch);
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
		{ "GetEntityColors", Lua_EntityGetEntityColors },
		{ "SetEntityColors", Lua_EntitySetEntityColors },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY, functions);

	PatchAddWeakness();
}