#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetMultiShotParams) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int weaponType = (int)luaL_checkinteger(L, 2);
	Weapon_MultiShotParams* ud = (Weapon_MultiShotParams*)lua_newuserdata(L, sizeof(Weapon_MultiShotParams));
	ud = player->GetMultiShotParams(ud, (WeaponType)weaponType);
	luaL_setmetatable(L, lua::metatables::MultiShotParamsMT);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numTears);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numTears = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumLanesPerEye) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numLanesPerEye);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumLanesPerEye) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numLanesPerEye = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetSpreadAngleTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleTears);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetSpreadAngleTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleTears = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetSpreadAngleLaser) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleLaser);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetSpreadAngleLaser) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleLaser = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetSpreadAngleTechX) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->speadAngleTechX);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetSpreadAngleTechX) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->speadAngleTechX = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetSpreadAngleKnife) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleKnife);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetSpreadAngleKnife) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleKnife = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumEyesActive) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numEyesActive);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumEyesActive) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numEyesActive = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetMultiEyeAngle) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->multiEyeAngle);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetMultiEyeAngle) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->multiEyeAngle = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsCrossEyed) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isCrossEyed);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsCrossEyed) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isCrossEyed = lua_toboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsShootingBackwards) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingBackwards);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsShootingBackwards) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingBackwards = lua_toboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsShootingSideways) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingSideways);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsShootingSideways) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingSideways = lua_toboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumRandomDirTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numRandomDirTears);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumRandomDirTears) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numRandomDirTears = (int)luaL_checkinteger(L, 2);
	return 0;
}

static void RegisterMultiShotParams(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetMultiShotParams", Lua_GetMultiShotParams);

	luaL_Reg functions[] = {
		{ "GetNumTears", Lua_MultiShotParamsGetNumTears },
		{ "GetNumLanesPerEye", Lua_MultiShotParamsGetNumLanesPerEye },
		{ "GetSpreadAngleTears", Lua_MultiShotParamsGetSpreadAngleTears },
		{ "GetSpreadAngleLaser", Lua_MultiShotParamsGetSpreadAngleLaser },
		{ "GetSpreadAngleTechX", Lua_MultiShotParamsGetSpreadAngleTechX },
		{ "GetSpreadAngleKnife", Lua_MultiShotParamsGetSpreadAngleKnife },
		{ "GetNumEyesActive", Lua_MultiShotParamsGetNumEyesActive },
		{ "GetMultiEyeAngle", Lua_MultiShotParamsGetMultiEyeAngle },
		{ "IsCrossEyed", Lua_MultiShotParamsGetIsCrossEyed },
		{ "IsShootingBackwards", Lua_MultiShotParamsGetIsShootingBackwards },
		{ "IsShootingSideways", Lua_MultiShotParamsGetIsShootingSideways },
		{ "GetNumRandomDirTears", Lua_MultiShotParamsGetNumRandomDirTears },
		{ "SetNumTears", Lua_MultiShotParamsSetNumTears },
		{ "SetNumLanesPerEye", Lua_MultiShotParamsSetNumLanesPerEye },
		{ "SetSpreadAngleTears", Lua_MultiShotParamsSetSpreadAngleTears },
		{ "SetSpreadAngleLaser", Lua_MultiShotParamsSetSpreadAngleLaser },
		{ "SetSpreadAngleTechX", Lua_MultiShotParamsSetSpreadAngleTechX },
		{ "SetSpreadAngleKnife", Lua_MultiShotParamsSetSpreadAngleKnife },
		{ "SetNumEyesActive", Lua_MultiShotParamsSetNumEyesActive },
		{ "SetMultiEyeAngle", Lua_MultiShotParamsSetMultiEyeAngle },
		{ "SetIsCrossEyed", Lua_MultiShotParamsSetIsCrossEyed },
		{ "SetIsShootingBackwards", Lua_MultiShotParamsSetIsShootingBackwards },
		{ "SetIsShootingSideways", Lua_MultiShotParamsSetIsShootingSideways },
		{ "SetNumRandomDirTears", Lua_MultiShotParamsSetNumRandomDirTears },
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::MultiShotParamsMT, lua::metatables::MultiShotParamsMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	RegisterMultiShotParams(_state);
}

