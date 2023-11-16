#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_IsaacGetEntityConfig)
{
	EntityConfig** toLua = (EntityConfig**)lua_newuserdata(L, sizeof(EntityConfig*));
	*toLua = g_Manager->GetEntityConfig();
	luaL_setmetatable(L, lua::metatables::EntityConfigMT);

	return 1;
}

LUA_FUNCTION(Lua_EntityGetEntityConfigEntity)
{
	Entity* entity = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");

	EntityConfig_Entity* entityConfigEntity = g_Manager->GetEntityConfig()->GetEntity(*entity->GetType(), *entity->GetVariant(), *entity->GetSubType());

	if (entityConfigEntity == nullptr) {
		// how
		lua_pushnil(L);
	}
	else {
		EntityConfig_Entity** toLua = (EntityConfig_Entity**)lua_newuserdata(L, sizeof(EntityConfig_Entity*));
		*toLua = entityConfigEntity;
		luaL_setmetatable(L, lua::metatables::EntityConfigEntityMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigGetEntity)
{
	EntityConfig* entityConfig = *lua::GetUserdata<EntityConfig**>(L, 1, lua::metatables::EntityConfigMT);

	const int type = (int)luaL_checkinteger(L, 2);
	const int variant = (int)luaL_optinteger(L, 3, -1);
	const int subtype = (int)luaL_optinteger(L, 4, -1);

	EntityConfig_Entity* entity = entityConfig->GetEntity(type, variant, subtype);

	if (entity == nullptr) {
		lua_pushnil(L);
	}
	else {
		EntityConfig_Entity** toLua = (EntityConfig_Entity**)lua_newuserdata(L, sizeof(EntityConfig_Entity*));
		*toLua = entity;
		luaL_setmetatable(L, lua::metatables::EntityConfigEntityMT);
	}

	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetType)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->id);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetVariant)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->variant);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetSubType)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->subtype);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetName)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->name.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionDamage)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionDamage);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBossId)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->bossID);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadius)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->collisionRadius);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetCollisionRadiusMultiplier)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::CONST_VECTOR), entity->collisionRadiusMulti);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetMass)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->mass);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetGridCollisionPoints)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->gridCollisionPoints);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetFriction)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->friction);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetBaseHP)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->baseHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetStageHP)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->stageHP);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetAnm2Path)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushstring(L, entity->anm2Path.c_str());
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetModName)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	if (entity->modEntry == nullptr) {
		lua_pushnil(L);
	}
	else {
		lua_pushstring(L, entity->modEntry->GetName().c_str());
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetEntityTags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushinteger(L, entity->tags);
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityHasEntityTags)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	const int tags = (int)luaL_checkinteger(L, 2);
	if (tags <= 0) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, tags & entity->tags == tags);
	}
	return 1;
}

LUA_FUNCTION(Lua_EntityConfigEntityGetShadowSize)
{
	EntityConfig_Entity* entity = *lua::GetUserdata<EntityConfig_Entity**>(L, 1, lua::metatables::EntityConfigEntityMT);
	lua_pushnumber(L, entity->shadowSize);
	return 1;
}

static void RegisterEntityConfig(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetEntity", Lua_EntityConfigGetEntity },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityConfigMT, lua::metatables::EntityConfigMT, functions);
}

static void RegisterEntityConfigEntity(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetType", Lua_EntityConfigEntityGetType },
		{ "GetVariant", Lua_EntityConfigEntityGetVariant },
		{ "GetSubType", Lua_EntityConfigEntityGetSubType },
		{ "GetName", Lua_EntityConfigEntityGetName },
		{ "GetCollisionDamage", Lua_EntityConfigEntityGetCollisionDamage },
		{ "GetBossID", Lua_EntityConfigEntityGetBossId },
		{ "GetCollisionRadius", Lua_EntityConfigEntityGetCollisionRadius },
		{ "GetCollisionRadiusMultiplier", Lua_EntityConfigEntityGetCollisionRadiusMultiplier },
		{ "GetMass", Lua_EntityConfigEntityGetMass },
		{ "GetGridCollisionPoints", Lua_EntityConfigEntityGetGridCollisionPoints },
		{ "GetFriction", Lua_EntityConfigEntityGetFriction },
		{ "GetBaseHP", Lua_EntityConfigEntityGetBaseHP },
		{ "GetStageHP", Lua_EntityConfigEntityGetStageHP },
		{ "GetAnm2Path", Lua_EntityConfigEntityGetAnm2Path },
		{ "GetModName", Lua_EntityConfigEntityGetModName },
		{ "GetEntityTags", Lua_EntityConfigEntityGetEntityTags },
		{ "HasEntityTags", Lua_EntityConfigEntityHasEntityTags },
		{ "GetShadowSize", Lua_EntityConfigEntityGetShadowSize },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::EntityConfigEntityMT, lua::metatables::EntityConfigEntityMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetEntityConfig", Lua_IsaacGetEntityConfig);
	lua::RegisterFunction(_state, lua::Metatables::ENTITY, "GetEntityConfigEntity", Lua_EntityGetEntityConfigEntity);

	RegisterEntityConfig(_state);
	RegisterEntityConfigEntity(_state);
}
