#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "BossManager.h"

#undef max

/// TODO
static void AddCustomBoss(PersistentGameData& persistentData, int bossID);
/// @brief resizes the boss bitsets to the proper size
static void BossPool_PostInit(BossPool& pool);

/** 
 * @brief Reimplementation of RestoreGameState to account for custom boss ids.
 * 
 * The game hardcodes the size of the removedBosses vector to NUM_BOSSES and only iterates
 * up to NUM_BOSSES, so we need to patch this behavior to account for custom bosses.
 * 
 * The function has been reimplemented due it's simplicity, and because it makes it
 * easier to maintain compared to using ASM patches.
 */
static void BossPool_RestoreGameState(BossPool& pool, const GameStateBossPool& state);

void AddCustomBoss(PersistentGameData &persistentData, int bossID)
{
    if (persistentData.readonly)
    {
        return;
    }

    // TODO
}

static void BossPool_PostInit(BossPool& pool)
{
    assert(pool._removedBosses.size() == NUM_BOSSES);
    assert(pool._levelBlacklist.size() == NUM_BOSSES);

    size_t numBosses = std::max((size_t)NUM_BOSSES, g_Manager->GetEntityConfig()->GetBosses()->size());
    pool._removedBosses.resize(numBosses, false);
    pool._levelBlacklist.resize(numBosses, false);
}

static void BossPool_RestoreGameState(BossPool& pool, const GameStateBossPool& state)
{
    size_t numBosses = g_Manager->GetEntityConfig()->GetBosses()->size();
    assert(state._removedBosses.size() == numBosses);

    // restore seeds
    for (size_t i = 0; i < NUM_STB; i++)
    {
        pool._pool[i]._rng = RNG(state._poolSeeds[i], 17);
    }

    // restore removed
    pool._removedBosses.resize(numBosses, false);
    pool._levelBlacklist.resize(numBosses, false);

    for (size_t i = 0; i < numBosses; i++)
    {
        pool._removedBosses[i] = state._removedBosses[i];
        pool._levelBlacklist[i] = false;
    }
}

#pragma region Patches

HOOK_METHOD(EntityConfig, LoadBosses, (char * xmlpath) -> void)
{
    super(xmlpath);
    BossManager::detail::LoadModBosses(*this);
}

HOOK_METHOD(PersistentGameData, AddBoss, (int bossID) -> void)
{
    if (bossID >= NUM_BOSSES)
    {
        AddCustomBoss(*this, bossID);
        return;
    }

    super(bossID);
}

HOOK_METHOD(BossPool, Init, (uint32_t seed) -> void)
{
    super(seed);
    BossPool_PostInit(*this);
}

HOOK_METHOD_PRIORITY(BossPool, RestoreGameState, 9999, (GameStateBossPool* state) -> void)
{
    BossPool_RestoreGameState(*this, *state);
}

#pragma endregion