#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_BombGetHitList) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	std::vector<unsigned int> hitList = bomb->_hitList;

	lua_newtable(L);
	int idx = 1;
	for (int index : hitList) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, index);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_BombUpdateDirtColor)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->UpdateDirtColor();
	return 0;
}

LUA_FUNCTION(Lua_BombGetScale)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_scale);
	return 1;
}

LUA_FUNCTION(Lua_BombSetScale)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_scale = (float)luaL_checknumber(L, 2);
	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetHitList", Lua_BombGetHitList },
		{ "UpdateDirtColor", Lua_BombUpdateDirtColor },
		{ "GetScale", Lua_BombGetScale },
		{ "SetScale", Lua_BombSetScale },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_BOMB, functions);
}