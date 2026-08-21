#include "BossManager.h"
#include "../Patches/XMLData.h"
#include "../Patches/AchievementsStuff.h"
#include <charconv>

namespace BossPortraits
{
    const char* IMAGE_ROOT = "root"; // string
    const char* VERSUS_ANM2 = "anm2"; // string

    namespace Boss
    {
        namespace Attribute
        {
            const char* ID = "id"; // int (ignored)
            const char* NAME = "name"; // string
            const char* NAME_IMAGE = "nameimage"; // string
            const char* PORTRAIT = "portrait"; // string
            const char* PIVOT_X = "pivotx"; // float
            const char* PIVOT_Y = "pivoty"; // float
            const char* ACHIEVEMENT = "achievement"; // int or string
        }

        namespace Element
        {
            const char* ALT = "alt";
        }
    }

    namespace Alt
    {
        namespace Attribute
        {
            const char* STAGE = "stage"; // integer
            const char* PORTRAIT = "portrait"; // string
        }
    }
}

#pragma region Definitions

namespace {
    struct BossPortraitsRoot
    {
        std::string_view imageRoot;
    };
}

static int get_node_id_by_order(const XMLDataHolder& data, int order);
static const XMLChilds* get_childs_by_id(const XMLDataHolder& data, int id);
static const std::vector<XMLAttributes>* get_child_nodes(const XMLChilds& childs, const std::string& name);
static const std::string* get_attribute(const XMLAttributes& attributes, const std::string& name);
template <typename T>
static std::optional<T> string_to_number(const std::string_view& string);
static std::string concat_string(const std::string_view& string, const std::string_view& other);

static EntityConfig_Boss* parse_boss_node(EntityConfig& config, const BossPortraitsRoot& root, const XMLAttributes& node);
static void parse_boss_alt_node(EntityConfig_Boss* boss, const BossPortraitsRoot& root, const XMLAttributes& node);

#pragma endregion

static int get_node_id_by_order(const XMLDataHolder& data, int order)
{
    auto iter = data.byorder.find(order);
    assert(iter != data.byorder.end());
    return iter->second;
}

static const XMLChilds* get_childs_by_id(const XMLDataHolder& data, int id)
{
    auto childIter = data.childs.find(id);
    if (childIter != data.childs.end()) {
        return &childIter->second;
    }

    return nullptr;
}

static const std::vector<XMLAttributes>* get_child_nodes(const XMLChilds& childs, const std::string& name)
{
    auto it = childs.find(name);
    if (it != childs.end()) {
        return &it->second;
    }

    return nullptr;
}

static const std::string* get_attribute(const XMLAttributes& attributes, const std::string& name)
{
    auto iter = attributes.find(name);
    if (iter == attributes.end()) { return nullptr; }

    return &iter->second;
}

template <typename T>
static std::optional<T> string_to_number(const std::string_view& string)
{
    T value;
    const char* start = string.data();
    const char* end = string.data() + string.size();

    auto [ptr, ec] = std::from_chars(start, end, value);

    if (ec == std::errc{} && ptr == end) {
        return value;
    }

    return std::nullopt;
}

static std::string concat_string(const std::string_view& string, const std::string_view& other)
{
    std::string result;
    result.reserve(string.size() + other.size());
    result.append(string);
    result.append(other);

    return result;
}

static EntityConfig_Boss* parse_boss_node(EntityConfig& config, const BossPortraitsRoot& root, const XMLAttributes& node)
{
    using namespace BossPortraits::Boss;
    const std::string* name = nullptr;

    if (auto* attribute = get_attribute(node, Attribute::NAME); attribute) {
        name = attribute;
    }
    else {
        return nullptr;
    }

    // create new boss
    int id = config._bosses.size();
    EntityConfig_Boss& boss = config._bosses.emplace_back();
    boss.bossID = id;
    boss.name = *name;
    
    if (auto* attribute = get_attribute(node, Attribute::NAME_IMAGE); attribute) {
        boss.namePath = concat_string(root.imageRoot, *attribute);
    }

    if (auto* attribute = get_attribute(node, Attribute::PORTRAIT); attribute) {
        boss.portraitPath = concat_string(root.imageRoot, *attribute);
    }

    if (auto* attribute = get_attribute(node, Attribute::PIVOT_X); attribute) {
        float value = string_to_number<float>(*attribute).value_or(0.0);
        boss.pivot.x = 96.0f - value;
    }

    if (auto* attribute = get_attribute(node, Attribute::PIVOT_Y); attribute) {
        float value = string_to_number<float>(*attribute).value_or(0.0);
        boss.pivot.y = 132.0f - value;
    }

    if (auto* attribute = get_attribute(node, Attribute::ACHIEVEMENT); attribute) {
        std::optional<int> intValue = string_to_number<int>(*attribute);
        if (intValue) {
            int achievement = intValue.value();
            bool isVanillaAchievement = 0 <= achievement && achievement < (int)eAchievement::NUM_ACHIEVEMENTS;
            boss.achievement = isVanillaAchievement ? achievement : 0;
        }
        else {
            int achievement = GetAchievementIdByName(*attribute);
            bool found = achievement >= 0;
            boss.achievement = found ? achievement : 0;
        }
    }

    return &boss;
}

static void parse_boss_alt_node(EntityConfig_Boss* boss, const BossPortraitsRoot& root, const XMLAttributes& node)
{
    using namespace BossPortraits::Alt;

    EntityConfig_Boss_Alt& alt = boss->alts.emplace_back();

    if (auto* attribute = get_attribute(node, Attribute::STAGE); attribute) {
        int value = string_to_number<int>(*attribute).value_or(0);
        alt.stageId = (0 <= value && value < NUM_STB) ? value : 0;
    }

    if (auto* attribute = get_attribute(node, Attribute::PORTRAIT); attribute) {
        alt.portraitPath = concat_string(root.imageRoot, *attribute);
    }
}

void BossManager::detail::LoadModBosses(EntityConfig& config)
{
    XMLBossPortrait& xmlData = *XMLStuff.BossPortraitData;
    size_t numEntries = xmlData.nodes.size();
    size_t xmlMaxId = xmlData.maxid;

    config._bosses.reserve(xmlMaxId + 1);
    assert(config._bosses.size() == NUM_BOSSES);

    for (size_t i = 1; i <= numEntries; i++)
    {
        int nodeId = get_node_id_by_order(xmlData, i);
        const XMLAttributes* attributes = xmlData.GetNodeOrNullById(nodeId);
        const XMLChilds* childs = get_childs_by_id(xmlData, nodeId);
        assert(attributes != nullptr);

        bool isModded = *get_attribute(*attributes, "sourceid") != "BaseGame";
        if (!isModded)
        {
            continue;
        }

        BossPortraitsRoot root;
        if (auto* attribute = get_attribute(*attributes, BossPortraits::IMAGE_ROOT); attribute) {
            root.imageRoot = *attribute;
        }

        EntityConfig_Boss* boss = parse_boss_node(config, root, *attributes);
        if (!boss)
        {
            continue;
        }

        BossManager::m_data.bossMap[boss->name] = boss->bossID;

        if (!childs)
        {
            continue;
        }

        if (auto* childNodes = get_child_nodes(*childs, BossPortraits::Boss::Element::ALT); childNodes)
        {
            auto& altNodes = *childNodes;
            for (size_t i = 0; i < altNodes.size(); i++)
            {
                parse_boss_alt_node(boss, root, altNodes[i]);
            }
        }
    }
}