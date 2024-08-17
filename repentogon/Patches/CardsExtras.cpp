#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "XMLData.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMPatches.h"
#include "Log.h"

#include <cstring>
#include <sstream>

#include "CardsExtras.h"

std::vector<ItemConfig_Card_EX> cardList_EX;
CardPool cardPools[NUM_CARDPOOLS];

void ItemConfig_Card_EX::ClearAvailabilityCondition(lua_State* L)
{
    if (this->availabilityFuncRef == LUA_NOREF)
        return;

    luaL_unref(L, LUA_REGISTRYINDEX, this->availabilityFuncRef);
    this->availabilityFuncRef = LUA_NOREF;
}

void ItemConfig_Card_EX::SetAvailabilityCondition(lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TFUNCTION);
    this->ClearAvailabilityCondition(L);
    lua_pushvalue(L, idx);
    this->availabilityFuncRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

void CardPool::Reset()
{
    this->cardList.clear();
    this->totalWeight = 0.0f;
    this->invalidateVanillaMethod = false;
}

void CardPool::AddCard(ItemConfig_Card* cardConfig)
{
    auto cardConfig_EX = Cards_EX::GetCardConfigEX(cardConfig);
    this->cardList.push_back(PoolCard(cardConfig->id, cardConfig_EX->weight));
    this->totalWeight += cardConfig_EX->weight;
    this->invalidateVanillaMethod |= cardConfig_EX->invalidateVanillaMethod;
}

void CardPool::AddCard(int cardId, float weight)
{
    this->cardList.push_back(PoolCard(cardId, weight));
    this->totalWeight += weight;
    this->invalidateVanillaMethod |= weight != 1.0f;
}

int CardPool::PickCard(RNG& rng)
{
    if (this->totalWeight == 0.0f) // indirectly handles the case where size == 0
    {
        return CARD_NULL;
    }

    if (!this->invalidateVanillaMethod)
    {
        return this->cardList[rng.RandomInt(this->cardList.size())].id;
    }

    float targetWeight = rng.RandomFloat() * this->totalWeight;

    float sumOfWeights = 0.0f;
    for (const auto& card : this->cardList)
    {
        sumOfWeights += card.weight;
        if (sumOfWeights > targetWeight)
        {
            return card.id;
        }
    }

    return CARD_NULL;
}

inline void PrintLuaError(std::string errorMessage)
{
    g_Game->GetConsole()->PrintError(errorMessage);
    KAGE::LogMessage(0, (errorMessage + "\n").c_str());
}

inline const char* GetTranslatedPocketItemName(const std::string& rawName)
{
    if (rawName.find("#") == std::string::npos)
    {
        return rawName.c_str();
    }
    uint32_t unk;
    const char* translatedString = g_Manager->GetStringTable()->GetString("PocketItems", 0, rawName.substr(1, rawName.length()).c_str(), &unk);
    if (strcmp(translatedString, "StringTable::InvalidKey") == 0)
    {
        return rawName.c_str();
    }

    return translatedString;
}

namespace Cards_EX
{
    inline void TryAddCardToPool(ItemConfig_Card* cardConfig, float specialChance, float runeChance, float suitChance, bool allowNonCards)
    {
        int cardPool = CARDPOOL_TAROT;
        switch (cardConfig->cardType)
        {
            case CARDTYPE_TAROT:
                cardPool = CARDPOOL_TAROT;
                break;
            case CARDTYPE_SUIT:
                if (suitChance == 0.0f)
                    return;
                cardPool = CARDPOOL_SUIT;
                break;
            case CARDTYPE_RUNE:
                if (runeChance == 0.0f)
                    return;
                cardPool = CARDPOOL_RUNE;
                break;
            case CARDTYPE_SPECIAL:
                if (specialChance == 0.0f)
                    return;
                cardPool = CARDPOOL_SPECIAL;
                break;
            case CARDTYPE_OBJECT:
                if (specialChance == 0.0f || !allowNonCards)
                    return;
                cardPool = CARDPOOL_SPECIAL;
                break;
            default:
                return;
        }
        cardPools[cardPool].AddCard(cardConfig);
    }

    inline bool IsChosenPool(RNG& rng, float chance)
    {
        if (chance == 0.0f)
            return false;

        float reciprocal = 1.0f / chance;
        int integerChance = (int)std::round(reciprocal);

        if ((1.0f / integerChance) == chance)
            return rng.RandomInt(integerChance) == 0; // if the chance can be represented as 1/integer then use the vanilla method for compatibility

        return rng.RandomFloat() < chance;
    }

    inline int PickCard(RNG& rng, float specialChance, float runeChance, float suitChance, bool allowNonCards)
    {
        if (IsChosenPool(rng, specialChance) && cardPools[CARDPOOL_SPECIAL].cardList.size() != 0)
        {
            int specialCard = cardPools[CARDPOOL_SPECIAL].PickCard(rng);
            if (specialCard != CARD_NULL)
            {
                return specialCard;
            }
        }

        if (IsChosenPool(rng, runeChance) && cardPools[CARDPOOL_RUNE].cardList.size() != 0)
        {
            int rune = cardPools[CARDPOOL_RUNE].PickCard(rng);
            if (rune != CARD_NULL)
            {
                return rune;
            }

            if (runeChance == 1.0f || rng.RandomInt(5) == 0)
            {
                return RUNE_SHARD;
            }
        }

        if (IsChosenPool(rng, suitChance) && cardPools[CARDPOOL_SUIT].cardList.size() != 0)
        {
            int suitCard = cardPools[CARDPOOL_SUIT].PickCard(rng);
            if (suitCard != CARD_NULL)
            {
                return suitCard;
            }
        }

        return cardPools[CARDPOOL_TAROT].PickCard(rng);
    }

    int GetCard(int seed, float specialChance, float runeChance, float suitChance, bool onlyRunes, bool allowNonCards)
    {
        const auto itemConfig = g_Manager->GetItemConfig();
        const auto cardList = itemConfig->GetCards();

        for (size_t i = CARDPOOL_TAROT; i < NUM_CARDPOOLS; i++)
        {
            cardPools[i].Reset();
        }

        for (size_t i = CARD_FOOL; i < cardList->size(); i++)
        {
            if (i == RUNE_SHARD)
            {
                continue;
            }

            const auto cardConfig = (*cardList)[i];
            if (cardConfig == nullptr)
            {
                continue;
            }

            const auto cardConfig_EX = GetCardConfigEX(cardConfig);
            if (cardConfig->IsAvailable())
            {
                TryAddCardToPool(cardConfig, specialChance, runeChance, suitChance, allowNonCards);
            }
        }

        CardPool& runePool = cardPools[CARDPOOL_RUNE];
        if ((runePool.cardList.size() != 0 || onlyRunes))
        {
            while (runePool.cardList.size() < 6)
            {
                runePool.AddCard(CARD_NULL, 1.0f);
            }
        }

        RNG rng = RNG();
        rng.SetSeed(seed, 21);

        int pickedCard = PickCard(rng, specialChance, runeChance, suitChance, allowNonCards);

        if (pickedCard >= CARD_FOOL && pickedCard <= CARD_WORLD) // IsMajorArcanaCard
        {
            int reverseCard = pickedCard + (CARD_REVERSE_FOOL - 1);
            if (rng.RandomInt(7) == 0 && reverseCard < (int)cardList->size() && (*cardList)[reverseCard]->IsAvailable())
            {
                pickedCard = reverseCard;
            }
        }

        if (pickedCard == CARD_NULL)
        {
            KAGE::LogMessage(3, "No unlocked cards/runes to pick from!\n");
        }

        if (specialChance != 0 && g_Game->_playerManager.FirstPlayerByType(PLAYER_THELOST_B) != nullptr)
        {
            RNG holyRng = RNG();
            holyRng.SetSeed(rng._seed, 47);

            if (holyRng.RandomInt(10) == 0)
            {
                return CARD_HOLY;
            }
        }

        return pickedCard;
    }
}

const char* xmlParseErrorPrefix = "[Pocket Items] Error whilst Parsing Card XML";

inline void ParseCardNode(int cardId, int nodeId)
{
    XMLCard* xmlCardData = XMLStuff.CardData;
    if (xmlCardData->nodes.find(nodeId) == xmlCardData->nodes.end())
    {
        return;
    }

    const XMLAttributes attributes = XMLStuff.CardData->nodes.at(nodeId);
    auto& cardConfig_EX = cardList_EX[cardId];

    if (attributes.find("weight") != attributes.end())
    {
        std::string weight_string = attributes.at("weight");
        try {
            cardConfig_EX.initialWeight = std::stof(weight_string);
            cardConfig_EX.initialWeight = max(cardConfig_EX.initialWeight, 0.0f);
            cardConfig_EX.weight = cardConfig_EX.initialWeight;
            cardConfig_EX.invalidateVanillaMethod = cardConfig_EX.weight != 1.0f;
        }
        catch (const std::invalid_argument &) {
            ZHL::Log("%s: Invalid argument for <weight> (%s is not a valid float)\n", xmlParseErrorPrefix, weight_string.c_str());
        }
        catch (const std::out_of_range &) {
            ZHL::Log("%s: Invalid argument for <weight> (%s is out of range)\n", xmlParseErrorPrefix, weight_string.c_str());
        }
    }

    cardConfig_EX.hidden = (attributes.find("hidden") != attributes.end()) && (attributes.at("hidden") == "true");
}

inline void ParseBaseGameCards()
{
    for (int i = 0; i < NUM_CARDS; i++)
    {
        auto node = XMLStuff.CardData->GetNodeById(i);
        if (node.begin() == node.end()) {
            continue;
        }

        ParseCardNode(i, i);
    }
}

inline void ParseModCards(int startCardId, ModEntry* modEntry)
{
    XMLCard* xmlCardData = XMLStuff.CardData;
    std::string modId = modEntry->GetId();
    modId = modId.length() == 0 ? modEntry->GetDir() : modId;

    std::vector<int> modCards = xmlCardData->bymod[modId];
    if (modCards.size() == 0)
    {
        return;
    }
    int startNodeId = modCards[0];
    
    for (int i = startCardId, j = startNodeId; i < (int)g_Manager->GetItemConfig()->GetCards()->size() && j <= xmlCardData->maxid; i++, j++)
    {
        ParseCardNode(i, j);
    }
}

HOOK_METHOD(ItemConfig, LoadPocketItems, (char* xmlPath, ModEntry* modEntry) -> void)
{
    int startCardId = g_Manager->GetItemConfig()->GetCards()->size();

    super(xmlPath, modEntry);

    cardList_EX.resize(g_Manager->GetItemConfig()->GetCards()->size());
    if (modEntry == nullptr)
    {
        ParseBaseGameCards();
    }
    else
    {
        ParseModCards(startCardId, modEntry);
    }    
}

HOOK_METHOD(ItemConfig, Unload, () -> void)
{
    super();
    cardList_EX.clear();
}

HOOK_METHOD(ItemConfig_Card, IsAvailable, () -> bool)
{
    if (!super())
        return false;

    auto cardConfig_EX = Cards_EX::GetCardConfigEX(this);

    if (cardConfig_EX->hidden)
        return false;

    if (g_LuaEngine != nullptr && cardConfig_EX->availabilityFuncRef != LUA_NOREF)
    {
        lua_State* L = g_LuaEngine->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, cardConfig_EX->availabilityFuncRef);

        if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        {
            std::stringstream err;
            err << "Error whilst checking availability of card \"" << GetTranslatedPocketItemName(this->name) << "\": " << lua_tostring(L, -1);
            PrintLuaError(err.str());

            return false;
        }

        bool isAvailable = lua_isboolean(L, -1) && lua_toboolean(L, -1);
        lua_pop(L, 1);

        if (!isAvailable)
            return false;
    }

    return true;
}

inline float ToFloatChance(int chance)
{
    if (chance == 0)
        return 0.0f;
    
    return 1.0f / std::abs(chance);
}

HOOK_STATIC(ItemPool, GetCardEx, (int seed, int specialChance, int runeChance, int suitChance, bool allowNonCards) -> int, __stdcall)
{
    return Cards_EX::GetCard(seed, ToFloatChance(specialChance), ToFloatChance(runeChance), ToFloatChance(suitChance), runeChance == -1, allowNonCards);
}

// Edit the vanilla function so that when OnlyRunes is set to true the RuneChance is set to -1 instead of 1
void ASMPatchGetCard()
{
    SigScan scanner("8d4f??eb??33c9");
    scanner.Scan();
    void* setOnlyRunesChanceSig = scanner.GetAddress();

    ZHL::Log("[REPENTOGON] Patching ItemPool::GetCard at %p\n", setOnlyRunesChanceSig);

    ASMPatch patch;
    patch.AddBytes("\x8D\x4F\xF5");
    sASMPatcher.FlatPatch(setOnlyRunesChanceSig, &patch, true);
}

void ASMPatchesForCardsExtras()
{
    ASMPatchGetCard();
}