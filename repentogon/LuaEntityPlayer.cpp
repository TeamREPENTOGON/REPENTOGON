#include <lua.hpp>
#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "LuaWeapon.h"

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

static constexpr const char* PocketItemMT = "PocketItem";

std::map<int, int> fakeItems;

int Lua_GetMultiShotPositionVelocity(lua_State* L) // This *should* be in the API, but magically vanished some point after 1.7.8.
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

static int Lua_GetMultiShotParams(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int weaponType = (int)luaL_checkinteger(L, 2);
	Weapon_MultiShotParams* ud = (Weapon_MultiShotParams*)lua_newuserdata(L, sizeof(Weapon_MultiShotParams));
	ud = player->GetMultiShotParams(ud, (WeaponType)weaponType);
	luaL_setmetatable(L, lua::metatables::MultiShotParamsMT);
	return 1;
}

static int Lua_MultiShotParamsGetNumTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numTears);
	return 1;
}

static int Lua_MultiShotParamsSetNumTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numTears = (int) luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetNumLanesPerEye(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numLanesPerEye);
	return 1;
}

static int Lua_MultiShotParamsSetNumLanesPerEye(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numLanesPerEye = (unsigned int) luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetSpreadAngleTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleTears);
	return 1;
}

static int Lua_MultiShotParamsSetSpreadAngleTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleTears = (float) luaL_checknumber(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetSpreadAngleLaser(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleLaser);
	return 1;
}

static int Lua_MultiShotParamsSetSpreadAngleLaser(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleLaser = (float)luaL_checknumber(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetSpreadAngleTechX(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->speadAngleTechX);
	return 1;
}

static int Lua_MultiShotParamsSetSpreadAngleTechX(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->speadAngleTechX = (float)luaL_checknumber(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetSpreadAngleKnife(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->spreadAngleKnife);
	return 1;
}

static int Lua_MultiShotParamsSetSpreadAngleKnife(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->spreadAngleKnife = (float)luaL_checknumber(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetNumEyesActive(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numEyesActive);
	return 1;
}

static int Lua_MultiShotParamsSetNumEyesActive(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numEyesActive = (int)luaL_checkinteger(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetMultiEyeAngle(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushnumber(L, params->multiEyeAngle);
	return 1;
}

static int Lua_MultiShotParamsSetMultiEyeAngle(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->multiEyeAngle = (float)luaL_checknumber(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetIsCrossEyed(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isCrossEyed);
	return 1;
}

static int Lua_MultiShotParamsSetIsCrossEyed(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isCrossEyed = lua_toboolean(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetIsShootingBackwards(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingBackwards);
	return 1;
}

static int Lua_MultiShotParamsSetIsShootingBackwards(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingBackwards = lua_toboolean(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetIsShootingSideways(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushboolean(L, params->isShootingSideways);
	return 1;
}

static int Lua_MultiShotParamsSetIsShootingSideways(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->isShootingSideways = lua_toboolean(L, 2);
	return 0;
}

static int Lua_MultiShotParamsGetNumRandomDirTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	lua_pushinteger(L, params->numRandomDirTears);
	return 1;
}

static int Lua_MultiShotParamsSetNumRandomDirTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, lua::metatables::MultiShotParamsMT);

	params->numRandomDirTears = (int)luaL_checkinteger(L, 2);
	return 0;
}

static void RegisterMultiShotParams(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetMultiShotParams");
	lua_pushcfunction(L, Lua_GetMultiShotParams);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, lua::metatables::MultiShotParamsMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	lua::TableAssoc(L, "GetNumTears", Lua_MultiShotParamsGetNumTears);
	lua::TableAssoc(L, "GetNumLanesPerEye", Lua_MultiShotParamsGetNumLanesPerEye);
	lua::TableAssoc(L, "GetSpreadAngleTears", Lua_MultiShotParamsGetSpreadAngleTears);
	lua::TableAssoc(L, "GetSpreadAngleLaser", Lua_MultiShotParamsGetSpreadAngleLaser);
	lua::TableAssoc(L, "GetSpreadAngleTechX", Lua_MultiShotParamsGetSpreadAngleTechX);
	lua::TableAssoc(L, "GetSpreadAngleKnife", Lua_MultiShotParamsGetSpreadAngleKnife);
	lua::TableAssoc(L, "GetNumEyesActive", Lua_MultiShotParamsGetNumEyesActive);
	lua::TableAssoc(L, "GetMultiEyeAngle", Lua_MultiShotParamsGetMultiEyeAngle);
	lua::TableAssoc(L, "IsCrossEyed", Lua_MultiShotParamsGetIsCrossEyed);
	lua::TableAssoc(L, "IsShootingBackwards", Lua_MultiShotParamsGetIsShootingBackwards);
	lua::TableAssoc(L, "IsShootingSideways", Lua_MultiShotParamsGetIsShootingSideways);
	lua::TableAssoc(L, "GetNumRandomDirTears", Lua_MultiShotParamsGetNumRandomDirTears);

	lua::TableAssoc(L, "SetNumTears", Lua_MultiShotParamsSetNumTears);
	lua::TableAssoc(L, "SetNumLanesPerEye", Lua_MultiShotParamsSetNumLanesPerEye);
	lua::TableAssoc(L, "SetSpreadAngleTears", Lua_MultiShotParamsSetSpreadAngleTears);
	lua::TableAssoc(L, "SetSpreadAngleLaser", Lua_MultiShotParamsSetSpreadAngleLaser);
	lua::TableAssoc(L, "SetSpreadAngleTechX", Lua_MultiShotParamsSetSpreadAngleTechX);
	lua::TableAssoc(L, "SetSpreadAngleKnife", Lua_MultiShotParamsSetSpreadAngleKnife);
	lua::TableAssoc(L, "SetNumEyesActive", Lua_MultiShotParamsSetNumEyesActive);
	lua::TableAssoc(L, "SetMultiEyeAngle", Lua_MultiShotParamsSetMultiEyeAngle);
	lua::TableAssoc(L, "SetIsCrossEyed", Lua_MultiShotParamsSetIsCrossEyed);
	lua::TableAssoc(L, "SetIsShootingBackwards", Lua_MultiShotParamsSetIsShootingBackwards);
	lua::TableAssoc(L, "SetIsShootingSideways", Lua_MultiShotParamsSetIsShootingSideways);
	lua::TableAssoc(L, "SetNumRandomDirTears", Lua_MultiShotParamsSetNumRandomDirTears);

	lua_pop(L, 1);
}

int Lua_InitTwin(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* twinPlayer = player->InitTwin(playerType);
	lua::luabridge::UserdataPtr::push(L, twinPlayer, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
				
	return 1;
}

int Lua_InitPostLevelInitStats(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	player->InitPostLevelInitStats();

	return 0;
}

int Lua_PlayerSetItemState(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	CollectibleType item = (CollectibleType)luaL_checkinteger(L, 2);

	player->SetItemState(item);

	return 0;
}

int Lua_PlayerAddCacheFlags(lua_State* L)
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

int Lua_PlayerGetHealthType(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetHealthType());
	return 1;
}

int Lua_PlayerGetTotalActiveCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetTotalActiveCharge(slot));
	return 1;
}

int Lua_PlayerGetActiveMaxCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int item = (int)luaL_checkinteger(L, 2);
	int vardata = (int)luaL_checkinteger(L, 3);

	lua_pushinteger(L, player->GetActiveMaxCharge(item,vardata));
	return 1;
}

int Lua_PlayerGetActiveMinUsableCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = (int)luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMinUsableCharge(slot));
	return 1;
}

int Lua_PlayerSetActiveVarData(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int vardata = (int)luaL_checkinteger(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);

	player->SetActiveVarData(vardata, slot);
	return 0;
}

int Lua_PlayerAddActiveCharge(lua_State* L) {
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

int Lua_PlayerDropCollectible(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectible = (int)luaL_checkinteger(L, 2);

	player->DropCollectible(collectible, 0, false);
	return 0;
}

int Lua_PlayerIncrementPlayerFormCounter(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int ePlayerForm = (int)luaL_checkinteger(L, 2);
	int num = (int)luaL_checkinteger(L, 3);

	player->IncrementPlayerFormCounter(ePlayerForm, num);
	return 0;
}

int Lua_PlayerTryPreventDeath(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryPreventDeath());
	return 1;
}

int lua_PlayerRemoveCollectibleByHistoryIndex(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = (int)luaL_checkinteger(L, 2);
	player->RemoveCollectibleByHistoryIndex(index, true);
	return 0;
}

int Lua_PlayerSetCanShoot(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	bool canShoot = lua_toboolean(L, 2);
	*player->GetCanShoot() = canShoot;
	return 0;
}

int Lua_PlayerGetDeadEyeCharge(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetDeadEyeCharge());
	return 1;
}

int Lua_PlayerTeleport(lua_State* L)
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

int Lua_PlayerGetMegaBlastDuration(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMegaBlastDuration());
	return 1;
}

int Lua_PlayerSetMegaBlastDuration(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetMegaBlastDuration() = (int)luaL_checkinteger(L,2);
	return 0;
}

int Lua_PlayerGetActiveItemDesc(lua_State* L)
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

int Lua_PlayerTryFakeDeath(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->TryFakeDeath());
	return 1;
}

int Lua_PlayerGetBoCContent(lua_State* L) {
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

int Lua_PlayerSetBoCContent(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	// TODO: Implement this
	return 0;
}

int Lua_PlayerGetBoCSlot(lua_State* L) {
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

int Lua_PlayerSetBoCSlot(lua_State* L) {
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

int Lua_PlayerGetBagOfCraftingOutput(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetBagOfCraftingOutput());
	return 1;
}

int Lua_PlayerSetBagOfCraftingOutput(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetBagOfCraftingOutput() = (int)luaL_checkinteger(L, 2);
	return 0;
}

int Lua_PlayerGetFireDelayModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetFireDelayModifier());
	return 1;
}

int Lua_PlayerSetFireDelayModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetFireDelayModifier() = (int)luaL_checkinteger(L, 2);
	return 0;
}

int Lua_PlayerGetDamageModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetDamageModifier());
	return 1;
}

int Lua_PlayerSetDamageModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetDamageModifier() = (int)luaL_checkinteger(L, 2);
	return 0;
}

int Lua_PlayerGetRedStewBonusDuration(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetRedStewBonusDuration());
	return 1;
}

int Lua_PlayerSetRedStewBonusDuration(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*player->GetRedStewBonusDuration() = (int)luaL_checkinteger(L, 2);
	return 0;
}

int Lua_PlayerGetWeaponModifiers(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, player->GetWeaponModifiers());
	return 1;
}

int Lua_PlayerEnableWeaponType(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int weaponType = (int)luaL_checkinteger(L, 2);
	bool set = lua_toboolean(L, 3);
	player->EnableWeaponType((WeaponType)weaponType, set);
	return 0;
}

int Lua_PlayerGetD8DamageModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8DamageModifier());
	return 1;
}

int Lua_PlayerGetD8SpeedModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8SpeedModifier());
	return 1;
}

int Lua_PlayerGetD8RangeModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8RangeModifier());
	return 1;
}

int Lua_PlayerGetD8FireDelayModifier(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *player->GetD8FireDelayModifier());
	return 1;
}

int Lua_PlayerGetEpiphoraCharge(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetEpiphoraCharge());
	return 1;
}

int Lua_PlayerGetPeeBurstCooldown(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetPeeBurstCooldown());
	return 1;
}

int Lua_PlayerGetMaxPeeBurstCooldown(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMaxPeeBurstCooldown());
	return 1;
}

int Lua_PlayerGetMetronomeCollectibleID(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetMetronomeCollectibleID());
	return 1;
}

int Lua_PlayerGetMarkedTarget(lua_State* L) {
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

int Lua_PlayerGetPocketItem(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");	
	int slotID = (int)luaL_checkinteger(L, 2);
	PocketItem** ud = (PocketItem**)lua_newuserdata(L, sizeof(PocketItem*));
	*ud = player->GetPocketItem(slotID);
	luaL_setmetatable(L, PocketItemMT);
	return 1;
}

int Lua_PocketItemGetSlot(lua_State* L) {
	PocketItem* pocketItem = *lua::GetUserdata<PocketItem**>(L, 1, PocketItemMT);
	lua_pushinteger(L, pocketItem->_slot);
	return 1;
}

int Lua_PocketItemGetType(lua_State* L) {
	PocketItem* pocketItem = *lua::GetUserdata<PocketItem**>(L, 1, PocketItemMT);
	lua_pushinteger(L, pocketItem->_type);
	return 1;
}

int Lua_PlayerIsLocalPlayer(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushboolean(L, player->IsLocalPlayer());
	return 1;
}

static void RegisterPocketItem(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetPocketItem");
	lua_pushcfunction(L, Lua_PlayerGetPocketItem);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PocketItemMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "GetSlot", Lua_PocketItemGetSlot},
		{ "GetType", Lua_PocketItemGetType},
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);
}

int Lua_PlayerGetWildCardItem(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItem());
	return 1;
}

int Lua_PlayerGetWildCardItemType(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *player->GetWildCardItemType());
	return 1;
}

static int Lua_PlayerSetWeapon(lua_State* L) {
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

static int Lua_PlayerAddLocust(lua_State* L) {
	Entity_Player* ent = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectibleType = (int)luaL_checkinteger(L, 2);
	Vector* pos = lua::GetUserdata<Vector*>(L, 3, lua::Metatables::VECTOR, "Vector");
	Isaac::SpawnLocust(ent, collectibleType, pos);

	return 0;
}

static int Lua_PlayerGetPonyCharge(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPonyCharge());

	return 1;
}

static int Lua_PlayerSetPonyCharge(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPonyCharge() = (int)luaL_checkinteger(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenSpeed(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenSpeed());

	return 1;
}

static int Lua_PlayerSetEdenSpeed(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenFireDelay(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenFireDelay());

	return 1;
}

static int Lua_PlayerSetEdenFireDelay(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenFireDelay() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenDamage(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenDamage());

	return 1;
}

static int Lua_PlayerSetEdenDamage(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenRange(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenRange());

	return 1;
}

static int Lua_PlayerSetEdenRange(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenRange() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenShotSpeed(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenShotSpeed());

	return 1;
}

static int Lua_PlayerSetEdenShotSpeed(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenShotSpeed() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetEdenLuck(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushnumber(L, *plr->GetEdenLuck());

	return 1;
}

static int Lua_PlayerSetEdenLuck(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetEdenLuck() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerTriggerRoomClear(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	plr->TriggerRoomClear();

	return 0;

}

static int Lua_PlayerShuffleCostumes(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int seed = (int)luaL_checkinteger(L, 2);
	plr->ShuffleCostumes(seed);

	return 0;
}

static int Lua_PlayerGetCollectiblesList(lua_State* L)
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

static int Lua_PlayerGetPurityState(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	lua_pushinteger(L, *plr->GetPurityState());

	return 1;
}

static int Lua_PlayerSetPurityState(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetPurityState() = (int)luaL_checkinteger(L, 2);

	return 0;
}

static int Lua_PlayerSetTearPoisonDamage(lua_State* L) {
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	*plr->GetTearPoisonDamage() = (float)luaL_checknumber(L, 2);

	return 0;
}

static int Lua_PlayerGetVoidedCollectiblesList(lua_State* L)
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

static int Lua_PlayerAddInnateCollectible(lua_State* L)
{
	Entity_Player* plr = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	/*std::set<uint32_t>& itemWispList = *plr->GetItemWispsList();
	itemWispList.insert(luaL_checkinteger(L,2));
	*/

	const int collectibleID = (int) luaL_checkinteger(L, 2);
	const int amount = (int) luaL_optinteger(L, 3, 1);

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

static int Lua_PlayerGetWispCollecitblesList(lua_State* L)
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
	const int pos = (int) luaL_checkinteger(L, 2);
	const int spell = (int) luaL_checkinteger(L, 3);

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
	const int pos = (int) luaL_optinteger(L, 2, 0);

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

	player->SwapForgottenForm(lua_toboolean(L, 2), lua_toboolean(L, 3));
	return 0;
}

LUA_FUNCTION(Lua_SpawnAquariusCreep) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	Entity* castPlayer = (Entity*)player;

	TearParams* params = player->GetTearHitParams(params, 1, (*player->GetTearPoisonDamage() * 0.666f) / player->_damage, (-(int)Isaac::Random(2) != 0) & 2, 0x0);

	Entity_Effect* creep = (Entity_Effect*)g_Game->Spawn(1000, 54, *castPlayer->GetPosition(), Vector(0.0, 0.0), player, 0, Random(), 0);
	creep->_varData = params->_flags;

	Entity* castEffect = (Entity*)creep;
	castEffect->_sprite->_scale *= (Isaac::RandomFloat() * 0.5f) + 0.2f;
	castEffect->_collisionDamage = unkDamage;
	castEffect->SetColor(params->_tearColor, unkDuration, -1, true, false);
	creep->Update();

	lua::luabridge::UserdataPtr::push(L, creep, lua::GetMetatableKey(lua::Metatables::ENTITY_EFFECT));

	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMultiShotParams(state); // should probably move MultiShotParams to its own file, for consistency
	RegisterPocketItem(state);
	lua::Metatables mt = lua::Metatables::ENTITY_PLAYER;
	lua::RegisterFunction(state, mt, "GetMultiShotPositionVelocity", Lua_GetMultiShotPositionVelocity);
	lua::RegisterFunction(state, mt, "InitTwin", Lua_InitTwin);
	lua::RegisterFunction(state, mt, "InitPostLevelInitStats", Lua_InitPostLevelInitStats);
	lua::RegisterFunction(state, mt, "SetItemState", Lua_PlayerSetItemState);
	lua::RegisterFunction(state, mt, "AddCacheFlags", Lua_PlayerAddCacheFlags);
	lua::RegisterFunction(state, mt, "GetHealthType", Lua_PlayerGetHealthType);
	lua::RegisterFunction(state, mt, "GetTotalActiveCharge", Lua_PlayerGetTotalActiveCharge);
	lua::RegisterFunction(state, mt, "GetActiveMaxCharge", Lua_PlayerGetActiveMaxCharge);
	lua::RegisterFunction(state, mt, "GetActiveMinUsableCharge", Lua_PlayerGetActiveMinUsableCharge);
	lua::RegisterFunction(state, mt, "SetActiveVarData", Lua_PlayerSetActiveVarData);
	lua::RegisterFunction(state, mt, "AddActiveCharge", Lua_PlayerAddActiveCharge);
	lua::RegisterFunction(state, mt, "DropCollectible", Lua_PlayerDropCollectible);
	lua::RegisterFunction(state, mt, "IncrementPlayerFormCounter", Lua_PlayerIncrementPlayerFormCounter);
	lua::RegisterFunction(state, mt, "TryPreventDeath", Lua_PlayerTryPreventDeath);
	lua::RegisterFunction(state, mt, "SetCanShoot", Lua_PlayerSetCanShoot);
	lua::RegisterFunction(state, mt, "GetDeadEyeCharge", Lua_PlayerGetDeadEyeCharge);
	lua::RegisterFunction(state, mt, "RemoveCollectibleByHistoryIndex", lua_PlayerRemoveCollectibleByHistoryIndex);
	lua::RegisterFunction(state, mt, "Teleport", Lua_PlayerTeleport);
	lua::RegisterFunction(state, mt, "GetMegaBlastDuration", Lua_PlayerGetMegaBlastDuration);
	lua::RegisterFunction(state, mt, "SetMegaBlastDuration", Lua_PlayerSetMegaBlastDuration);
	lua::RegisterFunction(state, mt, "GetActiveItemDesc", Lua_PlayerGetActiveItemDesc);
	lua::RegisterFunction(state, mt, "TryFakeDeath", Lua_PlayerTryFakeDeath);
	lua::RegisterFunction(state, mt, "GetBagOfCraftingContent", Lua_PlayerGetBoCContent);
	// lua::RegisterFunction(state, mt, "SetBagOfCraftingContent", Lua_PlayerSetBoCContent);
	lua::RegisterFunction(state, mt, "SetBagOfCraftingSlot", Lua_PlayerSetBoCSlot);
	lua::RegisterFunction(state, mt, "GetBagOfCraftingSlot", Lua_PlayerGetBoCSlot);
	lua::RegisterFunction(state, mt, "GetBagOfCraftingOutput", Lua_PlayerGetBagOfCraftingOutput);
	lua::RegisterFunction(state, mt, "SetBagOfCraftingOutput", Lua_PlayerSetBagOfCraftingOutput);
	lua::RegisterFunction(state, mt, "GetFireDelayModifier", Lua_PlayerGetFireDelayModifier);
	lua::RegisterFunction(state, mt, "SetFireDelayModifier", Lua_PlayerSetFireDelayModifier);
	lua::RegisterFunction(state, mt, "GetDamageModifier", Lua_PlayerGetDamageModifier);
	lua::RegisterFunction(state, mt, "SetDamageModifier", Lua_PlayerSetDamageModifier);
	lua::RegisterFunction(state, mt, "GetRedStewBonusDuration", Lua_PlayerGetRedStewBonusDuration);
	lua::RegisterFunction(state, mt, "SetRedStewBonusDuration", Lua_PlayerSetRedStewBonusDuration);
	lua::RegisterFunction(state, mt, "GetWeaponModifiers", Lua_PlayerGetWeaponModifiers);
	lua::RegisterFunction(state, mt, "EnableWeaponType", Lua_PlayerEnableWeaponType);
	lua::RegisterFunction(state, mt, "GetD8DamageModifier", Lua_PlayerGetD8DamageModifier);
	lua::RegisterFunction(state, mt, "GetD8SpeedModifier", Lua_PlayerGetD8SpeedModifier);
	lua::RegisterFunction(state, mt, "GetD8RangeModifier", Lua_PlayerGetD8RangeModifier);
	lua::RegisterFunction(state, mt, "GetD8FireDelayModifier", Lua_PlayerGetD8FireDelayModifier);
	lua::RegisterFunction(state, mt, "GetEpiphoraCharge", Lua_PlayerGetEpiphoraCharge);
	lua::RegisterFunction(state, mt, "GetPeeBurstCooldown", Lua_PlayerGetPeeBurstCooldown);
	lua::RegisterFunction(state, mt, "GetMaxPeeBurstCooldown", Lua_PlayerGetMaxPeeBurstCooldown);
	lua::RegisterFunction(state, mt, "GetMetronomeCollectibleID", Lua_PlayerGetMetronomeCollectibleID);
	lua::RegisterFunction(state, mt, "GetMarkedTarget", Lua_PlayerGetMarkedTarget);
	lua::RegisterFunction(state, mt, "IsLocalPlayer", Lua_PlayerIsLocalPlayer);
	lua::RegisterFunction(state, mt, "GetWildCardItem", Lua_PlayerGetWildCardItem);
	lua::RegisterFunction(state, mt, "GetWildCardItemType", Lua_PlayerGetWildCardItemType);
	lua::RegisterFunction(state, mt, "SetWeapon", Lua_PlayerSetWeapon);
	lua::RegisterFunction(state, mt, "AddLocust", Lua_PlayerAddLocust);
	lua::RegisterFunction(state, mt, "GetPonyCharge", Lua_PlayerGetPonyCharge);
	lua::RegisterFunction(state, mt, "SetPonyCharge", Lua_PlayerSetPonyCharge);
	lua::RegisterFunction(state, mt, "GetEdenSpeed", Lua_PlayerGetEdenSpeed);
	lua::RegisterFunction(state, mt, "SetEdenSpeed", Lua_PlayerSetEdenSpeed);
	lua::RegisterFunction(state, mt, "GetEdenFireDelay", Lua_PlayerGetEdenFireDelay);
	lua::RegisterFunction(state, mt, "SetEdenFireDelay", Lua_PlayerSetEdenFireDelay);
	lua::RegisterFunction(state, mt, "GetEdenDamage", Lua_PlayerGetEdenDamage);
	lua::RegisterFunction(state, mt, "SetEdenDamage", Lua_PlayerSetEdenDamage);
	lua::RegisterFunction(state, mt, "GetEdenRange", Lua_PlayerGetEdenRange);
	lua::RegisterFunction(state, mt, "SetEdenRange", Lua_PlayerSetEdenRange);
	lua::RegisterFunction(state, mt, "GetEdenShotSpeed", Lua_PlayerGetEdenShotSpeed);
	lua::RegisterFunction(state, mt, "SetEdenShotSpeed", Lua_PlayerSetEdenShotSpeed);
	lua::RegisterFunction(state, mt, "GetEdenLuck", Lua_PlayerGetEdenLuck);
	lua::RegisterFunction(state, mt, "SetEdenLuck", Lua_PlayerSetEdenLuck);
	lua::RegisterFunction(state, mt, "TriggerRoomClear", Lua_PlayerTriggerRoomClear);
	lua::RegisterFunction(state, mt, "GetCollectiblesList", Lua_PlayerGetCollectiblesList);
	lua::RegisterFunction(state, mt, "ShuffleCostumes", Lua_PlayerShuffleCostumes);
	lua::RegisterFunction(state, mt, "GetPurityState", Lua_PlayerGetPurityState);
	lua::RegisterFunction(state, mt, "SetPurityState", Lua_PlayerSetPurityState);
	lua::RegisterFunction(state, mt, "SetTearPoisonDamage", Lua_PlayerSetTearPoisonDamage);
	lua::RegisterFunction(state, mt, "GetVoidedCollectiblesList", Lua_PlayerGetVoidedCollectiblesList);
	lua::RegisterFunction(state, mt, "AddInnateCollectible", Lua_PlayerAddInnateCollectible);
	lua::RegisterFunction(state, mt, "GetWispCollectiblesList", Lua_PlayerGetWispCollecitblesList);
	lua::RegisterFunction(state, mt, "GetImmaculateConceptionState", Lua_PlayerGetImmaculateConceptionState);
	lua::RegisterFunction(state, mt, "SetImmaculateConceptionState", Lua_PlayerSetImmaculateConceptionState);
	lua::RegisterFunction(state, mt, "GetCambionConceptionState", Lua_PlayerGetCambionConceptionState);
	lua::RegisterFunction(state, mt, "SetCambionConceptionState", Lua_PlayerSetCambionConceptionState);
	lua::RegisterFunction(state, mt, "UpdateIsaacPregnancy", Lua_PlayerUpdateIsaacPregnancy);
	lua::RegisterFunction(state, mt, "GetCambionPregnancyLevel", Lua_PlayerGetCambionPregnancyLevel);
	lua::RegisterFunction(state, mt, "GetBladderCharge", Lua_PlayerGetBladderCharge);
	lua::RegisterFunction(state, mt, "SetBladderCharge", Lua_PlayerSetBladderCharge);
	lua::RegisterFunction(state, mt, "GetMaxBladderCharge", Lua_PlayerGetMaxBladderCharge);
	lua::RegisterFunction(state, mt, "SetMaxBladderCharge", Lua_PlayerSetMaxBladderCharge);
	lua::RegisterFunction(state, mt, "ClearDeadEyeCharge", Lua_ClearDeadEyeCharge);
	lua::RegisterFunction(state, mt, "IsUrethraBlocked", Lua_PlayerIsUrethraBlocked);
	lua::RegisterFunction(state, mt, "SetUrethraBlock", Lua_PlayerSetUrethraBlock);
	lua::RegisterFunction(state, mt, "GetNextUrethraBlockFrame", Lua_PlayerGetNextUrethraBlockFrame);
	lua::RegisterFunction(state, mt, "SetNextUrethraBlockFrame", Lua_PlayerSetNextUrethraBlockFrame);
	lua::RegisterFunction(state, mt, "GetHeldSprite", Lua_PlayerGetHeldSprite);
	lua::RegisterFunction(state, mt, "GetHeldEntity", Lua_PlayerGetHeldEntity);
	lua::RegisterFunction(state, mt, "GetActiveWeaponNumFired", Lua_PlayerGetActiveWeaponNumFired);
	lua::RegisterFunction(state, mt, "SetPoopSpell", Lua_PlayerSetPoopSpell);
	lua::RegisterFunction(state, mt, "RemovePoopSpell", Lua_PlayerRemovePoopSpell);
	lua::RegisterFunction(state, mt, "GetFlippedForm", Lua_PlayerGetBackupPlayer);
	lua::RegisterFunction(state, mt, "SwapForgottenForm", Lua_SwapForgottenForm);
	lua::RegisterFunction(state, mt, "SpawnAquariusCreep", Lua_SpawnAquariusCreep);
}