#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
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

MOD_EXPORT Entity_Player* L_PlayerManager_FirstCollectibleOwner(int collectible, bool lazSharedGlobalTag) {
	return PlayerManagerQuery::Collectible(collectible).SetLazSharedGlobalTag(lazSharedGlobalTag).GetFirstOwner();
}

MOD_EXPORT bool L_PlayerManager_AnyoneHasCollectible(int collectible, bool ignoreModifiers) {
	return PlayerManagerQuery::Collectible(collectible).SetIgnoreModifiers(ignoreModifiers).AnyoneHasItem();
}

MOD_EXPORT Entity_Player* L_PlayerManager_SpawnCoPlayer2(int playerType) {
	return g_Game->GetPlayerManager()->SpawnCoPlayer2(playerType);
}

MOD_EXPORT bool L_PlayerManager_IsCoopPlay() {
	return g_Game->GetPlayerManager()->IsCoopPlay();
}

MOD_EXPORT int L_PlayerManager_GetNumCollectibles(int collectible, bool ignoreModifiers) {
	return PlayerManagerQuery::Collectible(collectible).SetIgnoreModifiers(ignoreModifiers).GetTotalCount();
}

MOD_EXPORT int L_PlayerManager_GetTotalTrinketMultiplier(int trinket) {
	return PlayerManagerQuery::Trinket(trinket).GetTotalCount();
}

MOD_EXPORT Entity_Player* L_PlayerManager_FirstTrinketOwner(int trinket, bool lazSharedGlobalTag) {
	return PlayerManagerQuery::Trinket(trinket).SetLazSharedGlobalTag(lazSharedGlobalTag).GetFirstOwner();
}

MOD_EXPORT void L_PlayerManager_TriggerRoomClear() {
	g_Game->GetPlayerManager()->TriggerRoomClear();
}

MOD_EXPORT bool L_PlayerManager_AnyoneHasTrinket(int trinket, bool ignoreModifiers) {
	return PlayerManagerQuery::Trinket(trinket).SetIgnoreModifiers(ignoreModifiers).AnyoneHasItem();
}

MOD_EXPORT int L_PlayerManager_GetPlayerCount() {
	return (int)g_Game->GetPlayerManager()->_playerList.size();
}

MOD_EXPORT Entity_Player* L_PlayerManager_GetPlayerAt(int index) {
	return g_Game->GetPlayerManager()->_playerList[index];
}

MOD_EXPORT Entity_Player* L_PlayerManager_GetEsauJrState(int index) {
	return g_Game->GetPlayerManager()->_esauJrState[index];
}

MOD_EXPORT Entity_Player* L_PlayerManager_FirstPlayerByType(unsigned int playerType) {
	return g_Game->GetPlayerManager()->FirstPlayerByType(playerType);
}

MOD_EXPORT Entity_Player* L_PlayerManager_FirstBirthrightOwner() {
	return PlayerManagerQuery::Collectible(COLLECTIBLE_BIRTHRIGHT).SetLazSharedGlobalTag(false).GetFirstOwner();
}

MOD_EXPORT bool L_PlayerManager_AnyPlayerTypeHasBirthright(unsigned int playerType) {
	return PlayerManagerQuery::Collectible(COLLECTIBLE_BIRTHRIGHT).SetLazSharedGlobalTag(false).SetPlayerType(playerType).AnyoneHasItem();
}

MOD_EXPORT bool L_PlayerManager_AnyPlayerTypeHasTrinket(unsigned int playerType, int trinket, bool ignoreModifiers) {
	return PlayerManagerQuery::Trinket(trinket).SetLazSharedGlobalTag(false).SetIgnoreModifiers(ignoreModifiers).SetPlayerType(playerType).AnyoneHasItem();
}

MOD_EXPORT bool L_PlayerManager_AnyPlayerTypeHasCollectible(unsigned int playerType, int collectible, bool ignoreModifiers) {
	return PlayerManagerQuery::Collectible(collectible).SetLazSharedGlobalTag(false).SetIgnoreModifiers(ignoreModifiers).SetPlayerType(playerType).AnyoneHasItem();
}

MOD_EXPORT void L_PlayerManager_SpawnSelectedBaby(int babyType, int controllerIndex) {
	g_Game->GetPlayerManager()->spawn_selected_baby(babyType, controllerIndex);
}

MOD_EXPORT Entity_Player* L_PlayerManager_GetRandomCollectibleOwner(int collectible, unsigned int seed, RNG** rng) {
	Entity_Player* player = PlayerManagerQuery::Collectible((CollectibleType)collectible).SetLazSharedGlobalTag(false).GetRandomOwner(seed);
	*rng = player ? player->GetCollectibleRNG((CollectibleType)collectible) : nullptr;
	return player;
}

MOD_EXPORT Entity_Player* L_PlayerManager_GetRandomTrinketOwner(int trinket, unsigned int seed, RNG** rng) {
	Entity_Player* player = PlayerManagerQuery::Trinket(trinket).SetLazSharedGlobalTag(false).GetRandomOwner(seed);
	*rng = player ? player->GetTrinketRNG((TrinketType)trinket) : nullptr;
	return player;
}

MOD_EXPORT void L_PlayerManager_RemoveCoPlayer(Entity_Player* player) {
	g_Game->GetPlayerManager()->RemoveCoPlayer(player, false);
}
