#pragma once

#include "IsaacRepentance.h"
#include "../Patches/XMLData.h"
#include "rapidxml.hpp"

/* There are several observed properties tied to a Boss / BossEntity that are not given proper support:
 *  - BossEntity::CanAddEncounter
 *      - Prevents the boss entity from being added when calling Game::AddEncounteredBoss
 *      - The 'nodelirium' flag can be used to achieve the same effect, albeit this would totally prevent
 *        the entity from being added to the list, while the flag simply makes it unused.
 *  - BossEntity::CanTriggerBossDeath
 *      - Prevents the entity from doing anything meaningful in Room::TriggerBossDeath
 *      - FLAG_DONT_COUNT_BOSS_HP can be used to achieve a similar effect.
 *      - Also sounds incredibly risky, given that this prevents the normal boss reward from being spawned if used incorrectly.
 *  - Boss::HasNoBossBar
 *  - Boss::CustomVersusScreen
 */

/* StageAPI adds a few callbacks related to bosses, an attempt was made to provide similar callbacks
 * to make the transition simpler:
 *  - PRE_BOSS_SELECT -> MC_PRE_BOSS_SELECT
 *  - POST_SELECT_BOSS_MUSIC -> MC_GET_BOSS_MUSIC
 *  - PRE_STAGEAPI_SELECT_BOSS_ITEM -> MC_GET_BOSS_THEMATIC_ITEM
 */

/* TODO:
 *  - Implement AddBoss for Custom bosses
 *  - Choose what to display in the Dream Catcher sequence when a boss doesn't have a portrait.
 *      - The game defaults to the Larry Jr. portrait (death portrait 0 in bosses)
 *  - Save GameState's EncounteredBosses
 *  - Save GameState's RemovedBosses
 *  - Save Defeated Bosses.
 */

class BossManager
{
public:
    struct Data
    {
        std::unordered_map<std::string, int> bossMap;

        private: Data() = default;
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
        static void ApplyPatches();
    };

    static const EntityConfig_Boss* GetBossConfig(uint32_t bossId);
    static std::optional<uint32_t> GetBossIdByName(const std::string& name);
    static bool IsDoubleTroubleBossRoom(const RoomConfig_Room& room);
};

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