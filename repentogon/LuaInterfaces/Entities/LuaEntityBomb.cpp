#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_BombGetHitList) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
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
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->UpdateDirtColor();
	return 0;
}

LUA_FUNCTION(Lua_BombGetScale)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_scale);
	return 1;
}

LUA_FUNCTION(Lua_BombSetScale)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_scale = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombGetExplosionCountdown)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushinteger(L, *bomb->GetExplosionCountdown());
	return 1;
}

LUA_FUNCTION(Lua_BombGetFallSpeed)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_fallSpeed);
	return 1;
}

LUA_FUNCTION(Lua_BombSetFallSpeed)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_fallSpeed = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombGetFallAcceleration) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushnumber(L, bomb->_fallAcceleration);
	return 1;
}

LUA_FUNCTION(Lua_BombSetFallAcceleration) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_fallAcceleration = (float)luaL_checknumber(L, 2);
	return 0;
}

// Deprecated mislabeled Get/SetHeight
static bool printedHeightWarning = false;
static void HeightWarning() {
	if (!printedHeightWarning) {
		g_Game->GetConsole()->Print("[WARN] EntityBomb:Get/SetHeight is deprecated - It was mislabeled and actually accesses the FallSpeed. For bomb \"height\", use `PositionOffset.Y`. For falling speed, use EntityBomb:Get/SetFallSpeed.\n", 0xFFFCCA03, 0x96u);
		printedHeightWarning = true;
	}
}
LUA_FUNCTION(Lua_BombGetHeight_Deprecated) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	HeightWarning();
	lua_pushnumber(L, bomb->_fallSpeed);
	return 1;
}
LUA_FUNCTION(Lua_BombSetHeight_Deprecated) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	HeightWarning();
	bomb->_fallSpeed = (float)luaL_checknumber(L, 2);
	return 0;
}

// Deprecated mislabeled Get/SetFallingSpeed
static bool printedFallingSpeedWarning = false;
static void FallingSpeedWarning() {
	if (!printedFallingSpeedWarning) {
		g_Game->GetConsole()->Print("[WARN] EntityBomb:Get/SetFallingSpeed is deprecated - It was mislabeled and actually accesses the FallAcceleration. Please use EntityBomb:Get/SetFallSpeed or EntityBomb:Get/SetFallAcceleration instead.\n", 0xFFFCCA03, 0x96u);
		printedFallingSpeedWarning = true;
	}
}
LUA_FUNCTION(Lua_BombGetFallingSpeed_Deprecated) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	FallingSpeedWarning();
	lua_pushnumber(L, bomb->_fallAcceleration);
	return 1;
}
LUA_FUNCTION(Lua_BombSetFallingSpeed_Deprecated) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	FallingSpeedWarning();
	bomb->_fallAcceleration = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombIsLoadingCostumes) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	lua_pushboolean(L, bomb->_loadCostumes);
	return 1;
}

LUA_FUNCTION(Lua_BombSetLoadCostumes)
{
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");

	bomb->_loadCostumes = lua::luaL_optboolean(L, 2, true);
	return 0;
}

LUA_FUNCTION(Lua_BombGetCostumeLayerSprite) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	const int index = (int)luaL_checkinteger(L, 2);
	if (index >= 0 && index < 5) {
		lua::luabridge::UserdataPtr::push(L, &bomb->_bombCostumesSprites[index], lua::Metatables::SPRITE);
	}
	else {
		return luaL_error(L, "Invalid index %d, value must be between 0 and 4", index);
	}

	return 1;
}

LUA_FUNCTION(Lua_BombGetRocketAngle) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	lua_pushnumber(L, bomb->_rocketAngle);
	return 1;
}

LUA_FUNCTION(Lua_BombSetRocketAngle) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	bomb->_rocketAngle = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombGetRocketSpeed) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	lua_pushnumber(L, bomb->_rocketSpeed);
	return 1;
}

LUA_FUNCTION(Lua_BombSetRocketSpeed) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	bomb->_rocketSpeed = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_BombIsPrismTouched) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	lua_pushboolean(L, bomb->_prismTouched);
	return 1;
}

LUA_FUNCTION(Lua_BombSetPrismTouched) {
	Entity_Bomb* bomb = lua::GetLuabridgeUserdata<Entity_Bomb*>(L, 1, lua::Metatables::ENTITY_BOMB, "EntityBomb");
	bomb->_prismTouched = lua::luaL_checkboolean(L, 2);
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
		{ "GetFallSpeed", Lua_BombGetFallSpeed },
		{ "SetFallSpeed", Lua_BombSetFallSpeed },
		{ "GetFallAcceleration", Lua_BombGetFallAcceleration },
		{ "SetFallAcceleration", Lua_BombSetFallAcceleration },
		{ "IsLoadingCostumes", Lua_BombIsLoadingCostumes },
		{ "SetLoadCostumes", Lua_BombSetLoadCostumes },
		{ "GetCostumeLayerSprite", Lua_BombGetCostumeLayerSprite },
		{ "GetRocketAngle", Lua_BombGetRocketAngle },
		{ "SetRocketAngle", Lua_BombSetRocketAngle },
		{ "GetRocketSpeed", Lua_BombGetRocketSpeed },
		{ "SetRocketSpeed", Lua_BombSetRocketSpeed },
		{ "IsPrismTouched", Lua_BombIsPrismTouched },
		{ "SetPrismTouched", Lua_BombSetPrismTouched },
		// Previously misidentified, kept as legacy
		{ "GetHeight", Lua_BombGetHeight_Deprecated },
		{ "SetHeight", Lua_BombSetHeight_Deprecated },
		{ "GetFallingSpeed", Lua_BombGetFallingSpeed_Deprecated },
		{ "SetFallingSpeed", Lua_BombSetFallingSpeed_Deprecated },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_BOMB, functions);
}