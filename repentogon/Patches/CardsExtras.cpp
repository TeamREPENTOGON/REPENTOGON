#include <cstring>
#include <sstream>

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "XMLData.h"
#include "HookSystem.h"
#include "ASMPatcher.hpp"
#include "ASMPatches.h"
#include "Log.h"

#include "AchievementsStuff.h"
#include "CardsExtras.h"

std::vector<ItemConfig_Card_EX> CardsEX::g_CardConfigsEX;
CardPool CardsEX::g_CardPools[NUM_CARDPOOLS];

static inline void print_lua_error(std::string errorMessage)
{
    g_Game->GetConsole()->PrintError(errorMessage);
    KAGE::LogMessage(0, (errorMessage + "\n").c_str());
}

static inline const char* get_localized_pocket_item_name(const std::string& rawName)
{
    if (rawName.find("#") == std::string::npos)
    {
        return rawName.c_str();
    }
    bool unk;
    const char* translatedString = g_Manager->GetStringTable()->GetString("PocketItems", 0, rawName.substr(1, rawName.length()).c_str(), &unk);
    if (strcmp(translatedString, "StringTable::InvalidKey") == 0)
    {
        return rawName.c_str();
    }

    return translatedString;
}

CardChance::CardChance(int chance)
{
    this->chance.intChance = chance;
    this->isFloat = false;
}

CardChance::CardChance(float chance)
{
    this->chance.floatChance = chance;
    this->isFloat = true;
}

bool CardChance::Check(RNG& rng) const
{
    if (this->IsZero())
    {
        return false;
    }

    if (this->isFloat)
    {
        return rng.RandomFloat() < chance.floatChance;
    }
    else
    {
        return rng.RandomInt(chance.intChance) == 0;
    }
}

bool CardChance::IsGuaranteed() const
{
    if (this->isFloat)
    {
        return this->chance.floatChance == 1.0f;
    }
    return this->chance.intChance == 1;
}

static inline bool is_card_available_ex(ItemConfig_Card* card)
{
    auto cardConfig_EX = CardsEX::GetCardConfigEX(card);

    if (cardConfig_EX->hidden)
        return false;

    if (cardConfig_EX->availabilityFuncRef != LUA_NOREF) // checking for g_LuaEngine != nullptr is unnecessary since a function can only be set after it is initialized
    {
        lua_State* L = g_LuaEngine->_state;
        lua_rawgeti(L, LUA_REGISTRYINDEX, cardConfig_EX->availabilityFuncRef);

        if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        {
            std::stringstream err;
            err << "Error whilst checking availability of card \"" << get_localized_pocket_item_name(card->name) << "\": " << lua_tostring(L, -1);
            print_lua_error(err.str());
            lua_pop(L, 1);
        }
        else
        {
            bool isAvailable = lua_isboolean(L, -1) && lua_toboolean(L, -1);
            lua_pop(L, 1);

            if (!isAvailable)
                return false;
        }

    }

    return true;
}

ItemConfig_Card_EX::~ItemConfig_Card_EX()
{
    if (g_LuaEngine)
    {
        this->ClearAvailabilityCondition(g_LuaEngine->_state);
    }
}

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

void CardPool::ClearPool()
{
    this->cardList.clear();
    this->totalWeight = 0.0f;
    this->invalidateVanillaMethod = false;
}

void CardPool::AddCandidate(int cardId)
{
    this->candidateCards.push_back(cardId);
}

void CardPool::BuildPool()
{
    this->ClearPool();

    ItemConfig* itemConfig = g_Manager->GetItemConfig();
    for (int candidate : this->candidateCards)
    {
        ItemConfig_Card* cardConfig = itemConfig->GetCard(candidate);
        if (cardConfig->IsAvailable())
        {
            this->AddCard(cardConfig);
        }
    }
}

void CardPool::BuildSpecialPool(bool allowNonCards)
{
    this->ClearPool();

    ItemConfig* itemConfig = g_Manager->GetItemConfig();
    for (int candidate : this->candidateCards)
    {
        ItemConfig_Card* cardConfig = itemConfig->GetCard(candidate);
        if (cardConfig->cardType == CARDTYPE_OBJECT && !allowNonCards)
        {
            continue;
        }

        if (cardConfig->IsAvailable())
        {
            this->AddCard(cardConfig);
        }
    }
}

void CardPool::AddCard(ItemConfig_Card* cardConfig)
{
    auto cardConfig_EX = CardsEX::GetCardConfigEX(cardConfig);
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

int CardPool::PickCard(RNG& rng) const
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

namespace CardsEX
{
    static inline int pick_card(RNG& rng, CardChance specialChance, CardChance runeChance, CardChance suitChance, bool onlyRunes, bool allowNonCards)
    {
        if (specialChance.Check(rng))
        {
            CardPool& specialPool = g_CardPools[CARDPOOL_SPECIAL];
            specialPool.BuildSpecialPool(allowNonCards);
            int specialCard = specialPool.PickCard(rng);

            if (specialCard != CARD_NULL)
            {
                return specialCard;
            }
        }

        if (runeChance.Check(rng))
        {
            CardPool& runePool = g_CardPools[CARDPOOL_RUNE];
            runePool.BuildPool();

            if ((runePool.cardList.size() != 0 || onlyRunes))
            {
                while (runePool.cardList.size() < 6)
                {
                    runePool.AddCard(CARD_NULL, 1.0f);
                }
            }

            int rune = runePool.PickCard(rng);

            if (rune != CARD_NULL)
            {
                return rune;
            }

            if (runeChance.IsGuaranteed() || rng.RandomInt(5) == 0)
            {
                return RUNE_SHARD;
            }
        }

        if (suitChance.Check(rng))
        {
            CardPool& suitPool = g_CardPools[CARDPOOL_SUIT];
            suitPool.BuildPool();
            int suitCard = suitPool.PickCard(rng);

            if (suitCard != CARD_NULL)
            {
                return suitCard;
            }
        }

        CardPool& tarotPool = g_CardPools[CARDPOOL_TAROT];
        tarotPool.BuildPool();
        return tarotPool.PickCard(rng);
    }

    static int get_card(int seed, CardChance specialChance, CardChance runeChance, CardChance suitChance, bool onlyRunes, bool allowNonCards)
    {
        const auto itemConfig = g_Manager->GetItemConfig();
        const auto cardList = itemConfig->GetCards();

        RNG rng = RNG();
        rng.SetSeed(seed, 21);

        int pickedCard = pick_card(rng, specialChance, runeChance, suitChance, onlyRunes, allowNonCards);

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

        if (!specialChance.IsZero() && g_Game->_playerManager.FirstPlayerByType(PLAYER_THELOST_B) != nullptr)
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

static inline void try_add_card_to_pool(ItemConfig_Card* cardConfig)
{
    if (cardConfig->id == CARD_NULL || cardConfig->id == RUNE_SHARD)
    {
        return;
    }

    switch (cardConfig->cardType)
    {
    case CARDTYPE_TAROT:
        CardsEX::g_CardPools[CARDPOOL_TAROT].AddCandidate(cardConfig->id);
        break;
    case CARDTYPE_SUIT:
        CardsEX::g_CardPools[CARDPOOL_SUIT].AddCandidate(cardConfig->id);
        break;
    case CARDTYPE_RUNE:
        CardsEX::g_CardPools[CARDPOOL_RUNE].AddCandidate(cardConfig->id);
        break;
    case CARDTYPE_SPECIAL:
    case CARDTYPE_OBJECT:
        CardsEX::g_CardPools[CARDPOOL_SPECIAL].AddCandidate(cardConfig->id);
        break;
    default:
        return;
    }
}

static inline void parse_card_node(int cardId, int nodeId)
{
    XMLCard* xmlCardData = XMLStuff.CardData;
    if (xmlCardData->nodes.find(nodeId) == xmlCardData->nodes.end())
    {
        return;
    }

    const XMLAttributes attributes = XMLStuff.CardData->nodes.at(nodeId);
    auto& cardConfig_EX = CardsEX::g_CardConfigsEX[cardId];

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
    try_add_card_to_pool(g_Manager->GetItemConfig()->GetCard(cardId));
}

static inline void parse_base_game_cards()
{
    for (int i = 0; i < NUM_CARDS; i++)
    {
        auto node = XMLStuff.CardData->GetNodeById(i);
        if (node.begin() == node.end()) {
            continue;
        }

        parse_card_node(i, i);
    }
}

static inline void parse_mod_cards(int startCardId, ModEntry* modEntry)
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
        parse_card_node(i, j);
    }
}

HOOK_METHOD(ItemConfig, LoadPocketItems, (char* xmlPath, ModEntry* modEntry) -> void)
{
    int startCardId = g_Manager->GetItemConfig()->GetCards()->size();

    super(xmlPath, modEntry);

    CardsEX::g_CardConfigsEX.resize(g_Manager->GetItemConfig()->GetCards()->size());
    if (modEntry == nullptr)
    {
        parse_base_game_cards();
    }
    else
    {
        parse_mod_cards(startCardId, modEntry);
    }    
}

HOOK_METHOD(ItemConfig, Unload, () -> void)
{
    super();
    CardsEX::g_CardConfigsEX.clear();
    for (size_t i = CARDPOOL_TAROT; i < NUM_CARDPOOLS; i++)
    {
        CardsEX::g_CardPools[i].candidateCards.clear();
    }
}

HOOK_METHOD(ItemConfig_Card, IsAvailable, () -> bool)
{
    return super() && is_card_available_ex(this);
}

HOOK_STATIC(ItemPool, GetCardEx, (int seed, int specialChance, int runeChance, int suitChance, bool allowNonCards) -> int, __stdcall)
{
    bool onlyRunes = runeChance == -1;
    runeChance = runeChance == -1 ? 1 : runeChance;
    return CardsEX::get_card(seed, specialChance, runeChance, suitChance, onlyRunes, allowNonCards);
}

// Edit the vanilla function so that when OnlyRunes is set to true the RuneChance is set to -1 instead of 1
static void asm_patch_get_card()
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
    asm_patch_get_card();
}