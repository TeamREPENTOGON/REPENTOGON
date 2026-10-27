#include "BossManager.h"
#include "../VirtualRoomConfig/VirtualRoomSetManager.h"
#include "../RoomConfigUtility.h"
#include "../Utils/Array.h"

/// @brief Loads, finalizes and adds the rooms from the passed stb.
static void load_stb_custom_bosses(VirtualRoomSet::ScopedRoomLoader& loader, const std::string& filePath, const XMLBossPortrait::BossStbDesc& desc, const ModEntry& mod);

static void load_stb_custom_bosses(VirtualRoomSet::ScopedRoomLoader& loader, const std::string& filePath, const XMLBossPortrait::BossStbDesc& desc, const ModEntry& mod)
{
    bool error = false;

    auto stbRooms = RoomConfigUtility::ParseStb(filePath, STB_SPECIAL_ROOMS);
    if (!stbRooms) { return; }

    auto rooms = REPENTOGON::Array(stbRooms->roomSet._configs, stbRooms->roomSet._count);
    for (size_t i = 0; i < rooms.size(); i++)
    {
        RoomConfig_Room& room = rooms[i];
        if (room.Type != eRoomType::ROOM_BOSS)
        {
            error = true;
            ZHL::Log("[ERROR] - room '%s' (variant: %d) is not a boss room in '%s'\n", room.Name.c_str(), room.Variant, filePath.c_str());
            continue;
        }

        uint32_t relativeId = room.Subtype;
        auto it = desc.idMap.find(relativeId);
        if (it == desc.idMap.end())
        {
            error = true;
            ZHL::Log("[ERROR] - unable to map boss %d in '%s'\n", room.Subtype, filePath.c_str());
            continue;
        }

        room.Subtype = it->second;
        loader.AddRoom(room);
    }

    if (error)
    {
        g_Game->GetConsole()->PrintError(REPENTOGON::StringFormat("[ERROR] Something went wrong while parsing '%s' from mod '%s'\n", filePath.c_str(), mod._name.c_str()));
    }
}

void BossManager::detail::LoadCustomBossRooms()
{
    auto set = VirtualRoomSetManager::GetSet(STB_SPECIAL_ROOMS, 0);
    
    {
        auto loader = set.LoadRooms();

        const auto& stbDescs = XMLStuff.BossPortraitData->stbDescs;
        for (size_t i = 0; i < stbDescs.size(); i++)
        {
            const auto& bossStbDesc = stbDescs[i];
            auto contentPaths = REPENTOGON::GetAllModContentPaths("rooms/" + bossStbDesc.filePath);
            for (auto& contentPath : contentPaths)
            {
                load_stb_custom_bosses(loader, contentPath.second, bossStbDesc, *contentPath.first);
            }
        }
    }
}