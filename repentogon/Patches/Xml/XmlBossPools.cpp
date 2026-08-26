#include "../XMLData.h"
#include "IsaacRepentance.h"
#include "../../MiscFunctions.h"
#include "rapidxml_print.hpp"

using NodeMap = std::unordered_map<int, rapidxml::xml_node<char>*>;
// for each document we keep track of the nodes by id.
// This, in order to avoid problems caused by the presence of multiple entries with the same id.
using DocumentMap = std::unordered_map<int, std::pair<rapidxml::xml_node<char>*, XML::ParsedAttributes>>;

namespace
{
    struct ParseContext
    {
        // the main doc in which to merge all content
        rapidxml::xml_document<char>& mainDoc;
        // the main doc's root node
        rapidxml::xml_node<char>* mainRoot;
        NodeMap nodeMap;
        // the currently parsed doc
        rapidxml::xml_document<char>* doc;
        // used to implement "last-write-wins"
        std::set<int> docNodeMap;
        // the mod id of the current content doc
        // also encodes "isContent"
        std::optional<std::string_view> modId;
    
        ParseContext(rapidxml::xml_document<char>& mainDoc, rapidxml::xml_node<char>* mainRoot)
            : mainDoc(mainDoc), mainRoot(mainRoot), doc(&mainDoc)
        {}
    
        void BeginNewDoc(rapidxml::xml_document<char>* doc, std::optional<std::string_view> modId);
        /// @brief maps the node in the node map and return the mapped node
        /// if the node comes from a content document it also adds the node to the main document.
        std::pair<rapidxml::xml_node<char>*, bool> MapNode(rapidxml::xml_node<char>* node, int nodeId);
        /// @brief inserts the new node for the current doc, if not duplicate
        /// @return isUnique
        bool InsertDocNode(int nodeId);
    };
}

enum class ePoolAttribute
{
    NAME,
    DOUBLE_TROUBLE,
};

enum class eBossAttribute
{
    ID,
    WEIGHT,
    ROOM,
};

static std::optional<std::string> s_sourceCache;
static std::string s_sourceTextBuffer;

const std::unordered_map<std::string_view, size_t> POOL_ATTRIBUTE_MAP = {
    {"name", (size_t)ePoolAttribute::NAME},
    {"doubletrouble", (size_t)ePoolAttribute::DOUBLE_TROUBLE}
};

const std::unordered_map<std::string_view, size_t> BOSS_ATTRIBUTE_MAP = {
    {"id", (size_t)eBossAttribute::ID},
    {"weight", (size_t)eBossAttribute::WEIGHT},
    {"room", (size_t)eBossAttribute::ROOM}
};

static const char* FILE_NAME = "bosspools.xml";
static const char* NODE_ROOT = "bosspools";
static const char* NODE_POOL = "pool";
static const char* NODE_BOSS = "boss";
static const char* ATTRIBUTE_DOUBLE_TROUBLE = "doubletrouble";

static std::array<std::pair<std::string_view, int>, 26> BASE_POOLS = {{
    {"basement", eStbType::STB_BASEMENT},
    {"cellar", eStbType::STB_CELLAR},
    {"caves", eStbType::STB_CAVES},
    {"catacombs", eStbType::STB_CATACOMBS},
    {"depths", eStbType::STB_DEPTHS},
    {"necropolis", eStbType::STB_NECROPOLIS},
    {"womb", eStbType::STB_WOMB},
    {"utero", eStbType::STB_UTERO},
    {"sheol", eStbType::STB_SHEOL},
    {"cathedral", eStbType::STB_CATHEDRAL},
    {"dark room", eStbType::STB_DARK_ROOM},
    {"chest", eStbType::STB_CHEST},
    {"burning basement", eStbType::STB_BURNING_BASEMENT},
    {"flooded caves", eStbType::STB_FLOODED_CAVES},
    {"dank depths", eStbType::STB_DANK_DEPTHS},
    {"scarred womb", eStbType::STB_SCARRED_WOMB},
    {"blue womb", eStbType::STB_BLUE_WOMB},
    {"void", eStbType::STB_THE_VOID},
    {"downpour", eStbType::STB_DOWNPOUR},
    {"mines", eStbType::STB_MINES},
    {"mausoleum", eStbType::STB_MAUSOLEUM},
    {"corpse", eStbType::STB_CORPSE},
    {"dross", eStbType::STB_DROSS},
    {"ashpit", eStbType::STB_ASHPIT},
    {"gehenna", eStbType::STB_GEHENNA},
    {"mortis", eStbType::STB_MORTIS},
}};

static std::array<std::pair<int, std::string_view>, 9> HARDCODED_DOUBLE_TROUBLE = {{
    {STB_CAVES, "3700"},
    {STB_CATACOMBS, "3700"},
    {STB_FLOODED_CAVES, "3700"},
    {STB_DEPTHS, "3750"},
    {STB_NECROPOLIS, "3750"},
    {STB_DANK_DEPTHS, "3750"},
    {STB_WOMB, "3800"},
    {STB_UTERO, "3800"},
    {STB_SCARRED_WOMB, "3800"},
}};

static void process_root(ParseContext& ctx, rapidxml::xml_node<char>* root);
static void process_pool_node(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node);
/// @return notRemoved
static bool process_boss_node(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node, int parentId);
/// @return notRemoved
static bool process_pool_child(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node, int parentId);
static void BuildXmlSourceText(char* sourceText);

void ParseContext::BeginNewDoc(rapidxml::xml_document<char>* doc, std::optional<std::string_view> modId)
{
    this->doc = doc;
    this->docNodeMap.clear();
    this->modId = modId;
}

std::pair<rapidxml::xml_node<char>*, bool> ParseContext::MapNode(rapidxml::xml_node<char>* node, int nodeId)
{
    auto [it, inserted] = this->nodeMap.emplace(nodeId, nullptr);
    if (inserted)
    {
        bool isContent = &this->mainDoc != this->doc;
        if (isContent)
        {
            // Given that we are iterating through the nodes in reverse, content nodes are to the main doc in reverse.
            // However this shouldn't be an issue, as the order in which pools are processed has no inpact on the final "result".
            it->second = XML::CloneNode(this->mainDoc, this->mainRoot, node);
        }
        else
        {
            it->second = node;
        }
    }

    return {it->second, inserted};
}

bool ParseContext::InsertDocNode(int nodeId)
{
    auto [it, inserted] = this->docNodeMap.emplace(nodeId);
    return inserted;
}

void process_root(ParseContext& ctx, rapidxml::xml_node<char>* root)
{
    XMLBossPools& xmlData = *XMLStuff.BossPoolData;
    int startOrder = xmlData.nodes.size() + 1;

    // Process in reverse so that, for duplicate IDs, the last occurrence in the
    // document is inserted first and earlier occurrences are discarded.
    rapidxml::xml_node<char>* node = root->last_node();
    while (node)
    {
        auto* nextNode = node->previous_sibling();
        process_pool_node(ctx, root, node);
        node = nextNode;
    }

    int endOrder = xmlData.nodes.size();
    while(startOrder < endOrder)
    {
        int* first = REPENTOGON::FindInMap(xmlData.byorder, startOrder);
        int* second = REPENTOGON::FindInMap(xmlData.byorder, endOrder);
        assert(first && second); // both should exist

        std::swap(*first, *second);
        startOrder++;
        endOrder--;
    }    
}

void process_pool_node(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node)
{
    XMLBossPools& xmlData = *XMLStuff.BossPoolData;
    auto parsed = XML::ParseAttributes(node, POOL_ATTRIBUTE_MAP);
    bool isContent = ctx.modId.has_value();

    // get id
    std::optional<int> id;
    if (auto* attribute = parsed.attributes[(size_t)ePoolAttribute::NAME]; attribute)
    {
        std::string_view name = XML::GetValue(attribute);
        auto it = xmlData.byname.find(std::string(name));
        if (it != xmlData.byname.end())
        {
            // pool ids are strictly tied to stage id, it wouldn't make sense to create a custom boss pool.
            id = it->second;
        }
    }

    if (!id)
    {
        parent->remove_node(node);
        return;
    }

    int nodeId = id.value();
    if (!ctx.InsertDocNode(nodeId)) // not isUnique
    {
        // do not override
        parent->remove_node(node);
        return;
    }

    // double trouble is very hardcoded, allowing edits of this value won't lead to desired results.
    if (auto* attribute = parsed.attributes[(size_t)ePoolAttribute::DOUBLE_TROUBLE]; attribute)
    {
        node->remove_attribute(attribute);
    }

    bool isVanillaPool = 0 <= nodeId && nodeId < NUM_STB;
    rapidxml::xml_node<char>* mainNode = nullptr;
    if (isVanillaPool)
    {
        parsed.all["sourceid"] = XML::BASE_GAME_ID;
        auto insertionResults = ctx.MapNode(node, nodeId);
        mainNode = insertionResults.first;
        bool inserted = insertionResults.second;

        if (inserted)
        {
            // we want to process the children of the mapped node
            assert(mainNode != nullptr);
            node = mainNode;
        }
    }
    
    #pragma region process childs
    // we are currently processing child nodes of another node
    // we need to append them to the main node.
    bool shouldAppendChild = mainNode != nullptr && mainNode != node;
    auto* child = node->first_node();
    while(child)
    {
        auto* nextChild = child->next_sibling();
        bool notRemoved = true;
        if (XML::GetName(child) == NODE_BOSS)
        {
            notRemoved = process_boss_node(ctx, node, child, nodeId);
        }
        else
        {
            notRemoved = process_pool_child(ctx, node, child, nodeId);
        }

        if (shouldAppendChild && notRemoved)
        {
            XML::CloneNode(ctx.mainDoc, mainNode, child);
        }

        child = nextChild;
    }

    #pragma endregion

    if (mainNode == node) // isMainNode
    {
        XMLAttributes& bossPool = parsed.all;
        bossPool.insert_or_assign("sourceid", XML::BASE_GAME_ID);

        xmlData.nodes[nodeId] = bossPool;
        xmlData.bymod[bossPool["sourceid"]].push_back(nodeId);
        xmlData.byorder[xmlData.nodes.size()] = nodeId;
    }
}

bool process_boss_node(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node, int parentId)
{
    XMLBossPools& xmlData = *XMLStuff.BossPoolData;
    auto parsed = XML::ParseAttributes(node, BOSS_ATTRIBUTE_MAP);

    std::optional<int> bossId;
    if (auto* attribute = parsed.attributes[(size_t)eBossAttribute::ID]; attribute)
    {
        bossId = REPENTOGON::StringToNumber<int>(XML::GetValue(attribute));
        if (bossId)
        {
            // prevent mods from using non vanilla ids as boss ids, modded bosses must be specified using a name
            bool isVanilla = 1 <= bossId && bossId < NUM_BOSSES;
            if (!isVanilla)
            {
                bossId = std::nullopt;
            }
        }
        else
        {
            // bossId = XMLStuff.BossPortraitData->GetIdByName(XML::GetValue(attribute));
            bossId = std::nullopt; // don't add support for id by name yet.
            if (bossId)
            {
                auto [it, inserted] = parsed.all.insert_or_assign("id", std::to_string(bossId.value()));
                auto& value = it->second;
                XML::SetValueCopy(*ctx.doc, attribute, value); // we could avoid making a copy, but it's probably better to be on the safe side.
            }
        }
    }

    if (!bossId)
    {
        parent->remove_node(node);
        return false;
    }

    if (auto* attribute = parsed.attributes[(size_t)eBossAttribute::ROOM]; attribute)
    {
        // room is unused and is unsafe for mods to use.
        node->remove_attribute(attribute);
    }

    // add to xml data
    XMLAttributes& child = parsed.all;
    if (ctx.modId)
    {
        child["sourceid"] = ctx.modId.value();
    }
    else
    {
        child["sourceid"] = XML::BASE_GAME_ID;
    }

    xmlData.childs[parentId][stringlower(XML::GetName(node))].push_back(child);
    return true;
}

bool process_pool_child(ParseContext& ctx, rapidxml::xml_node<char>* parent, rapidxml::xml_node<char>* node, int parentId)
{
    XMLBossPools& xmlData = *XMLStuff.BossPoolData;
    XMLAttributes child = XML::ParseAttributes(node);
    parent->remove_node(node); // all child nodes are treated as boss nodes, so we remove them to avoid having an incorrect view in xmlData.

    if (ctx.modId)
    {
        child["sourceid"] = ctx.modId.value();
    }
    else
    {
        child["sourceid"] = XML::BASE_GAME_ID;
    }

    xmlData.childs[parentId][stringlower(XML::GetName(node))].push_back(child);
    return false;
}

void BuildXmlSourceText(char* sourceText)
{
    XMLBossPools& xmlData = *XMLStuff.BossPoolData;

    XML::Document document = XML::GetResourcesDoc(FILE_NAME, sourceText);
    auto& doc = *document.GetDocument();

    // build default doc, if necessary
    auto* root = doc.first_node(NODE_ROOT);
    if (!root)
    {
        XMLBossPools::BuildDefaultDoc(doc);
        root = doc.first_node(NODE_ROOT);
    }

    // init byname map
    for (auto [name, stageId] : BASE_POOLS)
    {
        xmlData.byname[std::string(name)] = stageId;
    }
    xmlData.maxid = NUM_STAGES - 1;

    // process resources
    ParseContext ctx(doc, root);
    process_root(ctx, root);

    // content
    for (auto& contentDoc : XML::GetContentDocs(FILE_NAME))
    {
        if (auto* contentRoot = contentDoc.GetDocument()->first_node(NODE_ROOT))
        {
            ctx.BeginNewDoc(contentDoc.GetDocument(), XML::GetModId(contentDoc.mod));
            process_root(ctx, contentRoot);
        }
    }

    // force hardcoded double trouble
    for (auto [id, doubleTrouble] : HARDCODED_DOUBLE_TROUBLE)
    {
        if (auto** nodePtr = REPENTOGON::FindInMap(ctx.nodeMap, id))
        {
            auto* node = *nodePtr;
            assert(node);
            assert(node->first_attribute(ATTRIBUTE_DOUBLE_TROUBLE) == nullptr);
            XML::AppendNewAttribute(doc, node, ATTRIBUTE_DOUBLE_TROUBLE, doubleTrouble);
        }
    }

    // reset
    s_sourceCache = std::string();
    rapidxml::print(std::back_inserter(s_sourceCache.value()), doc);

    // OLD iterator
    // std::string filePath = FILE_NAME;
    // for (ModEntry* mod : g_Manager->GetModManager()->_mods)
    // {
    //     if (!mod->_loaded) continue;

    //     std::string contentPath;
    //     mod->GetContentPath(&contentPath, &filePath);
    //     std::optional<std::string> expandedPath = g_ContentManager.GetMountedFilePath(contentPath.c_str());

    //     if (!expandedPath)
    //     {
    //         continue;
    //     }

    //     std::unique_ptr<rapidxml::xml_document<char>> contentDocPtr = std::make_unique<rapidxml::xml_document<char>>();
    //     rapidxml::xml_document<char>& contentDoc = *contentDocPtr.get();
    //     std::string contentSourceText;
    //     dump_source_text(contentSourceText, expandedPath.value());
    //     if (!XML::ParseDoc(contentDoc, contentSourceText.data(), expandedPath.value()))
    //     {
    //         continue;
    //     }

    //     auto* contentRoot = contentDoc.first_node(NODE_ROOT);
    //     if (!contentRoot)
    //     {
    //         continue;
    //     }
        
    //     ctx.BeginNewDoc(&contentDoc, XML::GetModId(mod));
    //     process_root(ctx, contentRoot);
    // }
}

void XMLBossPools::BuildDefaultDoc(rapidxml::xml_document<char> &doc)
{
    doc.clear();
    auto* root = XML::AppendNewNode(doc, &doc, NODE_ROOT);

    for (const auto& [name, id] : BASE_POOLS)
    {
        auto* pool = XML::AppendNewNode(doc, root, NODE_POOL);
        XML::AppendNewAttribute(doc, pool, "name", name);
    }
}

std::string& XMLBossPools::GetXmlSourceText(char *sourceText)
{
    if (!s_sourceCache)
    {
        BuildXmlSourceText(sourceText);
    }

    assert(s_sourceCache.has_value());
    s_sourceTextBuffer = s_sourceCache.value();
    return s_sourceTextBuffer;
}