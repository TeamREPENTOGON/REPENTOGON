#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Log.h"
#include "IsaacRepentance.h"
#include "document.h"      // rapidjson
#include "stringbuffer.h"  // rapidjson
#include "writer.h"        // rapidjson
#include "CustomCache.h"
#include "XMLData.h"
#include "../LuaInterfaces/CustomCallbacks.h"

namespace ItemSpoofSystem
{
    // Call right before an appropriate function call to signal to the ItemSpoofSystem that the context of the call comes from Lua.
    // This allows reworked collectibles to be treated normally for that particular call. Can optionally also disable all item spoofing.
    // Intended for HasCollectible, GetCollectibleNum, HasTrinket, GetTrinketMultiplier, and HasGoldenTrinket.
    // The "context" set by this function is consumed by the next call of any of the above functions and does not need to be reset manually.
    // Used by lua functions in LuaEntityPlayer.cpp.
    void StartLuaRequest(bool ignoreSpoof = false);

    void ReworkCollectible(int collectible);
    void ReworkBirthright(int playerType);
    void ReworkTrinket(int trinket);

    // Represents a set of innate items, either collectibles or trinkets.
    // Can contain both persistent or temporary copies of items.
    class InnateItems {
    private:
        // Wrapper class around an `std::unordered_map<int, int>` used to maintain counts per collectible/trinket ID.
        class InnateItemCounts {
        public:
            void Add(int id, int amount) {
                if (amount > 0) {
                    counts_[id] += amount;
                }
            }

            int Remove(int id, int amount) {
                if (amount > 0) {
                    int current = counts_[id];
                    if (current > amount) {
                        counts_[id] -= amount;
                        return amount;
                    } else {
                        counts_.erase(id);
                        return current;
                    }
                }
                return 0;
            }

            std::map<int, int> Clear() {
                std::map<int, int> removedItems;
                for (const auto& [id, count] : counts_) {
                    removedItems[id] += count;
                }
                counts_.clear();
                return removedItems;
            }

            int Count(int id) const {
                if (auto it = counts_.find(id);  it != counts_.end()) {
                    return it->second;
                }
                return 0;
            }
            int Has(int id) const {
                return Count(id) > 0;
            }
            bool IsEmpty() const {
                return counts_.empty();
            }

            const std::unordered_map<int, int>& Get() const {
                return counts_;
            }

        private:
            std::unordered_map<int, int> counts_;
        };

        // Represents an innate item with a timer that should tick down every frame and remove itself when it hits 0.
        struct TemporaryInnateItem {
            int id;
            int count;
            int timer;
        };

        // Encapsulates a set of "temporary" innate items with set durations, which should be ticked down every frame and removed when they hit 0.
        class TemporaryInnateItems {
        public:
            void Add(int id, int amount, int duration) {
                if (amount > 0) {
                    list_.emplace_back(TemporaryInnateItem{ id, amount, duration });
                    cachedTotals_.Add(id, amount);
                }
            }

            int Remove(int id, int amount) {
                if (Has(id)) {
                    return RemoveIf([&](TemporaryInnateItem& item) {
                        return item.id == id;
                    }, amount)[id];
                }
                return 0;
            }

            std::map<int, int> Clear() {
                list_.clear();
                return cachedTotals_.Clear();
            }

            int Count(int id) const {
                return cachedTotals_.Count(id);
            }
            int Has(int id) const {
                return cachedTotals_.Has(id);
            }
            bool IsEmpty() const {
                return list_.empty();
            }

            std::map<int, int> UpdateTimers() {
                return RemoveIf([&](TemporaryInnateItem& item) {
                    return --item.timer <= 0;
                });
            }

            const std::vector<TemporaryInnateItem>& GetList() const {
                return list_;
            }
            const std::unordered_map<int, int>& GetCounts() const {
                return cachedTotals_.Get();
            }

        private:
            // Remove temporary innate items based on some condition.
            // If amountToRemove is negative, as many items as possible will be removed.
            // If amountToRemove is positive, only that many items will be removed, oldest first.
            template <typename Func>
            std::map<int, int> RemoveIf(Func&& func, int amountToRemove = -1) {
                std::map<int, int> removedItems;

                // A slightly convoluted iteration is performed here due the nuances on how we want to handle removals.
                // For removing a certain number of items and returning the removed items, we must iterate in order,
                // since the items at the beginning of the vector are the oldest ones.
                // std::remove_if does not guaruntee order, so this method is more deterministic.

                // We are essentially iterating over the vector once and shifting elements down as we go, if we've removed anything.
                // If we want to keep an item in the vector, we move it to the position of this iterator, and increment this iterator.
                // If we are to "remove" an item, we simply do not shift/write it, so it will either get overwritten or truncated.
                auto writeIt = list_.begin();

                for (auto readIt = list_.begin(); readIt != list_.end(); ++readIt) {
                    // Check if the item meets our removal condition.
                    const bool meetsCondition = func(*readIt);

                    // Remove the item from the vector if we are removing everything (negative amountToRemove),
                    // or if we are removing more than the count of this item.
                    const bool fullRemove = meetsCondition && (amountToRemove < 0 || amountToRemove >= readIt->count);

                    // If we are removing only a certain number of items that is less than the count of this item,
                    // we'll decrease its count without removing it outright.
                    const bool partialRemove = meetsCondition && !fullRemove && amountToRemove > 0 && amountToRemove < readIt->count;

                    if (fullRemove) {
                        // Remove the whole item from the vector.
                        cachedTotals_.Remove(readIt->id, readIt->count);  // Update cache
                        removedItems[readIt->id] += readIt->count;  // Add to removedItems
                        if (amountToRemove > 0) amountToRemove -= readIt->count;  // Decrement amountToRemove
                        // The item will be erased since we did not update the write iterator.
                    } else {
                        if (partialRemove) {
                            // Partially remove this item.
                            cachedTotals_.Remove(readIt->id, amountToRemove);  // Update cache
                            readIt->count -= amountToRemove;  // Decrement the count in-place
                            removedItems[readIt->id] += amountToRemove;  // Add to removedItems
                            amountToRemove = 0;  // We don't need to remove anything else
                        }
                        // The item should not be removed from the vector.
                        // Move it to its new position if needed, and update the write iterator.
                        if (readIt != writeIt) {
                            *writeIt = std::move(*readIt);
                        }
                        ++writeIt;
                    }
                }

                // Finalize the removals by truncating the vector to our write iterator.
                list_.erase(writeIt, list_.end());

                return removedItems;
            }

            std::vector<TemporaryInnateItem> list_;

            // Contains the total counts for temporary innate items.
            // So that temporaryList_ does not need to be constantly
            // iterated over. Must be kept in sync!
            InnateItemCounts cachedTotals_;
        };

    public:
        void Add(int id, int amount, int duration) {
            if (duration > 0) {
                temporary_.Add(id, amount, duration);
            } else {
                persistent_.Add(id, amount);
            }
        }

        int Remove(int id, int amount) {
            int removed = temporary_.Remove(id, amount);
            if (removed < amount) {
                removed += persistent_.Remove(id, amount - removed);
            }
            return removed;
        }

        std::map<int, int> Clear() {
            std::map<int, int> removedItems = persistent_.Clear();
            for (const auto [id, amount] : temporary_.Clear()) {
                removedItems[id] += amount;
            }
            return removedItems;
        }

        bool HasAny(int id) const {
            return persistent_.Has(id) || temporary_.Has(id);
        }
        int TotalCount(int id) const {
            return persistent_.Count(id) + temporary_.Count(id);
        }
        bool IsEmpty() const {
            return persistent_.IsEmpty() && temporary_.IsEmpty();
        }

        const std::unordered_map<int, int>& GetPersistentCounts() const {
            return persistent_.Get();
        }
        const std::unordered_map<int, int>& GetTemporaryCounts() const {
            return temporary_.GetCounts();
        }
        
        std::map<int, int> UpdateTemporaryTimers() {
            return temporary_.UpdateTimers();
        }

        // Serialize this set of items as a JSON array.
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const;

    private:
        InnateItemCounts persistent_;
        TemporaryInnateItems temporary_;
    };

    // Separate instances of InnateItems mapped by unique string keys.
    // The empty string key is treated as the "legacy" group, and has some special handling (no gamestate persistence).
    using InnateItemGroups = std::map<std::string, InnateItems>;

    // Serializes the provided InnateItemGroups map to a JSON object.
    rapidjson::Value SerializeInnateItems(const InnateItemGroups& items, rapidjson::Document::AllocatorType& allocator);

    // Deserializes an InnateItemGroups map from json and returns it.
    InnateItemGroups DeserializeInnateItems(const rapidjson::Value& input);

    // Abstract base class for managing spoofing for either collectibles or trinkets.
    class ItemSpoof {
    public:
        // Returns true if this instance is spoofing trinkets, false for collectibles.
        virtual bool IsTrinket() const = 0;

        // Returns true if the ID corresponds to a valid item.
        virtual bool IsValidID(int id) const = 0;

        // Returns the ItemConfig_Item corresponding to the ID, or nullptr if there is none.
        virtual ItemConfig_Item* GetItemConfigItem(int id) const = 0;

        // Returns true if the player has the item, counting spoofs as well.
        virtual bool PlayerHas(Entity_Player& player, int id) const = 0;

        // Strips any unwanted flags from input IDs that may have come from lua.
        virtual int SanitizeID(int id, bool forBlocklist = false) const { return id; };
        
        // Adds the CacheFlags & CustomCache of the item, optionally triggering eval immediately.
        void TriggerCache(Entity_Player& player, int id, bool evaluateItems) const;

        // Runs logic that should happen when an item is added (customes, caches, callbacks, etc).
        void TriggerAdded(Entity_Player& player, int id, int amount, bool addCostume, bool evaluateItems) const;

        // Runs logic that should happen when an item is removed (customes, caches, callbacks, etc).
        void TriggerRemoved(Entity_Player& player, int id, int amount, bool evaluateItems) const;

        // Executes MC_POST_ADD_INNATE_COLLECTIBLE/TRINKET
        void RunAddedCallback(Entity_Player& player, const std::string& key, int id, int amount, int duration) const;

        // Executes MC_POST_REMOVE_INNATE_COLLECTIBLE/TRINKET
        void RunRemovedCallback(Entity_Player& player, const std::string& key, int id, int amount, bool expiredDuration) const;

        // Returns true if the item is currently blocked.
        bool IsBlocked(int id) const;

        // Block the specified item.
        // If held, will trigger a cache eval.
        void Block(Entity_Player& player, int id);

        // Unblock the specified item.
        // If held, will trigger a cache eval.
        void Unblock(Entity_Player& player, int id);

        // Returns the set of blocked IDs.
        const std::unordered_set<int>& GetBlockedIDs() const;

        // Returns true if an innate copy of the item exists in the specified group.
        bool HasInnate(int id, const std::string& key) const;

        // Returns true if an innate copy of the item exists in any group.
        bool HasInnate(int id) const;

        // Returns how many innate copies of the item exist in the specified group.
        int GetInnateCount(int id, const std::string& key) const;

        // Returns the total count of innate copies of the item across all groups.
        int GetInnateCount(int id) const;

        // Add one or more innate items to the specified group, with the specified parameters.
        void AddInnate(Entity_Player& player, int id, int amount, const std::string& key, int duration, bool addCostume);
        
        // Remove one or more innate items to the specified group, with the specified parameters.
        // Returns the actual number of items removed.
        int RemoveInnate(Entity_Player& player, int id, int amount, const std::string& key, bool legacyRemove);

        // Adds or removes innate items as needed to reach the target count in the specified group.
        // Appropriately triggers all added/removed effects.
        int SetInnateCount(Entity_Player& player, int id, int newCount, const std::string& key, bool addCostume);

        // Given a map of ID->count, updates the contents of the specified innate item group to match.
        // Any items not specified in the input will be removed (ie, set to 0).
        // Triggers added/removed effects accordingly.
        void SetInnateGroup(Entity_Player& player, const std::unordered_map<int, int>& inputCounts, const std::string& key, bool addCostume);

        // Decrease temporary innate item timers by 1, and remove them if they hit 0.
        void UpdateTemporaryTimers(Entity_Player& player);

        // Removes all innate items under a particular group.
        void ClearItemGroup(Entity_Player& player, const std::string& key);

        // Returns the innate item counts for a particular group.
        std::unordered_map<int, int> GetInnateGroupCounts(const std::string& key) const;

        // Returns the counts of all innate items from all groups, flattened to a single map.
        std::unordered_map<int, int> GetInnateTotalCounts() const;

        // Returns a copy of the current innate items, for storage in a save state.
        // The legacy group (empty string key) is excluded.
        InnateItemGroups SaveInnateItems();

        // Loads a set of innate items from a save state.
        // The legacy group (empty string key) is not overwritten.
        void LoadInnateItems(const InnateItemGroups& state);

    private:
        std::unordered_set<int> blocks_;
        InnateItemGroups innates_;
    };

    // Represents blocked & innate collectibles.
    class CollectibleSpoof : public ItemSpoof {
        bool IsTrinket() const override { return false; }

        bool IsValidID(int id) const override {
            return ItemConfig::IsValidCollectible(id);
        }
        ItemConfig_Item* GetItemConfigItem(int id) const override {
            return g_Manager->GetItemConfig()->GetCollectible(id);
        }
        bool PlayerHas(Entity_Player& player, int id) const override {
            return player.HasCollectible(id, false);
        }
    };

    // Represents blocked & innate trinkets.
    // Note that golden trinkets are considered distinct from their regular counterparts.
    class TrinketSpoof : public ItemSpoof {
        bool IsTrinket() const override { return true; }

        bool IsValidID(int id) const override {
            return ItemConfig::IsValidTrinket(id);
        }
        ItemConfig_Item* GetItemConfigItem(int id) const override {
            return g_Manager->GetItemConfig()->GetTrinket(id);
        }
        bool PlayerHas(Entity_Player& player, int id) const override {
            return player.HasTrinket(id, false);
        }

        int SanitizeID(int id, bool forBlocklist) const override {
            if (forBlocklist) {
                return id & TRINKET_ID_MASK;
            }
            // We will not tolerate any nonsense with the trinket flags.
            return id & (TRINKET_ID_MASK | TRINKET_GOLDEN_FLAG);
        }
    };

    // Encapsulates all of the dynamic item spoofing being done for a single player (blocked items and innate items).
    // Stored in EntityPlayerPlus.
    class PlayerItemSpoofs {
    public:
        CollectibleSpoof& GetCollectibleSpoof() {
            return collectibleSpoof_;
        }
        TrinketSpoof& GetTrinketSpoof() {
            return trinketSpoof_;
        }
        
        // Check for a PlayerType change, and if a change is detected, "trigger" the add/remove of PlayerType-based innate items.
        // Note that this is not required for the PlayerType-based innates to function correctly - this only handles stuff like TriggerCollectibleRemoved.
        // If `restoringGameState`, update the lastPlayerType_ without triggering anything.
        void CheckPlayerType(Entity_Player& player, bool restoringGameState);

    private:
        CollectibleSpoof collectibleSpoof_;
        TrinketSpoof trinketSpoof_;
        int lastPlayerType_ = -1;
    };

}  // namespace ItemSpoofSystem
