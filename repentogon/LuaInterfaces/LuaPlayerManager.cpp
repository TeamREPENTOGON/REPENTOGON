#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetPlayerManager) {
	Game* game = lua::GetUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
	PlayerManager** ud = (PlayerManager**)lua_newuserdata(L, sizeof(PlayerManager*));
	*ud = game->GetPlayerManager();
	luaL_setmetatable(L, lua::metatables::PlayerManagerMT);
	return 1;
}
*/

LUA_FUNCTION(Lua_FirstCollectibleOwner)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int collectible = (int)luaL_checkinteger(L, 1);
	bool lazSharedGlobalTag = lua::luaL_optboolean(L, 2, true);
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, nullptr, lazSharedGlobalTag);
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
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int collectible = (int)luaL_checkinteger(L, 1);
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
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int playerType = (int)luaL_checkinteger(L, 1);

	Entity_Player* player = playerManager->SpawnCoPlayer2(playerType);
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerIsCoopPlay)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();

	bool result = playerManager->IsCoopPlay();
	lua_pushboolean(L, result);
	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerGetNumCollectibles)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int collectibleID = (int)luaL_checkinteger(L, 1);
	lua_pushinteger(L, playerManager->GetNumCollectibles((CollectibleType)collectibleID));

	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerGetTrinketMultiplier)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int trinketID = (int)luaL_checkinteger(L, 1);
	lua_pushinteger(L, playerManager->GetTrinketMultiplier((TrinketType)trinketID));

	return 1;
}

LUA_FUNCTION(Lua_FirstTrinketOwner)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int trinket = (int)luaL_checkinteger(L, 1);
	RNG* rng = nullptr;
	if (lua_type(L, 2) == LUA_TUSERDATA) {
		rng = lua::GetUserdata<RNG*>(L, 2, lua::Metatables::RNG, "RNG");
	}

	bool lazSharedGlobalTag = lua::luaL_optboolean(L, 3, true);
	Entity_Player* player = playerManager->FirstTrinketOwner((TrinketType)trinket, &rng, lazSharedGlobalTag);
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
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	playerManager->TriggerRoomClear();

	return 0;
}

LUA_FUNCTION(Lua_AnyoneHasTrinket)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	int trinket = (int)luaL_checkinteger(L, 1);
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
	PlayerManager* playerManager = g_Game->GetPlayerManager();

	std::vector<Entity_Player*>& players = playerManager->_playerList;

	lua_newtable(L);

	for (size_t i = 0; i < players.size(); i++) {
		lua_pushinteger(L, i + 1);
		lua::luabridge::UserdataPtr::push(L, players[i], lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		lua_rawset(L, -3);
	}

	return 1;
}

LUA_FUNCTION(Lua_GetEsauJrState) {
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	const int index = (int)luaL_optinteger(L, 1, 0);

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

LUA_FUNCTION(Lua_FirstPlayerByType)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	Entity_Player* player = playerManager->FirstPlayerByType(playerType);
	
	player ? lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER)) : lua_pushnil(L);

	return 1;
}

LUA_FUNCTION(Lua_AnyoneIsPlayerType)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	Entity_Player* player = playerManager->FirstPlayerByType(playerType);

	lua_pushboolean(L, player ? true : false);

	return 1;
}

LUA_FUNCTION(Lua_FirstBirthrightOwner)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	Entity_Player* player = playerManager->FirstBirthrightOwner(playerType);

	player ? lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER)) : lua_pushnil(L);

	return 1;
}

LUA_FUNCTION(Lua_AnyPlayerTypeHasBirthright)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	Entity_Player* player = playerManager->FirstBirthrightOwner(playerType);

	lua_pushboolean(L, player ? true : false);

	return 1;
}

static void RegisterPlayerManager(lua_State* L) {
	//lua::RegisterFunction(L, lua::Metatables::GAME, "GetPlayerManager", Lua_GetPlayerManager);
	lua_newtable(L);
		lua::TableAssoc(L, "FirstCollectibleOwner", Lua_FirstCollectibleOwner );
		lua::TableAssoc(L, "AnyoneHasCollectible", Lua_AnyoneHasCollectible);
		lua::TableAssoc(L, "SpawnCoPlayer2", Lua_SpawnCoPlayer2 );
		lua::TableAssoc(L, "IsCoopPlay", Lua_PlayerManagerIsCoopPlay);
		lua::TableAssoc(L, "GetNumCollectibles", Lua_PlayerManagerGetNumCollectibles);
		lua::TableAssoc(L, "GetTotalTrinketMultiplier", Lua_PlayerManagerGetTrinketMultiplier);
		lua::TableAssoc(L, "FirstTrinketOwner", Lua_FirstTrinketOwner );
		lua::TableAssoc(L, "TriggerRoomClear", Lua_TriggerRoomClear );
		lua::TableAssoc(L, "AnyoneHasTrinket", Lua_AnyoneHasTrinket);
		lua::TableAssoc(L, "GetPlayers", Lua_GetPlayers);
		lua::TableAssoc(L, "GetEsauJrState", Lua_GetEsauJrState);
		lua::TableAssoc(L, "FirstPlayerByType", Lua_FirstPlayerByType);
		lua::TableAssoc(L, "AnyoneIsPlayerType", Lua_AnyoneIsPlayerType);
		lua::TableAssoc(L, "FirstBirthrightOwner", Lua_FirstBirthrightOwner);
		lua::TableAssoc(L, "AnyPlayerTypeHasBirthright", Lua_AnyPlayerTypeHasBirthright);
		

		lua_setglobal(L, "PlayerManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPlayerManager(_state);
}