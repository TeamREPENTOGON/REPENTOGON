#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetMultiShotParams) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int weaponType = (int)luaL_optinteger(L, 2, 1);
	Weapon_MultiShotParams* ud = (Weapon_MultiShotParams*)lua_newuserdata(L, sizeof(Weapon_MultiShotParams));
	ud = player->GetMultiShotParams(ud, (WeaponType)weaponType);
	luaL_setmetatable(L, lua::metatables::MultiShotParamsMT);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumTears) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numTears);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumTears) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numTears = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumLanesPerEye) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numLanesPerEye);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumLanesPerEye) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numLanesPerEye = (unsigned int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetSpreadAngle) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);
	int weaponType = (int)luaL_checkinteger(L, 2);

	switch (weaponType)
	{
	case 1: //	WEAPON_TEARS
	case 5: //	WEAPON_BOMBS
		lua_pushnumber(L, params->spreadAngleTears);
		break;
	case 2: //	WEAPON_BRIMSTONE
	case 3: //	WEAPON_LASER
		lua_pushnumber(L, params->spreadAngleLaser);
		break;
	case 4: //	WEAPON_KNIFE
		lua_pushnumber(L, params->spreadAngleKnife);
		break;
	case 9: //	WEAPON_TECH_X
	case 14: //	WEAPON_FETUS  - C Section
		lua_pushnumber(L, params->speadAngleTechX);
		break;
	case 10: //	WEAPON_BONE
	case 11: //	WEAPON_NOTCHED_AXE
		lua_pushnumber(L, params->spreadAngleKnife*3); // Needs to be multiplied by 3 to get the correct angle
		break;
	case 6: //	WEAPON_ROCKETS
	case 7: //	WEAPON_MONSTROS_LUNGS
	case 8: //	WEAPON_LUDOVICO_TECHNIQUE
	case 12: //	WEAPON_URN_OF_SOULS
	case 13: //	WEAPON_SPIRIT_SWORD
	case 15: //	WEAPON_UMBILICAL_WHIP
		lua_pushnumber(L, 0); // return 0. These weapons dont use spread angle
		break;
	default:
		return luaL_error(L, "WeaponTypes bigger 15 are not supported!");
	}
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetSpreadAngle) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);
	int weaponType = (int)luaL_checkinteger(L, 2);
	float targetAngle = (float)luaL_checknumber(L, 3);

	switch (weaponType)
	{
	case 1: //	WEAPON_TEARS
	case 5: //	WEAPON_BOMBS
		params->spreadAngleTears = targetAngle;
		break;
	case 2: //	WEAPON_BRIMSTONE
	case 3: //	WEAPON_LASER
		params->spreadAngleLaser = targetAngle;
		break;
	case 4: //	WEAPON_KNIFE
		params->spreadAngleKnife = targetAngle;
		break;
	case 9: //	WEAPON_TECH_X
	case 14: //	WEAPON_FETUS  - C Section
		params->speadAngleTechX = targetAngle;
		break;
	case 10: //	WEAPON_BONE
	case 11: //	WEAPON_NOTCHED_AXE
		params->spreadAngleKnife = targetAngle / 3; // somehow needs to be divided by 3 to get the correct angle
		break;
	case 6: //	WEAPON_ROCKETS
	case 7: //	WEAPON_MONSTROS_LUNGS
	case 8: //	WEAPON_LUDOVICO_TECHNIQUE
	case 12: //	WEAPON_URN_OF_SOULS
	case 13: //	WEAPON_SPIRIT_SWORD
	case 15: //	WEAPON_UMBILICAL_WHIP
		return luaL_error(L, "The given WeaponType cant change its spread angle!");
	default:
		return luaL_error(L, "A WeaponType bigger 15 is not supported!");
	}

	if (params->numLanesPerEye < 2) // spread angle changes only work when number of lines is bigger 1
	{
		params->numLanesPerEye = 2;
	}

	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumEyesActive) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numEyesActive);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumEyesActive) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numEyesActive = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetMultiEyeAngle) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->multiEyeAngle);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetMultiEyeAngle) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->multiEyeAngle = (float)luaL_checknumber(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsCrossEyed) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isCrossEyed);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsCrossEyed) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isCrossEyed = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsShootingBackwards) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingBackwards);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsShootingBackwards) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingBackwards = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetIsShootingSideways) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingSideways);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetIsShootingSideways) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingSideways = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_MultiShotParamsGetNumRandomDirTears) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numRandomDirTears);
	return 1;
}

LUA_FUNCTION(Lua_MultiShotParamsSetNumRandomDirTears) {
	Weapon_MultiShotParams* params = lua::GetRawUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numRandomDirTears = (int)luaL_checkinteger(L, 2);
	return 0;
}

static void RegisterMultiShotParams(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::ENTITY_PLAYER, "GetMultiShotParams", Lua_GetMultiShotParams);

	luaL_Reg functions[] = {
		{ "GetNumTears", Lua_MultiShotParamsGetNumTears },
		{ "GetNumLanesPerEye", Lua_MultiShotParamsGetNumLanesPerEye },
		{ "GetSpreadAngle", Lua_MultiShotParamsGetSpreadAngle },
		{ "GetNumEyesActive", Lua_MultiShotParamsGetNumEyesActive },
		{ "GetNumRandomDirTears", Lua_MultiShotParamsGetNumRandomDirTears },
		{ "GetMultiEyeAngle", Lua_MultiShotParamsGetMultiEyeAngle },
		{ "IsCrossEyed", Lua_MultiShotParamsGetIsCrossEyed },
		{ "IsShootingBackwards", Lua_MultiShotParamsGetIsShootingBackwards },
		{ "IsShootingSideways", Lua_MultiShotParamsGetIsShootingSideways },
		{ "SetNumTears", Lua_MultiShotParamsSetNumTears },
		{ "SetNumLanesPerEye", Lua_MultiShotParamsSetNumLanesPerEye },
		{ "SetSpreadAngle", Lua_MultiShotParamsSetSpreadAngle },
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

