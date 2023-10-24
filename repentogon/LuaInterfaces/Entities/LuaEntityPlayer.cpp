#include <algorithm>
#include <random>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../LuaWeapon.h"

/*

		 .___.
	 / /\ /\ \
	||●  `\ ●||
	 \   ⌓   /
		/  ᴥ  \
	 ||  _  ||
		|_| |_|

	 "code"
 (copycat of kilburn's ascii art of Isaac)
*/


// this is from LuaInit but with a range of 0, 1 instead of -1, 1
static std::uniform_real_distribution<float> _distrib(0, 1);
static std::random_device rd;
static std::mt19937 gen(rd());

std::map<int, int> fakeItems;


LUA_FUNCTION(Lua_GetMultiShotPositionVelocity) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY, "EntityPlayer");
	int loopIndex = (int)luaL_checkinteger(L, 2);
	int weaponType = (int)luaL_checkinteger(L, 3);
	Vector* shotDirection = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::ENTITY, "Vector");
	float shotSpeed = (float)luaL_checknumber(L, 5);

	Weapon_MultiShotParams* multiShotParams = lua::GetUserdata<Weapon_MultiShotParams*>(L, 6, lua::metatables::MultiShotParamsMT);
	if (multiShotParams->numTears < loopIndex) {
		luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.NumTears");
	};

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), player->GetMultiShotPositionVelocity(loopIndex, (WeaponType)weaponType, *shotDirection, shotSpeed, *multiShotParams));

	return 1;
}

LUA_FUNCTION(Lua_InitTwin)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* twinPlayer = player->InitTwin(playerType);
	lua::luabridge::UserdataPtr::push(L, twinPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

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
	bool evaluateCache = false;
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int flags = (int)luaL_checkinteger(L, 2);
	if (lua_isboolean(L, 3))
		evaluateCache = lua_toboolean(L, 3);

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
	int item = (int)luaL_checkinteger(L, 2);
	int vardata = (int)luaL_checkinteger(L, 3);

	lua_pushinteger(L, player->GetActiveMaxCharge(item, vardata));
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
	bool unknown = lua_toboolean(L, 4);
	bool overcharge = lua_toboolean(L, 5);
	bool force = lua_toboolean(L, 6);

	int ret = player->AddActiveCharge(charge, slot, unknown, overcharge, force);
	lua_pushinteger(L, ret);

	return 1;
}

LUA_FUNCTION(Lua_PlayerDropCollectible) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectible = (int)luaL_checkinteger(L, 2);

	player->DropCollectible(collectible, 0, false);
	return 0;
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
	bool canShoot = lua_toboolean(L, 2);
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
	bool doEffects = true;
	bool teleportTwinPlayers = false;

	if (lua_isboolean(L, 3))
		doEffects = lua_toboolean(L, 3);

	if (lua_isboolean(L, 4))
		teleportTwinPlayers = lua_toboolean(L, 4);

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
		luaL_argerror(L, 2, "ActiveSlot cannot be higher than 3");
	}
	ActiveItemDesc* desc = player->GetActiveItemDesc(index);
	lua::luabridge::UserdataPtr::push(L, desc, lua::GetMetatableKey(lua::Metatables::ACTIVE_ITEM_DESC));

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
		luaL_error(L, "EntityPlayer::SetBagOfCraftingContent: Expected a table as second argument");
		return 0;
	}

	BagOfCraftingPickup list[8]{};
	size_t length = (size_t)lua_rawlen(L, 2);
	if (length > 0)
	{
		if (length > 8) {
			luaL_error(L, "EntityPlayer::SetBagOfCraftingContent: Table cannot be larger than 8 pickups");
		}

		size_t index;
		for (index = 0; index < length; index++)
		{
			lua_rawgeti(L, 2, index + 1);
			int pickup = luaL_checkinteger(L, -1);
			lua_pop(L, 1);
			if (pickup < 0 || pickup > 29) {
				luaL_error(L, "EntityPlayer::SetBagOfCraftingContent: Invalid pickup %d at index %d", pickup, index + 1);
			}
			list[index] = (BagOfCraftingPickup)pickup;
		}
	}
	memcpy(&player->_bagOfCraftingContent, list, sizeof(list));

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int n = lua_gettop(L);
	if (n != 2) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: expected 1 parameter, got %d\n", n - 1);
	}

	int slot = (int)luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	lua_pushinteger(L, player->GetBagOfCraftingContent()[slot]);
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetBoCSlot) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int n = lua_gettop(L);
	if (n > 3 || n < 2) {
		return luaL_error(L, "EntityPlayer::SetBagOfCraftingSlot: expected at least 1 parameter and at most 2, got %d\n", n - 1);
	}

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

LUA_FUNCTION(Lua_PlayerGetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetFireDelayModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetFireDelayModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetFireDelayModifier() = (int)luaL_checkinteger(L, 2);
	return 0;
}

LUA_FUNCTION(Lua_PlayerGetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetDamageModifier());
	return 1;
}

LUA_FUNCTION(Lua_PlayerSetDamageModifier)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetDamageModifier() = (int)luaL_checkinteger(L, 2);
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
	int weaponType = (int)luaL_checkinteger(L, 2);
	bool set = lua_toboolean(L, 3);
	player->EnableWeaponType((WeaponType)weaponType, set);
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
	int seed = (int)luaL_checkinteger(L, 2);
	plr->ShuffleCostumes(seed);

	return 0;
}

LUA_FUNCTION(Lua_PlayerGetCollectiblesList)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	std::vector<int> collecitbleInv = plr->GetCollectiblesList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collecitbleInv) {
		lua_pushnumber(L, idx);
		lua_pushinteger(L, collectible);
		lua_settable(L, -3);
		idx++;
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
	std::vector<int> collecitbleInv = plr->GetVoidedCollectiblesList();

	lua_newtable(L);
	int idx = 1;
	for (int collectible : collecitbleInv) {
		lua_pushnumber(L, idx);
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
	bool cambion = lua_toboolean(L, 2);
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
	*plr->IsUrethraBlocked() = lua_toboolean(L, 2);
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
	if (heldEntity == nullptr) {
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
	Weapon* wep = NULL;

	wep = *plr->GetWeapon(0);
	if ((wep == nullptr) && ((wep = *plr->GetWeapon(1)) == nullptr)) {
		lua_pushnil(L);
	}
	lua_pushinteger(L, wep->GetNumFired());

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
	bool IgnoreHealth = false;
	bool NoEffects = false;
	if (lua_isboolean(L, 2)) {
		IgnoreHealth = lua_toboolean(L, 2);
	}

	if (lua_isboolean(L, 3)) {
		NoEffects = lua_toboolean(L, 3);
	}
	player->SwapForgottenForm(IgnoreHealth, NoEffects);
	return 0;
}

LUA_FUNCTION(Lua_SpawnAquariusCreep) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* castPlayer = (Entity*)player;

	TearParams params;

	if (lua_gettop(L) >= 2) {
		params = *lua::GetUserdata<TearParams*>(L, 2, lua::Metatables::TEAR_PARAMS, "TearParams");
	}
	else {
		player->GetTearHitParams(&params, 1, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, (-(int)(Isaac::Random(2) != 0) & 2) - 1, 0);
	}

	Entity* ent = g_Game->Spawn(1000, 54, *castPlayer->GetPosition(), Vector(0.0, 0.0), player, 0, Random(), 0);

	ent->_sprite._scale *= ((_distrib(gen) * 0.5f) + 0.2f);
	ent->_collisionDamage = params._tearDamage;
	ent->SetColor(&params._tearColor, 0, -1, true, false);

	Entity_Effect* creep = (Entity_Effect*)ent;
	creep->_varData = params._flags;
	creep->Update();

	lua::luabridge::UserdataPtr::push(L, creep, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

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
		{ "IncrementPlayerFormCounter", Lua_PlayerIncrementPlayerFormCounter },
		{ "TryPreventDeath", Lua_PlayerTryPreventDeath },
		{ "SetCanShoot", Lua_PlayerSetCanShoot },
		{ "GetDeadEyeCharge", Lua_PlayerGetDeadEyeCharge },
		{ "RemoveCollectibleByHistoryIndex", lua_PlayerRemoveCollectibleByHistoryIndex },
		{ "Teleport", Lua_PlayerTeleport },
		{ "GetMegaBlastDuration", Lua_PlayerGetMegaBlastDuration },
		{ "SetMegaBlastDuration", Lua_PlayerSetMegaBlastDuration },
		{ "GetActiveItemDesc", Lua_PlayerGetActiveItemDesc },
		{ "TryFakeDeath", Lua_PlayerTryFakeDeath },
		{ "GetBagOfCraftingContent", Lua_PlayerGetBoCContent },
		{ "SetBagOfCraftingContent", Lua_PlayerSetBoCContent },
		{ "SetBagOfCraftingSlot", Lua_PlayerSetBoCSlot },
		{ "GetBagOfCraftingSlot", Lua_PlayerGetBoCSlot },
		{ "GetBagOfCraftingOutput", Lua_PlayerGetBagOfCraftingOutput },
		{ "SetBagOfCraftingOutput", Lua_PlayerSetBagOfCraftingOutput },
		{ "GetFireDelayModifier", Lua_PlayerGetFireDelayModifier },
		{ "SetFireDelayModifier", Lua_PlayerSetFireDelayModifier },
		{ "GetDamageModifier", Lua_PlayerGetDamageModifier },
		{ "SetDamageModifier", Lua_PlayerSetDamageModifier },
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
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::ENTITY_PLAYER, functions);

	// fix BabySkin Variable
	lua::RegisterVariable(_state, lua::Metatables::ENTITY_PLAYER, "BabySkin", Lua_PlayerGetBabySkin, Lua_PlayerSetBabySkin);
}