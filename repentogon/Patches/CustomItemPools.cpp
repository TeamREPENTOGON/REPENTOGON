#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMPatches.h"
#include "XMLData.h"
#include "JsonSavedata.h"

#include "CustomItemPools.h"

enum GameStateSlot
{
    NULL_SLOT = -1,
    SAVE_FILE = 0,
    GLOWING_HOURGLASS_1 = 1,
    GLOWING_HOURGLASS_2 = 2,
};

static inline int GetGameStateSlot(GameState* gameState)
{
    if (gameState == &g_Game->_glowingHourglassStates[0]._gameState)
    {
        return GameStateSlot::GLOWING_HOURGLASS_1;
    }

    if (gameState == &g_Game->_glowingHourglassStates[1]._gameState)
    {
        return GameStateSlot::GLOWING_HOURGLASS_2;
    }

    if (gameState == &g_Manager->_gamestate)
    {
        return GameStateSlot::SAVE_FILE;
    }

    return GameStateSlot::NULL_SLOT;
}

#define GetGameStateSlotFromField(gameStateSlot, fieldState, fieldName) uint8_t* statePtr = reinterpret_cast<uint8_t*>(fieldState); \
GameState* gameStatePtr = reinterpret_cast<GameState*>(statePtr - offsetof(GameState, fieldName)); \
int gameStateSlot = GetGameStateSlot(gameStatePtr);

static inline ItemConfig_Item* GetCollectibleItemConfig(const int itemID)
{
    if (itemID < 0)
    {
        if (g_Game == nullptr) 
        {
            return nullptr;
        }

        int proceduralItemID = ~itemID;
        std::vector<ProceduralItem*>* proceduralItems = g_Game->GetProceduralItemManager()->GetProceduralItems();

        if (proceduralItemID >= proceduralItems->size())
        {
            return nullptr;
        }

        return (*proceduralItems)[proceduralItemID]->item;
    }

    std::vector<ItemConfig_Item*>* collectiblesConfig = g_Manager->GetItemConfig()->GetCollectibles();

    if (itemID >= collectiblesConfig->size())
    {
        return nullptr;
    }

    return (*collectiblesConfig)[itemID];
}

template <typename T>
static inline void PoolVectorToUnorderedMap(const std::vector<T>& poolVector, std::unordered_map<std::string, T>& map)
{
    for (size_t customPoolID = 0; customPoolID < poolVector.size(); customPoolID++)
    {
        map[CustomItemPool::poolConfigs[customPoolID].name] = poolVector[customPoolID];
    }
}

namespace CustomItemPool
{
    void CustomItemPool::GameState::StoreGameState()
    {
        this->poolItemWeights.assign(itemPools.size(), std::vector<float>());
        this->bibleUpgrades.clear();
        this->bibleUpgrades.reserve(itemPools.size());

        for (size_t pool = 0; pool < itemPools.size(); pool++)
        {
            ItemPool_Item& itemPool = itemPools[pool];
            std::vector<float>& poolItemWeights = this->poolItemWeights[pool];
            poolItemWeights.reserve(itemPool._poolList.size());

            for (size_t poolItem = 0; poolItem < itemPool._poolList.size(); poolItem++)
            {
                poolItemWeights.push_back(itemPool._poolList[poolItem]._weight);
            }

            this->bibleUpgrades.push_back(itemPool._bibleUpgrade);
        }
        
        this->remainingGenesisItems = CustomItemPool::remainingGenesisItems;
    }

    void CustomItemPool::GameState::RestoreGameState()
    {
        for (size_t pool = 0; pool < itemPools.size(); pool++)
        {
            ItemPool_Item& itemPool = itemPools[pool];
            std::vector<float>& poolItemWeights = this->poolItemWeights[pool];

            size_t lowestSize = std::min(itemPool._poolList.size(), poolConfigs.size());
            for (size_t poolItem = 0; poolItem < lowestSize; poolItem++)
            {
                itemPools[pool]._poolList[poolItem]._weight = poolItemWeights[poolItem];
            }

            itemPool._bibleUpgrade = this->bibleUpgrades[pool];
        }

        CustomItemPool::remainingGenesisItems = this->remainingGenesisItems;
    }

    inline void SaveData::TryFixItemPoolType(int* itemPoolType)
    {
        if (*itemPoolType < NUM_ITEMPOOLS)
        {
            return;
        }

        try {
            *itemPoolType = this->oldIDsConversionMap.at(*itemPoolType - NUM_ITEMPOOLS);
        }
        catch (const std::out_of_range& e) {
            *itemPoolType = POOL_TREASURE;
        }
    }

    void SaveData::FixItemPool()
    {
        ItemPool itemPool = g_Game->_itemPool;
        this->TryFixItemPoolType(&itemPool._lastPool);
        itemPool._remainingGenesisItems[POOL_TREASURE] += this->danglingGenesisItems;
    }

    inline void SaveData::FixPlayerHistory(Entity_Player* player)
    {
        std::vector<History_HistoryItem> historyItems = player->GetHistory()->_historyItems;
        for (size_t i = 0; i < historyItems.size(); i++)
        {
            this->TryFixItemPoolType(&historyItems[i]._itemPoolType);
        }
    }

    void SaveData::FixPlayersHistory()
    {
        vector_EntityPlayerPtr& playerList =  g_Game->_playerManager._playerList;
        for (size_t playerID = 0; playerID < playerList.size(); playerID++)
        {
            Entity_Player* player = playerList[playerID];
            this->FixPlayerHistory(player);
            if (player->_backupPlayer != nullptr)
            {
                this->FixPlayerHistory(player->_backupPlayer);
            }
        }

        for (size_t esauJrID = 0; esauJrID < 4; esauJrID++)
        {
            Entity_Player* esauJr = g_Game->_playerManager._esauJrState[esauJrID];
            if (esauJr != nullptr)
            {
                this->FixPlayerHistory(esauJr);
            }
        }
    }

    void SaveData::SetSaveSlot(uint32_t slot)
    {
        int saveslot = slot > 0 ? slot : 1;
        this->jsonPath = std::string(REPENTOGON::GetRepentogonDataPath());
        this->jsonPath.append("customitempool_gamestate").append(to_string(saveslot)).append(".json");
    }

    void SaveData::Store()
    {
        this->StoreGameState();
    }

    void SaveData::SaveToJson()
    {
        rapidjson::Document doc;
        doc.SetObject();

        unordered_map<std::string, std::vector<float>> poolItemWeights;
        PoolVectorToUnorderedMap(saveData.poolItemWeights, poolItemWeights);
        ArrayOfArrayToJson(&doc, "PoolItemWeights", poolItemWeights);

        unordered_map<std::string, uint32_t> bibleUpgrades;
        PoolVectorToUnorderedMap(saveData.bibleUpgrades, bibleUpgrades);
        ArrayToJson(&doc, "BibleUpgrades", bibleUpgrades);

        unordered_map<std::string, uint16_t> remainingGenesisItems;
        PoolVectorToUnorderedMap(saveData.remainingGenesisItems, remainingGenesisItems);
        ArrayToJson(&doc, "RemainingGenesisItems", remainingGenesisItems);

        unordered_map<std::string, int> nameIdMapping;
        for (size_t customPoolID = 0; customPoolID < poolConfigs.size(); customPoolID++)
        {
            nameIdMapping[poolConfigs[customPoolID].name] = customPoolID;
        }
        ArrayToJson(&doc, "NameIdMapping", nameIdMapping);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        ofstream ofs(this->jsonPath);
        ofs << buffer.GetString() << std::endl;
        logViewer.AddLog("[REPENTOGON]", "CustomItemPool GameState saved to : %s\n", this->jsonPath.c_str());
    }

    inline void SaveData::JsonToPoolItemWeightsMap(const rapidjson::Value& jsonValue, std::unordered_map<std::string, std::vector<float>>& array)
    {
        for (auto itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); ++itr)
        {
            string key = itr->name.GetString();
            std::vector<float> innerArray;
            const rapidjson::Value& jsonArray = itr->value;
            innerArray.assign(jsonArray.Size(), 0);
            for (rapidjson::SizeType i = 0; i < jsonArray.Size(); ++i)
            {
                if (jsonArray[i].IsNumber())
                {
                    innerArray[i] = jsonArray[i].GetFloat();
                }
            }
            array[key] = innerArray;
        }
    }

    inline void SaveData::SerializedGenesisMapToVector(const std::unordered_map<std::string, uint16_t>& poolMap, std::vector<uint16_t>& vector)
    {
        this->danglingGenesisItems = 0;
        vector.resize(CustomItemPool::poolConfigs.size());
        for (const auto& pair : poolMap)
        {
            int itemPoolType = CustomItemPool::GetPoolIdByName(pair.first);
            if (itemPoolType >= NUM_ITEMPOOLS)
            {
                vector[itemPoolType - NUM_ITEMPOOLS] = pair.second;
            }
            else
            {
                this->danglingGenesisItems += pair.second;
            }
        }
    }

    inline void SaveData::BuildOldIDsConversionMap(unordered_map<std::string, int>& nameIdMapping)
    {
        this->oldIDsConversionMap.clear();
        for (const auto& pair : nameIdMapping)
        {
            int newPoolId = GetPoolIdByName(pair.first);
            newPoolId = (newPoolId >= NUM_ITEMPOOLS) ? newPoolId : POOL_TREASURE;
            this->oldIDsConversionMap[pair.second] = newPoolId;
        }
    }

    bool SaveData::LoadFromJson()
    {
        this->oldIDsConversionMap.clear();

        rapidjson::Document doc = GetJsonDoc(&this->jsonPath);
        if (!doc.IsObject())
        {
            logViewer.AddLog("[REPENTOGON]", "No CustomItemPool GameState for saveslot in: %s\n", this->jsonPath.c_str());
            return false;
        }

        this->StoreGameState();

        unordered_map<std::string, std::vector<float>> poolItemWeightsMap;
        this->JsonToPoolItemWeightsMap(doc["PoolItemWeights"], poolItemWeightsMap);
        this->SerializedPoolMapToVector(poolItemWeightsMap, this->poolItemWeights);

        unordered_map<std::string, uint32_t> bibleUpgradesMap;
        JsonToArray(doc["BibleUpgrades"], bibleUpgradesMap);
        this->SerializedPoolMapToVector(bibleUpgradesMap, this->bibleUpgrades);

        unordered_map<std::string, uint16_t> remainingGenesisItemsMap;
        JsonToArray(doc["RemainingGenesisItems"], remainingGenesisItemsMap);
        this->SerializedGenesisMapToVector(remainingGenesisItemsMap, this->remainingGenesisItems);

        unordered_map<std::string, int> nameIdMapping;
        JsonToArray(doc["NameIdMapping"], nameIdMapping);
        this->BuildOldIDsConversionMap(nameIdMapping);

        logViewer.AddLog("[REPENTOGON]", "CustomItemPool GameState loaded from: %s\n", this->jsonPath.c_str());

        return true;
    }


    std::vector<ItemPoolConfig> poolConfigs;
    std::vector<ItemPool_Item> itemPools;
    std::vector<uint16_t> remainingGenesisItems;

    CustomItemPool::GameState gameStates[2];
    SaveData saveData;

    std::unordered_map<std::string, int> InitReservedNames()
    {
        std::unordered_map<std::string, int> reservedNames;

        reservedNames["treasure"] = POOL_TREASURE;
        reservedNames["shop"] = POOL_SHOP;
        reservedNames["boss"] = POOL_BOSS;
        reservedNames["devil"] = POOL_DEVIL;
        reservedNames["angel"] = POOL_ANGEL;
        reservedNames["secret"] = POOL_SECRET;
        reservedNames["library"] = POOL_LIBRARY;
        reservedNames["shellGame"] = POOL_SHELL_GAME;
        reservedNames["goldenChest"] = POOL_GOLDEN_CHEST;
        reservedNames["redChest"] = POOL_RED_CHEST;
        reservedNames["beggar"] = POOL_BEGGAR;
        reservedNames["demonBeggar"] = POOL_DEMON_BEGGAR;
        reservedNames["curse"] = POOL_CURSE;
        reservedNames["keyMaster"] = POOL_KEY_MASTER;
        reservedNames["batteryBum"] = POOL_BATTERY_BUM;
        reservedNames["momsChest"] = POOL_MOMS_CHEST;
        reservedNames["greedTreasure"] = POOL_GREED_TREASURE;
        reservedNames["greedBoss"] = POOL_GREED_BOSS;
        reservedNames["greedShop"] = POOL_GREED_SHOP;
        reservedNames["greedDevil"] = POOL_GREED_DEVIL;
        reservedNames["greedAngel"] = POOL_GREED_ANGEL;
        reservedNames["greedCurse"] = POOL_GREED_CURSE;
        reservedNames["greedSecret"] = POOL_GREED_SECRET;
        reservedNames["craneGame"] = POOL_CRANE_GAME;
        reservedNames["ultraSecret"] = POOL_ULTRA_SECRET;
        reservedNames["bombBum"] = POOL_BOMB_BUM;
        reservedNames["planetarium"] = POOL_PLANETARIUM;
        reservedNames["oldChest"] = POOL_OLD_CHEST;
        reservedNames["babyShop"] = POOL_BABY_SHOP;
        reservedNames["woodenChest"] = POOL_WOODEN_CHEST;
        reservedNames["rottenBeggar"] = POOL_ROTTEN_BEGGAR;

        return reservedNames;
    }

    std::unordered_map<std::string, int> reservedNames = InitReservedNames();
    
    std::unordered_map<int, bool> InitVanillaGreedPools()
    {
        std::unordered_map<int, bool> vanillaGreedPools;

        vanillaGreedPools[POOL_GREED_TREASURE] = true;
        vanillaGreedPools[POOL_GREED_BOSS] = true;
        vanillaGreedPools[POOL_GREED_SHOP] = true;
        vanillaGreedPools[POOL_GREED_DEVIL] = true;
        vanillaGreedPools[POOL_GREED_ANGEL] = true;
        vanillaGreedPools[POOL_GREED_CURSE] = true;
        vanillaGreedPools[POOL_GREED_SECRET] = true;

        return vanillaGreedPools;
    }

    std::unordered_map<int, bool> vanillaGreedPools = InitVanillaGreedPools();

    inline int CreateNewPool(const std::string& name, bool isGreed)
    {
        poolConfigs.push_back(ItemPoolConfig(name, isGreed));
        return poolConfigs.size() - 1;
    }

    inline int GetPoolIdByName(const std::string& name)
    {
        auto iterator = reservedNames.find(name);
        if (iterator != reservedNames.end())
        {
            return iterator->second;
        }

        for (size_t i = 0; i < poolConfigs.size(); i++)
        {
            if (poolConfigs[i].name == name)
            {
                return i + NUM_ITEMPOOLS;
            }
        }

        return -1;
    }

    inline int GetCustomItemPoolId(int itemPoolType)
    {
        if (itemPoolType < NUM_ITEMPOOLS)
        {
            return -1;
        }

        if (itemPoolType >= poolConfigs.size() + NUM_ITEMPOOLS)
        {
            return -1;
        }

        return itemPoolType - NUM_ITEMPOOLS;
    }

    const char* xmlParseErrorPrefix = "[Custom ItemPool] Error whilst Parsing XML";

    inline void ParseItemNode(const XMLAttributes& itemNode, XMLPoolItem& item)
    {
        if (itemNode.find("id") != itemNode.end())
        {
            std::string itemID_string = itemNode.at("id");
            try {
                item.itemID = std::stoi(itemID_string);
            } catch (const std::invalid_argument& e) {
                ZHL::Log("%s: Invalid argument for <id> (%s is not a valid integer)", xmlParseErrorPrefix, itemID_string);
            } catch (const std::out_of_range& e) {
                ZHL::Log("%s: Invalid argument for <id> (%s is out of range)", xmlParseErrorPrefix, itemID_string);
            }
        }
        else
        {
            if (itemNode.find("name") != itemNode.end())
            {
                std::string name = itemNode.at("name");
                int itemID = LuaEngine::Isaac_GetItemIdByName(&name);
                if (itemID < 0)
                {
                    item.itemID = itemID;
                }
            }
        }

        if (itemNode.find("weight") != itemNode.end())
        {
            std::string weight_string = itemNode.at("weight");
            try {
                item.weight = std::stof(weight_string);
            } catch (const std::invalid_argument& e) {
                ZHL::Log("%s: Invalid argument for <weight> (%s is not a valid float)", xmlParseErrorPrefix, weight_string);
            } catch (const std::out_of_range& e) {
                ZHL::Log("%s: Invalid argument for <weight> (%s is out of range)", xmlParseErrorPrefix, weight_string);
            }
        }

        if (itemNode.find("decreaseby") != itemNode.end())
        {
            std::string decreaseBy_string = itemNode.at("decreaseby");
            try {
                item.decreaseBy = std::stof(decreaseBy_string);
            } catch (const std::invalid_argument& e) {
                ZHL::Log("%s: Invalid argument for <decreaseby> (%s is not a valid float)", xmlParseErrorPrefix, decreaseBy_string);
            } catch (const std::out_of_range& e) {
                ZHL::Log("%s: Invalid argument for <decreaseby> (%s is out of range)", xmlParseErrorPrefix, decreaseBy_string);
            }
        }

        if (itemNode.find("removeon") != itemNode.end())
        {
            std::string removeOn_string = itemNode.at("removeon");
            try {
                item.removeOn = std::stof(removeOn_string);
            } catch (const std::invalid_argument& e) {
                ZHL::Log("%s: Invalid argument for <removeon> (%s is not a valid float)", xmlParseErrorPrefix, removeOn_string);
            } catch (const std::out_of_range& e) {
                ZHL::Log("%s: Invalid argument for <removeon> (%s is out of range)", xmlParseErrorPrefix, removeOn_string);
            }
        }
    }

    inline void GetItemsFromXMLNode(uint32_t nodeId, std::vector<XMLPoolItem>& items)
    {
        const std::tuple<XMLAttributes, XMLChilds> parentChildsPair = XMLStuff.PoolData->GetXMLNodeNChildsById(nodeId);
        const XMLChilds& childNodes = std::get<1>(parentChildsPair);

        if (childNodes.find("item") == childNodes.end())
        {
            return;
        }

        const std::vector<XMLAttributes>& itemNodes = childNodes.at("item");
        items.resize(itemNodes.size());

        for (size_t i = 0; i < itemNodes.size(); i++)
        {
            ParseItemNode(itemNodes[i], items[i]);
        }
    }

    inline int AddPoolItems(const int poolID, const std::vector<XMLPoolItem>& newItems)
    {
        if (poolID < 0)
        {
            return ~AddError::NullPool;
        }

        if (poolID < NUM_ITEMPOOLS)
        {
            return ~AddError::VanillaPool;
        }

        ItemPoolConfig& pool = poolConfigs[poolID - NUM_ITEMPOOLS];
        pool.poolList.insert(pool.poolList.end(), newItems.begin(), newItems.end());
        return 0;
    }

    void ParseXML_Names()
    {
        XMLDataHolder* xml = XMLStuff.PoolData;
        for (int nodeID = 1; nodeID <= xml->maxid; nodeID++)
        {
            XMLAttributes node = xml->GetNodeById(nodeID);
            if (node.find("name") == node.end())
            {
                continue;
            }
            std::string name = node["name"];
            if (GetPoolIdByName(name) != -1)
            {
                continue;
            }
            bool isGreed = (node.find("isgreed") != node.end()) && (node["isgreed"] == "true");
            CreateNewPool(name, isGreed);
        }
    }

    void ParseXML_Items()
    {
        XMLDataHolder* xml = XMLStuff.PoolData;
        for (int nodeID = 1; nodeID <= xml->maxid; nodeID++)
        {
            const XMLAttributes& node = xml->GetNodeById(nodeID);
            if (node.find("name") == node.end())
            {
                continue;
            }

            const std::string& name = node.at("name");
            int poolType = GetPoolIdByName(name);

            if (poolType == -1)
            {
                ZHL::Log("[Custom ItemPools] Warning: xml contains a pool that wasn't registered beforehand, this could cause problems");
                bool isGreed = (node.find("isgreed") != node.end()) && (node.at("isgreed") == "true");
                poolType = CreateNewPool(name, isGreed);
            }

            if (poolType < NUM_ITEMPOOLS)
            {
                continue;
            }

            std::vector<XMLPoolItem> itemList;
            GetItemsFromXMLNode(nodeID, itemList);
            AddPoolItems(poolType, itemList);
        }
    }

    int LoadPool(const uint32_t customPoolID)
    {
        if (g_Game == nullptr)
        {
            return ~LoadError::NoGame;
        }

        if (poolConfigs.size() <= customPoolID)
        {
            return ~LoadError::InvalidID;
        }

        if (itemPools.max_size() <= customPoolID)
        {
            itemPools.resize(poolConfigs.size());
        }

        ItemPool_Item& pool = itemPools[customPoolID];
        RNG& rng = g_Game->_itemPool._itemPoolRNG;

        pool._totalWeight = 0;
        pool._poolList.clear();
        pool._rng1.SetSeed(rng.Next(), 35);
        pool._rng2.SetSeed(rng.Next(), 35);
        pool._bibleUpgrade = 0;

        ItemPoolConfig& poolConfig = poolConfigs[customPoolID];

        pool._poolList.reserve(poolConfig.poolList.size());

        for (size_t i = 0; i < poolConfig.poolList.size(); i++)
        {
            XMLPoolItem& xmlItem = poolConfig.poolList[i];

            pool._poolList.emplace_back();
            PoolItem& poolItem = pool._poolList[i];

            poolItem._itemID = xmlItem.itemID;
            poolItem._initialWeight = xmlItem.weight;
            poolItem._weight = xmlItem.weight;
            poolItem._decreaseBy = xmlItem.decreaseBy;
            poolItem._removeOn = xmlItem.removeOn;
            poolItem._isUnlocked = false;
            poolItem._isSpecial = false;

            pool._totalWeight += xmlItem.weight;

            ItemConfig_Item* itemConfig = GetCollectibleItemConfig(xmlItem.itemID);
            if (itemConfig == nullptr)
            {
                continue;
            }

            if (g_Manager->GetPersistentGameData()->Unlocked(itemConfig->achievementId))
            {
                poolItem._isUnlocked = true;
            }

            poolItem._isSpecial = itemConfig->special;
        }

        return 0;
    }

    void LoadPools()
    {
        itemPools.assign(poolConfigs.size(), ItemPool_Item());
        for (size_t customPoolID = 0; customPoolID < poolConfigs.size(); customPoolID++)
        {
            LoadPool(customPoolID);
        }
    }

    inline void SwapPoolItems(PoolItem& Left, PoolItem& Right)
    {
        PoolItem tmp = Left;
        Left = Right;
        Right = tmp;
    }

    inline void ShufflePool(const int customPoolID, MTRNG& mtRNG)
    {
        if (itemPools.size() <= customPoolID )
        {
            return;
        }

        std::vector<PoolItem>& poolItems = itemPools[customPoolID]._poolList;
        for (size_t lastIndex = poolItems.size() - 1; lastIndex > 0; lastIndex--)
        {
            uint32_t randomIndex = mtRNG.Next() % (lastIndex + 1);
            if (randomIndex == lastIndex)
            {
                continue;
            }

            SwapPoolItems(poolItems[randomIndex], poolItems[lastIndex]);
        }
    }

    void ShufflePools()
    {
        MTRNG mtRNG;
        mtRNG.Init(g_Game->_itemPool._itemPoolRNG._seed);

        for (size_t i = 0; i < itemPools.size(); i--)
        {
            ShufflePool(i, mtRNG);
        }
    }

    void Init()
    {
        LoadPools();
        ShufflePools();
        remainingGenesisItems.assign(poolConfigs.size(), 0);
    }
}

HOOK_METHOD(ModManager, LoadConfigs, () -> void)
{
    std::string path = "itempools.xml";
    std::string redirectedPath;
    g_Manager->GetModManager()->TryRedirectPath(&redirectedPath,&path);
    g_Game->_itemPool.Init(2853650767, &redirectedPath[0]);

    CustomItemPool::ParseXML_Names();
	super();
    CustomItemPool::ParseXML_Items();
}

// Hooking into shuffle_pools to Init CustomItemPool, so that it uses the "correct" itemPoolRNG
HOOK_METHOD(ItemPool, shuffle_pools, () -> void)
{
    super();
    CustomItemPool::Init();
}

HOOK_METHOD(Manager, SetSaveSlot, (uint32_t slot) -> void)
{
	CustomItemPool::saveData.SetSaveSlot(slot);
	super(slot);
}

HOOK_METHOD(ItemPool, StoreGameState, (GameStateItemPool* itemPoolState) -> void)
{
    super(itemPoolState);
    GetGameStateSlotFromField(slot, itemPoolState, _itemPool);
    if (slot == GameStateSlot::NULL_SLOT)
    {
        ZHL::Log("[REPENTOGON]", "CustomItemPools: failed to find Equivalent GameState on Store");
        return;
    }

    if (slot == GameStateSlot::SAVE_FILE)
    {
        CustomItemPool::saveData.Store();
        CustomItemPool::saveData.SaveToJson();
        return;
    }

    CustomItemPool::gameStates[slot - GameStateSlot::GLOWING_HOURGLASS_1].StoreGameState();
}

HOOK_METHOD(ItemPool, RestoreGameState, (GameStateItemPool* itemPoolState) -> void)
{
    super(itemPoolState);
    GetGameStateSlotFromField(slot, itemPoolState, _itemPool);
    if (slot == GameStateSlot::NULL_SLOT)
    {
        ZHL::Log("[REPENTOGON]", "CustomItemPools: failed to find Equivalent GameState on Restore");
        return;
    }

    if (slot == GameStateSlot::SAVE_FILE)
    {
        if (CustomItemPool::saveData.LoadFromJson())
        {
            CustomItemPool::saveData.RestoreGameState();
        }
        CustomItemPool::saveData.FixItemPool();
        return;
    }

    CustomItemPool::gameStates[slot - GameStateSlot::GLOWING_HOURGLASS_1].RestoreGameState();
}

HOOK_METHOD(PlayerManager, RestoreGameState, (GameState* state) -> void)
{
    super(state);

    int slot = GetGameStateSlot(state);
    if (slot != GameStateSlot::SAVE_FILE)
    {
        return;
    }

    CustomItemPool::saveData.FixPlayersHistory();
}

// Hooking into GetCollectible so that if the ID is invalid the default treasure pool is used
HOOK_METHOD(ItemPool, GetCollectible, (int itemPoolType, uint32_t seed, uint32_t flags, int defaultItem) -> int)
{
    if (itemPoolType < POOL_NULL || itemPoolType >= CustomItemPool::itemPools.size() + NUM_ITEMPOOLS)
    {
        itemPoolType = POOL_TREASURE;
    }

    return super(itemPoolType, seed, flags, defaultItem);
}

// Recreating get_chaos_pool because it's easier than using ASM patches
HOOK_METHOD(ItemPool, get_chaos_pool, (RNG * rng) -> int)
{
	WeightedOutcomePicker picker;
    const uint32_t scaleFactor = 100;
    bool isGreedMode = g_Game->IsGreedMode();

	for (int poolType = POOL_TREASURE; poolType < NUM_ITEMPOOLS; poolType++)
    {
		if (isGreedMode != (CustomItemPool::vanillaGreedPools.find(poolType) != CustomItemPool::vanillaGreedPools.end()))
        {
			continue;
		}

		ItemPool_Item& pool = this->_pools[poolType];
		WeightedOutcomePicker_Outcome outcome{poolType, (uint32_t)(pool._totalWeight * scaleFactor)};
		picker.AddOutcomeWeight(outcome, false);
	}

    for (size_t i = 0; i < CustomItemPool::poolConfigs.size(); i++)
    {
        if (isGreedMode != CustomItemPool::poolConfigs[i].isGreed)
        {
            continue;
        }

        ItemPool_Item& pool = CustomItemPool::itemPools[i];
        WeightedOutcomePicker_Outcome outcome{i + NUM_ITEMPOOLS, (uint32_t)(pool._totalWeight * scaleFactor)};
        picker.AddOutcomeWeight(outcome, false);
    }    

	rng->Next();

	RNG pickerRNG;
	pickerRNG.SetSeed(rng->_seed, 35);

	return picker.PickOutcome(pickerRNG);
}

// Pass the correct ItemPool_Item to pick_collectible
ItemPool_Item* __stdcall GetItemPool_Item(uint32_t itemPoolOffset)
{
    int itemPoolType = itemPoolOffset / sizeof(ItemPool_Item);
    if (itemPoolType < NUM_ITEMPOOLS)
    {
        return g_Game->_itemPool.GetPool(itemPoolType);
    }

    if (itemPoolType >= CustomItemPool::itemPools.size() + NUM_ITEMPOOLS)
    {
        return g_Game->_itemPool.GetPool(POOL_TREASURE);
    }

    return &CustomItemPool::itemPools[itemPoolType - NUM_ITEMPOOLS];
}

// This covers both pick_collectible and the BibleUpgrade evaluation
void ASMPatchFirstPickCollectible()
{
	SigScan scanner("8d77??03f0");
	scanner.Scan();
	void* firstPickSig = scanner.GetAddress();

	printf("[REPENTOGON] Patching ItemPool::GetCollectible (First Pick) at %p for CustomItemPools\n", firstPickSig);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::ESI, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
        .Push(ASMPatch::Registers::EAX) // itemPoolOffset
		.AddInternalCall(GetItemPool_Item) // call GetItemPool_Item
        .CopyRegister(ASMPatch::Registers::ESI, ASMPatch::Registers::EAX) // pool = result
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)firstPickSig + 0x5);
	sASMPatcher.PatchAt(firstPickSig, &patch);
}

// On retries the code was compiled differently, though luckily the only difference for our purposes is that the result should remain in EAX rather than being moved to ESI
// This also covers both pick_collectible and the BibleUpgrade evaluation
void ASMPatchRetryPickCollectible()
{
	SigScan scanner("83c00403c78945");
	scanner.Scan();
	void* retryPickSig = scanner.GetAddress();

	printf("[REPENTOGON] Patching ItemPool::GetCollectible (Retry Pick) at %p for CustomItemPools\n", retryPickSig);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
        .Push(ASMPatch::Registers::EAX) // itemPoolOffset
		.AddInternalCall(GetItemPool_Item) // call GetItemPool_Item
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)retryPickSig + 0x5);
	sASMPatcher.PatchAt(retryPickSig, &patch);
}

void __stdcall IncreaseRemainingGenesisItem(int itemPoolType)
{
    uint32_t customPoolID = itemPoolType - NUM_ITEMPOOLS;
    CustomItemPool::remainingGenesisItems[customPoolID] += 1;
}

void ASMPatchPreTriggerGenesis()
{
    SigScan scanner("66ff84??????????8b5c24");
    scanner.Scan();
    void* initGenesisItemsSig = scanner.GetAddress();

    printf("[REPENTOGON] Patching Entity_Player::PreTriggerGenesis at %p for CustomItemPools\n", initGenesisItemsSig);

    std::string checkVanillaItemPools = "\x83\xf8"; checkVanillaItemPools += static_cast<char>(NUM_ITEMPOOLS); // cmp eax, NUM_ITEMPOOLS
    ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;
    patch.AddBytes(checkVanillaItemPools) // cmp itemPoolType, NUM_ITEMPOOLS
        .AddBytes("\x73\x0d") // jae (current addr + 0x0d)
        .AddBytes(ByteBuffer().AddAny((char*)initGenesisItemsSig, 0x8)) // restore overwritten bytes
        .AddRelativeJump((char*)initGenesisItemsSig + 0x8) // resume
        .PreserveRegisters(reg)
        .Push(ASMPatch::Registers::EAX) // itemPoolType
        .AddInternalCall(IncreaseRemainingGenesisItem) // call IncreaseRemainingGenesisItem
        .RestoreRegisters(reg)
        .AddRelativeJump((char*)initGenesisItemsSig + 0x8); // resume
    sASMPatcher.PatchAt(initGenesisItemsSig, &patch);
}

void __stdcall AddGenesisPoolsWeight(WeightedOutcomePicker* outcomePicker, bool* hasAddedPool)
{
    const auto& remainingGenesisItems = CustomItemPool::remainingGenesisItems;
    for (size_t i = 0; i < remainingGenesisItems.size(); i++)
    {
        if (remainingGenesisItems[i] <= 0)
        {
            continue;
        }
        WeightedOutcomePicker_Outcome outcome = {i + NUM_ITEMPOOLS, remainingGenesisItems[i]};
        outcomePicker->AddOutcomeWeight(outcome, false);
        *hasAddedPool = true;
    }
}

void ASMPatchAddGenesisPoolWeight()
{
    SigScan scanner("80bd????????008bbd????????0f85");
    scanner.Scan();
    void* addGenesisPoolWeightSig = scanner.GetAddress();

    SigScan helperScanner("8d8d????????e8????????8bf08b85");
    helperScanner.Scan();
    void* leaWeightedOutcomePickerSig = helperScanner.GetAddress();

    printf("[REPENTOGON] Patching Room::Update (Add Genesis Pool Weight) at %p for CustomItemPools\n", addGenesisPoolWeightSig);

    ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;
    patch.PreserveRegisters(reg)
        .AddBytes("\x8D\x85").AddBytes(ByteBuffer().AddAny((char*)addGenesisPoolWeightSig + 0x2, 0x4)) // lea eax, [hasAddedPool]
        .AddBytes(ByteBuffer().AddAny((char*)leaWeightedOutcomePickerSig, 0x6)) // lea ecx, [WeightedOutcomePicker]
        .Push(ASMPatch::Registers::EAX) // hasAddedPool*
        .Push(ASMPatch::Registers::ECX) // WeightedOutcomePicker*
        .AddInternalCall(AddGenesisPoolsWeight) // call AddGenesisPoolsWeight
        .RestoreRegisters(reg)
        .AddBytes(ByteBuffer().AddAny((char*)addGenesisPoolWeightSig, 0x7)) // restore overwritten bytes
        .AddRelativeJump((char*)addGenesisPoolWeightSig + 0x7); // resume
    sASMPatcher.PatchAt(addGenesisPoolWeightSig, &patch);
}

void __stdcall DecreaseRemainingGenesisItem(int itemPoolType)
{
    uint32_t customPoolID = itemPoolType - NUM_ITEMPOOLS;
    CustomItemPool::remainingGenesisItems[customPoolID] -= 1;
}

void ASMPatchDecreaseRemainingGenesisItems()
{
    SigScan scanner("66ff8c");
    scanner.Scan();
    void* decreaseGenesisItemsSig = scanner.GetAddress();

    printf("[REPENTOGON] Patching Room::Update (Decrease Genesis Items) at %p for CustomItemPools\n", decreaseGenesisItemsSig);

    std::string checkVanillaItemPools = "\x83\xfe"; checkVanillaItemPools += static_cast<char>(NUM_ITEMPOOLS); // cmp esi, NUM_ITEMPOOLS
    ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
    ASMPatch patch;
    patch.AddBytes(checkVanillaItemPools) // cmp itemPoolType, NUM_ITEMPOOLS
        .AddBytes("\x73\x0d") // jae (current addr + 0x0d)
        .AddBytes(ByteBuffer().AddAny((char*)decreaseGenesisItemsSig, 0x8)) //restore overwritten bytes
        .AddRelativeJump((char*)decreaseGenesisItemsSig + 0x8) // resume
        .PreserveRegisters(reg)
        .Push(ASMPatch::Registers::ESI) // itemPoolType
        .AddInternalCall(DecreaseRemainingGenesisItem) // call DecreaseRemainingGenesisItem
        .RestoreRegisters(reg)
        .AddRelativeJump((char*)decreaseGenesisItemsSig + 0x8); // resume
    sASMPatcher.PatchAt(decreaseGenesisItemsSig, &patch);
}

void ASMPatchesForCustomItemPools()
{
    ASMPatchFirstPickCollectible();
    ASMPatchRetryPickCollectible();
    ASMPatchPreTriggerGenesis();
    ASMPatchAddGenesisPoolWeight();
    ASMPatchDecreaseRemainingGenesisItems();
}

LUA_FUNCTION(Lua_CustomItemPoolGetPoolIdByName)
{
    std::string poolName = luaL_checkstring(L, 1);
    lua_pushinteger(L, CustomItemPool::GetPoolIdByName(poolName));
    return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
    lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetPoolIdByName", Lua_CustomItemPoolGetPoolIdByName);
}