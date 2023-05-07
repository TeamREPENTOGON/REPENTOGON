#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


int Lua_EntityAddBleeding(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddBleeding(*ref, duration);
	return 0;
}

int Lua_EntityAddMagnetized(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddMagnetized(*ref, duration);
	return 0;
}

int Lua_EntityAddBaited(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddBaited(*ref, duration);
	return 0;
}

int Lua_EntityAddWeakness(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddWeakness(*ref, duration);
	return 0;
}

int Lua_EntityAddBrimstoneMark(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddBrimstoneMark(*ref, duration);
	return 0;
}

int Lua_EntityAddIce(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddIce(*ref, duration);
	return 0;
}

int Lua_EntityAddKnockback(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector"); //replace with const?
	int duration = luaL_checkinteger(L, 4);
	bool unk = lua_toboolean(L, 5);
	ent->AddKnockback(*ref, *pos, duration, unk);
	return 0;
}

static int Lua_EntityGetShadowSize(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushnumber(L, *entity->GetShadowSize());
	return 1;
}

static int Lua_EntitySetShadowSize(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	float shadowSize = luaL_checknumber(L, 2);
	*entity->GetShadowSize() = shadowSize;
	return 0;
}

/*int Lua_EntityCopyStatusEffects(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* copyEnt lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

	ent->CopyStatusEffects(copyEnt); bruh code

	return 1;
}
*/

static int Lua_EntityGetNullOffset(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	const char* nullLayerName = luaL_checkstring(L, 2);

	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), entity->GetNullOffset(nullLayerName));

	return 1;
}

static int lua_Entity_GetType(lua_State* L) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetType());
	return 1;
}

static int lua_Entity_GetPosVel(lua_State* L) {
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Vector* pos = (Vector*)((char*)entity + 0x294);
	Vector* vel = (Vector*)((char*)entity + 0x2B8);

	PosVel posVel = PosVel(*pos, *vel);

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), posVel);

	return 1;
}

static int Lua_EntityGetBossStatusEffectCooldown(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	lua_pushinteger(L, *entity->GetBossStatusEffectCooldown());
	return 1;
}

static int Lua_EntitySetBossStatusEffectCooldown(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	int bossStatusEffectCooldown = luaL_checkinteger(L, 2);
	*entity->GetBossStatusEffectCooldown() = bossStatusEffectCooldown;
	return 0;
}

static int Lua_EntityForceCollide(lua_State* L)
{
	Entity* first = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	Entity* second = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	bool low = lua_toboolean(L, 3);
	lua_pushboolean(L, first->ForceCollide(first, second, low));
	return 1;
}

static int Lua_EntitySetDead(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isDead = lua_toboolean(L, 2);
	*entity->IsDead() = isDead;
	return 0;
}

static int Lua_EntitySetInvincible(lua_State* L)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	bool isInvincible = lua_toboolean(L, 2);
	*entity->IsInvincible() = isInvincible;
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::ENTITY;
	lua::RegisterFunction(state, mt, "AddBleeding", Lua_EntityAddBleeding);
	lua::RegisterFunction(state, mt, "AddMagnetized", Lua_EntityAddMagnetized);
	lua::RegisterFunction(state, mt, "AddBaited", Lua_EntityAddBaited);
	lua::RegisterFunction(state, mt, "AddWeakness", Lua_EntityAddWeakness);
	lua::RegisterFunction(state, mt, "AddBrimstoneMark", Lua_EntityAddBrimstoneMark);
	lua::RegisterFunction(state, mt, "AddIce", Lua_EntityAddIce);
	lua::RegisterFunction(state, mt, "AddKnockback", Lua_EntityAddKnockback);
	lua::RegisterFunction(state, mt, "GetShadowSize", Lua_EntityGetShadowSize);
	lua::RegisterFunction(state, mt, "SetShadowSize", Lua_EntitySetShadowSize);
	lua::RegisterFunction(state, mt, "AddKnockback", Lua_EntityAddKnockback);
	lua::RegisterFunction(state, mt, "GetNullOffset", Lua_EntityGetNullOffset);
	lua::RegisterFunction(state, mt, "GetType", lua_Entity_GetType);
	lua::RegisterFunction(state, mt, "GetPosVel", lua_Entity_GetPosVel);
	lua::RegisterFunction(state, mt, "GetBossStatusEffectCooldown", Lua_EntityGetBossStatusEffectCooldown);
	lua::RegisterFunction(state, mt, "SetBossStatusEffectCooldown", Lua_EntitySetBossStatusEffectCooldown);
	lua::RegisterFunction(state, mt, "ForceCollide", Lua_EntityForceCollide);
	lua::RegisterFunction(state, mt, "SetDead", Lua_EntitySetDead);
	lua::RegisterFunction(state, mt, "SetInvincible", Lua_EntitySetInvincible);
}