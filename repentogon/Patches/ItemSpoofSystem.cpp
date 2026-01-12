#include "ItemSpoofSystem.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"

static bool s_isLuaRequest = false;
static std::bitset<CollectibleType::NUM_COLLECTIBLES> s_reworkedCollectibles;
static std::bitset<ePlayerType::NUM_PLAYER_TYPES> s_reworkedBirthrights;
static std::bitset<TrinketType::NUM_TRINKETS> s_reworkedTrinkets;

void ItemSpoofSystem::StartLuaRequest()
{
    s_isLuaRequest = true;
}

void ItemSpoofSystem::EndLuaRequest()
{
    s_isLuaRequest = false;
}

void ItemSpoofSystem::ReworkCollectible(int collectible)
{
    assert(CollectibleType::COLLECTIBLE_NULL < collectible && collectible < CollectibleType::NUM_COLLECTIBLES);
    s_reworkedCollectibles.set(collectible, true);
}

void ItemSpoofSystem::ReworkBirthright(int playerType)
{
    assert(0 <= playerType && playerType < ePlayerType::NUM_PLAYER_TYPES);
    s_reworkedBirthrights.set(playerType, true);
}

void ItemSpoofSystem::ReworkTrinket(int trinket)
{
    assert(TrinketType::TRINKET_NULL < trinket && trinket < TrinketType::NUM_TRINKETS);
    s_reworkedTrinkets.set(trinket, true);
}

static bool is_reworked_collectible(int collectible, int playerType)
{
    if (collectible == CollectibleType::COLLECTIBLE_BIRTHRIGHT)
    {
        if (!(0 <= playerType && playerType < ePlayerType::NUM_PLAYER_TYPES))
        {
            return false;
        }

        return s_reworkedBirthrights.test(playerType);
    }

    if (!(CollectibleType::COLLECTIBLE_NULL < collectible && collectible < CollectibleType::NUM_COLLECTIBLES))
    {
        return false;
    }


    return s_reworkedCollectibles.test(collectible);
}

static bool is_reworked_trinket(int trinket)
{
    if (!(TrinketType::TRINKET_NULL < trinket && trinket < TrinketType::NUM_TRINKETS))
    {
        return false;
    }

    return s_reworkedTrinkets.test(trinket);
}

HOOK_METHOD_PRIORITY(Entity_Player, HasCollectible, -9999, (int collectible, bool ignoreModifiers) -> bool)
{
    if (!s_isLuaRequest && is_reworked_collectible(collectible, this->_playerType))
    {
        return false;
    }

    bool isLuaRequest = s_isLuaRequest;
    bool result = super(collectible, ignoreModifiers);
    s_isLuaRequest = isLuaRequest;
    return result;
};

HOOK_METHOD_PRIORITY(Entity_Player, GetCollectibleNum, -9999, (int collectible, bool ignoreModifiers) -> int)
{
    if (!s_isLuaRequest && is_reworked_collectible(collectible, this->_playerType))
    {
        return false;
    }

    bool isLuaRequest = s_isLuaRequest;
    int result = super(collectible, ignoreModifiers);
    s_isLuaRequest = isLuaRequest;
    return result;
};

HOOK_METHOD_PRIORITY(Entity_Player, HasTrinket, -9999, (unsigned int trinket, bool ignoreModifiers) -> bool)
{
    if (!s_isLuaRequest && is_reworked_trinket(trinket))
    {
        return false;
    }

    bool isLuaRequest = s_isLuaRequest;
    bool result = super(trinket, ignoreModifiers);
    s_isLuaRequest = isLuaRequest;
    return result;
};

HOOK_METHOD_PRIORITY(Entity_Player, GetTrinketMultiplier, -9999, (unsigned int trinket) -> int)
{
    if (!s_isLuaRequest && is_reworked_trinket(trinket))
    {
        return false;
    }

    bool isLuaRequest = s_isLuaRequest;
    int result = super(trinket);
    s_isLuaRequest = isLuaRequest;
    return result;
};