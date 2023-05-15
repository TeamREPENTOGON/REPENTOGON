#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static int Lua_GridEntityRockSpawnDrops(lua_State* L)
{
	Vector* position = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	int gridType = luaL_checkinteger(L, 2);
	int gridVariant = luaL_checkinteger(L, 3);
	unsigned int seed = luaL_checkinteger(L, 4);
	bool unk = lua_toboolean(L, 5);
	int backdropType = luaL_optinteger(L, 6, 0);
	GridEntity_Rock::SpawnDrops(*position, gridType, gridVariant, seed, unk, backdropType);
	return 0;
}

static int Lua_GridEntityHurtDamage(lua_State* L)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "Entity");
	Entity* entity = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	int damage = luaL_checkinteger(L, 3);
	int damageFlags = luaL_checkinteger(L, 4);
	float unk3 = luaL_checknumber(L, 5);
	bool unk4 = lua_toboolean(L, 6);
	gridEnt->hurt_func(entity, damage, damageFlags, unk3, unk4);
	return 0;
}

static int Lua_GridEntityHurtSurroundings(lua_State* L)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "Entity");
	int damage = luaL_checkinteger(L, 2);
	int damageFlags = luaL_checkinteger(L, 3);
	float unk3 = luaL_checknumber(L, 4);
	bool unk4 = lua_toboolean(L, 5);
	gridEnt->hurt_surroundings(damage, damageFlags, unk3, unk4);
	return 0;
}

static void RegisterSpawnRockDrops(lua_State* L) {
	lua_getglobal(L, "Isaac");
	lua_pushstring(L, "SpawnRockDrops");
	lua_pushcfunction(L, Lua_GridEntityRockSpawnDrops);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::GRID_ENTITY;
	lua::RegisterFunction(state, mt, "HurtDamage", Lua_GridEntityHurtDamage);
	lua::RegisterFunction(state, mt, "HurtSurroundings", Lua_GridEntityHurtSurroundings);
	//lua::Metatables rockMt = lua::Metatables::CONST_GRID_ENTITY_ROCK;
	//lua::RegisterFunction(state, rockMt, "SpawnDrops", Lua_GridEntityRockSpawnDrops);
	RegisterSpawnRockDrops(state);
}