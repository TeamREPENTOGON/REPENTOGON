#pragma once

#include "IsaacRepentance.h"
#include "../Patches/XMLData.h"
#include "../SaveStateManagement/GameStateManagement.h"
#include "rapidxml.hpp"

/* There are several observed properties tied to a Boss / BossEntity that are not given proper support:
 *  - BossEntity::CanAddEncounter
 *      - Prevents the boss entity from being added when calling Game::AddEncounteredBoss
 *      - The 'nodelirium' flag can be used to achieve the same effect, albeit this would totally prevent
 *        the entity from being added to the list, while the flag simply makes it unused.
 *  - BossEntity::CanTriggerBossDeath
 *      - Prevents the entity from doing anything meaningful in Room::TriggerBossDeath
 *      - FLAG_DONT_COUNT_BOSS_HP can be used to achieve a similar effect.
 *      - Also sounds risky, given that this prevents the normal boss reward from being spawned if used incorrectly.
 *  - Boss::HasNoBossBar
 *  - Boss::CustomVersusScreen
 *      - MC_POST_BOSS_INTRO_SHOW can be used to achieve a similar effect.
 */

/* StageAPI adds a few callbacks related to bosses, an attempt was made to provide similar callbacks
 * to make the transition simpler:
 *  - PRE_BOSS_SELECT -> MC_PRE_BOSS_SELECT
 *  - POST_SELECT_BOSS_MUSIC -> MC_GET_BOSS_MUSIC
 *  - PRE_STAGEAPI_SELECT_BOSS_ITEM -> MC_GET_BOSS_THEMATIC_ITEM
 */

/* Known issues:
 * - If a mod doesn't provide a portrait for a boss Larry Jr. will appear in the dream catcher sequence.
 *      - This is because Larry Jr. is the portrait located at frame 0 of the Bosses animation, and is the de facto default portrait.
 */

/* TODO:
 *  - Add modded support for DoubleTrouble rooms.
 */

class BossManager
{
private:
    struct PersistentBossData
    {
        std::string name;
        bool defeated = false;

        PersistentBossData(std::string_view name, bool defeated)
            : name(name), defeated(defeated) {}
    };

public:
    struct Data
    {
        std::unordered_map<std::string, int> bossMap;
        /// @brief an extension of the PersistentData::_bosses boolean array for custom bosses
        ///
        /// WARNING: It's size needs to correctly reflect the amount of custom bosses in the game.
        std::vector<bool> defeatedCustomBosses;
        /// @brief persistent data for bosses that don't exist in current session.
        std::vector<PersistentBossData> persistentBossData;

        private: Data() = default;
        Data(const Data&) = delete;
        Data& operator=(const Data&) = delete;
        friend BossManager;
    };

private:
    inline static Data m_data{};

public:
    class detail
    {
    public:
        static void LoadModBosses(EntityConfig& config);
        /// @brief Loads all custom bosses .stb, and adds the rooms to the STB_SPECIAL_ROOMS set.
        static void LoadCustomBossRooms();
        /// @brief parses the entity's bossid attribute and patches it's value, so the game's parser can automatically handle it.
        static void PatchEntityBossId(xml_document<char>& doc, xml_node<char>& node, XMLAttributes& entity);
        static void ClearPersistentGameData();
        static void SavePersistentGameData(uint32_t saveSlot);
        static void LoadPersistentGameData(uint32_t saveSlot);
        /// @brief Saves the custom boss ids used in the GameState using a persistent id (name)
        static void SaveGameState(const GameStateSaveInfo& info, bool isRerun);
        /// @brief Remaps values tied to a boss id to the new boss ids.
        static void LoadGameState(const GameStateSaveInfo& info, bool isRerun);
        static void DeleteGameState(const GameStateSaveInfo& info, bool isRerun);
        static void ApplyPatches();
        static void AddCustomBossDefeated(uint32_t bossId);
    };

    static size_t GetNumBosses();
    static uint32_t ToCustomBossId(uint32_t bossId);
    static const EntityConfig_Boss* GetBossConfig(uint32_t bossId);
    static std::optional<uint32_t> GetBossIdByName(const std::string& name);
    static bool IsDoubleTroubleBossRoom(const RoomConfig_Room& room);
    static void AddBossDefeated(uint32_t bossId);
    static bool IsBossDefeated(uint32_t bossId);
    static bool WasBossRemoved(uint32_t bossId);
};

inline size_t BossManager::GetNumBosses()
{
    size_t numBosses = g_Manager->GetEntityConfig()->GetBosses()->size();
    assert(numBosses >= NUM_BOSSES); // most code expects the amount of bosses to be at least NUM_BOSSES
    return numBosses;
}

inline uint32_t BossManager::ToCustomBossId(uint32_t bossId)
{
    assert(NUM_BOSSES <= bossId && bossId < BossManager::GetNumBosses());
    return bossId - NUM_BOSSES;
}

inline void BossManager::detail::AddCustomBossDefeated(uint32_t bossId)
{
    assert(NUM_BOSSES <= (size_t)bossId && (size_t)bossId < BossManager::GetNumBosses());

    if (!g_Manager->_persistentGameData.readonly)
    {
        BossManager::m_data.defeatedCustomBosses[BossManager::ToCustomBossId(bossId)] = true;
        BossManager::detail::SavePersistentGameData(g_Manager->_currentSaveSlot);
    }
}

inline const EntityConfig_Boss* BossManager::GetBossConfig(uint32_t bossId)
{
    const auto& bosses = g_Manager->GetEntityConfig()->_bosses;
    if (bossId >= bosses.size())
    {
        return nullptr;
    }

    return &bosses[bossId];
}

inline std::optional<uint32_t> BossManager::GetBossIdByName(const std::string& name)
{
    auto& map = BossManager::m_data.bossMap;
    auto it = map.find(name);
    if (it != map.end()) { return it->second; }
    return std::nullopt;
}

inline bool BossManager::IsDoubleTroubleBossRoom(const RoomConfig_Room& room)
{
    // game implementation, though we will definitely want to change it when adding proper modded support.
    return room.Type == eRoomType::ROOM_BOSS
        && (RoomConfig_Room::BOSS_DOUBLE_TROUBLE_START <= room.Variant && room.Variant < RoomConfig_Room::BOSS_DOUBLE_TROUBLE_END);
}

inline void BossManager::AddBossDefeated(uint32_t bossId)
{
    assert(0 <= (size_t)bossId && (size_t)bossId < BossManager::GetNumBosses());
    g_Manager->_persistentGameData.AddBoss(bossId);
}

inline bool BossManager::IsBossDefeated(uint32_t bossId)
{
    assert(0 <= (size_t)bossId && (size_t)bossId < BossManager::GetNumBosses());
    if (bossId < NUM_BOSSES)
    {
        return g_Manager->_persistentGameData.bosses[bossId];
    }
    else
    {
        return BossManager::m_data.defeatedCustomBosses[BossManager::ToCustomBossId(bossId)];
    }
}

inline bool BossManager::WasBossRemoved(uint32_t bossId)
{
    assert(0 <= (size_t)bossId && (size_t)bossId < BossManager::GetNumBosses());
    return g_Game->_bossPool.WasBossRemoved(bossId);
}