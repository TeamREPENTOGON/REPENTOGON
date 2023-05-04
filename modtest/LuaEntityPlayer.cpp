#include <lua.hpp>
#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MultiShotParamsMT = "MultiShotParams";

std::vector<int> fakeItems;

int Lua_GetMultiShotPositionVelocity(lua_State* L) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY, "EntityPlayer");
	int loopIndex = luaL_checkinteger(L, 2);
	int weaponType = luaL_checkinteger(L, 3);
	Vector* shotDirection = lua::GetUserdata<Vector*>(L, 1, lua::Metatables::ENTITY, "Vector");
	float shotSpeed = luaL_checknumber(L, 5);

	Weapon_MultiShotParams* multiShotParams = lua::GetUserdata<Weapon_MultiShotParams*>(L, 6, MultiShotParamsMT);
	if (multiShotParams->numTears < loopIndex) {
		luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.NumTears");
	};

	lua::luabridge::UserdataValue<PosVel>::push(L, lua::GetMetatableKey(lua::Metatables::POS_VEL), player->GetMultiShotPositionVelocity(loopIndex, (WeaponType)weaponType, *shotDirection, shotSpeed, *multiShotParams));

	return 1;
}

static int Lua_GetMultiShotParams(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int weaponType = luaL_checkinteger(L, 2);
	Weapon_MultiShotParams* ud = (Weapon_MultiShotParams*)lua_newuserdata(L, sizeof(Weapon_MultiShotParams));
	*ud = player->GetMultiShotParams((WeaponType)weaponType);
	luaL_setmetatable(L, MultiShotParamsMT);
	return 1;
}

static int Lua_MultiShotParamsGetNumTears(lua_State* L) {
	Weapon_MultiShotParams* params = lua::GetUserdata<Weapon_MultiShotParams*>(L, 1, MultiShotParamsMT);

	lua_pushinteger(L, params->numTears);

	return 1;
};

static void RegisterMultiShotParams(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetMultiShotParams");
	lua_pushcfunction(L, Lua_GetMultiShotParams);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, MultiShotParamsMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	lua_pushstring(L, "GetNumTears");
	lua_pushcfunction(L, Lua_MultiShotParamsGetNumTears);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

int Lua_InitTwin(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int playerType = luaL_checkinteger(L, 2);

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
	int flags = luaL_checkinteger(L, 2);
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
	int slot = luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetTotalActiveCharge(slot));
	return 1;
}

int Lua_PlayerGetActiveMaxCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int item = luaL_checkinteger(L, 2);
	int vardata = luaL_checkinteger(L, 3);

	lua_pushinteger(L, player->GetActiveMaxCharge(item,vardata));
	return 1;
}

int Lua_PlayerGetActiveMinUsableCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int slot = luaL_checkinteger(L, 2);

	lua_pushinteger(L, player->GetActiveMinUsableCharge(slot));
	return 1;
}

int Lua_PlayerSetActiveVarData(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int vardata = luaL_checkinteger(L, 2);
	int slot = luaL_checkinteger(L, 3);

	player->SetActiveVarData(vardata, slot);
	return 0;
}

int Lua_PlayerAddActiveCharge(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	unsigned int charge = luaL_checkinteger(L, 2);
	int slot = luaL_checkinteger(L, 3);
	bool unknown = lua_toboolean(L, 4);
	bool overcharge = lua_toboolean(L, 5);
	bool force = lua_toboolean(L, 6);

	int ret = player->AddActiveCharge(charge, slot, unknown, overcharge, force);
	lua_pushinteger(L, ret);

	return 1;
}

int Lua_PlayerDropCollectible(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int collectible = luaL_checkinteger(L, 2);

	player->DropCollectible(collectible, 0, false);
	return 0;
}

int Lua_PlayerIncrementPlayerFormCounter(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int ePlayerForm = luaL_checkinteger(L, 2);
	int num = luaL_checkinteger(L, 3);

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
	int index = luaL_checknumber(L, 2);
	player->RemoveCollectibleByHistoryIndex(index);
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
	*player->GetMegaBlastDuration() = luaL_checkinteger(L,2);
	return 0;
}

int Lua_PlayerGetActiveItemDesc(lua_State* L)
{
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int index = luaL_optinteger(L, 2, 0);
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
	return 0;
}

int Lua_PlayerGetBoCSlot(lua_State* L) {
	Entity_Player* player = lua::GetUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");
	int n = lua_gettop(L);
	if (n != 2) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: expected 1 parameter, got %d\n", n - 1);
	}

	int slot = luaL_checkinteger(L, 2);
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

	int slot = luaL_checkinteger(L, 2);
	if (slot < 0 || slot > 7) {
		return luaL_error(L, "EntityPlayer::GetBagOfCraftingSlot: invalid slot id %d\n", slot);
	}

	int8_t pickup = luaL_optinteger(L, 3, BagOfCraftingPickup::BOC_NONE);
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
	*player->GetBagOfCraftingOutput() = luaL_checkinteger(L, 2);
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
	*player->GetFireDelayModifier() = luaL_checkinteger(L, 2);
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
	*player->GetDamageModifier() = luaL_checkinteger(L, 2);
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
	*player->GetRedStewBonusDuration() = luaL_checkinteger(L, 2);
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
	int weaponType = luaL_checkinteger(L, 2);
	bool set = lua_toboolean(L, 3);
	player->EnableWeaponType((WeaponType)weaponType, set);
	return 0;
}


HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterMultiShotParams(state); // should probably move MultiShotParams to its own file, for consistency
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
}