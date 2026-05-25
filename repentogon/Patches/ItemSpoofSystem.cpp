#include "ItemSpoofSystem.h"

#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "EntityPlus.h"


// Context handling

struct ItemSpoofCallContext
{
    bool isLuaRequest = false;
    bool ignoreSpoof = false;

    void Reset()
    {
        isLuaRequest = false;
        ignoreSpoof = false;
    }
};
static ItemSpoofCallContext s_ItemSpoofCallContext;

ItemSpoofCallContext GetContextAndReset()
{
    ItemSpoofCallContext context = s_ItemSpoofCallContext;
    s_ItemSpoofCallContext.Reset();
    return context;
}

void ItemSpoofSystem::StartLuaRequest(bool ignoreSpoof) {
    s_ItemSpoofCallContext.isLuaRequest = true;
    s_ItemSpoofCallContext.ignoreSpoof = ignoreSpoof;
}


// "Reworked" Items

static std::bitset<CollectibleType::NUM_COLLECTIBLES> s_reworkedCollectibles;
static std::bitset<ePlayerType::NUM_PLAYER_TYPES> s_reworkedBirthrights;
static std::bitset<TrinketType::NUM_TRINKETS> s_reworkedTrinkets;

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


// XML Innate Items

static std::unordered_map<int, std::unordered_map<int, int>> s_playerTypeInnateCollectibles;
static std::unordered_map<int, std::unordered_map<int, int>> s_playerTypeInnateTrinkets;

const std::unordered_map<int, int>* GetPlayerTypeInnateCollectibles(int playerType) {
    if (auto it = s_playerTypeInnateCollectibles.find(playerType); it != s_playerTypeInnateCollectibles.end())
        return &it->second;
    return nullptr;
}

int GetPlayerTypeInnateCollectibleCount(Entity_Player* player, int collectible) {
    if (const auto* ptypeCollectibles = GetPlayerTypeInnateCollectibles(player->_playerType)) {
        if (auto it = ptypeCollectibles->find(collectible); it != ptypeCollectibles->end()) {
            return it->second;
        }
    }
    return 0;
}

const std::unordered_map<int, int>* GetPlayerTypeInnateTrinkets(int playerType) {
    if (auto it = s_playerTypeInnateTrinkets.find(playerType); it != s_playerTypeInnateTrinkets.end())
        return &it->second;
    return nullptr;
}

int GetPlayerTypeInnateTrinketCount(Entity_Player* player, int collectible) {
    if (const auto* ptypeTrinkets = GetPlayerTypeInnateTrinkets(player->_playerType)) {
        if (auto it = ptypeTrinkets->find(collectible); it != ptypeTrinkets->end()) {
            return it->second;
        }
    }
    return 0;
}

std::vector<int> ParseCommaSeparatedIdList(const std::string& str) {
    std::vector<int> ids;

    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ',')) {
        int id = std::atoi(item.c_str());
        if (id > 0) {
            ids.push_back(id);
        }
    }

    return ids;
}

HOOK_METHOD_PRIORITY(ModManager, LoadConfigs, -9999, () -> void) {
    RegisterCustomXMLAttr(XMLStuff.PlayerData, "innateitems", XMLStuff.ItemData);
    RegisterCustomXMLAttr(XMLStuff.PlayerData, "innatetrinkets", XMLStuff.TrinketData);

    super();

    s_playerTypeInnateCollectibles.clear();
    s_playerTypeInnateTrinkets.clear();

    for (const auto& [playerType, playerData] : XMLStuff.PlayerData->nodes) {
        if (auto it = playerData.find("innateitems"); it != playerData.end()) {
            for (const int id : ParseCommaSeparatedIdList(it->second)) {
                s_playerTypeInnateCollectibles[playerType][id]++;
            }
        }
        if (auto it = playerData.find("innatetrinkets"); it != playerData.end()) {
            for (const int id : ParseCommaSeparatedIdList(it->second)) {
                s_playerTypeInnateTrinkets[playerType][id]++;
            }
        }
    }
}


// Core ItemSpoofSystem hooks

HOOK_METHOD_PRIORITY(Entity_Player, HasCollectible, -9999, (int collectible, bool ignoreModifiers) -> bool)
{
    const auto context = GetContextAndReset();

    if (!context.isLuaRequest && is_reworked_collectible(collectible, this->_playerType))
    {
        return false;
    }

    if (!ignoreModifiers && !context.ignoreSpoof)
    {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
        {
            const ItemSpoofSystem::CollectibleSpoof& spoofs = playerPlus->itemSpoofs.GetCollectibleSpoof();
            if (spoofs.IsBlocked(collectible))
            {
                return false;
            }
            if (spoofs.HasInnate(collectible))
            {
                return true;
            }
        }

        if (GetPlayerTypeInnateCollectibleCount(this, collectible) > 0) {
            return true;
        }
    }

    return super(collectible, ignoreModifiers);
};

HOOK_METHOD_PRIORITY(Entity_Player, GetCollectibleNum, -9999, (int collectible, bool ignoreModifiers) -> int)
{
    const auto context = GetContextAndReset();

    if (!context.isLuaRequest && is_reworked_collectible(collectible, this->_playerType))
    {
        return 0;
    }

    int innateCount = 0;

    if (!ignoreModifiers && !context.ignoreSpoof)
    {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
        {
            const ItemSpoofSystem::CollectibleSpoof& spoofs = playerPlus->itemSpoofs.GetCollectibleSpoof();
            if (spoofs.IsBlocked(collectible))
            {
                return 0;
            }
            innateCount += spoofs.GetInnateCount(collectible);
        }

        innateCount += GetPlayerTypeInnateCollectibleCount(this, collectible);
    }
   
    return innateCount + super(collectible, ignoreModifiers);
};

HOOK_METHOD_PRIORITY(Entity_Player, HasTrinket, -9999, (unsigned int trinket, bool ignoreModifiers) -> bool)
{
    const auto context = GetContextAndReset();

    if (!context.isLuaRequest && is_reworked_trinket(trinket))
    {
        return false;
    }

    if (!ignoreModifiers && !context.ignoreSpoof)
    {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
        {
            const ItemSpoofSystem::TrinketSpoof& spoofs = playerPlus->itemSpoofs.GetTrinketSpoof();
            if (spoofs.IsBlocked(trinket))
            {
                return false;
            }
            if (spoofs.HasInnate(trinket & TRINKET_ID_MASK) || spoofs.HasInnate(trinket & TRINKET_GOLDEN_FLAG))
            {
                return true;
            }
        }

        if (GetPlayerTypeInnateTrinketCount(this, trinket) > 0) {
            return true;
        }
    }

    return super(trinket, ignoreModifiers);
};

HOOK_METHOD_PRIORITY(Entity_Player, GetTrinketMultiplier, -9999, (unsigned int trinket) -> int)
{
    const auto context = GetContextAndReset();

    if (!context.isLuaRequest && is_reworked_trinket(trinket))
    {
        return 0;
    }

    int innateMult = 0;
    
    if (!context.ignoreSpoof)
    {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
        {
            const ItemSpoofSystem::TrinketSpoof& spoofs = playerPlus->itemSpoofs.GetTrinketSpoof();
            if (spoofs.IsBlocked(trinket))
            {
                return 0;
            }
            innateMult += spoofs.GetInnateCount(trinket & TRINKET_ID_MASK) + spoofs.GetInnateCount(trinket | TRINKET_GOLDEN_FLAG) * 2;
        }

        innateMult += GetPlayerTypeInnateTrinketCount(this, trinket);
    }

    int vanillaMult = super(trinket);
    
    // Only add +1 for Mom's Box if the game didn't.
    if (innateMult > 0 && vanillaMult == 0 && this->HasCollectible(COLLECTIBLE_MOMS_BOX, false))
    {
        innateMult++;
    }

    return innateMult + vanillaMult;
};

HOOK_METHOD_PRIORITY(Entity_Player, HasGoldenTrinket, -9999, (unsigned int trinket) -> bool)
{
    const auto context = GetContextAndReset();

    if (!context.isLuaRequest && is_reworked_trinket(trinket))
    {
        return 0;
    }

    if (!context.ignoreSpoof)
    {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
        {
            const ItemSpoofSystem::TrinketSpoof& spoofs = playerPlus->itemSpoofs.GetTrinketSpoof();
            if (spoofs.IsBlocked(trinket))
                return false;
            if (spoofs.HasInnate(trinket | TRINKET_GOLDEN_FLAG))
                return true;
        }
    }

    return super(trinket);
}

HOOK_METHOD_PRIORITY(Entity_Player, UpdateEffects, -9999, () -> void)
{
    if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this))
    {
        playerPlus->itemSpoofs.CheckPlayerType(*this, false);
        playerPlus->itemSpoofs.GetCollectibleSpoof().UpdateTemporaryTimers(*this);
        playerPlus->itemSpoofs.GetTrinketSpoof().UpdateTemporaryTimers(*this);
    }
    super();
}

// Detect PlayerType changes where they can happen.
HOOK_METHOD_PRIORITY(Entity_Player, ChangePlayerType, 9999, (int playerType, bool unk) -> void) {
    super(playerType, unk);
    if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this)) {
        playerPlus->itemSpoofs.CheckPlayerType(*this, false);
    }
}
HOOK_METHOD_PRIORITY(Entity_Player, InitPostLevelInitStats, 9999, () -> void) {
    if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this)) {
        playerPlus->itemSpoofs.CheckPlayerType(*this, false);
    }
    super();
}
HOOK_METHOD_PRIORITY(Entity_Player, TriggerDeath, 9999, (bool checkOnly) -> bool) {
    bool res = super(checkOnly);
    if (!checkOnly) {
        if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this)) {
            playerPlus->itemSpoofs.CheckPlayerType(*this, false);
        }
    }
    return res;
}
HOOK_METHOD_PRIORITY(Entity_Player, RestoreGameState, -9999, (GameStatePlayer* saveState) -> void) {
    super(saveState);
    if (EntityPlayerPlus* playerPlus = GetEntityPlayerPlus(this)) {
        playerPlus->itemSpoofs.CheckPlayerType(*this, true);
    }
}


// Serialization & Data Structure Details

namespace ItemSpoofSystem
{
    rapidjson::Value InnateItems::Serialize(rapidjson::Document::AllocatorType& allocator) const {
        rapidjson::Value output(rapidjson::kArrayType);

        for (const auto& [id, count] : persistent_.Get()) {
            if (count > 0) {
                rapidjson::Value item(rapidjson::kObjectType);
                item.AddMember("id", id, allocator);
                item.AddMember("count", count, allocator);
                output.PushBack(item, allocator);
            }
        }

        for (const TemporaryInnateItem& temporaryInnateItem : temporary_.GetList()) {
            if (temporaryInnateItem.count > 0 && temporaryInnateItem.timer > 0) {
                rapidjson::Value item(rapidjson::kObjectType);
                item.AddMember("id", temporaryInnateItem.id, allocator);
                item.AddMember("count", temporaryInnateItem.count, allocator);
                item.AddMember("timer", temporaryInnateItem.timer, allocator);
                output.PushBack(item, allocator);
            }
        }

        return output;
    }

    rapidjson::Value SerializeInnateItems(const InnateItemGroups& items, rapidjson::Document::AllocatorType& allocator) {
        rapidjson::Value output(rapidjson::kObjectType);

        for (const auto& [key, items] : items) {
            if (!key.empty() && !items.IsEmpty()) {
                rapidjson::Value serialized = items.Serialize(allocator);
                if (!serialized.Empty()) {
                    output.AddMember(rapidjson::Value(key.c_str(), allocator).Move(), serialized, allocator);
                }
            }
        }

        return output;
    }

    InnateItemGroups DeserializeInnateItems(const rapidjson::Value& input) {
        InnateItemGroups itemGroups;

        if (!input.IsObject()) {
            ZHL::Log("[ItemSpoofSystem::DeserializeInnateItems] [ERROR] - Input is not an object!\n");
            return itemGroups;
        }
        for (const auto& group : input.GetObject()) {
            if (group.name.IsString() && group.value.IsArray()) {
                const std::string key = group.name.GetString();
                for (const auto& item : group.value.GetArray()) {
                    if (!item.HasMember("id") || !item["id"].IsInt()) {
                        ZHL::Log("[ItemSpoofSystem::DeserializeInnateItems] [ERROR] - Skipping item with no ID!\n");
                        continue;
                    }
                    const int id = item["id"].GetInt();

                    if (!item.HasMember("count") || !item["count"].IsInt()) {
                        ZHL::Log("[ItemSpoofSystem::DeserializeInnateItems] [ERROR] - Skipping item with no count!\n");
                        continue;
                    }
                    const int count = item["count"].GetInt();

                    int timer = -1;
                    if (item.HasMember("timer")) {
                        if (!item["timer"].IsInt() || item["timer"].GetInt() <= 0) {
                            ZHL::Log("[ItemSpoofSystem::DeserializeInnateItems] [ERROR] - Skipping item with a malformed timer value!\n");
                            continue;
                        }
                        timer = item["timer"].GetInt();
                    }

                    itemGroups[key].Add(id, count, timer);
                }
            }
        }

        return itemGroups;
    }

    void ItemSpoof::TriggerCache(Entity_Player& player, int id, bool evaluateItems) const {
        if (ItemConfig_Item* item = GetItemConfigItem(id)) {
            TriggerItemCustomCache(&player, item, evaluateItems);
            player.AddCacheFlags(item->cacheFlags);
            if (evaluateItems) {
                player.EvaluateItems();
            }
        }
    }

    void ItemSpoof::TriggerAdded(Entity_Player& player, int id, int amount, bool addCostume, bool evaluateItems) const {
        ItemConfig_Item* item = GetItemConfigItem(id);
        if (!item) return;

        if (addCostume && item->addCostumeOnPickup) {
            player.AddCostume(item, false);
        }

        if (IsTrinket()) {
            // TriggerTrinketAdded will add both CacheFlags and CustomCaches
            CustomCallbacks::TriggerInnateTrinketsAdded(player, id, amount);
        } else {
            // Need to trigger both CacheFlags and CustomCache
            TriggerCache(player, id, false);
            CustomCallbacks::TriggerCollectibleAdded(player, id, false, true);
        }

        if (evaluateItems) {
            player.EvaluateItems();
        }
    }

    void ItemSpoof::TriggerRemoved(Entity_Player& player, int id, int amount, bool evaluateItems) const {
        ItemConfig_Item* item = GetItemConfigItem(id);
        if (!item) return;

        if (item->addCostumeOnPickup && !PlayerHas(player, id)) {
            player.RemoveCostume(item);
        }

        if (IsTrinket()) {
            // TriggerTrinketRemoved will add both CacheFlags and CustomCaches
            CustomCallbacks::TriggerInnateTrinketsRemoved(player, id, amount);
        } else {
            // TriggerCollectibleRemoved will add CustomCaches
            player.AddCacheFlags(item->cacheFlags);
            CustomCallbacks::TriggerInnateCollectibleRemoved(player, id);
        }

        if (evaluateItems) {
            player.EvaluateItems();
        }
    }

    void ItemSpoof::RunAddedCallback(Entity_Player& player, const std::string& key, int id, int amount, int duration) const {
        CustomCallbacks::TriggerInnateItemAddedCallback(player, id, IsTrinket(), key, amount, duration);
    }

    void ItemSpoof::RunRemovedCallback(Entity_Player& player, const std::string& key, int id, int amount, bool expiredDuration) const {
        CustomCallbacks::TriggerInnateItemRemovedCallback(player, id, IsTrinket(), key, amount, expiredDuration);
    }

    bool ItemSpoof::IsBlocked(int id) const {
        return blocks_.count(SanitizeID(id, true));
    }

    void ItemSpoof::Block(Entity_Player& player, int id) {
        id = SanitizeID(id, true);
        if (!IsBlocked(id)) {
            bool trigger = PlayerHas(player, id);
            blocks_.insert(id);
            if (trigger) {
                TriggerCache(player, id, true);
            }
        }
    }

    void ItemSpoof::Unblock(Entity_Player& player, int id) {
        id = SanitizeID(id, true);
        if (IsBlocked(id)) {
            blocks_.erase(id);
            if (PlayerHas(player, id)) {
                TriggerCache(player, id, true);
            }
        }
    }

    const std::unordered_set<int>& ItemSpoof::GetBlockedIDs() const {
        return blocks_;
    }

    bool ItemSpoof::HasInnate(int id, const std::string& key) const {
        if (auto it = innates_.find(key); it != innates_.end())
            return it->second.HasAny(SanitizeID(id));
        return false;
    }

    bool ItemSpoof::HasInnate(int id) const {
        id = SanitizeID(id);
        for (const auto& [key, items] : innates_) {
            if (items.HasAny(id)) {
                return true;
            }
        }
        return false;
    }

    int ItemSpoof::GetInnateCount(int id, const std::string& key) const {
        if (auto it = innates_.find(key); it != innates_.end())
            return it->second.TotalCount(SanitizeID(id));
        return 0;
    }

    int ItemSpoof::GetInnateCount(int id) const {
        id = SanitizeID(id);
        int count = 0;
        for (const auto& [key, items] : innates_) {
            count += items.TotalCount(id);
        }
        return count;
    }

    void ItemSpoof::AddInnate(Entity_Player& player, int id, int amount, const std::string& key, int duration, bool addCostume) {
        id = SanitizeID(id);
        if (amount > 0 && IsValidID(id)) {
            innates_[key].Add(id, amount, duration);
            TriggerAdded(player, id, amount, addCostume, true);
            RunAddedCallback(player, key, id, amount, duration);
        }
    }

    int ItemSpoof::RemoveInnate(Entity_Player& player, int id, int amount, const std::string& key, bool legacyRemove) {
        id = SanitizeID(id);
        if (amount > 0 && IsValidID(id)) {
            int removed = innates_[key].Remove(id, amount);
            if (innates_[key].IsEmpty()) {
                innates_.erase(key);
            }
            if (removed > 0) {
                if (legacyRemove) {
                    TriggerCache(player, id, true);
                } else {
                    TriggerRemoved(player, id, removed, true);
                }
                RunRemovedCallback(player, key, id, removed, false);
            }
            return removed;
        }
        return 0;
    }

    int ItemSpoof::SetInnateCount(Entity_Player& player, int id, int newCount, const std::string& key, bool addCostume) {
        id = SanitizeID(id);
        if (newCount < 0) {
            newCount = 0;
        }
        int currentCount = GetInnateCount(id, key);
        int diff = newCount - currentCount;
        if (diff > 0) {
            AddInnate(player, id, diff, key, -1, addCostume);
            return diff;
        } else if (diff < 0) {
            return -RemoveInnate(player, id, -diff, key, false);
        }
        return 0;
    }

    void ItemSpoof::SetInnateGroup(Entity_Player& player, const std::unordered_map<int, int>& inputCounts, const std::string& key, bool addCostume) {
        // Combine existing IDs and the input counts into one map, so that we can set unspecified items to 0, and apply changes in deterministic order.
        std::map<int, int> updatedCounts;
        if (auto it = innates_.find(key); it != innates_.end()) {
            for (const auto& [id, count] : it->second.GetPersistentCounts())
                updatedCounts[id] = 0;
            for (const auto& [id, count] : it->second.GetTemporaryCounts())
                updatedCounts[id] = 0;
        }
        for (const auto& [id, count] : inputCounts)
            updatedCounts[SanitizeID(id)] = count;

        // Apply the new counts.
        for (const auto& [id, newCount] : updatedCounts)
            SetInnateCount(player, id, newCount, key, addCostume);
    }

    void ItemSpoof::UpdateTemporaryTimers(Entity_Player& player) {
        std::set<std::string> emptyGroups;
        std::map<std::string, std::map<int, int>> removedItemsPerGroup;

        // For each group, update their temporary timers and collect any items removed as a result.
        for (auto& [key, items] : innates_) {
            removedItemsPerGroup[key] = items.UpdateTemporaryTimers();
            if (items.IsEmpty()) {
                emptyGroups.insert(key);
            }
        }
        // If a group became empty, erase it.
        for (const std::string& key : emptyGroups) {
            innates_.erase(key);
        }

        // Merge the removed items into a single set, and trigger cache evals and such all together.
        std::map<int, int> combinedRemovedItems;
        for (const auto& [key, removedItems] : removedItemsPerGroup) {
            for (const auto& [id, amount] : removedItems) {
                combinedRemovedItems[id] += amount;
            }
        }

        // Trigger removal logic (caches, etc).
        for (const auto& [id, amount] : combinedRemovedItems) {
            TriggerRemoved(player, id, amount, false);
        }
        player.EvaluateItems();

        // Run the innate-specific callbacks last, per group.
        for (const auto& [key, removedItems] : removedItemsPerGroup) {
            for (const auto& [id, amount] : removedItems) {
                RunRemovedCallback(player, key, id, amount, true);
            }
        }
    }

    void ItemSpoof::ClearItemGroup(Entity_Player& player, const std::string& key) {
        const std::map<int, int> removedItems = innates_[key].Clear();
        innates_.erase(key);

        // Trigger removal logic (caches, etc).
        for (const auto& [id, amount] : removedItems) {
            TriggerRemoved(player, id, amount, false);
        }
        player.EvaluateItems();

        // Run innate removal callbacks last.
        for (const auto& [id, amount] : removedItems) {
            RunRemovedCallback(player, key, id, amount, false);
        }
    }

    std::unordered_map<int, int> ItemSpoof::GetInnateGroupCounts(const std::string& key) const {
        if (auto it = innates_.find(key); it != innates_.end()) {
            std::unordered_map<int, int> out = it->second.GetPersistentCounts();
            for (const auto& [id, count] : it->second.GetTemporaryCounts())
                out[id] += count;
            return out;
        }
        return std::unordered_map<int, int>();
    }

    std::unordered_map<int, int> ItemSpoof::GetInnateTotalCounts() const {
        std::unordered_map<int, int> out;
        for (auto& [key, items] : innates_) {
            for (const auto& [id, count] : items.GetPersistentCounts())
                out[id] += count;
            for (const auto& [id, count] : items.GetTemporaryCounts())
                out[id] += count;
        }
        return out;
    }

    InnateItemGroups ItemSpoof::SaveInnateItems() {
        InnateItemGroups copy = innates_;
        copy.erase("");
        return copy;
    }

    void ItemSpoof::LoadInnateItems(const InnateItemGroups& state) {
        InnateItems legacyGroup = innates_[""];
        innates_ = state;
        if (!legacyGroup.IsEmpty()) {
            innates_[""] = std::move(legacyGroup);
        }
    }

    void PlayerItemSpoofs::CheckPlayerType(Entity_Player& player, bool restoringGameState) {
        const int prevPlayerType = lastPlayerType_;
        const int currentPlayerType = player._playerType;
        if (prevPlayerType != currentPlayerType) {
            lastPlayerType_ = currentPlayerType;
            if (restoringGameState) {
                return;
            }
            if (const auto* oldCols = GetPlayerTypeInnateCollectibles(prevPlayerType)) {
                for (const auto& [id, count] : *oldCols) {
                    collectibleSpoof_.TriggerRemoved(player, id, count, false);
                }
            }
            if (const auto* newCols = GetPlayerTypeInnateCollectibles(currentPlayerType)) {
                for (const auto& [id, count] : *newCols) {
                    collectibleSpoof_.TriggerAdded(player, id, count, false, false);
                }
            }
            if (const auto* oldTrinkets = GetPlayerTypeInnateTrinkets(prevPlayerType)) {
                for (const auto& [id, count] : *oldTrinkets) {
                    trinketSpoof_.TriggerRemoved(player, id, count, false);
                }
            }
            if (const auto* newTrinkets = GetPlayerTypeInnateTrinkets(currentPlayerType)) {
                for (const auto& [id, count] : *newTrinkets) {
                    trinketSpoof_.TriggerAdded(player, id, count, false, false);
                }
            }
            player.EvaluateItems();
        }
    }

}  // namespace ItemSpoofSystem
