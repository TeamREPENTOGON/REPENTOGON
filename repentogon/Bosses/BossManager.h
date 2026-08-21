#pragma once

#include "IsaacRepentance.h"

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