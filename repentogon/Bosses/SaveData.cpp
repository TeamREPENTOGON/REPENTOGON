#include "BossManager.h"
#include "../Utils/Array.h"
#include "../Utils/StringUtils.h"
#include "../MiscFunctions.h"
#include "document.h" // rapidjson
#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson

#undef max

#define LOG_INFO_HEADER "[INFO] [BossManager] - "
#define LOG_WARN_HEADER "[WARN] [BossManager] - "
#define LOG_ERROR_HEADER "[ERROR] [BossManager] - "

static uint32_t PERSISTENT_GAME_DATA_VERSION = 1;
static uint32_t GAME_STATE_VERSION = 1;

#pragma region Declarations

static std::string get_persistent_game_data_path(uint32_t saveSlot);
static std::string get_game_state_path(const GameStateSaveInfo& info);

/// @brief Loads the id map from the specified json path.
/// @param idMap reperesents a map that ties newCustomId -> gameStateCustomId.
/// It is expected have size() == numCustomBosses and have all values initialized to a default value.
static void load_game_state_json(const std::string& loadPath, std::vector<int>& idMap);

#pragma endregion

static std::string get_persistent_game_data_path(uint32_t saveSlot)
{
    return REPENTOGON::StringConcat(REPENTOGON::GetRepentogonDataPath(), "BossManager/persistentgamedata", to_string(saveSlot), ".json");
}

static std::string get_game_state_path(const GameStateSaveInfo& info)
{
    return REPENTOGON::StringConcat(REPENTOGON::GetRepentogonDataPath(), "BossManager/", info.fileName, ".json");
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
        ZHL::Log(LOG_ERROR_HEADER "Unable to save Persistent Game Data to '%s': failed to create parent path\n", savePath.c_str());
        return;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

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
            ZHL::Log(LOG_ERROR_HEADER "Unable to save Persistent Game Data to '%s': failed to open file for writing\n", savePath.c_str());
            return;
        }

        ofs.write(buffer.GetString(), buffer.GetSize());
        ofs.flush();
        if (!ofs)
        {
            ofs.close();
            ZHL::Log(LOG_ERROR_HEADER "Unable to save Persistent Game Data to '%s': unable to write data\n", savePath.c_str());
            std::filesystem::remove(tmpSave, std::error_code());
            return;
        }
    }

    std::error_code ec;
    std::filesystem::rename(tmpSave, savePath, ec);
    if (ec)
    {
        std::filesystem::remove(tmpSave, std::error_code());   // rename failed: don't leave the temp behind
        ZHL::Log(LOG_ERROR_HEADER "Unable to save Persistent Game Data to '%s': unable to rename temporary file\n", savePath.c_str());
        return;
    }

    ZHL::Log(LOG_INFO_HEADER "Saved Persistent Game Data to '%s'\n", savePath.c_str());
}

void BossManager::detail::LoadPersistentGameData(uint32_t saveSlot)
{
    BossManager::detail::ClearPersistentGameData();

    if (saveSlot == 0)
    {
        return;
    }

    auto& data = BossManager::m_data;
    std::string loadPath = get_persistent_game_data_path(saveSlot);

    // Read the whole file
    std::string text;
    {
        std::ifstream ifs(loadPath, std::ios::binary);
        if (!ifs)
        {
            ZHL::Log(LOG_INFO_HEADER "No Persistent Game Data at '%s'\n", loadPath.c_str());
            return;
        }
        text.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }

    rapidjson::Document doc;
    doc.Parse(text.c_str());
    if (doc.HasParseError() || !doc.IsObject())
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load Persistent Game Data from '%s': unable to parse json\n", loadPath.c_str());
        return;
    }

    auto version = doc.FindMember("version");
    if (version == doc.MemberEnd() || !version->value.IsUint() || version->value.GetUint() != PERSISTENT_GAME_DATA_VERSION)
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load Persistent Game Data from '%s': missing or unsupported version\n", loadPath.c_str());
        return;
    }

    auto entries = doc.FindMember("entries");
    if (entries == doc.MemberEnd() || !entries->value.IsArray())
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load Persistent Game Data from '%s': missing 'entries' array\n", loadPath.c_str());
        return;
    }

    for (const auto& entry : entries->value.GetArray())
    {
        if (!entry.IsArray() || entry.Size() != 2)
        {
            ZHL::Log(LOG_WARN_HEADER "Persistent Game Data Load: skipping malformed 'entries' entry in '%s'\n", loadPath.c_str());
            continue;
        }

        auto& json_name = entry[0];
        auto& json_defeated = entry[1];
    
        if (!json_name.IsString() || !json_defeated.IsBool())
        {
            ZHL::Log(LOG_WARN_HEADER "Skipping malformed entry in '%s'\n", loadPath.c_str());
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
            ZHL::Log(LOG_WARN_HEADER "Entry refers to vanilla boss in '%s'\n", loadPath.c_str());
            continue;
        }

        data.defeatedCustomBosses[BossManager::ToCustomBossId(*bossId)] = defeated;
    }

    ZHL::Log(LOG_INFO_HEADER "Loaded Persistent Game Data from '%s'\n", loadPath.c_str());
}

void BossManager::detail::SaveGameState(const GameStateSaveInfo& info, bool isRerun)
{
    if (isRerun)
    {
        return;
    }

    auto& data = BossManager::m_data;
    auto savePath = get_game_state_path(info);

    if (!REPENTOGON::EnsureParentPathExists(savePath))
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to save GameState to '%s': failed to create parent path\n", savePath.c_str());
        return;
    }

    auto& bosses = g_Manager->GetEntityConfig()->_bosses;
    assert(bosses.size() == g_Game->_bossPool._removedBosses.size());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject(); // doc

    writer.Key("version");
    writer.Uint(GAME_STATE_VERSION);

    writer.Key("checksum");
    writer.Uint(g_Manager->_gamestate._checksum);

    writer.Key("idMap");
    writer.StartArray(); // idMap

    for (size_t i = NUM_BOSSES; i < bosses.size(); i++)
    {
        auto& name = bosses[i].name;
        writer.String(name.c_str(), name.size());
    }

    writer.EndArray(); // idMap

    writer.EndObject(); // doc

    std::ofstream ofs(savePath, std::ios::binary);
    if (!ofs)
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to save GameState to '%s': failed to open file for writing\n", savePath.c_str());
        return;
    }

    ofs.write(buffer.GetString(), buffer.GetSize());
    ofs.flush();
    if (!ofs)
    {
        ofs.close();
        ZHL::Log(LOG_ERROR_HEADER "Unable to save GameState to '%s': unable to write data\n", savePath.c_str());
        std::filesystem::remove(savePath, std::error_code());
        return;
    }

    ZHL::Log(LOG_INFO_HEADER "Saved GameState to '%s'\n", savePath.c_str());
}

static void load_game_state_json(const std::string& loadPath, std::vector<int>& idMap)
{
    std::string text;
    {
        std::ifstream ifs(loadPath, std::ios::binary);
        if (!ifs)
        {
            ZHL::Log(LOG_INFO_HEADER "No GameState Data at '%s'\n", loadPath.c_str());
            return;
        }
        text.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }

    rapidjson::Document doc;
    doc.Parse(text.c_str(), text.size());
    if (doc.HasParseError() || !doc.IsObject())
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load GameState Data from '%s': unable to parse json\n", loadPath.c_str());
        return;
    }

    auto version = doc.FindMember("version");
    if (version == doc.MemberEnd() || !version->value.IsUint() || version->value.GetUint() != GAME_STATE_VERSION)
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load GameState from '%s': missing or unsupported version\n", loadPath.c_str());
        return;
    }

    auto checksum = doc.FindMember("checksum");
    if (checksum == doc.MemberEnd() || !checksum->value.IsUint() || checksum->value.GetUint() != g_Manager->_gamestate._checksum)
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load GameState from '%s': missing or incorrect checksum\n", loadPath.c_str());
        return;
    }

    auto json_idMap = doc.FindMember("idMap");
    if (json_idMap == doc.MemberEnd() || !json_idMap->value.IsArray())
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to load GameState from '%s': missing 'idMap' array\n", loadPath.c_str());
        return;
    }

    {
        size_t it = 0;
        for (const auto& json_name : json_idMap->value.GetArray())
        {
            size_t customId = it++;

            if (!json_name.IsString())
            {
                ZHL::Log(LOG_WARN_HEADER "GameState Load: skipping malformed 'idMap' entry in '%s'\n", loadPath.c_str());
                continue;
            }
    
            const std::string name(json_name.GetString(), json_name.GetStringLength());
            auto id = BossManager::GetBossIdByName(name);
            if (id && *id >= NUM_BOSSES)
            {
                idMap[BossManager::ToCustomBossId(*id)] = customId;
            }
        }
    }

    ZHL::Log(LOG_INFO_HEADER "Loaded GameState from '%s'\n", loadPath.c_str());
}

void BossManager::detail::LoadGameState(const GameStateSaveInfo& info, bool isRerun)
{
    if (isRerun)
    {
        return;
    }

    auto& gameState = g_Manager->_gamestate;
    // removed bosses is expected to be in the same state as when it was saved, both in terms of values and size
    auto& removedBosses = gameState._bossPool._removedBosses;
    std::vector<int> idMap;
    idMap.resize(BossManager::GetNumBosses() - NUM_BOSSES, -1);

    std::string loadPath = get_game_state_path(info);
    load_game_state_json(loadPath, idMap);

    // remap the bosses' state.

    std::vector<bool> gameStateRemovedCustomBosses;
    if (removedBosses.size() > NUM_BOSSES) // gameState has custom bosses
    {
        // create a copy composed of just the custom bosses slice.
        gameStateRemovedCustomBosses.assign(removedBosses.begin() + NUM_BOSSES, removedBosses.end());
    }
    
    // resize the vector to match actual number of bosses
    removedBosses.resize(BossManager::GetNumBosses(), false);

    for (size_t i = NUM_BOSSES; i < removedBosses.size(); i++)
    {
        int gameStateCustomId = idMap[BossManager::ToCustomBossId(i)];
        bool value = gameStateCustomId >= 0 ? gameStateRemovedCustomBosses[gameStateCustomId] : false;
        removedBosses[i] = value;
    }
}

void BossManager::detail::DeleteGameState(const GameStateSaveInfo& info, bool isRerun)
{
    if (isRerun)
    {
        return;
    }

    auto savePath = get_game_state_path(info);
    std::error_code ec;
    std::filesystem::remove(savePath, ec);

    if (ec)
    {
        ZHL::Log(LOG_ERROR_HEADER "Unable to Delete GameState at '%s': %s\n", savePath.c_str(), ec.message().c_str());
        return;
    }
    
    ZHL::Log(LOG_INFO_HEADER "Deleted GameState at '%s'\n", savePath.c_str());
}