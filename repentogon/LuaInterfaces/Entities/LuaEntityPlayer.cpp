#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

#include "../LuaWeapon.h"
#include "../../Patches/ASMPatches/ASMPlayer.h"

/*

..........___.
......./ /\ /\ \
......||●  `\ ●||
.......\   ⌓   /
 ......./  ᴥ  \
...... ||  _  ||
........|_| |_|

	 "code"
 (copycat of kilburn's ascii art of Isaac)
*/

std::map<int, int> fakeItems;

LUA_FUNCTION(Lua_GetMultiShotPositionVelocity) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY, "EntityPlayer");
	int loopIndex = (int)luaL_checkinteger(L, 2);
	int weaponType = (int)luaL_checkinteger(L, 3);
	Vector* shotDirection = lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	float shotSpeed = (float)luaL_checknumber(L, 5);

	Weapon_MultiShotParams* multiShotParams = lua::GetUserdata<Weapon_MultiShotParams*>(L, 6, lua::metatables::MultiShotParamsMT);
	if (multiShotParams->numTears < loopIndex) {
		return luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.NumTears");
	};

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), player->GetMultiShotPositionVelocity(loopIndex, (WeaponType)weaponType, *shotDirection, shotSpeed, *multiShotParams));

	return 1;
}

LUA_FUNCTION(Lua_InitTwin)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* twinPlayer = player->InitTwin(playerType);
	if (!twinPlayer) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, twinPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	
	return 1;
}

LUA_FUNCTION(Lua_InitPostLevelInitStats)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->InitPostLevelInitStats();

	return 0;
}

LUA_FUNCTION(Lua_PlayerSetItemState)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType item = (CollectibleType)luaL_checkinteger(L, 2);

	player->SetItemState(item);

	return 0;
}

LUA_FUNCTION(Lua_PlayerAddCacheFlags)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int flags = (int)luaL_checkinteger(L, 2);
	bool evaluateCache = lua::luaL_optboolean(L, 3, false);

	player->AddCacheFlags(flags);
	if (evaluateCache) {
		player->EvaluateItems();
	}

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetHealthType)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetHealthType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetTotalActiveCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetTotalActiveCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveMaxCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMaxCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveMinUsableCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMinUsableCharge(slot));
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetActiveVarData) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int vardata = (int)luaL_checkinteger(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);

	player->SetActiveVarData(vardata, slot);
	return 0;
}

LUA_FUNCTION(Lua_PlayerAddActiveCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int charge = (unsigned int)luaL_checkinteger(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);
	bool flashHUD = lua::luaL_optboolean(L, 4, true);
	bool overcharge = lua::luaL_optboolean(L, 5, false);
	bool force = lua::luaL_optboolean(L, 6, false);

	int ret = player->AddActiveCharge(charge, slot, flashHUD, overcharge, force);
	lua_pushinteger(L, ret);

	return 1;
}

LUA_FUNCTION(Lua_PlayerDropCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectible = (int)luaL_checkinteger(L, 2);
	Entity_Pickup* pickup = nullptr;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pickup = lua::GetUserdata<Entity_Pickup*>(L, 3, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	}
	bool removeFromForm = lua::luaL_optboolean(L, 4, false);

	player->DropCollectible(collectible, pickup, removeFromForm);
	if (!pickup) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, pickup, lua::Metatables::ENTITY_PICKUP);
	}
	
	return 1;
}

LUA_FUNCTION(Lua_PlayerDropCollectibleByHistoryIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int idx = (int)luaL_checkinteger(L, 2);
	Entity_Pickup* pickup = nullptr;
	if (lua_type(L, 3) == LUA_TUSERDATA) {
		pickup = lua::GetUserdata<Entity_Pickup*>(L, 3, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	}

	player->DropCollectibleByHistoryIndex(idx, pickup);
	if (!pickup) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, pickup, lua::Metatables::ENTITY_PICKUP);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerIncrementPlayerFormCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int ePlayerForm = (int)luaL_checkinteger(L, 2);
	int num = (int)luaL_checkinteger(L, 3);

	player->IncrementPlayerFormCounter(ePlayerForm, num);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryPreventDeath) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryPreventDeath());
	return 1;
}

LUA_FUNCTION(lua_PlayerRemoveCollectibleByHistoryIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_checkinteger(L, 2);
	player->RemoveCollectibleByHistoryIndex(index, true);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetCanShoot)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool canShoot = lua::luaL_checkboolean(L, 2);
	*player->GetCanShoot() = canShoot;
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetDeadEyeCharge)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetDeadEyeCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerTeleport)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	bool doEffects = lua::luaL_optboolean(L, 3, true);
	bool teleportTwinPlayers = lua::luaL_optboolean(L, 4, false);

	player->Teleport(position, doEffects, teleportTwinPlayers);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMegaBlastDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMegaBlastDuration());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMegaBlastDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetMegaBlastDuration() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetActiveItemDesc)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_optinteger(L, 2, 0);
	if (index > 3) {
		return luaL_argerror(L, 2, "ActiveSlot cannot be higher than 3");
	}
	ActiveItemDesc* desc = player->GetActiveItemDesc(index);
	if (!desc) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, desc, lua::GetMetatableKey(lua::Metatables::ACTIVE_ITEM_DESC));
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveItemSlot)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetActiveItemSlot((int)luaL_checkinteger(L, 2)));
	return 1;
}

LUA_FUNCTION(Lua_PlayerTryFakeDeath)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryFakeDeath());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBoCContent) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_newtable(L);
	BagOfCraftingPickup* content = player->GetBagOfCraftingContent();
	for (int i = 0; i < 8; ++i) {
		lua_pushinteger(L, i + 1);
		lua_pushinteger(L, *content++);
		lua_rawset(L, -3);
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBoCContent) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	if (!lua_istable(L, 2))
	{
		return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Expected a table as second argument");
	}

	BagOfCraftingPickup list[8]{};
	size_t length = (size_t)lua_rawlen(L, 2);
	if (length > 0)
	{
		if (length > 8) {
			return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Table cannot be larger than 8 pickups");
		}

		size_t index;
		for (index = 0; index < length; index++)
		{
			lua_rawgeti(L, 2, index + 1);
			int pickup = (int)luaL_checkinteger(L, -1);
			lua_pop(L, 1);
			if (pickup < 0 || pickup > 29) {
				return luaL_error(L, "EntityPlayer:SetBagOfCraftingContent: Invalid pickup %d at index %d", pickup, index + 1);
			}
			list[index] = (BagOfCraftingPickup)pickup;
		}
	}
	memcpy(&player->_bagOfCraftingContent, list, sizeof(list));

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer:GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	lua_pushinteger(L, player->GetBagOfCraftingContent()[slot]);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	int8_t pickup = (int8_t)luaL_optinteger(L, 3, BagOfCraftingPickup::BOC_NONE);
	if (pickup < 0 || pickup >= BagOfCraftingPickup::BOC_MAX) {
		return luaL_error(L, "EntityPlayer::SetBagOfCraftingSlot: invalid pickup id %d\n", pickup);
	}

	player->GetBagOfCraftingContent()[slot] = (BagOfCraftingPickup)pickup;
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBagOfCraftingOutput)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetBagOfCraftingOutput());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBagOfCraftingOutput)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetBagOfCraftingOutput() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_speedModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_speedModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_fireDelayModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_fireDelayModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_damageModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_damageModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetTearRangeModifier) // ._.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_tearRangeModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetShotSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_shotSpeedModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetShotSpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_shotSpeedModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetLuckModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_luckModifier);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetLuckModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_luckModifier = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetRedStewBonusDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetRedStewBonusDuration());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetRedStewBonusDuration)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetRedStewBonusDuration() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetWeaponModifiers)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetWeaponModifiers());
	return 1;
}

LUA_FUNCTION(Lua_PlayerEnableWeaponType)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	WeaponType weaponType = (WeaponType)luaL_checkinteger(L, 2);
	bool set = lua::luaL_checkboolean(L, 3);
	player->EnableWeaponType(weaponType, set);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetD8DamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8DamageModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8SpeedModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8SpeedModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8RangeModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8RangeModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetD8FireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8FireDelayModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEpiphoraCharge)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetEpiphoraCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetPeeBurstCooldown)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetPeeBurstCooldown());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMaxPeeBurstCooldown)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMaxPeeBurstCooldown());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMetronomeCollectibleID)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMetronomeCollectibleID());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMarkedTarget) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Effect* target = player->GetMarkedTarget();
	if (!target) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, target, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsLocalPlayer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsLocalPlayer());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetWildCardItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItem());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetWildCardItemType) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItemType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetWeapon) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	WeaponData* weaponData = lua::GetUserdata<WeaponData*>(L, 2, lua::metatables::WeaponMT);
	int index = (int)luaL_checkinteger(L, 3);
	if (index < 0 || index > 4) {
		return luaL_argerror(L, 2, "Index must be between 0 and 4");
	}
	*player->GetWeapon(index) = weaponData->weapon;
	weaponData->owner = player;
	weaponData->slot = index;

	return 0;
}

LUA_FUNCTION(Lua_PlayerAddLocust) {
	Entity_Player* ent = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectibleType = (int)luaL_checkinteger(L, 2);
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Isaac::SpawnLocust(ent, collectibleType, pos);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetPonyCharge) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPonyCharge());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPonyCharge) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPonyCharge() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenSpeed());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenFireDelay) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenFireDelay());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenFireDelay) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenFireDelay() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenDamage());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenRange) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenRange());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenRange) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenRange() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenShotSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenShotSpeed());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenShotSpeed) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenShotSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetEdenLuck) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenLuck());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEdenLuck) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenLuck() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerTriggerRoomClear) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	plr->TriggerRoomClear();

	return 0;

}

LUA_FUNCTION(Lua_PlayerShuffleCostumes) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int seed = (unsigned int)luaL_optinteger(L, 2, min(Isaac::genrand_int32(), 1));
	plr->ShuffleCostumes(seed);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCollectiblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<int>& collectibleInv = plr->GetCollectiblesList();

	lua_newtable(L);

	for (size_t i = 1; i < collectibleInv.size(); i++) {
		lua_pushinteger(L, i);
		lua_pushinteger(L, collectibleInv[i]);
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetPurityState) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPurityState());

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPurityState) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPurityState() = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerSetTearPoisonDamage) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetTearPoisonDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetVoidedCollectiblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<int>& collecitbleInv = plr->GetVoidedCollectiblesList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collecitbleInv) {
		lua_pushinteger(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddInnateCollectible)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	/*std::set<uint32_t>& itemWispList = *plr->GetItemWispsList();
	itemWispList.insert(luaL_checkinteger(L,2));
	*/

	const int collectibleID = (int)luaL_checkinteger(L, 2);
	const int amount = (int)luaL_optinteger(L, 3, 1);

	ItemConfig itemConfig = g_Manager->_itemConfig;
	ItemConfig_Item* item = itemConfig.GetCollectibles()[0][collectibleID];

	//ItemConfig_Item* item_ptr = item;
	if (item == nullptr) {
		return luaL_error(L, "Invalid item");
	}

	std::map<int, int>& wispMap = *plr->GetItemWispsList();
	wispMap[collectibleID] += amount;

	if (amount > 0 && item->addCostumeOnPickup) {
		plr->AddCostume(item, false);
	}

	plr->AddCacheFlags(item->cacheFlags);
	plr->EvaluateItems();

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetWispCollecitblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	/*std::set<uint32_t> collectibles = *plr->GetItemWispsList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collectibles) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
	}
	*/

	std::map<int, int> wispMap = plr->_itemWispsList;

	lua_newtable(L);

	for (auto& item : wispMap) {
		lua_pushinteger(L, item.first); // push the collectible
		lua_pushinteger(L, item.second); // push the amount
		lua_settable(L, -3); // set the table entry
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetImmaculateConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetImmaculateConceptionState());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetImmaculateConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetImmaculateConceptionState() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCambionConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetCambionConceptionState());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetCambionConceptionState)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetCambionConceptionState() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerUpdateIsaacPregnancy)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool cambion = lua::luaL_checkboolean(L, 2);
	plr->UpdateIsaacPregnancy(cambion);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCambionPregnancyLevel)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, plr->GetCambionPregnancyLevel());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetBladderCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetBladderCharge() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMaxBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetMaxBladderCharge());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMaxBladderCharge)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetMaxBladderCharge() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerIsUrethraBlocked)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, *plr->IsUrethraBlocked());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetUrethraBlock)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->IsUrethraBlocked() = lua::luaL_checkboolean(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetNextUrethraBlockFrame)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetNextUrethraBlockFrame());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetNextUrethraBlockFrame)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetNextUrethraBlockFrame() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetHeldSprite)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua::luabridge::UserdataPtr::push(L, plr->GetHeldSprite(), lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetHeldEntity)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* heldEntity = *plr->GetHeldEntity();
	if (!heldEntity) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, heldEntity, lua::GetMetatableKey(lua::Metatables::ENTITY));
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetActiveWeaponNumFired)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Weapon* wep = nullptr;

	wep = *plr->GetWeapon(0);
	if ((wep == nullptr) && ((wep = *plr->GetWeapon(1)) == nullptr)) {
		lua_pushnil(L);
	}
	else
	{
		lua_pushinteger(L, wep->GetNumFired());
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetPoopSpell)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pos = (int)luaL_checkinteger(L, 2);
	const int spell = (int)luaL_checkinteger(L, 3);

	if (pos < 0 || pos > 5) {
		return luaL_argerror(L, 2, "Invalid Poop Spell queue position");
	}
	if (spell < 1 || spell > 11) {
		// At least until we decide to add custom PoopSpellType support :^)
		return luaL_argerror(L, 3, "Invalid PoopSpellType");
	}

	plr->_poopSpellQueue[pos] = spell;

	return 0;
}

LUA_FUNCTION(Lua_PlayerRemovePoopSpell)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pos = (int)luaL_optinteger(L, 2, 0);

	if (pos < 0 || pos > 5) {
		return luaL_argerror(L, 2, "Invalid Poop Spell queue position");
	}

	for (int i = pos; i < 5; i++) {
		plr->_poopSpellQueue[i] = plr->_poopSpellQueue[i + 1];
	}
	plr->_poopSpellQueue[5] = 0;
	plr->CheckPoopSpellQueue();

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBackupPlayer) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Player* backupPlayer = plr->_backupPlayer;

	if (!backupPlayer) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, backupPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}

	return 1;
}

LUA_FUNCTION(Lua_ClearDeadEyeCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	if (lua_toboolean(L, 2)) {
		player->_deadEyeCharges = 0;
		player->_deadEyeMisses = 0;
		player->_cacheFlags |= 1;
		player->EvaluateItems();
	}
	else {
		player->ClearDeadEyeCharge();
	}

	return 0;
}

LUA_FUNCTION(Lua_SwapForgottenForm) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool IgnoreHealth = lua::luaL_optboolean(L, 2, false);
	bool NoEffects = lua::luaL_optboolean(L, 3, false);
	player->SwapForgottenForm(IgnoreHealth, NoEffects);
	return 0;
}

LUA_FUNCTION(Lua_SpawnAquariusCreep) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	TearParams params;

	if (lua_gettop(L) >= 2) {
		params = *lua::GetUserdata<TearParams*>(L, 2, lua::Metatables::TEAR_PARAMS, "TearParams");
	}
	else {
		player->GetTearHitParams(&params, (int)WeaponType::WEAPON_TEARS, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, (-(int)(Isaac::Random(2) != 0) & 2) - 1, 0);
	}

	Entity_Effect* effect = (Entity_Effect*)g_Game->Spawn(1000, 54, *player->GetPosition(), Vector(0.0, 0.0), player, 0, Random(), 0);

	if (!effect) {
		lua_pushnil(L);
	}
	else
	{
		float random = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		effect->_sprite._scale *= ((random * 0.5f) + 0.2f);
		effect->_collisionDamage = params._tearDamage;
		effect->SetColor(&params._tearColor, 0, -1, true, false);

		effect->_varData = params._flags;
		effect->Update();

		lua::luabridge::UserdataPtr::push(L, effect, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBabySkin) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_babySkin);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBabySkin) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_babySkin = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetMaggySwingCooldown) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_maggySwingCooldown);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetMaggySwingCooldown) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_maggySwingCooldown = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerPlayDelayedSFX) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int soundEffectID = (unsigned int)luaL_checkinteger(L, 2);
	int soundDelay = (int)luaL_optinteger(L, 3, 0);
	int frameDelay = (int)luaL_optinteger(L, 4, 2);
	float volume = (float)luaL_optnumber(L, 5, 1.0f);
	player->PlayDelayedSFX(soundEffectID, soundDelay, frameDelay, volume);

	return 0;
}

LUA_FUNCTION(Lua_PlayerCanUsePill) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int pillEffect = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->CanUsePill(pillEffect));

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddSmeltedTrinket) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	const int trinketID = (int)luaL_checkinteger(L, 2);
	const bool firstTime = lua::luaL_optboolean(L, 3, true);

	lua_pushboolean(L, player->AddSmeltedTrinket(trinketID, firstTime));
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetSmeltedTrinkets) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<SmeltedTrinketDesc>& smeltedTrinkets = player->_smeltedTrinkets;

	lua_newtable(L);
	for (size_t i = 1; i < smeltedTrinkets.size(); i++) {
		lua_pushinteger(L, i); 

		lua_newtable(L);

		lua_pushstring(L, "trinketAmount");
		lua_pushinteger(L, smeltedTrinkets[i]._trinketNum);
		lua_settable(L, -3);

		lua_pushstring(L, "goldenTrinketAmount");
		lua_pushinteger(L, smeltedTrinkets[i]._goldenTrinketNum);
		lua_settable(L, -3);

		lua_settable(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetCostumeLayerMap)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	PlayerCostumeMap* costumeLayerMap = plr->_playerCostumeMap;

	lua_newtable(L);
	for (int idx = 0; idx < 15; idx++) {
		lua_pushinteger(L, idx + 1);

		lua_newtable(L);

		lua_pushstring(L, "costumeIndex");
		lua_pushinteger(L, costumeLayerMap[idx]._index);
		lua_settable(L, -3);

		lua_pushstring(L, "layerID");
		lua_pushinteger(L, costumeLayerMap[idx]._layerID);
		lua_settable(L, -3);

		lua_pushstring(L, "priority");
		lua_pushinteger(L, costumeLayerMap[idx]._priority);
		lua_settable(L, -3);

		lua_pushstring(L, "isBodyLayer");
		lua_pushboolean(L, costumeLayerMap[idx]._isBodyLayer);
		lua_settable(L, -3);

		lua_settable(L, -3);
	}
	return 1;
}


LUA_FUNCTION(Lua_PlayerIsItemCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	ItemConfig_Item* item = lua::GetUserdata<ItemConfig_Item*>(L, 2, lua::Metatables::ITEM, "Item");
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsItemCostumeVisible(item, layerID));
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsCollectibleCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsCollectibleCostumeVisible(collectibleType, layerID));
	return 1;
}

LUA_FUNCTION(Lua_PlayerIsNullItemCostumeVisible) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int nullItem = (int)luaL_checkinteger(L, 2);
	int layerID = 0;
	if (lua_type(L, 3) == LUA_TSTRING) {
		const char* layerName = luaL_checkstring(L, 3);
		LayerState* layerState = plr->_sprite.GetLayer(layerName);
		if (layerState != nullptr) {
			layerID = layerState->GetLayerID();
		}
		else
		{
			return luaL_error(L, "Invalid layer name %s", layerName);
		}
	}
	else {
		layerID = (int)luaL_checkinteger(L, 3);
		if (layerID < 0 || (const unsigned int)layerID + 1 > plr->_sprite.GetLayerCount()) {
			return luaL_error(L, "Invalid layer ID %d", layerID);
		}
	}

	lua_pushboolean(L, plr->IsNullItemCostumeVisible(nullItem, layerID));
	return 1;
}


LUA_FUNCTION(Player_PlayCollectibleAnim) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	bool unk = lua::luaL_checkboolean(L, 3);
	std::string animName = luaL_checkstring(L, 4);
	int frameNum = (int)luaL_optinteger(L, 5, -1);

	plr->PlayCollectibleAnim(collectibleType, unk, animName, frameNum, false);
	return 0;
}

LUA_FUNCTION(Player_IsCollectibleAnimFinished) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);
	std::string animName = luaL_checkstring(L, 3);

	lua_pushboolean(L, plr->IsCollectibleAnimFinished(collectibleType, animName));
	return 1;
}

LUA_FUNCTION(Player_ClearCollectibleAnim) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 2);

	plr->ClearCollectibleAnim(collectibleType);
	return 0;
}

static void FamiliarStorageToLua(lua_State* L, std::vector<Entity_Familiar*>& familiars) {
	lua_newtable(L);
	for (size_t i = 0; i < familiars.size(); ++i) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, familiars[i], lua::GetMetatableKey(lua::Metatables::ENTITY_FAMILIAR));
		lua_rawset(L, -3);
	}

	familiarsStorage.familiars.clear();
	familiarsStorage.inUse = false;
}

LUA_FUNCTION(Lua_EntityPlayer_CheckFamiliarEx) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int variant = (int)luaL_checkinteger(L, 2);
	int targetCount = (int)luaL_checkinteger(L, 3);
	RNG* rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, lua::metatables::RngMT);
	ItemConfig_Item* configPtr = nullptr;
	if (lua_type(L, 5) == LUA_TUSERDATA) {
		configPtr = lua::GetUserdata<ItemConfig_Item*>(L, 5, lua::Metatables::ITEM, "ItemConfigItem");
	}
	
	int subtype = (int)luaL_optinteger(L, 6, -1);

	std::vector<Entity_Familiar*>& familiars = InitFamiliarStorage();
	plr->CheckFamiliar(variant, targetCount, rng, configPtr, subtype);
	FamiliarStorageToLua(L, familiars);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEveSumptoriumCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_eveSumptoriumCharge);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetEveSumptoriumCharge) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_eveSumptoriumCharge = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetPlayerFormCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerFormType = (int)luaL_checkinteger(L, 2);

	if (playerFormType >= 0 && playerFormType <= 14) {
		lua_pushinteger(L, player->_playerForms[playerFormType]);
	}
	else {
		return luaL_error(L, "Invalid PlayerForm %d", playerFormType);
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetMaxPocketItems) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetMaxPocketItems());

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddBoneOrbital) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* position = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	player->AddBoneOrbital(position);

	return 0;
}

/*
// this seems to be super hardcoded to not work outside of cantripped
LUA_FUNCTION(Lua_PlayerAddItemCard) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (g_Manager->_itemConfig.GetCollectible(id) == nullptr) {
		std::string error("Invalid collectible ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushinteger(L, player->AddItemCard(id));

	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerAddLeprosy) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->AddLeprosy();

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetUrnSouls) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_urnSouls);

	return 1;
}

LUA_FUNCTION(Lua_PlayerAddUrnSouls) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int count = (int)luaL_checkinteger(L, 2);
	player->AddUrnSouls(count);

	return 0;
}

LUA_FUNCTION(Lua_PlayerCanAddCollectibleToInventory) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->CanAddCollectibleToInventory(id));

	return 1;
}

LUA_FUNCTION(Lua_PlayerCanOverrideActiveItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 3) {
		std::string error("Invalid slot ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushboolean(L, player->CanOverrideActiveItem(id));

	return 1;
}

LUA_FUNCTION(Lua_PlayerClearItemAnimCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	player->ClearItemAnimCollectible(id);

	return 0;
}

LUA_FUNCTION(Lua_PlayerClearItemAnimNullItems) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->ClearItemAnimNullItems();

	return 0;
}

/*
// Spawns club, immediately kills it. Needs investigation
LUA_FUNCTION(Lua_PlayerFireBoneClub) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* parent = nullptr;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		parent = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");
	}
	int variant = (int)luaL_checkinteger(L, 3);
	bool unk = lua::luaL_checkboolean(L, 4);

	player->FireBoneClub(parent, variant, unk);

	return 0;
}
*/


// might need asm patch to retrieve laser. this is wacky and can spawn both an effect and a laser
LUA_FUNCTION(Lua_PlayerFireBrimstoneBall) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	Vector* vel = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Vector offset;
	if (lua_type(L, 4) == LUA_TUSERDATA) {
		offset = *lua::GetUserdata<Vector*>(L, 4, lua::Metatables::VECTOR, "Vector");
	}

	Entity_Effect* effect = player->FireBrimstoneBall(*pos, *vel, offset, 0, 0, nullptr);
	if (!effect) {
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, effect, lua::Metatables::ENTITY_EFFECT);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetBodyMoveDirection) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	Vector dir;
	player->GetBodyMoveDirection(&dir);
	lua::luabridge::UserdataPtr::push(L, &dir, lua::Metatables::VECTOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetDeathAnimName) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushstring(L, player->GetDeathAnimName());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGlitchBabySubType) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetGlitchBabySubType());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGreedsGulletHearts) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetGreedsGulletHearts());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetSpecialGridCollision) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector pos = *player->GetPosition();
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		pos = *lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	}

	lua_pushinteger(L, player->GetSpecialGridCollision(&pos));
	return 1;
}

LUA_FUNCTION(Lua_PlayerCanCrushRocks) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->CanCrushRocks());
	return 1;
}

LUA_FUNCTION(Lua_PlayerGetEnterPosition) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector dir;
	player->GetEnterPosition(&dir);
	lua::luabridge::UserdataPtr::push(L, &dir, lua::Metatables::VECTOR);
	return 1;
}

/*
// needs return register override support
LUA_FUNCTION(Lua_PlayerGetExplosionRadiusMultiplier) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	BitSet128* flags = lua::GetUserdata<BitSet128*>(L, 2, lua::Metatables::BITSET_128, "BitSet128");
	lua_pushnumber(L, player->GetExplosionRadiusMultiplier(*flags));
	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerGetFocusEntity) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* entity = player->GetFocusEntity();
	if (!entity)
	{
		lua_pushnil(L);
	}
	else
	{
		lua::luabridge::UserdataPtr::push(L, entity, lua::Metatables::ENTITY);
	}

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetGlyphOfBalanceDrop) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int variant = (int)luaL_optinteger(L, 2, -1);
	int subtype = (int)luaL_optinteger(L, 3, -1);
	player->GetGlyphOfBalanceDrop(&variant, &subtype);

	lua_newtable(L);
	lua_pushinteger(L, 1);
	lua_pushinteger(L, variant);
	lua_rawset(L, -3);
	lua_pushinteger(L, 2);
	lua_pushinteger(L, subtype);
	lua_rawset(L, -3);

	return 1;
}

LUA_FUNCTION(Lua_PlayerGetLaserColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua::luabridge::UserdataPtr::push(L, &player->_laserColor, lua::Metatables::COLOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetLaserColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_laserColor = *lua::GetUserdata<ColorMod*>(L, 2, lua::Metatables::COLOR, "Color");

	return 0;
}

/*
// needs return register override support
LUA_FUNCTION(Lua_PlayerGetSoundPitch) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, player->GetSoundPitch());

	return 1;
}


LUA_FUNCTION(Lua_PlayerGetSalvationScale) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, player->GetSalvationScale());

	return 1;
}
*/

LUA_FUNCTION(Lua_PlayerHasInstantDeathCurse) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->HasInstantDeathCurse());

	return 1;
}

LUA_FUNCTION(Lua_PlayerHasPoisonImmunity) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->HasPoisonImmunity());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsEntityValidTarget) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* target = lua::GetUserdata<Entity*>(L, 2, lua::Metatables::ENTITY, "Entity");

	lua_pushboolean(L, player->IsEntityValidTarget(target));

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsFootstepFrame) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int foot = (int)luaL_optinteger(L, 2, -1);
	if (foot < -1 || foot > 1) {
		std::string error("Invalid foot ID ");
		error.append(std::to_string(foot));
		error.append(", valid range is -1 to 1");
		return luaL_argerror(L, 2, error.c_str());
	}
	lua_pushboolean(L, player->IsFootstepFrame(foot));

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsHeadless) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsHeadless());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsHologram) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsHologram());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsInvisible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsInvisible());

	return 1;
}

LUA_FUNCTION(Lua_PlayerIsPacifist) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, g_Game->_room->_pacifist);

	return 1;
}

LUA_FUNCTION(Lua_PlayerMorphToCoopGhost) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->MorphToCoopGhost();

	return 0;
}

LUA_FUNCTION(Lua_PlayerRemovePocketItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	if (id < 0 || id > 3) {
		std::string error("Invalid slot ID ");
		error.append(std::to_string(id));
		return luaL_argerror(L, 2, error.c_str());
	}
	player->RemovePocketItem(id);

	return 0;
}

LUA_FUNCTION(Lua_PlayerRerollAllCollectibles) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	RNG* rng;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	}
	else
	{
		rng = &player->_dropRNG;
	}

	bool includeActives = lua::luaL_optboolean(L, 3, false);

	player->RerollAllCollectibles(rng, includeActives);

	return 0;
}

LUA_FUNCTION(Lua_PlayerResetPlayer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->ResetPlayer();

	return 0;
}

LUA_FUNCTION(Lua_PlayerReviveCoopGhost) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool res = false;
	
	//printf("IsPlayerGhost: %s\n", player->_isCoopGhost ? "TRUE" : "FALSE");
	if (player->_isCoopGhost) {
		//printf("attempting revive");
		player->RevivePlayerGhost();
		res = true;
	}
	
	lua_pushboolean(L, res);
	return 1;
}

int ValidatePool(lua_State* L, unsigned int pos)
{
	int ret = (int)luaL_optinteger(L, pos, -1);
	if (ret > 30 || ret < 0) {
		ret = -1;
	}
	return ret;
}

LUA_FUNCTION(Lua_PlayerSalvageCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = nullptr;
	RNG* rng = nullptr;
	unsigned int subtype, seed;
	int pool = -1;

	// pickup override
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 2, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
		subtype = pickup->_subtype;
		pos = pickup->GetPosition();
		if (lua_type(L, 3) == LUA_TUSERDATA) {
			rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
		}
		else
		{
			rng = &pickup->_dropRNG;
		}
		pool = ValidatePool(L, 4);

		pickup->Remove();
	}
	// CollectibleType override
	else {
		subtype = (int)luaL_checkinteger(L, 2);
		if (lua_type(L, 3) == LUA_TUSERDATA) {
			pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
		}
		else
		{
			pos = player->GetPosition();
		}
		if (lua_type(L, 4) == LUA_TUSERDATA) {
			rng = lua::GetUserdata<RNG*>(L, 4, lua::Metatables::RNG, "RNG");
		}
		else
		{
			rng = &player->_dropRNG;
		}
		pool = ValidatePool(L, 5);
	}
	seed = rng->Next();

	player->SalvageCollectible(pos, subtype, seed, pool);
	return 0;
}

LUA_FUNCTION(Lua_PlayerSetControllerIndex) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int idx = (int)luaL_checkinteger(L, 2);
	player->SetControllerIndex(idx);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetFootprintColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	KColor* color = nullptr;
	if (lua::luaL_checkboolean(L, 2)) {
		color = &player->_footprintColor2;
	}
	else
	{
		color = &player->_footprintColor1;
	}
	lua::luabridge::UserdataPtr::push(L, color, lua::Metatables::KCOLOR);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetFootprintColor) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	KColor* color = lua::GetUserdata<KColor*>(L, 2, lua::Metatables::KCOLOR, "KColor");
	bool unk = lua::luaL_optboolean(L, 3, false);
	player->SetFootprintColor(color, unk);

	return 0;
}

// todo: asm patch to return effect
LUA_FUNCTION(Lua_PlayerShootBlueCandle) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* dir = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	player->ShootBlueCandle(dir);
	return 0;
}

// not sure if this returns the clot or not
LUA_FUNCTION(Lua_PlayerSpawnClot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* pos = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	bool canKillPlayer = lua::luaL_optboolean(L, 3, false);
	player->SpawnClot(pos, canKillPlayer);
	return 0;
}

//  todo: asm patch to return tears
LUA_FUNCTION(Lua_PlayerSpawnSaturnusTears) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->SpawnSaturnusTears());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSyncConsumableCounts) {
	Entity_Player* player1 = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Player* player2 = lua::GetUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int bitflags = (int)luaL_checkinteger(L, 3);
	player1->SyncConsumableCounts(player2, bitflags);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryAddToBagOfCrafting) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_Pickup* pickup = lua::GetUserdata<Entity_Pickup*>(L, 2, lua::Metatables::ENTITY_PICKUP, "EntityPickup");
	player->TryAddToBagOfCrafting(pickup);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryDecreaseGlowingHourglassUses) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int unk1 = (int)luaL_checkinteger(L, 2);
	bool unk2 = lua::luaL_checkboolean(L, 3);
	player->TryDecreaseGlowingHourglassUses(unk1, unk2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerTryForgottenThrow) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Vector* dir = lua::GetUserdata<Vector*>(L, 2, lua::Metatables::VECTOR, "Vector");
	lua_pushboolean(L, player->TryForgottenThrow(dir));
	return 1;
}

LUA_FUNCTION(Lua_PlayerTryRemoveSmeltedTrinket) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int id = (int)luaL_checkinteger(L, 2);
	player->TryRemoveSmeltedTrinket(id);
	return 0;
}

/*
// this seems to rely on a struct of door outline effects that doesn't exist when red key or cracked key aren't in possession
LUA_FUNCTION(Lua_PlayerUseRedKey) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->UseRedKey();
	return 0;
}
*/

LUA_FUNCTION(Lua_PlayerVoidHasCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int id = (int)luaL_checkinteger(L, 2);
	lua_pushboolean(L, player->VoidHasCollectible(id));
	return 1;
}

LUA_FUNCTION(Lua_PlayerAddColEffect) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int colid = (int)luaL_checkinteger(L, 2);
	bool costume = lua::luaL_checkboolean(L, 3);
	int cooldown = luaL_optinteger(L, 4, -6942069); //lol
	bool additive = lua::luaL_optboolean(L, 5, true);
	
	TemporaryEffects* effs = &player->_temporaryeffects;
	if (additive && (cooldown != -6942069)) {
		TemporaryEffect* coleff = effs->GetCollectibleEffect(colid);
		if (coleff && (coleff->_count > 0)) {
			cooldown += coleff->_cooldown;
		}
		if (cooldown < 1) { cooldown = 1; }
	}
	effs->AddCollectibleEffect(colid, costume, 1);
	if ((!additive) || (cooldown != -6942069)) {
		effs->GetCollectibleEffect(colid)->_cooldown = cooldown;
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerAddNullEffect) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int colid = (int)luaL_checkinteger(L, 2);
	bool costume = lua::luaL_checkboolean(L, 3);
	int cooldown = luaL_optinteger(L, 4, -6942069); //lol
	bool additive = lua::luaL_optboolean(L, 5, true);

	TemporaryEffects* effs = &player->_temporaryeffects;
	if (additive && (cooldown != -6942069)) {
		TemporaryEffect* coleff = effs->GetNullEffect(colid);
		if (coleff && (coleff->_count > 0)) {
			cooldown += coleff->_cooldown;
		}
		if (cooldown < 1) { cooldown = 1; }
	}
	effs->AddNullEffect(colid, costume, 1);
	if ((!additive) || (cooldown != -6942069)) {
		effs->GetNullEffect(colid)->_cooldown = cooldown;
	}
	return 1;
}

LUA_FUNCTION(Lua_PlayerAddTrinketEffect) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int colid = (int)luaL_checkinteger(L, 2);
	bool costume = lua::luaL_checkboolean(L, 3);
	int cooldown = luaL_optinteger(L, 4, -6942069); //lol
	bool additive = lua::luaL_optboolean(L, 5, true);

	TemporaryEffects* effs = &player->_temporaryeffects;
	if (additive && (cooldown != -6942069)) {
		TemporaryEffect* coleff = effs->GetTrinketEffect(colid);
		if (coleff && (coleff->_count > 0)) {
			cooldown += coleff->_cooldown;
		}
		if (cooldown < 1) { cooldown = 1; }
	}
	effs->AddTrinketEffect(colid, costume, 1);
	if ((!additive) || (cooldown != -6942069)) {
		effs->GetTrinketEffect(colid)->_cooldown = cooldown;
	}
	return 1;
}

/*
// doesn't seem to work
LUA_FUNCTION(Lua_PlayerAttachMinecart) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity_NPC* minecart = lua::GetUserdata<Entity_NPC*>(L, 2, lua::Metatables::ENTITY_NPC, "EntityNPC");
	player->AttachMinecart(minecart);
	return 0;
}
*/

LUA_FUNCTION(Lua_PlayerGetKeepersSackBonus) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_keepersSackBonus);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetKeepersSackBonus) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_keepersSackBonus = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetGnawedLeafTimer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_gnawedLeafTimer);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetGnawedLeafTimer) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_gnawedLeafTimer = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBloodLustCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_bloodLustCounter);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBloodLustCounter) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_bloodLustCounter = (short)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBombPlaceDelay) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_bombPlaceDelay);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBombPlaceDelay) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->_bombPlaceDelay = (int)luaL_checkinteger(L, 2);
	return 0;
}


LUA_FUNCTION(Lua_PlayerClearQueueItem) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->ClearQueueItem();
	return 0;
}

const char* headAnims[4] = {
	"HeadLeft",
	"HeadUp",
	"HeadRight",
	"HeadDown",
};

LUA_FUNCTION(Lua_PlayerSetHeadDirection) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int direction = (int)luaL_checkinteger(L, 2);
	int time = (int)luaL_checkinteger(L, 3);
	bool force = lua::luaL_optboolean(L, 4, false);

	if (direction < 0 || direction > 3) {
		return luaL_argerror(L, 2, "Invalid Direction");
	}

	if (force || player->_headDirectionTime < 0) {
		if (player->_headDirection != direction) {
			player->_headDirection = direction;
			player->_headAnim = headAnims[direction];
		}
		player->_headDirectionTime = time;
	}

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetHeadDirectionLockTime) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->_headDirectionTime);

	return 1;
}

LUA_FUNCTION(Lua_PlayerSetHeadDirectionLockTime) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int time = (int)luaL_checkinteger(L, 2);

	player->_headDirectionTime = time;

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "GetMultiShotPositionVelocity", Lua_GetMultiShotPositionVelocity },
		{ "InitTwin", Lua_InitTwin },
		{ "InitPostLevelInitStats", Lua_InitPostLevelInitStats },
		{ "SetItemState", Lua_PlayerSetItemState },
		{ "AddCacheFlags", Lua_PlayerAddCacheFlags },
		{ "GetHealthType", Lua_PlayerGetHealthType },
		{ "GetTotalActiveCharge", Lua_PlayerGetTotalActiveCharge },
		{ "GetActiveMaxCharge", Lua_PlayerGetActiveMaxCharge },
		{ "GetActiveMinUsableCharge", Lua_PlayerGetActiveMinUsableCharge },
		{ "SetActiveVarData", Lua_PlayerSetActiveVarData },
		{ "AddActiveCharge", Lua_PlayerAddActiveCharge },
		{ "DropCollectible", Lua_PlayerDropCollectible },
		{ "DropCollectibleByHistoryIndex", Lua_PlayerDropCollectibleByHistoryIndex },
		{ "IncrementPlayerFormCounter", Lua_PlayerIncrementPlayerFormCounter },
		{ "TryPreventDeath", Lua_PlayerTryPreventDeath },
		{ "SetCanShoot", Lua_PlayerSetCanShoot },
		{ "GetDeadEyeCharge", Lua_PlayerGetDeadEyeCharge },
		{ "RemoveCollectibleByHistoryIndex", lua_PlayerRemoveCollectibleByHistoryIndex },
		{ "Teleport", Lua_PlayerTeleport },
		{ "GetMegaBlastDuration", Lua_PlayerGetMegaBlastDuration },
		{ "SetMegaBlastDuration", Lua_PlayerSetMegaBlastDuration },
		{ "GetActiveItemDesc", Lua_PlayerGetActiveItemDesc },
		{ "GetActiveItemSlot", Lua_PlayerGetActiveItemSlot },
		{ "TryFakeDeath", Lua_PlayerTryFakeDeath },
		{ "GetBagOfCraftingContent", Lua_PlayerGetBoCContent },
		{ "SetBagOfCraftingContent", Lua_PlayerSetBoCContent },
		{ "SetBagOfCraftingSlot", Lua_PlayerSetBoCSlot },
		{ "GetBagOfCraftingSlot", Lua_PlayerGetBoCSlot },
		{ "GetBagOfCraftingOutput", Lua_PlayerGetBagOfCraftingOutput },
		{ "SetBagOfCraftingOutput", Lua_PlayerSetBagOfCraftingOutput },
		{ "GetSpeedModifier", Lua_PlayerGetSpeedModifier },
		{ "SetSpeedModifier", Lua_PlayerSetSpeedModifier },
		{ "GetFireDelayModifier", Lua_PlayerGetFireDelayModifier },
		{ "SetFireDelayModifier", Lua_PlayerSetFireDelayModifier },
		{ "GetDamageModifier", Lua_PlayerGetDamageModifier },
		{ "SetDamageModifier", Lua_PlayerSetDamageModifier },
		{ "SetTearRangeModifier", Lua_PlayerSetTearRangeModifier }, // .-.
		{ "GetShotSpeedModifier", Lua_PlayerGetShotSpeedModifier },
		{ "SetShotSpeedModifier", Lua_PlayerSetShotSpeedModifier },
		{ "GetLuckModifier", Lua_PlayerGetLuckModifier },
		{ "SetLuckModifier", Lua_PlayerSetLuckModifier },
		{ "GetRedStewBonusDuration", Lua_PlayerGetRedStewBonusDuration },
		{ "SetRedStewBonusDuration", Lua_PlayerSetRedStewBonusDuration },
		{ "GetWeaponModifiers", Lua_PlayerGetWeaponModifiers },
		{ "EnableWeaponType", Lua_PlayerEnableWeaponType },
		{ "GetD8DamageModifier", Lua_PlayerGetD8DamageModifier },
		{ "GetD8SpeedModifier", Lua_PlayerGetD8SpeedModifier },
		{ "GetD8RangeModifier", Lua_PlayerGetD8RangeModifier },
		{ "GetD8FireDelayModifier", Lua_PlayerGetD8FireDelayModifier },
		{ "GetEpiphoraCharge", Lua_PlayerGetEpiphoraCharge },
		{ "GetPeeBurstCooldown", Lua_PlayerGetPeeBurstCooldown },
		{ "GetMaxPeeBurstCooldown", Lua_PlayerGetMaxPeeBurstCooldown },
		{ "GetMetronomeCollectibleID", Lua_PlayerGetMetronomeCollectibleID },
		{ "GetMarkedTarget", Lua_PlayerGetMarkedTarget },
		{ "IsLocalPlayer", Lua_PlayerIsLocalPlayer },
		{ "GetWildCardItem", Lua_PlayerGetWildCardItem },
		{ "GetWildCardItemType", Lua_PlayerGetWildCardItemType },
		{ "SetWeapon", Lua_PlayerSetWeapon },
		{ "AddLocust", Lua_PlayerAddLocust },
		{ "GetPonyCharge", Lua_PlayerGetPonyCharge },
		{ "SetPonyCharge", Lua_PlayerSetPonyCharge },
		{ "GetEdenSpeed", Lua_PlayerGetEdenSpeed },
		{ "SetEdenSpeed", Lua_PlayerSetEdenSpeed },
		{ "GetEdenFireDelay", Lua_PlayerGetEdenFireDelay },
		{ "SetEdenFireDelay", Lua_PlayerSetEdenFireDelay },
		{ "GetEdenDamage", Lua_PlayerGetEdenDamage },
		{ "SetEdenDamage", Lua_PlayerSetEdenDamage },
		{ "GetEdenRange", Lua_PlayerGetEdenRange },
		{ "SetEdenRange", Lua_PlayerSetEdenRange },
		{ "GetEdenShotSpeed", Lua_PlayerGetEdenShotSpeed },
		{ "SetEdenShotSpeed", Lua_PlayerSetEdenShotSpeed },
		{ "GetEdenLuck", Lua_PlayerGetEdenLuck },
		{ "SetEdenLuck", Lua_PlayerSetEdenLuck },
		{ "TriggerRoomClear", Lua_PlayerTriggerRoomClear },
		{ "GetCollectiblesList", Lua_PlayerGetCollectiblesList },
		{ "ShuffleCostumes", Lua_PlayerShuffleCostumes },
		{ "GetPurityState", Lua_PlayerGetPurityState },
		{ "SetPurityState", Lua_PlayerSetPurityState },
		{ "SetTearPoisonDamage", Lua_PlayerSetTearPoisonDamage },
		{ "GetVoidedCollectiblesList", Lua_PlayerGetVoidedCollectiblesList },
		{ "AddInnateCollectible", Lua_PlayerAddInnateCollectible },
		{ "GetWispCollectiblesList", Lua_PlayerGetWispCollecitblesList },
		{ "GetImmaculateConceptionState", Lua_PlayerGetImmaculateConceptionState },
		{ "SetImmaculateConceptionState", Lua_PlayerSetImmaculateConceptionState },
		{ "GetCambionConceptionState", Lua_PlayerGetCambionConceptionState },
		{ "SetCambionConceptionState", Lua_PlayerSetCambionConceptionState },
		{ "UpdateIsaacPregnancy", Lua_PlayerUpdateIsaacPregnancy },
		{ "GetCambionPregnancyLevel", Lua_PlayerGetCambionPregnancyLevel },
		{ "GetBladderCharge", Lua_PlayerGetBladderCharge },
		{ "SetBladderCharge", Lua_PlayerSetBladderCharge },
		{ "GetMaxBladderCharge", Lua_PlayerGetMaxBladderCharge },
		{ "SetMaxBladderCharge", Lua_PlayerSetMaxBladderCharge },
		{ "ClearDeadEyeCharge", Lua_ClearDeadEyeCharge },
		{ "IsUrethraBlocked", Lua_PlayerIsUrethraBlocked },
		{ "SetUrethraBlock", Lua_PlayerSetUrethraBlock },
		{ "GetNextUrethraBlockFrame", Lua_PlayerGetNextUrethraBlockFrame },
		{ "SetNextUrethraBlockFrame", Lua_PlayerSetNextUrethraBlockFrame },
		{ "GetHeldSprite", Lua_PlayerGetHeldSprite },
		{ "GetHeldEntity", Lua_PlayerGetHeldEntity },
		{ "GetActiveWeaponNumFired", Lua_PlayerGetActiveWeaponNumFired },
		{ "SetPoopSpell", Lua_PlayerSetPoopSpell },
		{ "RemovePoopSpell", Lua_PlayerRemovePoopSpell },
		{ "GetFlippedForm", Lua_PlayerGetBackupPlayer },
		{ "SwapForgottenForm", Lua_SwapForgottenForm },
		{ "SpawnAquariusCreep", Lua_SpawnAquariusCreep },
		{ "GetMaggySwingCooldown", Lua_PlayerGetMaggySwingCooldown },
		{ "SetMaggySwingCooldown", Lua_PlayerSetMaggySwingCooldown },
		{ "PlayDelayedSFX", Lua_PlayerPlayDelayedSFX },
		{ "CanUsePill", Lua_PlayerCanUsePill },
		{ "AddSmeltedTrinket", Lua_PlayerAddSmeltedTrinket },
		{ "GetSmeltedTrinkets", Lua_PlayerGetSmeltedTrinkets },
		{ "GetCostumeLayerMap", Lua_PlayerGetCostumeLayerMap },
		{ "IsItemCostumeVisible", Lua_PlayerIsItemCostumeVisible },
		{ "IsCollectibleCostumeVisible", Lua_PlayerIsCollectibleCostumeVisible },
		{ "IsNullItemCostumeVisible", Lua_PlayerIsNullItemCostumeVisible },
		{ "PlayCollectibleAnim", Player_PlayCollectibleAnim },
		{ "IsCollectibleAnimFinished", Player_IsCollectibleAnimFinished },
		{ "ClearCollectibleAnim", Player_ClearCollectibleAnim },
		{ "CheckFamiliarEx", Lua_EntityPlayer_CheckFamiliarEx },
		{ "GetEveSumptoriumCharge", Lua_PlayerGetEveSumptoriumCharge },
		{ "SetEveSumptoriumCharge", Lua_PlayerSetEveSumptoriumCharge },
		{ "GetPlayerFormCounter", Lua_PlayerGetPlayerFormCounter },
		{ "GetMaxPocketItems", Lua_PlayerGetMaxPocketItems },
		{ "AddBoneOrbital", Lua_PlayerAddBoneOrbital },
		//{ "AddItemCard", Lua_PlayerAddItemCard },
		{ "AddLeprosy", Lua_PlayerAddLeprosy },
		{ "GetUrnSouls", Lua_PlayerGetUrnSouls },
		{ "AddUrnSouls", Lua_PlayerAddUrnSouls },
		{ "CanAddCollectibleToInventory", Lua_PlayerCanAddCollectibleToInventory },
		{ "CanCrushRocks", Lua_PlayerCanCrushRocks },
		{ "CanOverrideActiveItem", Lua_PlayerCanOverrideActiveItem },
		{ "ClearItemAnimCollectible", Lua_PlayerClearItemAnimCollectible },
		{ "ClearItemAnimNullItems", Lua_PlayerClearItemAnimNullItems },
		//{ "FireBoneClub", Lua_PlayerFireBoneClub },
		{ "FireBrimstoneBall", Lua_PlayerFireBrimstoneBall },
		{ "GetBodyMoveDirection", Lua_PlayerGetBodyMoveDirection },
		{ "GetDeathAnimName", Lua_PlayerGetDeathAnimName },
		{ "GetEnterPosition", Lua_PlayerGetEnterPosition },
		//{ "GetExplosionRadiusMultiplier", Lua_PlayerGetExplosionRadiusMultiplier },
		{ "GetFocusEntity", Lua_PlayerGetFocusEntity },
		{ "GetFootprintColor", Lua_PlayerGetFootprintColor },
		{ "SetFootprintColor", Lua_PlayerSetFootprintColor },
		{ "GetGlitchBabySubType", Lua_PlayerGetGlitchBabySubType },
		{ "GetGlyphOfBalanceDrop", Lua_PlayerGetGlyphOfBalanceDrop },
		{ "GetGreedsGulletHearts", Lua_PlayerGetGreedsGulletHearts },
		{ "GetLaserColor", Lua_PlayerGetLaserColor },
		{ "SetLaserColor", Lua_PlayerSetLaserColor },
		//{ "GetSalvationScale", Lua_PlayerGetSalvationScale },
		//{ "GetSoundPitch", Lua_PlayerGetSoundPitch },
		{ "GetSpecialGridCollision", Lua_PlayerGetSpecialGridCollision },
		{ "HasInstantDeathCurse", Lua_PlayerHasInstantDeathCurse },
		{ "HasPoisonImmunity", Lua_PlayerHasPoisonImmunity },
		{ "IsEntityValidTarget", Lua_PlayerIsEntityValidTarget },
		{ "IsFootstepFrame", Lua_PlayerIsFootstepFrame },
		{ "IsHeadless", Lua_PlayerIsHeadless },
		{ "IsHologram", Lua_PlayerIsHologram },
		{ "IsInvisible", Lua_PlayerIsInvisible },
		{ "IsPacifist", Lua_PlayerIsPacifist },
		{ "MorphToCoopGhost", Lua_PlayerMorphToCoopGhost },
		{ "RemovePocketItem", Lua_PlayerRemovePocketItem },
		{ "RerollAllCollectibles", Lua_PlayerRerollAllCollectibles },
		{ "ResetPlayer", Lua_PlayerResetPlayer },
		{ "ReviveCoopGhost", Lua_PlayerReviveCoopGhost },
		{ "SalvageCollectible", Lua_PlayerSalvageCollectible },
		{ "SetControllerIndex", Lua_PlayerSetControllerIndex },
		{ "ShootBlueCandle", Lua_PlayerShootBlueCandle },
		{ "SpawnClot", Lua_PlayerSpawnClot },
		{ "SpawnSaturnusTears", Lua_PlayerSpawnSaturnusTears },
		{ "SyncConsumableCounts", Lua_PlayerSyncConsumableCounts },
		{ "TryAddToBagOfCrafting", Lua_PlayerTryAddToBagOfCrafting },
		{ "TryDecreaseGlowingHourglassUses", Lua_PlayerTryDecreaseGlowingHourglassUses },
		{ "TryForgottenThrow", Lua_PlayerTryForgottenThrow },
		{ "TryRemoveSmeltedTrinket", Lua_PlayerTryRemoveSmeltedTrinket },
		//{ "UseRedKey", Lua_PlayerUseRedKey },
		{ "VoidHasCollectible", Lua_PlayerVoidHasCollectible },
		//{ "AttachMinecart", Lua_PlayerAttachMinecart },
		{ "GetKeepersSackBonus", Lua_PlayerGetKeepersSackBonus },
		{ "SetKeepersSackBonus", Lua_PlayerSetKeepersSackBonus },
		{ "GetGnawedLeafTimer", Lua_PlayerGetGnawedLeafTimer },
		{ "SetGnawedLeafTimer", Lua_PlayerSetGnawedLeafTimer },
		{ "GetBloodLustCounter", Lua_PlayerGetBloodLustCounter },
		{ "SetBloodLustCounter", Lua_PlayerSetBloodLustCounter },
		{ "GetBombPlaceDelay", Lua_PlayerGetBombPlaceDelay },
		{ "SetBombPlaceDelay", Lua_PlayerSetBombPlaceDelay },
		{ "ClearQueueItem", Lua_PlayerClearQueueItem },
		{ "GetHeadDirectionLockTime", Lua_PlayerGetHeadDirectionLockTime },
		{ "SetHeadDirectionLockTime", Lua_PlayerSetHeadDirectionLockTime },
		{ "SetHeadDirection", Lua_PlayerSetHeadDirection },
		{ "AddCollectibleEffect", Lua_PlayerAddColEffect },
		{ "AddCollectibleEffect", Lua_PlayerAddColEffect },
		{ "AddNullItemEffect", Lua_PlayerAddNullEffect },
		{ "AddTrinketEffect", Lua_PlayerAddTrinketEffect }, //this one is ass, literally does nothign, leaving it out of the docs
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PLAYER, functions);

	// fix BabySkin Variable
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_PLAYER, "BabySkin", Lua_PlayerGetBabySkin, Lua_PlayerSetBabySkin);
}