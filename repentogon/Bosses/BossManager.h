#pragma once

#include "IsaacRepentance.h"
#include "../Patches/XMLData.h"
#include "rapidxml.hpp"

class BossManager
{
public:
    struct Data
    {
        std::unordered_map<std::string, uint32_t> bossMap;

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