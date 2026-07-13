#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "../Patches/ItemSpoofSystem.h"

// Helper class that acts as a reimplementation for AnyoneHasCollectible, FirstTrinketOwner, etc.
// The primary purpose of doing this is to make sure that "Reworked" collectibles are properly respected.
class PlayerManagerQuery{
private:
	PlayerManagerQuery(int id, bool isTrinket) : _id(id), _isTrinket(isTrinket) {};

public:
	PlayerManagerQuery() = delete;

	static PlayerManagerQuery Collectible(int collectible) {
		return PlayerManagerQuery(collectible, false);
	}

	static PlayerManagerQuery Trinket(int trinket) {
		return PlayerManagerQuery(trinket, true);
	}

	PlayerManagerQuery& SetLazSharedGlobalTag(const bool lazSharedGlobalTag) {
		_lazSharedGlobalTag = lazSharedGlobalTag;
		return *this;
	}

	PlayerManagerQuery& SetIgnoreModifiers(const bool ignoreModifiers) {
		_ignoreModifiers = ignoreModifiers;
		return *this;
	}

	PlayerManagerQuery& SetPlayerType(const uint32_t playerType) {
		_playerType = playerType;
		return *this;
	}

	ItemConfig_Item* GetItem() const {
		if (_isTrinket) {
			return g_Manager->GetItemConfig()->GetTrinket(_id);
		}
		return g_Manager->GetItemConfig()->GetCollectible(_id);
	}

	bool ShouldCheckBackupPlayer() const {
		if (!_lazSharedGlobalTag) {
			return false;
		}
		ItemConfig_Item* item = GetItem();
		if (!item) {
			return false;
		}
		return (item->tags & 0x80000000) != 0;
	}

	bool PlayerHasItem(Entity_Player* player) const {
		if (player->_variant == 0 && (!_playerType || player->_playerType == *_playerType)) {
			ItemSpoofSystem::StartLuaRequest();
			if (!_isTrinket) {
				return player->HasCollectible(_id, _ignoreModifiers);
			} else if (_ignoreModifiers) {
				return player->HasTrinket(_id, true);
			} else {
				return player->GetTrinketMultiplier(_id) > 0;
			}
		}
		return false;
	}

	int GetCountForPlayer(Entity_Player* player) const {
		if (!_playerType || player->_playerType == *_playerType) {
			ItemSpoofSystem::StartLuaRequest();
			if (_isTrinket) {
				return player->GetTrinketMultiplier(_id);
			}
			return player->GetCollectibleNum(_id, _ignoreModifiers);
		}
		return 0;
	}

	std::vector<Entity_Player*> GetOwners() const {
		std::vector<Entity_Player*> owners;
		
		for (Entity_Player* player : g_Game->GetPlayerManager()->_playerList) {
			if (PlayerHasItem(player)) {
				owners.push_back(player);
			}
			if (player->_backupPlayer && ShouldCheckBackupPlayer() && PlayerHasItem(player->_backupPlayer)) {
				owners.push_back(player->_backupPlayer);
			}
		}

		return owners;
	}

	Entity_Player* GetFirstOwner() const {
		std::vector<Entity_Player*> owners = GetOwners();
		if (owners.empty()) {
			return nullptr;
		}
		return owners[0];
	}

	Entity_Player* GetRandomOwner(const uint32_t seed) const {
		std::vector<Entity_Player*> owners = GetOwners();
		if (owners.empty()) {
			return nullptr;
		}
		RNG rng;
		rng.SetSeed((seed > 0u) ? seed : 1u, 35);
		return owners[rng.RandomInt(owners.size())];
	}

	bool AnyoneHasItem() const {
		return GetFirstOwner() != nullptr;
	}

	int GetTotalCount() const {
		int count = 0;

		for (Entity_Player* player : g_Game->GetPlayerManager()->_playerList) {
			count += GetCountForPlayer(player);
			if (player->_backupPlayer && ShouldCheckBackupPlayer()) {
				count += GetCountForPlayer(player->_backupPlayer);
			}
		}

		return count;
	}

private:
	int _id;
	bool _isTrinket;
	bool _lazSharedGlobalTag = true;
	bool _ignoreModifiers = false;
	std::optional<uint32_t> _playerType = std::nullopt;
};

LUA_FUNCTION(Lua_FirstCollectibleOwner)
{
	int collectible = (int)luaL_checkinteger(L, 1);
	bool lazSharedGlobalTag = lua::luaL_optboolean(L, 2, true);

	Entity_Player* player = PlayerManagerQuery::Collectible(collectible).SetLazSharedGlobalTag(lazSharedGlobalTag).GetFirstOwner();
	if (!player) {
		lua_pushnil(L);
	} else {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	}
	return 1;
}


LUA_FUNCTION(Lua_AnyoneHasCollectible)
{
	int collectible = (int)luaL_checkinteger(L, 1);
	bool ignoreModifiers = lua::luaL_optboolean(L, 2, false);
	lua_pushboolean(L, PlayerManagerQuery::Collectible(collectible).SetIgnoreModifiers(ignoreModifiers).AnyoneHasItem());
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
	int collectibleID = (int)luaL_checkinteger(L, 1);
	bool ignoreModifiers = lua::luaL_optboolean(L, 2, false);
	lua_pushinteger(L, PlayerManagerQuery::Collectible(collectibleID).SetIgnoreModifiers(ignoreModifiers).GetTotalCount());
	return 1;
}

LUA_FUNCTION(Lua_PlayerManagerGetTrinketMultiplier)
{
	int trinketID = (int)luaL_checkinteger(L, 1);
	lua_pushinteger(L, PlayerManagerQuery::Trinket(trinketID).GetTotalCount());
	return 1;
}

LUA_FUNCTION(Lua_FirstTrinketOwner)
{
	const int trinket = (int)luaL_checkinteger(L, 1);

	// Sorry for this weird backwards compatability thing, I want to pretend that the old rng param never existed.
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

	Entity_Player* firstTrinketOwner = PlayerManagerQuery::Trinket(trinket).SetLazSharedGlobalTag(lazSharedGlobalTag).GetFirstOwner();
	if (firstTrinketOwner) {
		lua::luabridge::UserdataPtr::push(L, firstTrinketOwner, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	} else {
		lua_pushnil(L);
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
	const int trinket = (int)luaL_checkinteger(L, 1);
	bool ignoreModifiers = lua::luaL_optboolean(L, 2, false);
	lua_pushboolean(L, PlayerManagerQuery::Trinket(trinket).SetIgnoreModifiers(ignoreModifiers).AnyoneHasItem());
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
	
	lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));

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
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	Entity_Player* player = PlayerManagerQuery::Collectible(COLLECTIBLE_BIRTHRIGHT).SetLazSharedGlobalTag(false).GetFirstOwner();
	if (player) {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(Lua_AnyPlayerTypeHasBirthright)
{
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	lua_pushboolean(L, PlayerManagerQuery::Collectible(COLLECTIBLE_BIRTHRIGHT).SetLazSharedGlobalTag(false).SetPlayerType(playerType).AnyoneHasItem());
	return 1;
}

LUA_FUNCTION(Lua_AnyPlayerTypeHasTrinket)
{
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	int trinket = (int)luaL_checkinteger(L, 2);
	bool ignoreModifiers = lua::luaL_optboolean(L, 3, false);

	lua_pushboolean(L, PlayerManagerQuery::Trinket(trinket).SetLazSharedGlobalTag(false).SetIgnoreModifiers(ignoreModifiers).SetPlayerType(playerType).AnyoneHasItem());
	return 1;
}

LUA_FUNCTION(Lua_AnyPlayerTypeHasCollectible)
{
	unsigned int playerType = (unsigned int)luaL_checkinteger(L, 1);
	int collectible = (int)luaL_checkinteger(L, 2);
	bool ignoreModifiers = lua::luaL_optboolean(L, 3, false);

	lua_pushboolean(L, PlayerManagerQuery::Collectible(collectible).SetLazSharedGlobalTag(false).SetIgnoreModifiers(ignoreModifiers).SetPlayerType(playerType).AnyoneHasItem());
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

LUA_FUNCTION(Lua_GetRandomCollectibleOwner)
{
	CollectibleType collectibleType = (CollectibleType)luaL_checkinteger(L, 1);
	uint32_t seed = (uint32_t)luaL_checkinteger(L, 2);

	Entity_Player* player = PlayerManagerQuery::Collectible(collectibleType).SetLazSharedGlobalTag(false).GetRandomOwner(seed);
	if (player) {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		lua::luabridge::UserdataPtr::push(L, player->GetCollectibleRNG(collectibleType), lua::GetMetatableKey(lua::Metatables::RNG));
	} else {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	return 2;

}

LUA_FUNCTION(Lua_GetRandomTrinketOwner)
{
	TrinketType trinketType = (TrinketType)luaL_checkinteger(L, 1);
	uint32_t seed = (uint32_t)luaL_checkinteger(L, 2);

	Entity_Player* player = PlayerManagerQuery::Trinket(trinketType).SetLazSharedGlobalTag(false).GetRandomOwner(seed);
	if (player) {
		lua::luabridge::UserdataPtr::push(L, player, lua::GetMetatableKey(lua::Metatables::ENTITY_PLAYER));
		lua::luabridge::UserdataPtr::push(L, player->GetTrinketRNG(trinketType), lua::GetMetatableKey(lua::Metatables::RNG));
	} else {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	return 2;

}


static void RegisterPlayerManager(lua_State* L) {
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
		lua::TableAssoc(L, "GetRandomCollectibleOwner", Lua_GetRandomCollectibleOwner);
		lua::TableAssoc(L, "GetRandomTrinketOwner", Lua_GetRandomTrinketOwner);

		lua_setglobal(L, "PlayerManager");
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	RegisterPlayerManager(_state);
}