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

static void RegisterMultiShotPositionVelocity(lua_State* L)
{
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "GetMultiShotPositionVelocity");
	lua_pushcfunction(L, Lua_GetMultiShotPositionVelocity);
	lua_rawset(L, -3);
	lua_pop(L, 1);
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

	lua_pop(L, 2);
}

int Lua_InitTwin(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int playerType = luaL_checkinteger(L, 2);

	Entity_Player* toLua = lua::luabridge::UserdataValue<Entity_Player>::place(L, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	toLua = player->InitTwin(playerType);

	return 1;
}

static void RegisterInitTwin(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "InitTwin");
	lua_pushcfunction(L, Lua_InitTwin);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_InitPostLevelInitStats(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	player->InitPostLevelInitStats();

	return 1;
}

static void RegisterInitPostLevelInitStats(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "InitPostLevelInitStats");
	lua_pushcfunction(L, Lua_InitPostLevelInitStats);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

int Lua_PlayerSetItemState(lua_State* L)
{
	Entity_Player* player = *(Entity_Player**)((char*)lua::CheckUserdata(L, 1, lua::Metatables::ENTITY_PLAYER, "EntityPlayer") + 4);
	int item = luaL_checkinteger(L, 2);

	player->SetItemState(item);

	return 0;
}

static void RegisterPlayerSetItemState(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "SetItemState");
	lua_pushcfunction(L, Lua_PlayerSetItemState);
	lua_rawset(L, -3);
	lua_pop(L, 1);
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

static void RegisterNewAddCacheFlags(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::ENTITY_PLAYER);
	lua_pushstring(L, "AddCacheFlags");
	lua_pushcfunction(L, Lua_PlayerAddCacheFlags);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	RegisterMultiShotPositionVelocity(state);
	RegisterMultiShotParams(state);
	RegisterInitTwin(state);
	RegisterInitPostLevelInitStats(state);
	RegisterPlayerSetItemState(state);
	RegisterNewAddCacheFlags(state);
}