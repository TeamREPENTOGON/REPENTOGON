#include "BossManager.h"
#include "../Utils/StringUtils.h"
#include "../MiscFunctions.h"
#include "document.h" // rapidjson
#include "prettywriter.h" // rapidjson
#include "stringbuffer.h" // rapidjson

static uint32_t PERSISTENT_GAME_DATA_VERSION = 1;
static uint32_t GAME_STATE_VERSION = 1;

static std::string get_persistent_game_data_path(uint32_t saveSlot)
{
    return REPENTOGON::StringConcat(REPENTOGON::GetRepentogonDataPath(), "BossManager/persistentgamedata", to_string(saveSlot), ".json");
}

void BossManager::detail::ClearPersistentGameData()
{
    auto& bosses = BossManager::m_data.defeatedCustomBosses;
    for (size_t i = 0; i < bosses.size(); i++)
    {
        bosses[i] = false;
    }

    BossManager::m_data.persistentBossData.clear();
}

void BossManager::detail::SavePersistentGameData(uint32_t saveSlot)
{
    auto& data = BossManager::m_data;
    auto savePath = get_persistent_game_data_path(saveSlot);

    if (!REPENTOGON::EnsureParentPathExists(savePath))
    {
        ZHL::Log("[ERROR] [BossManager] - Unable to save Persistent Game Data to '%s': failed to create parent path", savePath.c_str());
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

    writer.StartObject(); // doc
    
    writer.Key("version");
    writer.Uint(PERSISTENT_GAME_DATA_VERSION);

    writer.Key("entries");
    writer.StartArray(); // entries

    for (size_t i = 0; i < data.defeatedCustomBosses.size(); i++)
    {
        if (data.defeatedCustomBosses[i])
        {
            writer.StartArray(); // entry

            const auto& config = BossManager::GetBossConfig(i + NUM_BOSSES);
            writer.String(config->name.c_str(), config->name.size());
            writer.Bool(true);

            writer.EndArray(); // entry
        }
    }

    for (size_t i = 0; i < data.persistentBossData.size(); i++)
    {
        writer.StartArray(); // entry

        auto& entry = data.persistentBossData[i];
        writer.String(entry.name.c_str(), entry.name.size());
        writer.Bool(entry.defeated);

        writer.EndArray(); // entry
    }

    writer.EndArray(); // entries

    writer.EndObject(); // doc

    std::string tmpSave = REPENTOGON::StringConcat(savePath, ".tmp");
    {
        std::ofstream ofs(tmpSave, std::ios::binary);
        if (!ofs)
        {
            ZHL::Log("[ERROR] [BossManager] - Unable to save Persistent Game Data to '%s': failed to open file for writing", savePath.c_str());
            return;
        }

        ofs.write(buffer.GetString(), buffer.GetSize());
        ofs.flush();
        if (!ofs)
        {
            ofs.close();
            ZHL::Log("[ERROR] [BossManager] - Unable to save Persistent Game Data to '%s': unable to write data", savePath.c_str());
            std::filesystem::remove(tmpSave, std::error_code());
            return;
        }
    }

    std::error_code ec;
    std::filesystem::rename(tmpSave, savePath, ec);
    if (ec)
    {
        std::filesystem::remove(tmpSave, std::error_code());   // rename failed: don't leave the temp behind
        ZHL::Log("[ERROR] [BossManager] - Unable to save Persistent Game Data to '%s': unable to rename temporary", savePath.c_str());
        return;
    }

    ZHL::Log("[INFO] [BossManager] - Saved Persistent Game Data to '%s'", savePath.c_str());
}

void BossManager::detail::LoadPersistentGameData(uint32_t saveSlot)
{
    BossManager::detail::ClearPersistentGameData();

    if (saveSlot == 0)
    {
        return;
    }

    auto& data = BossManager::m_data;
    auto loadPath = get_persistent_game_data_path(saveSlot);

    // Read the whole file
    std::string text;
    {
        std::ifstream ifs(loadPath, std::ios::binary);
        if (!ifs)
        {
            ZHL::Log("[INFO] [BossManager] - No Persistent Game Data at '%s'", loadPath.c_str());
            return;
        }
        text.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }

    rapidjson::Document doc;
    doc.Parse(text.c_str());
    if (doc.HasParseError() || !doc.IsObject())
    {
        ZHL::Log("[ERROR] [BossManager] - Unable to load Persistent Game Data from '%s': unable to parse json", loadPath.c_str());
        return;
    }

    auto version = doc.FindMember("version");
    if (version == doc.MemberEnd() || !version->value.IsUint() || version->value.GetUint() != PERSISTENT_GAME_DATA_VERSION)
    {
        ZHL::Log("[ERROR] [BossManager] - Unable to load Persistent Game Data from '%s': missing or unsupported version", loadPath.c_str());
        return;
    }

    auto entries = doc.FindMember("entries");
    if (entries == doc.MemberEnd() || !entries->value.IsArray())
    {
        ZHL::Log("[ERROR] [BossManager] - Unable to load Persistent Game Data from '%s': missing 'entries' array", loadPath.c_str());
        return;
    }

    for (const auto& entry : entries->value.GetArray())
    {
        if (!entry.IsArray() || entry.Size() != 2)
        {
            ZHL::Log("[WARN] [BossManager] - Skipping malformed entry in '%s'", loadPath.c_str());
            continue;
        }

        auto& json_name = entry[0];
        auto& json_defeated = entry[1];
    
        if (!json_name.IsString() || !json_defeated.IsBool())
        {
            ZHL::Log("[WARN] [BossManager] - Skipping malformed entry in '%s'", loadPath.c_str());
            continue;
        }

        const std::string_view name(json_name.GetString(), json_name.GetStringLength());
        const bool defeated = json_defeated.GetBool();
    
        auto bossId = BossManager::GetBossIdByName(std::string(name));
        if (!bossId)
        {
            data.persistentBossData.emplace_back(name, defeated);
            continue;
        }

        if (*bossId < NUM_BOSSES)
        {
            ZHL::Log("[WARN] [BossManager] - Entry refers to vanilla boss in '%s'", loadPath.c_str());
            continue;
        }

        size_t customBossId = *bossId - NUM_BOSSES;
        data.defeatedCustomBosses[customBossId] = defeated;
    }

    ZHL::Log("[INFO] [BossManager] - Loaded Persistent Game Data from '%s'", loadPath.c_str());
}