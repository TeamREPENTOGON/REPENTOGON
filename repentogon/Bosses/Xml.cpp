#include "BossManager.h"
#include "../Patches/XMLData.h"
#include "../Patches/AchievementsStuff.h"

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

#pragma region Declarations

namespace {
    struct BossPortraitsRoot
    {
        std::string_view imageRoot;
    };
}

static const XMLChilds* get_childs_by_id(const XMLDataHolder& data, int id);
static const std::vector<XMLAttributes>* get_child_nodes(const XMLChilds& childs, const std::string& name);
static const std::string* get_attribute(const XMLAttributes& attributes, const std::string& name);

static EntityConfig_Boss* parse_boss_node(EntityConfig& config, const BossPortraitsRoot& root, const XMLAttributes& node);
static void parse_boss_alt_node(EntityConfig_Boss* boss, const BossPortraitsRoot& root, const XMLAttributes& node);

#pragma endregion

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
        boss.namePath = REPENTOGON::StringConcat(root.imageRoot, *attribute);
    }

    if (auto* attribute = get_attribute(node, Attribute::PORTRAIT); attribute) {
        boss.portraitPath = REPENTOGON::StringConcat(root.imageRoot, *attribute);
    }

    if (auto* attribute = get_attribute(node, Attribute::PIVOT_X); attribute) {
        float value = REPENTOGON::StringToNumber<float>(*attribute).value_or(0.0);
        boss.pivot.x = 96.0f - value;
    }

    if (auto* attribute = get_attribute(node, Attribute::PIVOT_Y); attribute) {
        float value = REPENTOGON::StringToNumber<float>(*attribute).value_or(0.0);
        boss.pivot.y = 132.0f - value;
    }

    if (auto* attribute = get_attribute(node, Attribute::ACHIEVEMENT); attribute) {
        std::optional<int> intValue = REPENTOGON::StringToNumber<int>(*attribute);
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
        int value = REPENTOGON::StringToNumber<int>(*attribute).value_or(0);
        alt.stageId = (0 <= value && value < NUM_STB) ? value : 0;
    }

    if (auto* attribute = get_attribute(node, Attribute::PORTRAIT); attribute) {
        alt.portraitPath = REPENTOGON::StringConcat(root.imageRoot, *attribute);
    }
}

void BossManager::detail::LoadModBosses(EntityConfig& config)
{
    XMLBossPortrait& xmlData = *XMLStuff.BossPortraitData;
    size_t numEntries = xmlData.nodes.size();
    size_t xmlMaxId = xmlData.maxid;

    config._bosses.reserve(xmlMaxId + 1);
    assert(config._bosses.size() == NUM_BOSSES);

    for (size_t i = NUM_BOSSES; i <= xmlMaxId; i++)
    {
        const XMLAttributes* attributes = xmlData.GetNodeOrNullById(i);
        const XMLChilds* childs = get_childs_by_id(xmlData, i);
        assert(attributes != nullptr); // there should never be a gap in between modded ids.

        BossPortraitsRoot root;
        if (auto* attribute = get_attribute(*attributes, BossPortraits::IMAGE_ROOT); attribute) {
            root.imageRoot = *attribute;
        }

        EntityConfig_Boss* boss = parse_boss_node(config, root, *attributes);
        if (!boss)
        {
            continue;
        }

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

    // copy the lookup table.
    BossManager::m_data.bossMap = xmlData.byname;
}

void BossManager::detail::PatchEntityBossId(xml_document<char>& doc, xml_node<char>& node, XMLAttributes& entity)
{
    auto* attribute = get_attribute(entity, "bossid");
    if (!attribute)
    {
        return;
    }

    auto* xmlAttribute = node.first_attribute("bossID");
    if (!xmlAttribute)
    {
        return;
    }

    std::optional<int> intValue = REPENTOGON::StringToNumber<int>(*attribute);
    if (intValue)
    {
        bool isVanilla = 0 <= intValue && intValue < NUM_BOSSES;
        if (!isVanilla)
        {
            XML::SetValue(xmlAttribute, "0");
        }
        return;
    }

    std::optional<int> customId = BossManager::GetBossIdByName(*attribute);
    if (!customId)
    {
        XML::SetValue(xmlAttribute, "0");
    }
    else
    {
        XML::SetValueCopy(doc, xmlAttribute, std::to_string(customId.value()));
    }
}