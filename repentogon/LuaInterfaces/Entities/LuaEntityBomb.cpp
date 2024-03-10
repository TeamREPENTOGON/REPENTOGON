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

LUA_FUNCTION(Lua_BombGetExplosionCountdown)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushinteger(L, *bomb->GetExplosionCountdown());
	return 1;
}

LUA_FUNCTION(Lua_BombGetHeight)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_height);
	return 1;
}

LUA_FUNCTION(Lua_BombSetHeight)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_height = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombGetFallingSpeed)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_fallingSpeed);
	return 1;
}

LUA_FUNCTION(Lua_BombSetFallingSpeed)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_fallingSpeed = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombIsLoadingCostumes) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushboolean(L, bomb->_loadCostumes);
	return 1;
}

LUA_FUNCTION(Lua_BombSetLoadCostumes)
{
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_loadCostumes = lua::luaL_optboolean(L, 2, true);
	return 0;
}

LUA_FUNCTION(Lua_BombGetCostumeLayerSprite) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	const int index = (int)luaL_checkinteger(L, 2);
	if (index >= 0 && index <= 5) {
		lua::luabridge::UserdataPtr::push(L, &bomb->_bombCostumesSprites[index], lua::Metatables::SPRITE);
	}
	else {
		return luaL_error(L, "Invalid index %d, value must be between 0 and 4", index);
	}

	return 1;
}

LUA_FUNCTION(Lua_BombGetRocketAngle) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	lua_pushnumber(L, bomb->_rocketAngle);
	return 1;
}

LUA_FUNCTION(Lua_BombSetRocketAngle) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	bomb->_rocketAngle = luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombGetRocketSpeed) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	lua_pushnumber(L, bomb->_rocketSpeed);
	return 1;
}

LUA_FUNCTION(Lua_BombSetRocketSpeed) {
	Entity_Bomb* bomb = lua::GetUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	bomb->_rocketSpeed = luaL_checknumber(L, 2);
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
		{ "GetExplosionCountdown", Lua_BombGetExplosionCountdown },
		{ "GetHeight", Lua_BombGetHeight },
		{ "SetHeight", Lua_BombSetHeight },
		{ "GetFallingSpeed", Lua_BombGetFallingSpeed },
		{ "SetFallingSpeed", Lua_BombSetFallingSpeed },
		{ "IsLoadingCostumes", Lua_BombIsLoadingCostumes },
		{ "SetLoadCostumes", Lua_BombSetLoadCostumes },
		{ "GetCostumeLayerSprite", Lua_BombGetCostumeLayerSprite },
		{ "GetRocketAngle", Lua_BombGetRocketAngle },
		{ "SetRocketAngle", Lua_BombSetRocketAngle },
		{ "GetRocketSpeed", Lua_BombGetRocketSpeed },
		{ "SetRocketSpeed", Lua_BombSetRocketSpeed },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_BOMB, functions);
}