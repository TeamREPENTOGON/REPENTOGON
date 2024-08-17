#include "IsaacRepentance.h"
#include "LuaCore.h"

enum eCardPool
{
    CARDPOOL_TAROT = 0,
    CARDPOOL_SUIT = 1,
    CARDPOOL_RUNE = 2,
    CARDPOOL_SPECIAL = 3,

    NUM_CARDPOOLS
};

struct ItemConfig_Card_EX
{
    bool hidden = false;
    bool invalidateVanillaMethod = false;
    float initialWeight = 1.0f;
    float weight = 1.0f;
    int availabilityFuncRef = LUA_NOREF;

    void ClearAvailabilityCondition(lua_State* L);
    void SetAvailabilityCondition(lua_State* L, int idx);
};

struct PoolCard
{
    int id = CARD_NULL;
    float weight = 1.0f;

    PoolCard(int id, float weight) : id(id), weight(weight) {}
};

struct CardPool
{
    std::vector<PoolCard> cardList;
    float totalWeight = 0.0f;
    bool invalidateVanillaMethod = false; // Use the old method for picking cards if all weights are "vanilla"

    void Reset();
    void AddCard(ItemConfig_Card* cardConfig);
    void AddCard(int cardId, float weight);
    int PickCard(RNG& rng);
};

extern std::vector<ItemConfig_Card_EX> cardList_EX;
extern CardPool cardPools[NUM_CARDPOOLS];

namespace Cards_EX
{
    inline ItemConfig_Card_EX* GetCardConfigEX(ItemConfig_Card* cardConfig)
    {
        return &cardList_EX[cardConfig->id];
    }
}

void ASMPatchesForCardsExtras();