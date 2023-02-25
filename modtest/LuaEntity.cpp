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
	return 1;
}

static void RegisterEntityAddBleeding(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddBleeding");
	lua_pushcfunction(L, Lua_EntityAddBleeding);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddMagnetized(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddMagnetized(*ref, duration);
	return 1;
}

static void RegisterEntityAddMagnetized(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddMagnetized");
	lua_pushcfunction(L, Lua_EntityAddMagnetized);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddBaited(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddBaited(*ref, duration);
	return 1;
}

static void RegisterEntityAddBaited(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddBaited");
	lua_pushcfunction(L, Lua_EntityAddBaited);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddWeakness(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddWeakness(*ref, duration);
	return 1;
}

static void RegisterEntityAddWeakness(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddWeakness");
	lua_pushcfunction(L, Lua_EntityAddWeakness);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddBrimstoneMark(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddBrimstoneMark(*ref, duration);
	return 1;
}

static void RegisterEntityAddBrimstoneMark(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddBrimstoneMark");
	lua_pushcfunction(L, Lua_EntityAddBrimstoneMark);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddIce(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	int duration = luaL_checkinteger(L, 3);
	ent->AddIce(*ref, duration);
	return 1;
}

static void RegisterEntityAddIce(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddIce");
	lua_pushcfunction(L, Lua_EntityAddIce);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_EntityAddKnockback(lua_State* L)
{
	Entity* ent = lua::GetUserdata<Entity*>(L, 1, lua::Metatables::ENTITY, "Entity");
	EntityRef* ref = lua::GetUserdata<EntityRef*>(L, 2, lua::Metatables::ENTITY_REF, "EntityRef");
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector"); //replace with const?
	int duration = luaL_checkinteger(L, 4);
	bool unk = lua_toboolean(L, 5);
	ent->AddKnockback(*ref, *pos, duration, unk);
	return 1;
}

static void RegisterEntityAddKnockback(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "AddKnockback");
	lua_pushcfunction(L, Lua_EntityAddKnockback);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_EntityGetShadowSize(lua_State* L)
{
	Entity* entity = lua::UserdataToData<Entity*>(lua_touserdata(L, 1));
	lua_pushnumber(L, *entity->GetShadowSize());
	return 1;
}

static int Lua_EntitySetShadowSize(lua_State* L)
{
	Entity* entity = lua::UserdataToData<Entity*>(lua_touserdata(L, 1));
	float shadowSize = luaL_checknumber(L, 2);
	*entity->GetShadowSize() = shadowSize;
	return 0;
}

static void RegisterEntityShadowSize(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "GetShadowSize");
	lua_pushcfunction(L, Lua_EntityGetShadowSize);
	lua_rawset(L, -3);

	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "SetShadowSize");
	lua_pushcfunction(L, Lua_EntitySetShadowSize);
	lua_rawset(L, -3);

	lua_pop(L, 2);
}

/*int Lua_EntityCopyStatusEffects(lua_State* L)
{
	Entity* ent = *(Entity**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY, "Entity") + 4);
	Entity* copyEnt = *(Entity**)((char*)lua::CheckUserdata(L, 2, lua::Metatables::ENTITY, "Entity") + 4);

	ent->CopyStatusEffects(copyEnt); bruh code

	return 1;
}

static void RegisterEntityCopyStatusEffects(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "CopyStatusEffects");
	lua_pushcfunction(L, Lua_EntityCopyStatusEffects);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}
*/

static int Lua_EntityGetNullOffset(lua_State* L)
{
	Entity* entity = lua::UserdataToData<Entity*>(lua_touserdata(L, 1));
	const char* nullLayerName = luaL_checkstring(L, 2);
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	*toLua = entity->GetNullOffset(nullLayerName);
	return 1;
}

static void RegisterEntityGetNullOffset(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY);
	lua_pushstring(L, "GetNullOffset");
	lua_pushcfunction(L, Lua_EntityGetNullOffset);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterEntityAddBleeding(state);
	RegisterEntityAddMagnetized(state);
	RegisterEntityAddBaited(state);
	RegisterEntityAddWeakness(state);
	RegisterEntityAddBrimstoneMark(state);
	RegisterEntityAddIce(state);
	RegisterEntityAddKnockback(state);
	RegisterEntityShadowSize(state);
	RegisterEntityGetNullOffset(state);
}