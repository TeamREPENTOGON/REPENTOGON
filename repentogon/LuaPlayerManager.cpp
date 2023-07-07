#include <lua.hpp>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

static constexpr const char* PlayerManagerMT = "PlayerManager";

static int Lua_GetPlayerManager(lua_State* L) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	PlayerManager** ud = (PlayerManager**)lua_newuserdata(L, sizeof(PlayerManager*));
	*ud = game->GetPlayerManager();
	luaL_setmetatable(L, PlayerManagerMT);
	return 1;
}

int Lua_FirstCollectibleOwner(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int collectible = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua_toboolean(L, 4);
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, &rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}


int Lua_AnyoneHasCollectible(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int collectible = (int)luaL_checkinteger(L, 2);
	RNG* rng = new RNG(); 
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, &rng, true);
	if (!player) {
		lua_pushboolean(L,false);
	}
	else {
		lua_pushboolean(L, true);
	}
	return 1;
}

int Lua_SpawnCoPlayer2(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* player = playerManager->SpawnCoPlayer2(playerType);
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

int Lua_PlayerManagerIsCoopPlay(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);

	bool result = playerManager->IsCoopPlay();
	lua_pushboolean(L, result);
	return 1;
}

int Lua_PlayerManagerGetNumCollectibles(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int collectibleID = (int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, playerManager->GetNumCollectibles((CollectibleType)collectibleID));

	return 1;
}

int Lua_PlayerManagerGetTrinketMultiplier(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int trinketID = (int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, playerManager->GetTrinketMultiplier((TrinketType)trinketID));

	return 1;
}

int Lua_FirstTrinketOwner(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	int trinket = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua_toboolean(L, 4);
	Entity_Player* player = playerManager->FirstTrinketOwner((TrinketType)trinket, &rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

static int Lua_TriggerRoomClear(lua_State* L)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, PlayerManagerMT);
	playerManager->TriggerRoomClear();
	
	return 0;
}

static void RegisterPlayerManager(lua_State* L) {
	lua::PushMetatable(L, lua::Metatables::GAME);
	lua_pushstring(L, "GetPlayerManager");
	lua_pushcfunction(L, Lua_GetPlayerManager);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, PlayerManagerMT);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);

	luaL_Reg functions[] = {
		{ "FirstCollectibleOwner", Lua_FirstCollectibleOwner },
		{ "AnyoneHasCollectible", Lua_AnyoneHasCollectible},
		{ "SpawnCoPlayer2", Lua_SpawnCoPlayer2 },
		{ "IsCoopPlay", Lua_PlayerManagerIsCoopPlay},
		{ "GetNumCollectibles", Lua_PlayerManagerGetNumCollectibles},
		{ "GetTotalTrinketMultiplier", Lua_PlayerManagerGetTrinketMultiplier},
		{ "FirstTrinketOwner", Lua_FirstTrinketOwner },
		{ "TriggerRoomClear", Lua_TriggerRoomClear },
		{ NULL, NULL }
	};

	luaL_setfuncs(L, functions, 0);
	lua_pop(L, 1);

}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterPlayerManager(state);
}