#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GridEntityHurtEntity)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	Entity* entity = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	float npcDamage = (float)luaL_checknumber(L, 3);
	int playerDamage = (int)luaL_checkinteger(L, 4);
	unsigned int damageFlags = (int)luaL_checkinteger(L, 5);
	bool unk = lua::luaL_checkboolean(L, 6);
	gridEnt->hurt_func(entity, playerDamage, damageFlags, unk, npcDamage);
	return 0;
}

LUA_FUNCTION(Lua_GridEntityHurtSurroundings)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	float npcDistance = (float)luaL_checknumber(L, 2);
	float npcDamage = (float)luaL_checknumber(L, 3);
	float playerDistance = (float)luaL_checknumber(L, 4);
	int playerDamage = (int)luaL_checkinteger(L, 5);
	unsigned int damageFlags = (int)luaL_checkinteger(L, 6);
	bool unk = lua::luaL_checkboolean(L, 7);
	gridEnt->hurt_surroundings(playerDamage, damageFlags, unk, npcDistance, playerDistance, npcDamage);
	return 0;
}

LUA_FUNCTION(Lua_GridEntityGetRenderPosition)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	//Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* toLua = lua::luabridge::UserdataValue<Vector>::place(L, lua::GetMetatableKey(lua::Metatables::VECTOR));
	Vector* buffer = new Vector(0, 0);
	*toLua = *gridEnt->GetRenderPosition(buffer);
	return 1;
}

LUA_FUNCTION(Lua_GridEntityIsBreakableRock)
{
	GridEntity* gridEnt = lua::GetUserdata<GridEntity*>(L, 1, lua::Metatables::GRID_ENTITY, "GridEntity");
	lua_pushboolean(L, gridEnt->IsBreakableRock());
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	luaL_Reg functions[] = {
		{ "HurtEntity", Lua_GridEntityHurtEntity },
		{ "HurtSurroundings", Lua_GridEntityHurtSurroundings },
		{ "GetRenderPosition", Lua_GridEntityGetRenderPosition },
		{ "IsBreakableRock", Lua_GridEntityIsBreakableRock },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::GRID_ENTITY, functions);
}