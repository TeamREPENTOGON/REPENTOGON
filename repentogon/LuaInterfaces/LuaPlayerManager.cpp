#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

LUA_FUNCTION(Lua_GetPlayerManager) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	PlayerManager** ud = (PlayerManager**)lua_newuserdata(L, sizeof(PlayerManager*));
	*ud = game->GetPlayerManager();
	luaL_setmetatable(L, lua::metatables::PlayerManagerMT);
	return 1;
}

LUA_FUNCTION(Lua_FirstCollectibleOwner)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int collectible = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua::luaL_checkboolean(L, 4);
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, &rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}


LUA_FUNCTION(Lua_AnyoneHasCollectible)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int collectible = (int)luaL_checkinteger(L, 2);
	RNG* rng = new RNG();
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, &rng, true);
	if (!player) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, true);
	}
	return 1;
}

LUA_FUNCTION(Lua_SpawnCoPlayer2)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int playerType = (int)luaL_checkinteger(L, 2);

	Entity_Player* player = playerManager->SpawnCoPlayer2(playerType);
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerIsCoopPlay)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);

	bool result = playerManager->IsCoopPlay();
	lua_pushboolean(L, result);
	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerGetNumCollectibles)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int collectibleID = (int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, playerManager->GetNumCollectibles((CollectibleType)collectibleID));

	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerGetTrinketMultiplier)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int trinketID = (int)luaL_checkinteger(L, 2);
	lua_pushinteger(L, playerManager->GetTrinketMultiplier((TrinketType)trinketID));

	return 1;
}

LUA_FUNCTION(Lua_FirstTrinketOwner)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int trinket = (int)luaL_checkinteger(L, 2);
	RNG* rng = lua::GetUserdata<RNG*>(L, 3, lua::Metatables::RNG, "RNG");
	bool unk = lua::luaL_checkboolean(L, 4);
	Entity_Player* player = playerManager->FirstTrinketOwner((TrinketType)trinket, &rng, unk);
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}

LUA_FUNCTION(Lua_TriggerRoomClear)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	playerManager->TriggerRoomClear();

	return 0;
}

LUA_FUNCTION(Lua_AnyoneHasTrinket)
{
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	int trinket = (int)luaL_checkinteger(L, 2);
	RNG* rng = new RNG();
	Entity_Player* player = playerManager->FirstTrinketOwner((TrinketType)trinket, &rng, true);
	if (!player) {
		lua_pushboolean(L, false);
	}
	else {
		lua_pushboolean(L, true);
	}
	return 1;
}

LUA_FUNCTION(Lua_GetPlayers) {
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);

	std::vector<Entity_Player**> players = playerManager->_playerList;

	lua_newtable(L);

	for (size_t i = 0; i < players.size(); i++) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, players[i], lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_GetEsauJrState) {
	PlayerManager* playerManager = *lua::GetUserdata<PlayerManager**>(L, 1, lua::metatables::PlayerManagerMT);
	const int index = luaL_optinteger(L, 2, 0);

	if (index < 0 || index > 3) {
		return luaL_error(L, "Invalid index %d", index);
	}

	Entity_Player* player = playerManager->_esauJrState[index];
	if (!player) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}

	return 1;
}

static void RegisterPlayerManager(lua_State* L) {
	lua::RegisterFunction(L, lua::Metatables::GAME, "GetPlayerManager", Lua_GetPlayerManager);

	luaL_Reg functions[] = {
		{ "FirstCollectibleOwner", Lua_FirstCollectibleOwner },
		{ "AnyoneHasCollectible", Lua_AnyoneHasCollectible},
		{ "SpawnCoPlayer2", Lua_SpawnCoPlayer2 },
		{ "IsCoopPlay", Lua_PlayerManagerIsCoopPlay},
		{ "GetNumCollectibles", Lua_PlayerManagerGetNumCollectibles},
		{ "GetTotalTrinketMultiplier", Lua_PlayerManagerGetTrinketMultiplier},
		{ "FirstTrinketOwner", Lua_FirstTrinketOwner },
		{ "TriggerRoomClear", Lua_TriggerRoomClear },
		{ "AnyoneHasTrinket", Lua_AnyoneHasTrinket},
		{ "GetPlayers", Lua_GetPlayers},
		{ "GetEsauJrState", Lua_GetEsauJrState},
		{ NULL, NULL }
	};

	lua::RegisterNewClass(L, lua::metatables::PlayerManagerMT, lua::metatables::PlayerManagerMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPlayerManager(_state);
}