#pragma once

#include "IsaacRepentance.h"

#include <string>

namespace CustomCallbacks
{
    namespace detail
    {
        void ApplyPatches();
    }

    void TriggerInnateCollectibleRemoved(Entity_Player& player, int collectibleID);
    void TriggerInnateTrinketsAdded(Entity_Player& player, unsigned int trinketID, int num);
    void TriggerInnateTrinketsRemoved(Entity_Player& player, unsigned int trinketID, int num);
    void TriggerCollectibleAdded(Entity_Player& player, int collectibleID, bool firsttime, bool wispOrInnate);
    void TriggerInnateItemAddedCallback(Entity_Player& player, int id, bool isTrinket, const std::string& key, int amount, int duration);
    void TriggerInnateItemRemovedCallback(Entity_Player& player, int id, bool isTrinket, const std::string& key, int amount, bool expiredDuration);

    void PRE_ROOM_COLLISION_PASS();
}