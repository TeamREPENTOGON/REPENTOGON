#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GridEntityRockRenderTop)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	const Vector* offset = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	gridEnt->RenderTop(*offset);
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockTrySpawnWorms)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->TrySpawnWorms();
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockTrySpawnLadder)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->TrySpawnLadder();
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockUpdateNeighbors)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->UpdateNeighbors();
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockPlayBreakSound)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int gridType = (int)luaL_checkinteger(L, 2);
	int backdrop = (int)luaL_optinteger(L, 3, 0);
	gridEnt->PlayBreakSound(gridType, backdrop);
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockGetAltRockType)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int backdrop = (int)luaL_optinteger(L, 2, 0);
	lua_pushinteger(L, gridEnt->GetAltRockType(backdrop));
	return 1;
}

LUA_FUNCTION(Lua_GridEntityRockRegisterRockDestroyed)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	int gridType = (int)luaL_checkinteger(L, 2);
	gridEnt->RegisterRockDestroyed(gridType);
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockUpdateCollision)
{
	GridEntity_Rock* gridEnt = lua::GetUserdata<GridEntity_Rock*>(L, 1, lua::Metatables::GRID_ENTITY_ROCK, "GridEntityRock");
	gridEnt->update_collision();
	return 0;
}

LUA_FUNCTION(Lua_GridEntityRockSpawnDrops)
{
	Vector* position = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::VECTOR, "Vector");
	int gridType = (int)luaL_checkinteger(L, 2);
	int gridVariant = (int)luaL_checkinteger(L, 3);
	unsigned int seed = (unsigned int)luaL_checkinteger(L, 4);
	bool unk = lua_toboolean(L, 5);
	int backdropType = (int)luaL_optinteger(L, 6, 0);
	GridEntity_Rock::SpawnDrops(*position, gridType, gridVariant, seed, unk, backdropType);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	luaL_Reg functions[] = {
		{ "RenderTop", Lua_GridEntityRockRenderTop },
		{ "TrySpawnWorms", Lua_GridEntityRockTrySpawnWorms },
		{ "TrySpawnLadder", Lua_GridEntityRockTrySpawnLadder },
		{ "UpdateNeighbors", Lua_GridEntityRockUpdateNeighbors },
		{ "PlayBreakSound", Lua_GridEntityRockPlayBreakSound },
		{ "GetAltRockType", Lua_GridEntityRockGetAltRockType },
		{ "RegisterRockDestroyed", Lua_GridEntityRockRegisterRockDestroyed },
		{ "UpdateCollision", Lua_GridEntityRockUpdateCollision },
		//{ "SpawnDrops", Lua_GridEntityRockSpawnDrops },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GRID_ENTITY_ROCK, functions);
	lua::RegisterGlobalClassFunction(_state, "GridEntityRock", "SpawnDrops", Lua_GridEntityRockSpawnDrops);
}