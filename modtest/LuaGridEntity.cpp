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

static void RegisterSpawnRockDrops(lua_State* L) {
	lua_getglobal(L, "GridEntityRock");
	lua_pushstring(L, "SpawnDrops");
	lua_pushcfunction(L, Lua_GridEntityRockSpawnDrops);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

static int Lua_GridEntityHurtDamage(lua_State* L)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
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
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	int damage = luaL_checkinteger(L, 2);
	int damageFlags = luaL_checkinteger(L, 3);
	float unk3 = luaL_checknumber(L, 4);
	bool unk4 = lua_toboolean(L, 5);
	gridEnt->hurt_surroundings(damage, damageFlags, unk3, unk4);
	return 0;
}

static int Lua_GridEntityGetRenderPosition(lua_State* L)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	//Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	Vector* buffer = new Vector(0, 0);
	*toLua = *gridEnt->GetRenderPosition(buffer);
	return 1;
}

static int Lua_GridEntityIsBreakableRock(lua_State* L)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	lua_pushboolean(L, gridEnt->IsBreakableRock());
	return 1;
}

//GRIDENTITY_ROCK
static int Lua_GridEntityRockRenderTop(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	const Vector* offset = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	gridEnt->RenderTop(*offset);
	return 0;
}

static int Lua_GridEntityRockTrySpawnWorms(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->TrySpawnWorms();
	return 0;
}

static int Lua_GridEntityRockTrySpawnLadder(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->TrySpawnLadder();
	return 0;
}

static int Lua_GridEntityRockUpdateNeighbors(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->UpdateNeighbors();
	return 0;
}

static int Lua_GridEntityRockPlayBreakSound(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int gridType = luaL_checkinteger(L, 2);
	int backdrop = luaL_optinteger(L, 3, 0);
	gridEnt->PlayBreakSound(gridType, backdrop);
	return 0;
}

static int Lua_GridEntityRockGetAltRockType(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int backdrop = luaL_optinteger(L, 2, 0);
	lua_pushinteger(L, gridEnt->GetAltRockType(backdrop));
	return 1;
}

static int Lua_GridEntityRockRegisterRockDestroyed(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int gridType = luaL_checkinteger(L, 2);
	gridEnt->RegisterRockDestroyed(gridType);
	return 0;
}

static int Lua_GridEntityRockUpdateCollision(lua_State* L)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->update_collision();
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	lua::Metatables mt = lua::Metatables::GRID_ENTITY;
	lua::RegisterFunction(state, mt, "HurtDamage", Lua_GridEntityHurtDamage);
	lua::RegisterFunction(state, mt, "HurtSurroundings", Lua_GridEntityHurtSurroundings);
	lua::RegisterFunction(state, mt, "GetRenderPosition", Lua_GridEntityGetRenderPosition);
	lua::RegisterFunction(state, mt, "IsBreakableRock", Lua_GridEntityIsBreakableRock);

	lua::Metatables rockMt = lua::Metatables::GRID_ENTITY_ROCK;
	//lua::RegisterFunction(state, rockMt, "SpawnDrops", Lua_GridEntityRockSpawnDrops);
	lua::RegisterFunction(state, rockMt, "RenderTop", Lua_GridEntityRockRenderTop);
	lua::RegisterFunction(state, rockMt, "TrySpawnWorms", Lua_GridEntityRockTrySpawnWorms);
	lua::RegisterFunction(state, rockMt, "TrySpawnLadder", Lua_GridEntityRockTrySpawnLadder);
	lua::RegisterFunction(state, rockMt, "UpdateNeighbors", Lua_GridEntityRockUpdateNeighbors);
	lua::RegisterFunction(state, rockMt, "PlayBreakSound", Lua_GridEntityRockPlayBreakSound);
	lua::RegisterFunction(state, rockMt, "GetAltRockType", Lua_GridEntityRockGetAltRockType);
	lua::RegisterFunction(state, rockMt, "RegisterRockDestroyed", Lua_GridEntityRockRegisterRockDestroyed);
	lua::RegisterFunction(state, rockMt, "UpdateCollision", Lua_GridEntityRockUpdateCollision);
	RegisterSpawnRockDrops(state);
}