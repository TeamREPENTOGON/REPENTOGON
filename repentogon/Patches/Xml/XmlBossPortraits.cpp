#include "../XMLData.h"
#include "IsaacRepentance.h"
#include "../../Utils/StringUtils.h"

/* XMLBossPortrait:
 * - Each node represents a "boss" node
 * - Each boss node inherits the attributes from the root "bosses" node.
 * - lookup by relative id and source path is supported.
 * - boss nodes without an "id" attribute are ignored.
*/

/* BUG Fixes:
 * - Fix EntityConfig::LoadBosses accessing out of bounds ids when parsing 'alt' nodes.
 */

#define LOG_INFO_HEADER "[INFO] [XMLBossPortraits] - "
#define LOG_WARN_HEADER "[WARN] [XMLBossPortraits] - "
#define LOG_ERROR_HEADER "[ERROR] [XMLBossPortraits] - "

namespace
{

struct ParseContext
{
    // the mod id of the current content doc
    // also encodes "isContent"
    ModEntry* mod = nullptr;
    std::string_view sourceId;
    std::string_view xmlPath;

    ParseContext(ModEntry* mod, std::string_view xmlPath);
    void BeginNewDoc(ModEntry* mod, std::string_view xmlPath);
};

} // unnamed namespace

static void process_root_node(ParseContext& ctx, rapidxml::xml_node<char>* root);
static void process_boss_node(ParseContext& ctx, rapidxml::xml_node<char>* node, XMLBossPortrait::BossStbDesc* stbDesc);

enum class eBossAttribute
{
    ID,
    NAME,
};

static const std::unordered_map<std::string_view, size_t> BOSS_ATTRIBUTE_MAP = {
    {"id", (size_t)eBossAttribute::ID},
    {"name", (size_t)eBossAttribute::NAME},
};

static const char* FILE_NAME = "bossportraits.xml";
static const char* NODE_ROOT = "bosses";
static const char* ATTRIBUTE_STB_FILE_PATH = "stb";

ParseContext::ParseContext(ModEntry* mod, std::string_view xmlPath)
    : mod(mod), sourceId(mod ? XML::GetModId(mod) : XML::BASE_GAME_ID), xmlPath(xmlPath) {}

void ParseContext::BeginNewDoc(ModEntry* mod, std::string_view xmlPath)
{
    this->mod = mod;
    this->sourceId = mod ? XML::GetModId(mod) : XML::BASE_GAME_ID;
    this->xmlPath = xmlPath;
}

static void process_root_node(ParseContext& ctx, rapidxml::xml_node<char>* root)
{
    bool isContent = ctx.mod != nullptr;
    XMLBossPortrait::BossStbDesc* stbDesc = nullptr;
    if (isContent)
    {
        auto* stb_node = root->first_attribute(ATTRIBUTE_STB_FILE_PATH);
        if (!stb_node)
        {
            ZHL::Log(LOG_INFO_HEADER "No stb path specified in '%.*s', boss rooms will have to be manually inserted", (int)ctx.xmlPath.size(), ctx.xmlPath.data());
        }
        else
        {
            stbDesc = &XMLStuff.BossPortraitData->stbDescs.emplace_back(ctx.mod, XML::GetValue(stb_node));
        }
    }

    rapidxml::xml_node<char>* node = root->first_node("boss");
    while (node)
    {
        auto* next = node->next_sibling();
        process_boss_node(ctx, node, stbDesc);
        node = next;
    }
}

static void process_boss_node(ParseContext& ctx, rapidxml::xml_node<char>* node, XMLBossPortrait::BossStbDesc* stbDesc)
{
    XML::ParsedAttributes parsed = XML::ParseAttributes(node, BOSS_ATTRIBUTE_MAP);
    XMLAttributes& attributes = parsed.all;
    bool isContent = ctx.mod != nullptr;
    rapidxml::xml_node<char>* daddy = node->parent();

    int id = 0;
    if (!isContent)
    {
        // do security checks to prevent the vanilla from parsing invalid ids
        auto* id_node = parsed.attributes[(size_t)eBossAttribute::ID];
        if (!id_node) {
            daddy->remove_node(node); // don't let the node be parsed
            return;
        }

        auto id_number = REPENTOGON::StringToNumber<uint32_t>(XML::GetValue(id_node));
        if (!id_number) {
            daddy->remove_node(node);
            return;
        }

        id = id_number.value();
        bool inBounds = 0 < id && id < NUM_BOSSES; // 0 is not actually a valid id
        if (!inBounds) {
            daddy->remove_node(node); // don't let the node be parsed
            return;
        }
    }
    else
    {
        if (XML::GetName(node) != "boss") { return; } // incorrectly named node

        auto* id_node = parsed.attributes[(size_t)eBossAttribute::ID];
        if (!id_node) {
            ZHL::Log(LOG_ERROR_HEADER "no boss id in '%.*s'", (int)ctx.xmlPath.size(), ctx.xmlPath.data());
            return;
        }

        std::string_view id_value = XML::GetValue(id_node);
        auto id_number = REPENTOGON::StringToNumber<uint32_t>(id_value);
        if (!id_number) {
            ZHL::Log(LOG_ERROR_HEADER "invalid boss id '%.*s' in '%.*s'", (int)id_value.size(), id_value.data(), (int)ctx.xmlPath.size(), ctx.xmlPath.data());
            return;
        }

        if (parsed.attributes[(size_t)eBossAttribute::NAME] == nullptr)
        {
            ZHL::Log(LOG_ERROR_HEADER "boss id '%d' does not have a name in '%.*s'", id_number, (int)ctx.xmlPath.size(), ctx.xmlPath.data());
            return;
        }

        uint32_t relativeId = *id_number;
        id = ++XMLStuff.BossPortraitData->maxid;
        attributes[XML::ATTRIBUTE_RELATIVE_ID] = id_value;
        attributes["id"] = to_string(id);

        if (stbDesc)
        {
            auto [it, inserted] = stbDesc->idMap.try_emplace(relativeId, id);
            if (!inserted)
            {
                ZHL::Log(LOG_WARN_HEADER "duplicate id %d in '%.*s'", relativeId, (int)ctx.xmlPath.size(), ctx.xmlPath.data());
                it->second = id;
            }
        }
    }

    XML::InheritAttributes(daddy, attributes);

    attributes[XML::ATTRIBUTE_SOURCE_PATH] = ctx.xmlPath;
    attributes[XML::ATTRIBUTE_SOURCE_ID] = ctx.sourceId;

    XMLStuff.BossPortraitData->AddNode(*node, std::move(attributes), id);
    XMLStuff.ModData->bossportraits[std::string(ctx.sourceId)] += 1;
}

void XMLBossPortrait::BuildDefaultDoc(rapidxml::xml_document<char> &doc)
{
    doc.clear();
    auto* root = XML::AppendNewNode(doc, &doc, NODE_ROOT);
}

void XMLBossPortrait::ParseXml(rapidxml::xml_document<char>& resourcesDoc, const std::string& xmlPath)
{
    XMLBossPortrait& xmlData = *XMLStuff.BossPortraitData;

    // build default doc, if necessary
    auto* root = resourcesDoc.first_node(NODE_ROOT);
    if (!root)
    {
        XMLBossPortrait::BuildDefaultDoc(resourcesDoc);
        root = resourcesDoc.first_node(NODE_ROOT);
    }

    // process resources
    ParseContext ctx(nullptr, xmlPath);
    process_root_node(ctx, root);
    xmlData.maxid = NUM_BOSSES - 1;

    // process content
    for (auto& contentDoc : XML::GetContentDocs(FILE_NAME))
    {
        if (auto* contentRoot = contentDoc.GetDocument()->first_node(NODE_ROOT))
        {
            ctx.BeginNewDoc(contentDoc.mod, contentDoc.xmlPath);
            process_root_node(ctx, contentRoot);
        }
    }
}