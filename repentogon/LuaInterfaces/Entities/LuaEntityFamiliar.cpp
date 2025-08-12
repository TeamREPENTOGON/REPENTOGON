#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../../Patches/FamiliarTags.h"
#include "../../Patches/EntityPlus.h"

LUA_FUNCTION(Lua_FamiliarGetFollowerPriority)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	int priority = fam->GetFollowerPriority();
	lua_pushinteger(L, priority);
	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetPathFinder)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	NPCAI_Pathfinder* pathFinder = fam->GetPathFinder();
	lua::luabridge::UserdataPtr::push(L, pathFinder, lua::GetMetatableKey(lua::Metatables::PATHFINDER));

	return 1;
}

LUA_FUNCTION(Lua_FamiliarTryAimAtMarkedTarget)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Vector aimDirection;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		aimDirection = *lua::GetLuabridgeUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}
	int direction = (int)luaL_optinteger(L, 3, -1);
	Vector targetPosBuffer;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		targetPosBuffer = *lua::GetLuabridgeUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	}
	bool success = fam->TryAimAtMarkedTarget(&aimDirection, &direction, &targetPosBuffer);

	if (lua_gettop(L) == 3) {
		if (success) {
			lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), targetPosBuffer);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	lua_pushboolean(L, success);

	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), aimDirection);
	lua_rawset(L, -3);
	lua_pushinteger(L, 2);
	lua_pushinteger(L, direction);
	lua_rawset(L, -3);
	lua_pushinteger(L, 3);
	lua::luabridge::UserdataValue<Vector>::push(L, lua::GetMetatableKey(lua::Metatables::VECTOR), targetPosBuffer);
	lua_rawset(L, -3);
	
	return 2;
}

LUA_FUNCTION(Lua_FamiliarTriggerRoomClear)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->TriggerRoomClear();

	return 0;
}

LUA_FUNCTION(Lua_FamiliarUpdateDirtColor)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->UpdateDirtColor(true);
	return 0;
}

LUA_FUNCTION(Lua_FamiliarGetDirtColor)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");

	ColorMod* toLua = lua::luabridge::UserdataValue<ColorMod>::place(L, lua::GetMetatableKey(lua::Metatables::COLOR));
	*toLua = fam->_dirtColor;

	return 1;
}

LUA_FUNCTION(Lua_FamiliarRemoveFromPlayer)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->RemoveFromPlayer(true);
	return 0;
}

LUA_FUNCTION(Lua_FamiliarCanCharm)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	lua_pushboolean(L, fam->CanCharm());
	return 1;
}

LUA_FUNCTION(Lua_FamiliarIsCharmed)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Entity_Player* player = fam->_player;
	
	lua_pushboolean(L, player && (player->_spawnerType == 904 && player->_spawnerVariant == 0));
	return 1;
}

LUA_FUNCTION(Lua_FamiliarCanBeDamagedByEnemies)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	const int variant = *fam->GetVariant();
	const int subtype = *fam->GetSubType();
	// Ugh
	if (variant == 206) {
		// Wisps do get hurt by enemies, except the Vengeful Spirit ones.
		lua_pushboolean(L, subtype != 702);
	} else if(variant == 201 || variant == 216 || variant == 217 || variant == 228 || variant == 237 || variant == 238) {
		// Milk, Friendly dips, Tinytomas, Minisaacs, Item Wisps and Blood Babies do, in fact, get hurt by enemy contact.
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, fam->CanBeDamagedByEnemy());
	}
	return 1;
}

LUA_FUNCTION(Lua_FamiliarCanBeDamagedByProjectiles)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	const int variant = *fam->GetVariant();
	// Ugh 2
	if (variant == 201 || variant == 216 || variant == 217 || variant == 228 || variant == 238) {
		// Friendly dips, Tinytomas, Minisaacs and Blood Babies do, in fact, get hurt by projectiles.
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, FamiliarCanBeDamagedByProjectilesReimplementation(fam));
	}
	return 1;
}

LUA_FUNCTION(Lua_FamiliarCanBeDamagedByLasers)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	lua_pushboolean(L, FamiliarCanBeDamagedByLaserReimplementation(fam));
	return 1;
}

LUA_FUNCTION(Lua_FamiliarCanBlockProjectiles)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	lua_pushboolean(L, fam->CanBlockProjectiles());
	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetMoveDelayNum) {
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	lua_pushinteger(L, fam->_moveDelayNum);

	return 1;
}

LUA_FUNCTION(Lua_FamiliarSetMoveDelayNum) {
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	fam->_moveDelayNum = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_FamiliarGetItemConfig) {
	auto* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	ItemConfig_Item* config = fam->_item;
	lua::luabridge::UserdataPtr::push(L, config, lua::GetMetatableKey(lua::Metatables::ITEM));
	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetMultiplier) {
	auto* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	EntityFamiliarPlus* famPlus = GetEntityFamiliarPlus(fam);
	if (famPlus && !famPlus->cachedMultiplier) {
		// We can't use the return value yet due to where the float value ends up in memory.
		// However, just calling it will trigger re-evaluation & cache the result via my ASM patch.
		fam->GetMultiplier();
	}
	if (famPlus && famPlus->cachedMultiplier) {
		lua_pushnumber(L, *famPlus->cachedMultiplier);
	}
	else {
		// Uh oh
		lua_pushnumber(L, 1.0f);
	}
	return 1;
}

LUA_FUNCTION(Lua_FamiliarInvalidateCachedMultiplier) {
	auto* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	EntityFamiliarPlus* famPlus = GetEntityFamiliarPlus(fam);
	if (famPlus) {
		famPlus->cachedMultiplier = std::nullopt;
	}
	return 0;
}

LUA_FUNCTION(Lua_FamiliarIsLilDelirium) {
	auto* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");

	lua_pushboolean(L, fam->_isLilDelirium);
	return 1;
}

LUA_FUNCTION(Lua_SetLilDelirium) {
	auto* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	bool isLilDelirium = lua_toboolean(L, 2);

	fam->_isLilDelirium = isLilDelirium;
	
	if (isLilDelirium) {
		fam->delirium_morph();
	}

	return 0;
}

LUA_FUNCTION(Lua_GetRandomWisp) {
	RNG* rng = lua::GetLuabridgeUserdata<RNG*>(L, 1, lua::Metatables::RNG, "RNG");
	lua_pushinteger(L, Entity_Familiar::GetRandomWisp(*rng));
	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetActiveWeaponEntity)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Weapon* wep = fam->_weapon;
	if (wep == nullptr) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, wep->GetMainEntity(), lua::GetMetatableKey(lua::Metatables::ENTITY));
	}

	return 1;
}

LUA_FUNCTION(Lua_FamiliarGetActiveWeaponNumFired)
{
	Entity_Familiar* fam = lua::GetLuabridgeUserdata<Entity_Familiar*>(L, 1, lua::Metatables::ENTITY_FAMILIAR, "EntityFamiliar");
	Weapon* wep = fam->_weapon;
	if (wep == nullptr) {
		lua_pushnil(L);
	}
	else
	{
		lua_pushinteger(L, wep->_numFired);
	}

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetFollowerPriority", Lua_FamiliarGetFollowerPriority },
		{ "GetPathFinder", Lua_FamiliarGetPathFinder }, // depreciated
		{ "GetPathfinder", Lua_FamiliarGetPathFinder },
		{ "TryAimAtMarkedTarget", Lua_FamiliarTryAimAtMarkedTarget },
		{ "TriggerRoomClear", Lua_FamiliarTriggerRoomClear },
		{ "UpdateDirtColor", Lua_FamiliarUpdateDirtColor },
		{ "GetDirtColor", Lua_FamiliarGetDirtColor },
		{ "RemoveFromPlayer", Lua_FamiliarRemoveFromPlayer },
		{ "CanCharm", Lua_FamiliarCanCharm },
		{ "IsCharmed", Lua_FamiliarIsCharmed },
		{ "CanBeDamagedByEnemies", Lua_FamiliarCanBeDamagedByEnemies },
		{ "CanBeDamagedByProjectiles", Lua_FamiliarCanBeDamagedByProjectiles },
		{ "CanBeDamagedByLasers", Lua_FamiliarCanBeDamagedByLasers },
		{ "CanBlockProjectiles", Lua_FamiliarCanBlockProjectiles },
		{ "GetMoveDelayNum", Lua_FamiliarGetMoveDelayNum },
		{ "SetMoveDelayNum", Lua_FamiliarSetMoveDelayNum },
		{ "GetItemConfig", Lua_FamiliarGetItemConfig },
		{ "InvalidateCachedMultiplier", Lua_FamiliarInvalidateCachedMultiplier },
		{ "GetMultiplier", Lua_FamiliarGetMultiplier },
		{ "IsLilDelirium", Lua_FamiliarIsLilDelirium },
		{ "SetLilDelirium", Lua_SetLilDelirium },
		{ "GetRandomWisp", Lua_GetRandomWisp },
		{ "GetActiveWeaponEntity", Lua_FamiliarGetActiveWeaponEntity },
		{ "GetActiveWeaponNumFired", Lua_FamiliarGetActiveWeaponNumFired },
		{ NULL, NULL }
	};

	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_FAMILIAR, functions);

	lua::RegisterGlobalClassFunction(_state, "EntityFamiliar", "GetRandomWisp", Lua_GetRandomWisp);
}