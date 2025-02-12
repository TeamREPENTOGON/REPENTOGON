#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"

/*LUA_FUNCTION(Lua_GetPlayerManager) {
	Game* game = lua::GetRawUserdata<Game*>(L, 1, lua::Metatables::GAME, "Game");
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
	Entity_Player* player = playerManager->FirstCollectibleOwner((CollectibleType)collectible, nullptr, true);
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

// FirstTrinketOwner was changed to just return a boolean in rep+ (meaning it was actually replaced by AnyoneHasTrinket).
// This was a reasonable refactor for nicalis to do considering it doesn't seem like anything in the game ever used the returned player.
// I mean honestly, who specifically and only wants the arbitrarily FIRST player with the trinket?
// But the other similar functions like FirstCollectibleOwner were kept so, uh, erm
// Well, in any case joke's on us, we exposed this as an API function, we can't get rid of it! Someone out there is using it!!
// So, anyway, here's a reimplementation.
LUA_FUNCTION(Lua_FirstTrinketOwner)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	const int trinket = (int)luaL_checkinteger(L, 1);

	ItemConfig_Item* item = g_Manager->GetItemConfig()->GetTrinket(trinket);

	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	// Sorry for this weird backwards compatability thing, I want to pretend that the rng param never existed.
	// Everything below should work as expected. I even wrote unit tests!!
	// - FirstTrinketOwner(t, nil, bool) <- backwards compat
	// - FirstTrinketOwner(t, rng, bool) <- backwards compat
	// - FirstTrinketOwner(t, bool) <- new!
	// - FirstTrinketOwner(t) <- still fine
	bool lazSharedGlobalTag = true;

	const bool hasLegacyRngParam = lua_type(L, 2) == LUA_TUSERDATA && lua::TestUserdata(L, 2, lua::Metatables::RNG) != nullptr;
	const bool hasNil = lua_type(L, 2) == LUA_TNIL;
	if (!hasLegacyRngParam && !hasNil) {
		lazSharedGlobalTag = lua::luaL_optboolean(L, 2, true);
	} else if (lua_gettop(L) > 2 && lua_type(L, 3) == LUA_TBOOLEAN) {
		lazSharedGlobalTag = lua::luaL_checkboolean(L, 3);
	}
	
	const bool checkBackupPlayer = lazSharedGlobalTag ? ((item->tags & 0x80000000) != 0) : false;

	Entity_Player* firstTrinketOwner = nullptr;

	for (Entity_Player* player : playerManager->_playerList) {
		if (player->GetTrinketMultiplier(trinket) > 0) {
			firstTrinketOwner = player;
			break;
		}
		if (checkBackupPlayer && player->_backupPlayer && player->_backupPlayer->GetTrinketMultiplier(trinket) > 0) {
			firstTrinketOwner = player->_backupPlayer;
			break;
		}
	}

	if (!firstTrinketOwner) {
		lua_pushnil(L);
	}
	else {
		lua::luabridge::UserdataPtr::push(L, firstTrinketOwner, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
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
	const int trinket = (int)luaL_checkinteger(L, 1);
	lua_pushboolean(L, playerManager->AnyoneHasTrinket((TrinketType)trinket));
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

LUA_FUNCTION(Lua_AnyPlayerTypeHasTrinket)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	int trinket = (int)luaL_checkinteger(L, 2);
	bool ignoreModifiers = lua::luaL_optboolean(L, 3, false);

	for (Entity_Player* player : g_Game->_playerManager._playerList)
	{
		if (player->HasTrinket(trinket, ignoreModifiers) && player->GetPlayerType() == playerType)
		{
			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushboolean(L, false);

	return 1;
}

LUA_FUNCTION(Lua_AnyPlayerTypeHasCollectible)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	int collectible = (int)luaL_checkinteger(L, 2);
	bool ignoreModifiers = lua::luaL_optboolean(L, 3, false);

	for (Entity_Player* player : g_Game->_playerManager._playerList)
	{
		if (player->HasCollectible(collectible, ignoreModifiers) && player->GetPlayerType() == playerType)
		{
			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushboolean(L, false);

	return 1;
}

LUA_FUNCTION(Lua_SpawnSelectedBaby) 
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	const int babyType = (int)luaL_checkinteger(L, 1);
	const int controllerIndex = (int)luaL_checkinteger(L, 2);

	playerManager->spawn_selected_baby(babyType, controllerIndex);

	return 0;
}

LUA_FUNCTION(Lua_RemoveCoPlayer)
{
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	auto* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 1, lua::Metatables::ENTITY_PLAYER, "Entity_Player");

	playerManager->RemoveCoPlayer(player, false);
	return 0;
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
		lua::TableAssoc(L, "AnyPlayerTypeHasTrinket", Lua_AnyPlayerTypeHasTrinket);
		lua::TableAssoc(L, "AnyPlayerTypeHasCollectible", Lua_AnyPlayerTypeHasCollectible);
		lua::TableAssoc(L, "SpawnSelectedBaby", Lua_SpawnSelectedBaby);
		lua::TableAssoc(L, "RemoveCoPlayer", Lua_RemoveCoPlayer);

		lua_setglobal(L, "PlayerManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPlayerManager(_state);
}