#pragma once

#include "IsaacRepentance.h"
#include "XMLData.h"
#include "JsonSavedata.h"

struct XMLPoolItem // The default values are what's used by the game if a specific node is not found when parsing itempools.xml
{
    int itemID = COLLECTIBLE_NULL;
    float weight = 1.0;
    float decreaseBy = 0.5;
    float removeOn = 0.1;
};

namespace CustomItemPool
{
    struct ItemPoolConfig
    {
        std::string name;
        bool isGreed;
        std::vector<XMLPoolItem> poolList;

        ItemPoolConfig(std::string poolName, bool isGreedPool = false)
        {
            name = poolName;
            isGreed = isGreedPool;
        }
    };

    struct GameState
    {
        std::vector<std::vector<float>> poolItemWeights;
        std::vector<uint32_t> bibleUpgrades;
        std::vector<uint16_t> remainingGenesisItems;

        void StoreGameState();
        void RestoreGameState();
    };

    struct SaveData : CustomItemPool::GameState
    {
        std::string jsonPath;
        std::unordered_map<int, int> oldIDsConversionMap;
        uint16_t danglingGenesisItems;

        void SetSaveSlot(uint32_t slot);
        void Store();

        inline void TryFixItemPoolType(int* itemPoolType);
        void FixItemPool();
        void FixPlayerHistory(Entity_Player* player);
        void FixPlayersHistory();

        inline void JsonToPoolItemWeightsMap(const rapidjson::Value& jsonValue, std::unordered_map<std::string, std::vector<float>>& array);
        template <typename T>
        inline void SerializedPoolMapToVector(const std::unordered_map<std::string, T>& poolMap, std::vector<T>& vector)
        {
            vector.resize(CustomItemPool::poolConfigs.size());
            for (const auto& pair : poolMap)
            {
                int itemPoolType = CustomItemPool::GetPoolIdByName(pair.first);
                if (itemPoolType >= NUM_ITEMPOOLS)
                {
                    vector[itemPoolType - NUM_ITEMPOOLS] = pair.second;
                }
            }
        }
        inline void SerializedGenesisMapToVector(const std::unordered_map<std::string, uint16_t>& poolMap, std::vector<uint16_t>& vector);
        inline void BuildOldIDsConversionMap(unordered_map<std::string, int>& nameIdMapping);

        void SaveToJson();
        bool LoadFromJson();
    };

    extern std::vector<ItemPoolConfig> poolConfigs;
    extern std::vector<ItemPool_Item> itemPools;
    extern std::vector<uint16_t> remainingGenesisItems;

    extern CustomItemPool::GameState gameStates[2];
    extern SaveData saveData;
    extern std::unordered_map<std::string, int> reservedNames;
    extern std::unordered_map<int, bool> vanillaGreedPools;

    inline int CreateNewPool(const std::string& name, bool isGreed);
    inline int GetPoolIdByName(const std::string& name);
    size_t GetNumItemPools();
    inline int GetCustomItemPoolId(int itemPoolType);
    inline void ParseItemNode(const XMLAttributes& itemNode, XMLPoolItem& item);
    inline void GetItemsFromXMLNode(uint32_t nodeId, std::vector<XMLPoolItem>& items);

    void ParseXML_Names();
    void ParseXML_Items();

    enum AddError
    {
        NullPool,
        VanillaPool
    };

    int AddPoolItems(const int poolID, const std::vector<XMLPoolItem>& newItems);

    enum LoadError
    {
        NoGame,
        InvalidID
    };

    int LoadPool(const uint32_t truePoolID);
    void LoadPools();
    void ShufflePools();
    void Init();
};

void ASMPatchesForCustomItemPools();
