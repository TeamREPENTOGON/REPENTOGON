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
    };

    static std::optional<uint32_t> GetBossIdByName(const std::string& name);
};

inline std::optional<uint32_t> BossManager::GetBossIdByName(const std::string& name)
{
    auto& map = BossManager::m_data.bossMap;
    auto it = map.find(name);
    if (it != map.end()) { return it->second; }
    return std::nullopt;
}