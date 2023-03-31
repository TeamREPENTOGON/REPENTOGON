#include <lua.hpp>
#include <algorithm>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* MultiShotParamsMT = "MultiShotParams";

std::vector<int> fakeItems;

int Lua_GetMultiShotPositionVelocity(lua_State* L) // This *should* be in the API, but magically vanished some point after 1.7.8.
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int loopIndex = luaL_checkinteger(L, 2);
	WeaponType weaponType = (WeaponType)luaL_checkinteger(L, 3);
	Vector* shotDirection = *(Vector**)((char*)lua::CheckUserdata(L, 4, lua::Metatables::VECTOR, "Vector") + 4);
	float shotSpeed = luaL_checknumber(L, 5);
	Weapon_MultiShotParams* multiShotParams = lua::GetUserdata<Weapon_MultiShotParams*>(L, 6, MultiShotParamsMT);
	if (multiShotParams->numTears < loopIndex) {
		luaL_argerror(L, 2, "LoopIndex cannot be higher than MultiShotParams.NumTears");
	};

	PosVel* toLua = lua::luabridge::UserdataValue<PosVel>::place(L, lua::GetMetatableKey(lua::Metatables::POS_VEL));
	*toLua = player->GetMultiShotPositionVelocity(loopIndex, weaponType, *shotDirection, shotSpeed, *multiShotParams);

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
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int playerType = luaL_checkinteger(L, 2);

	Entity_Player* toLua = lua::luabridge::UserdataValue<Entity_Player>::place(L, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	toLua = player->InitTwin(playerType);

	return 1;
}

int Lua_InitPostLevelInitStats(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	player->InitPostLevelInitStats();

	return 0;
}

int Lua_PlayerSetItemState(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
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
}