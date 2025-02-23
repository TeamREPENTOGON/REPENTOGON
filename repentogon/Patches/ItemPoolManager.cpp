#include "ItemPoolManager.h"

#include "IsaacRepentance.h"
#include "Log.h"
#include "HookSystem.h"
#include "XMLData.h"
#include "../MiscFunctions.h"
#include "ASMPatcher.hpp"
#include "ASMPatches.h"

#include "rapidxml.hpp"
#include "writer.h" // rapidjson
#include "stringbuffer.h" // rapidjson
#include <bitset>
#include <tuple>
#include <utility>
#include <memory>
#include <optional>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <map>
#include <filesystem>

typedef ItemPoolManager::Warnings Warnings;
typedef ItemPoolManager::Error Error;

static inline std::string get_mod_name(const ModEntry* mod) noexcept 
{
	if (!mod)
	{
		return "Base Game";
	}

	return mod->_name;
}

static inline void log_warnings(const Warnings& warnings) noexcept 
{
	for (const auto& warning : warnings)
	{
		ZHL::Log("%s\n", warning.c_str());
	}
}

static inline std::string string_to_lower(const std::string& string) noexcept 
{
	std::string result = std::string(string);

	for (char& character : result)
	{
		character = std::tolower(character);
	}

	return result;
}

static inline std::vector<std::string> string_split(const std::string& string) noexcept
{
	std::vector<std::string> subStrings;
	size_t pos = 0;
	size_t findPos = 0;

	while ((findPos = string.find(' ', pos)) != std::string::npos)
	{
		if (findPos > pos)
		subStrings.emplace_back(string.substr(pos, findPos - pos));
		pos = findPos + 1;
	}

	if (pos < string.size())
	{
		subStrings.emplace_back(string.substr(pos));
	}

	return subStrings;
}

static inline size_t get_string_hash(const std::string& name) noexcept
{
	std::hash<std::string> hash;
	return hash(string_to_lower(name));
}

static inline int get_collectible_by_name(std::string& name) noexcept
{
	int itemId = LuaEngine::Isaac_GetItemIdByName(&name);
	return itemId > -1 ? itemId : COLLECTIBLE_NULL;
}

static inline void finalize_pool_item(PoolItem* poolItem) noexcept
{
	auto* collectibleConfig = g_Manager->GetItemConfig()->GetCollectible(poolItem->_itemID);
	if (!collectibleConfig)
	{
		return;
	}

	poolItem->_isSpecial = collectibleConfig->special;
	poolItem->_isUnlocked = g_Manager->GetPersistentGameData()->Unlocked(collectibleConfig->achievementId);
}

template <typename T>
static inline T cast_to_integer(long long value, Error& error) noexcept
{
	static_assert(std::is_integral<T>::value, "T must be an integer type");
	static_assert(std::is_signed<T>::value, "T must be a signed integer type");

	constexpr auto min = std::numeric_limits<T>::lowest();
	constexpr auto max = std::numeric_limits<T>::max();

	error.reset();

	if (value < static_cast<long long>(min) || value > static_cast<long long>(max))
	{
		error = "value " + std::to_string(value) + " is not within numeric limits(" + std::to_string(min) + ", " + std::to_string(max) + ")";
		return 0;
	}
	return static_cast<T>(value);
}

template <typename T>
static inline T cast_to_unsigned_integer(unsigned long long value, Error& error) noexcept
{
	static_assert(std::is_integral<T>::value, "T must be an integer type");
	static_assert(!std::is_signed<T>::value, "T must be an unsigned integer type");

	constexpr auto max = std::numeric_limits<T>::max();

	error.reset();

	if (value > static_cast<unsigned long long>(max))
	{
		error = "value " + std::to_string(value) + " is not within numeric limits (" + std::to_string(0) + ", " + std::to_string(max) + ")";
		return 0;
	}

	return static_cast<T>(value);
}

template <typename T>
static inline T cast_to_floating_point(long double value, Error& error) noexcept
{
	static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

	constexpr auto min = std::numeric_limits<T>::lowest();
	constexpr auto max = std::numeric_limits<T>::max();

	error.reset();

	if (value < static_cast<long double>(min) || value > static_cast<long double>(max))
	{
		error = "value \"" + std::to_string(value) + "\" is not within numeric limits (" + std::to_string(min) + ", " + std::to_string(max) + ")";
		return 0;
	}

	return static_cast<T>(value);
}

template <typename T>
static inline T string_to_numerical_value(const char* string, Error& error, int base = 10) noexcept
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, __FUNCTION__ " is only for numerical types.");

	error.reset();
	errno = 0;

	if constexpr (std::is_integral<T>::value)
	{
		if constexpr (std::is_signed<T>::value)
		{
			char* endPtr = nullptr;
			long long convertedValue = std::strtoll(string, &endPtr, base);

			if (endPtr == string)
			{
				error = "value " + std::string(string) + " is not a valid integer";
				return 0;
			}

			if (errno == ERANGE)
			{
				error = "value " + std::string(string) + " is out of range";
				return 0;
			}

			return cast_to_integer<T>(convertedValue, error);
		}
		else
		{
			char* endPtr = nullptr;
			unsigned long long convertedValue = std::strtoull(string, &endPtr, base);

			if (endPtr == string)
			{
				error = "value " + std::string(string) + " is not a valid integer";
				return 0;
			}

			if (errno == ERANGE)
			{
				error = "value " + std::string(string) + " is out of range";
				return 0;
			}

			return cast_to_unsigned_integer<T>(convertedValue, error);
		}
	}
	else
	{
		char* endPtr = nullptr;
		long double convertedValue = std::strtold(string, &endPtr);

		if (endPtr == string)
		{
			error = "value " + std::string(string) + " is not a valid number";
			return 0;
		}

		if (errno == ERANGE)
		{
			error = "value " + std::string(string) + " is out of range";
			return 0;
		}

		return cast_to_floating_point<T>(convertedValue, error);
	}
}

#pragma region XML Parsing

template <typename T>
static bool parse_xml_int_value(const XMLAttributes& node, T& value, const char* attributeName, Warnings& warnings) noexcept
{
	static_assert(std::is_integral<T>::value, __FUNCTION__ " is only for integer types.");
	auto it = node.find(attributeName);

	if (it == node.end())
	{
		return false;
	}

	Error error;
	T temp = string_to_numerical_value<T>(it->second.c_str(), error);
	if (error)
	{
		warnings.emplace_back("Invalid argument for <" + std::string(attributeName) + "> ( " + error.value() + ")");
		return false;
	}

	value = temp;
	return true;
}

template <typename T>
static inline bool parse_xml_float_value(const XMLAttributes& node, T& value, const char* attributeName, Warnings& warnings) noexcept
{
	static_assert(std::is_floating_point<T>::value, __FUNCTION__ " is only for floating point numbers.");
	auto it = node.find(attributeName);

	if (it == node.end())
	{
		return false;
	}

	Error error;
	T temp = string_to_numerical_value<T>(it->second.c_str(), error);
	if (error)
	{
		warnings.emplace_back("Invalid argument for <" + std::string(attributeName) + "> ( " + error.value() + ")");
		return false;
	}

	value = temp;
	return true;
}

static inline bool file_exists(const std::string& filePath) noexcept
{
	const char* path = g_ContentManager.GetMountedFilePath(filePath.c_str());
	bool result = path != nullptr;
	delete[] path;
	return result;
}

static inline std::vector<char> get_file_stream(const std::string& filePath, Error& error) noexcept
{
	std::vector<char> stream;
	error.reset();

	auto* file = g_FileManager.OpenRead(filePath.c_str());
	if (!file)
	{
		error = "Failed to open file \"" + filePath + "\"";
		return stream;
	}

	size_t streamSize = file->GetSize();
	stream.resize(streamSize + 1);

	file->Read(&stream.front(), 1, streamSize);
	stream[streamSize] = '\0';

	delete file;
	return stream;
}

static int get_xml_line_number(const char* data, const char* errorOffset) noexcept
{
	if (strlen(errorOffset) <= 0) { return 0; }

	int lineNumber = 1;
	const char* current = data;
	while (current < errorOffset) {
		if (*current == '\n') {
			lineNumber++;
		}
		current++;
	}
	return lineNumber;
}

static inline void parse_xml_document(const std::unique_ptr<rapidxml::xml_document<>>& document, char* stream, Error& error) noexcept
{
	error.reset();

	try
	{
		document->parse<0>(stream);
	}
	catch (const rapidxml::parse_error& e)
	{
		error = e.what() + std::string(" at line ") + std::to_string(get_xml_line_number(stream, e.where<char>()));
		return;
	}
}

constexpr size_t CHAOS_NONE = 0;
constexpr size_t CHAOS_NORMAL = 1;
constexpr size_t CHAOS_GREED = 2;
constexpr size_t CHAOS_ALL = 3;

static const std::map<size_t, int> s_ChaosModes = {
	{get_string_hash("none"), CHAOS_NONE},
	{get_string_hash("normal"), CHAOS_NORMAL},
	{get_string_hash("greed"), CHAOS_GREED},
	{get_string_hash("all"), CHAOS_ALL}
};

static std::bitset<ItemPoolManager::NUM_CHAOS_MODES> parse_chaos_mode(rapidxml::xml_attribute<>* xmlAttribute, Warnings& warnings) noexcept
{
	std::bitset<ItemPoolManager::NUM_CHAOS_MODES> chaosMode;
	auto chaosModes = string_split(xmlAttribute->value());

	for (const auto& mode : chaosModes)
	{
		switch (get_string_hash(mode))
		{
		case CHAOS_NONE:
			chaosMode.reset();
			return chaosMode;
		case CHAOS_NORMAL:
			chaosMode.set(ItemPoolManager::CHAOS_MODE_NORMAL);
			break;
		case CHAOS_GREED:
			chaosMode.set(ItemPoolManager::CHAOS_MODE_GREED);
			break;
		case CHAOS_ALL:
			chaosMode.set();
			return chaosMode;
		default:
			warnings.emplace_back("invalid chaos mode: \"" + mode + "\"");
		}
	}

	return chaosMode;
}

void ItemPoolManager::parse_xml_pool_names(const std::string& filePath, const ModEntry* mod) noexcept
{
	constexpr const char* errorMsg = "[ItemPoolManager] [ERROR] - " __FUNCTION__ " - Error while parsing \"itempools.xml\" from \"%s\": %s\n";
	Error error;

	auto stream = get_file_stream(filePath, error);
	if (error)
	{
		ZHL::Log(errorMsg, get_mod_name(mod).c_str(), error.value().c_str());
		return;
	}

	std::unique_ptr<rapidxml::xml_document<>> document = std::make_unique<rapidxml::xml_document<>>();
	parse_xml_document(document, &stream.front(), error);
	if (error)
	{
		ZHL::Log(errorMsg, get_mod_name(mod).c_str(), error.value().c_str());
		return;
	}

	auto* rootNode = document->first_node("ItemPools");
	if (!rootNode)
	{
		ZHL::Log(errorMsg, get_mod_name(mod).c_str(), "root node \"ItemPools\" not found");
		return;
	}

	auto* poolNode = rootNode = rootNode->first_node("Pool");
	for (; poolNode != nullptr; poolNode = poolNode->next_sibling("Pool"))
	{
		auto* name = poolNode->first_attribute("Name");
		if (name == nullptr)
		{
			ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - Found unnamed pool while parsing \"itempools.xml\" from \"%s\"\n", get_mod_name(mod).c_str());
			continue;
		}

		Warnings warnings;
		add_modded_pool(name->value(), poolNode, warnings);
		if (!warnings.empty())
		{
			ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - 1 or more warnings while parsing pool node \"%s\" from \"%s\":\n", name->value(), get_mod_name(mod).c_str());
			log_warnings(warnings);
		}
	}
}

static inline ItemPoolManager::PoolItemDesc parse_xml_pool_item(const XMLAttributes& itemNode, Warnings& warnings) noexcept
{
	ItemPoolManager::PoolItemDesc item;

	if (parse_xml_int_value(itemNode, item.itemId, "id", warnings))
	{
		if (item.itemId >= NUM_COLLECTIBLES)
		{
			warnings.emplace_back("<id> should not be used for modded items, prefer <name> instead");
		}
	}
	else
	{
		auto it = itemNode.find("name");
		if (it != itemNode.end())
		{
			std::string name = it->second;
			item.itemId = get_collectible_by_name(name);
		}
	}

	parse_xml_float_value(itemNode, item.weight, "weight", warnings);
	parse_xml_float_value(itemNode, item.decreaseBy, "decreaseby", warnings);
	parse_xml_float_value(itemNode, item.removeOn, "removeon", warnings);

	return item;
}

static inline void load_xml_pool_data(ItemPoolManager::ItemPool* itemPool) noexcept
{
	const std::string& name = itemPool->GetName();
	auto itemPoolNode = XMLStuff.PoolData->GetXMLNodeNChildsByName(name);
	auto& childNodes = std::get<1>(itemPoolNode);

	auto it = childNodes.find("item");
	if (it == childNodes.end())
	{
		ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - itempool \"%s\" has no items\n", name.c_str());
		return;
	}

	auto* pool = itemPool->GetPoolData();
	pool->_poolList.reserve(it->second.size());

	for (const auto& itemNode : it->second)
	{
		Warnings warnings;
		itemPool->EmplaceItem(pool->_poolList.size(), parse_xml_pool_item(itemNode, warnings));
		if (!warnings.empty())
		{
			ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - 1 or more errors occured while parsing an item node for itemPool \"%s\"\n", name.c_str());
			log_warnings(warnings);
		}
	}
}

#pragma endregion

#pragma region Initialization

ItemPoolManager::ItemPool::ItemPool(int id, const std::string& name) noexcept
	: m_Id(id), m_Name(name)
{
	for (size_t i = 0; i < 3; i++)
	{
		m_SaveStates[i] = std::make_unique<PoolSaveState>();
	}
}

ItemPoolManager::CustomItemPool::CustomItemPool(int id, const std::string& name) noexcept
	: ItemPool(id, name)
{
	for (size_t i = 0; i < 3; i++)
	{
		m_SaveStates[i] = std::make_unique<CustomPoolSaveState>();
	}
}

void ItemPoolManager::reset() noexcept
{
	m_AppendMode = false;
	m_ItemPoolInitialized = false;
	m_ItemPools.clear();
	m_PoolByName.clear();
}

ItemPoolManager::ItemPool* ItemPoolManager::add_pool(const std::string& name, bool isCustom) noexcept
{
	if (isCustom)
	{
		m_ItemPools.emplace_back(std::make_unique<CustomItemPool>(m_ItemPools.size(), name));
	}
	else
	{
		m_ItemPools.emplace_back(std::make_unique<ItemPool>(m_ItemPools.size(), name));
	}

	auto* itemPool = m_ItemPools.back().get();
	m_PoolByName[get_string_hash(name)] = itemPool;

	ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - created pool \"%s\" with id: %u\n", name.c_str(), itemPool->m_Id);

	return itemPool;
}

void ItemPoolManager::add_base_pool(const std::string& name, bool isGreedMode) noexcept
{
	auto* itemPool = this->add_pool(name, false);

	itemPool->m_ChaosMode.reset();
	uint32_t chaosMode = isGreedMode ? CHAOS_MODE_GREED : CHAOS_MODE_NORMAL;
	itemPool->m_ChaosMode.set(chaosMode);
	itemPool->m_IsChaosModeDefined = true;
}

void ItemPoolManager::add_modded_pool(const std::string& name, rapidxml::xml_node<>* xmlNode, Warnings& warnings) noexcept
{
	auto* itemPool = this->get_pool_by_name(name);
	if (!itemPool)
	{
		itemPool = this->add_pool(name, true);
	}

	auto* chaosMode = xmlNode->first_attribute("ChaosMode", 0U, false);
	if (!itemPool->m_IsChaosModeDefined && chaosMode)
	{
		itemPool->m_ChaosMode = parse_chaos_mode(chaosMode, warnings);
	}
}

void ItemPoolManager::add_base_pools() noexcept
{
	this->add_base_pool("treasure", false);
	this->add_base_pool("shop", false);
	this->add_base_pool("boss", false);
	this->add_base_pool("devil", false);
	this->add_base_pool("angel", false);
	this->add_base_pool("secret", false);
	this->add_base_pool("library", false);
	this->add_base_pool("shellGame", false);
	this->add_base_pool("goldenChest", false);
	this->add_base_pool("redChest", false);
	this->add_base_pool("beggar", false);
	this->add_base_pool("demonBeggar", false);
	this->add_base_pool("curse", false);
	this->add_base_pool("keyMaster", false);
	this->add_base_pool("batteryBum", false);
	this->add_base_pool("momsChest", false);
	this->add_base_pool("greedTreasure", true);
	this->add_base_pool("greedBoss", true);
	this->add_base_pool("greedShop", true);
	this->add_base_pool("greedDevil", true);
	this->add_base_pool("greedAngel", true);
	this->add_base_pool("greedCurse", true);
	this->add_base_pool("greedSecret", true);
	this->add_base_pool("craneGame", false);
	this->add_base_pool("ultraSecret", false);
	this->add_base_pool("bombBum", false);
	this->add_base_pool("planetarium", false);
	this->add_base_pool("oldChest", false);
	this->add_base_pool("babyShop", false);
	this->add_base_pool("woodenChest", false);
	this->add_base_pool("rottenBeggar", false);
}

void ItemPoolManager::add_modded_pools() noexcept
{
	auto* modManager = g_Manager->GetModManager();
	std::string xmlName = "itempools.xml";

	std::string xmlPath;
	modManager->TryRedirectPath(&xmlPath, &xmlName);

	this->parse_xml_pool_names(xmlPath, nullptr);

	for (auto* mod : modManager->_mods)
	{
		if (!mod->_loaded)
		{
			continue;
		}

		mod->GetContentPath(&xmlPath, &xmlName);
		if (file_exists(xmlPath))
		{
			this->parse_xml_pool_names(xmlPath, mod);
		}
	}
}

void ItemPoolManager::__Init() noexcept
{
	auto& instance = ItemPoolManager::Get();
	instance.reset();
	instance.add_base_pools();
	instance.add_modded_pools();
}

#pragma endregion

#pragma region Data Load

static inline void reset_pool(ItemPool_Item* pool) noexcept
{
	auto& itemPool = g_Game->_itemPool;

	pool->_totalWeight = 0.0f;
	pool->_poolList.clear();
	pool->_rng1.SetSeed(itemPool._itemPoolRNG.Next(), 35);
	pool->_rng2.SetSeed(itemPool._itemPoolRNG.Next(), 35);
	pool->_bibleUpgrade = 0;
}

PoolItem ItemPoolManager::PoolItemDesc::BuildPoolItem() const noexcept
{
	PoolItem poolItem;
	poolItem._itemID = this->itemId;
	poolItem._initialWeight = this->weight;
	poolItem._weight = poolItem._initialWeight;
	poolItem._decreaseBy = this->decreaseBy;
	poolItem._removeOn = this->removeOn;
	finalize_pool_item(&poolItem);

	return poolItem;
}

void ItemPoolManager::ItemPool::load_pool() noexcept
{
	auto* pool = this->GetPoolData();

	m_NumRegularItems = pool->_poolList.size();
	pool->_poolList.reserve(m_NumRegularItems + m_VirtualItems.size());

	for (size_t i = 0; i < m_AppendIndex; i++)
	{
		this->EmplaceItem(pool->_poolList.size(), m_VirtualItems[i]);
	}
}

void ItemPoolManager::CustomItemPool::load_pool() noexcept
{
	auto* pool = this->GetPoolData();

	reset_pool(pool);
	load_xml_pool_data(this);

	this->ItemPool::load_pool();
}

void ItemPoolManager::ItemPool::shuffle_pool(MTRNG& rng) noexcept
{
	auto* poolData = this->GetPoolData();

	auto begin = poolData->_poolList.begin();
	auto end = poolData->_poolList.end();

	if (begin == end)
	{
		return;
	}

	for (auto it = end - 1; it > begin; --it)
	{
		int randomIndex = rng.Next() % (it - begin + 1);
		std::iter_swap(it, begin + randomIndex);
	}
}

void ItemPoolManager::ItemPool::finalize_pool() noexcept
{
	auto* pool = this->GetPoolData();

	for (size_t i = m_AppendIndex; i < m_VirtualItems.size(); i++)
	{
		this->EmplaceItem(pool->_poolList.size(), m_VirtualItems[i]);
	}

	this->update_static_pool_list();
}

void ItemPoolManager::ItemPool::update_static_pool_list() noexcept
{
	auto& poolList = this->GetPoolData()->_poolList;

	m_StaticPoolList[0] = poolList.data();
	m_StaticPoolList[1] = poolList.data() + this->get_start_of_append();
	m_StaticPoolList[2] = poolList.data() + poolList.capacity();
}

void ItemPoolManager::__LoadPools() noexcept
{
	auto& instance = ItemPoolManager::Get();

	instance.m_AppendMode = true;
	instance.m_ItemPoolInitialized = true;

	for (auto& itemPool : instance.m_ItemPools)
	{
		itemPool->load_pool();
	}
}

void ItemPoolManager::__FinalizePools() noexcept
{
	auto& instance = ItemPoolManager::Get();

	MTRNG mtRNG = MTRNG();
	mtRNG.Init(g_Game->_itemPool._itemPoolRNG._seed);

	for (size_t i = NUM_ITEMPOOLS; i < instance.m_ItemPools.size(); i++)
	{
		instance.m_ItemPools[i]->shuffle_pool(mtRNG);
	}

	for (auto& itemPool : instance.m_ItemPools)
	{
		itemPool->finalize_pool();
	}
}

#pragma endregion

#pragma region SaveState

void ItemPoolManager::ItemPool::invalidate_state(size_t slot) noexcept
{
	m_SaveStates[slot]->m_IsUsable = false;
}

void ItemPoolManager::ItemPool::store_state(size_t saveSlot) noexcept
{
	auto* saveState = m_SaveStates[saveSlot].get();
	auto* poolData = this->GetPoolData();

	saveState->m_IsUsable = true;
	saveState->m_TemporaryItems.clear();

	size_t begin = this->get_start_of_temporary();
	size_t end = poolData->_poolList.size();
	begin = std::min(begin, end);

	size_t num_items = end - begin;
	saveState->m_TemporaryItems.resize(num_items);

	auto* temporaryItems = &poolData->_poolList[begin];
	for (size_t i = 0; i < num_items ; i++)
	{
		saveState->m_TemporaryItems[i] = temporaryItems[i];
	}
}

void ItemPoolManager::CustomItemPool::store_state(size_t saveSlot) noexcept
{
	auto* saveState = static_cast<CustomPoolSaveState*>(m_SaveStates[saveSlot].get());
	auto* poolData = this->GetPoolData();

	saveState->m_IsUsable = true;
	saveState->m_ItemWeights.clear();
	saveState->m_ItemWeights.reserve(poolData->_poolList.size());

	for (auto& poolItem : poolData->_poolList)
	{
		saveState->m_ItemWeights.push_back(poolItem._weight);
	}

	saveState->m_BibleUpgrades = poolData->_bibleUpgrade;
	saveState->m_RemainingGenesisItems = m_RemainingGenesisItems;

	this->ItemPool::store_state(saveSlot);
}

void ItemPoolManager::ItemPool::restore_state(size_t saveSlot) noexcept
{
	auto* saveState = m_SaveStates[saveSlot].get();
	auto* poolData = this->GetPoolData();

	if (!saveState->m_IsUsable)
	{
		return;
	}

	size_t begin = std::min(this->get_start_of_temporary(), poolData->_poolList.size());

	poolData->_poolList.resize(begin + saveState->m_TemporaryItems.size());

	auto* temporaryItems = &poolData->_poolList[begin];
	for (size_t i = 0; i < saveState->m_TemporaryItems.size(); i++)
	{
		temporaryItems[i] = saveState->m_TemporaryItems[i];
	}
}

void ItemPoolManager::CustomItemPool::restore_state(size_t saveSlot) noexcept
{
	auto* saveState = static_cast<CustomPoolSaveState*>(m_SaveStates[saveSlot].get());
	auto* poolData = this->GetPoolData();

	if (!saveState->m_IsUsable)
	{
		return;
	}

	size_t begin = 0;
	size_t end = std::min(poolData->_poolList.size(), saveState->m_ItemWeights.size());
	
	for (size_t i = begin; i < end; i++)
	{
		poolData->_poolList[i]._weight = saveState->m_ItemWeights[i];
	}

	poolData->_bibleUpgrade = saveState->m_BibleUpgrades;
	m_RemainingGenesisItems = saveState->m_RemainingGenesisItems;

	this->ItemPool::restore_state(saveSlot);
}

void ItemPoolManager::invalidate_state_slot(uint32_t slot) noexcept
{
	for (auto& itemPool : m_ItemPools)
	{
		itemPool.get()->invalidate_state(slot);
	}
}

void ItemPoolManager::__SaveState(uint32_t slot) noexcept
{
	auto& instance = ItemPoolManager::Get();

	for (auto& itemPool : instance.m_ItemPools)
	{
		itemPool.get()->store_state(slot);
	}
}

void ItemPoolManager::__RestoreState(uint32_t slot) noexcept
{
	auto& instance = ItemPoolManager::Get();

	for (auto& itemPool : instance.m_ItemPools)
	{
		itemPool.get()->restore_state(slot);
	}

	switch (slot)
	{
	case REPENTOGON::GameStateSlot::GLOWING_HOURGLASS_1:
		instance.invalidate_state_slot(REPENTOGON::GameStateSlot::GLOWING_HOURGLASS_2);
		break;
	case REPENTOGON::GameStateSlot::GLOWING_HOURGLASS_2:
		instance.invalidate_state_slot(REPENTOGON::GameStateSlot::GLOWING_HOURGLASS_1);
		break;
	}
}

#pragma endregion

#pragma region Serialization

constexpr uint32_t GAMESTATE_VERSION = 1;

static inline bool is_cloud_save(GameState* gameState) noexcept
{
	return !(gameState->_cloudGameStatePath.empty() || gameState->_cloudRerunStatePath.empty());
}

static inline std::string get_state_file_name(GameState* gameState, uint32_t saveSlot, bool isRerun) noexcept
{
	return (is_cloud_save(gameState) ? "cloud" : "local") + std::string("_") + (isRerun ? "rep+rerunstate" : "gamestate") + std::to_string(saveSlot);
}

static inline std::string get_state_file_name(GameState* gameState, GameStateIO* io) noexcept
{
	std::filesystem::path filePath(io->GetFilePath());
	return (is_cloud_save(gameState) ? "cloud" : "local") + std::string("_") + filePath.stem().string();
}

static inline std::string get_state_file_path(const std::string& fileName) noexcept
{
	return std::string(REPENTOGON::GetRepentogonDataPath()).append("ItemPoolManager/").append(fileName).append(".json");
}

static inline rapidjson::Document get_json_document(const std::string& filePath, Error& error) noexcept
{
	error.reset();
	rapidjson::Document doc;

	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		error = "Failed to open file \"" + filePath + "\"";
		return doc;
	}

	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	doc.Parse(content.c_str());

	if (doc.HasParseError())
	{
		error = "Failed to parse json file \"" + filePath + "\"";
		return doc;
	}

	return doc;
}

#pragma region JSON Validation

static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

// Type trait to map a type to its corresponding `rapidjson` type-check method and expected type name
template <typename T>
struct JsonValidatorTraits {
	static_assert(sizeof(T) == 0, "Unsupported type for ValidatorTraits");
};

template <>
struct JsonValidatorTraits<int64_t> {
	static constexpr auto IsValid = &rapidjson::Value::IsInt64;
	static constexpr const char* TypeName = "integer";
};

template <>
struct JsonValidatorTraits<uint64_t> {
	static constexpr auto IsValid = &rapidjson::Value::IsUint64;
	static constexpr const char* TypeName = "unsigned integer";
};

template <>
struct JsonValidatorTraits<double> {
	static constexpr auto IsValid = &rapidjson::Value::IsDouble;
	static constexpr const char* TypeName = "number";
};

template <>
struct JsonValidatorTraits<std::string> {
	static constexpr auto IsValid = &rapidjson::Value::IsString;
	static constexpr const char* TypeName = "string";
};

template <>
struct JsonValidatorTraits<std::vector<void>> { // Placeholder for JSON array
	static constexpr auto IsValid = &rapidjson::Value::IsArray;
	static constexpr const char* TypeName = "json array";
};

template <>
struct JsonValidatorTraits<std::map<void, void>> { // Placeholder for JSON object
	static constexpr auto IsValid = &rapidjson::Value::IsObject;
	static constexpr const char* TypeName = "json object";
};

template <typename T>
static bool validate_json(const rapidjson::Value& node, const char* attributeName, Warnings& warnings) noexcept
{
	constexpr auto IsValid = JsonValidatorTraits<T>::IsValid;    // Retrieve the type-check method
	constexpr const char* TypeName = JsonValidatorTraits<T>::TypeName; // Retrieve the expected type name

	if (attributeName && !node.HasMember(attributeName)) {
		return false;
	}

	const auto& jsonValue = attributeName ? node[attributeName] : node;
	if (!(jsonValue.*IsValid)()) {
		if (attributeName) {
			warnings.emplace_back("Invalid argument for \"" + std::string(attributeName) + "\" (type \"" + std::string(kTypeNames[jsonValue.GetType()]) + "\" is not a valid " + std::string(TypeName) + ")");
		}
		else {
			warnings.emplace_back("Invalid value (type \"" + std::string(kTypeNames[jsonValue.GetType()]) + "\" is not a valid " + std::string(TypeName) + ")");
		}
		return false;
	}

	return true;
}

// This function assumes that the json value has already been validated
template <typename T>
static bool assign_json_value(const rapidjson::Value& node, T& value, const char* attributeName, Warnings& warnings) noexcept
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, __FUNCTION__ " is only for numerical types.");

	const auto& jsonValue = attributeName ? node[attributeName] : node;
	Error error;

	if constexpr (std::is_integral<T>::value)
	{
		if constexpr (std::is_signed<T>::value)
		{
			value = cast_to_integer<T>(jsonValue.GetInt64(), error);
		}
		else
		{
			value = cast_to_unsigned_integer<T>(static_cast<uint64_t>(jsonValue.GetInt64()), error);
		}
	}
	else if constexpr (std::is_floating_point<T>::value)
	{
		value = cast_to_floating_point<T>(jsonValue.GetDouble(), error);
	}
	else
	{
		static_assert(sizeof(T) == 0, "Unsupported type for assign_json_value");
	}

	if (error)
	{
		std::string errorMessage;

		if (attributeName) 
		{
			errorMessage = "Invalid argument for \"" + std::string(attributeName) + "\" (" + error.value() + ")";
		}
		else
		{
			errorMessage = "Invalid value (" + error.value() + ")";
		}
		warnings.emplace_back(std::move(errorMessage));
		return false;
	}

	return true;
}

template <typename T>
static bool parse_json_numeric_value(const rapidjson::Value& node, T& value, const char* attributeName, Warnings& warnings) noexcept
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, __FUNCTION__ " is only for numeric types.");
	bool valid = false;
	if constexpr (std::is_integral<T>::value) {
		if constexpr (std::is_signed<T>::value) {
			valid = validate_json<int64_t>(node, attributeName, warnings);
		}
		else {
			valid = validate_json<uint64_t>(node, attributeName, warnings);
		}
	}
	else {
		valid = validate_json<double>(node, attributeName, warnings);
	}

	if (!valid)
	{
		return false;
	}

	return assign_json_value(node, value, attributeName, warnings);
}

static bool validate_json_string(const rapidjson::Value& node, const char* attributeName, Warnings& warnings) noexcept
{
	return validate_json<std::string>(node, attributeName, warnings);
}

static bool validate_json_array(const rapidjson::Value& node, const char* attributeName, Warnings& warnings) noexcept
{
	return validate_json<std::vector<void>>(node, attributeName, warnings);
}

static bool validate_json_object(const rapidjson::Value& node, const char* attributeName, Warnings& warnings) noexcept
{
	return validate_json<std::map<void, void>>(node, attributeName, warnings);
}

#pragma endregion

static inline rapidjson::Value serialize_pool_item(const PoolItem* poolItem, rapidjson::Document::AllocatorType& allocator) noexcept 
{
	rapidjson::Value jsonObj(rapidjson::kObjectType);

	auto* collectibleConfig = g_Manager->GetItemConfig()->GetCollectible(poolItem->_itemID);
	if (poolItem->_itemID >= NUM_ITEMPOOLS && collectibleConfig)
	{
		rapidjson::Value name(collectibleConfig->name.c_str(), allocator);
		jsonObj.AddMember("Name", name, allocator);
	}
	else
	{
		jsonObj.AddMember("ItemId", poolItem->_itemID, allocator);
	}
	jsonObj.AddMember("InitialWeight", poolItem->_initialWeight, allocator);
	jsonObj.AddMember("Weight", poolItem->_weight, allocator);
	jsonObj.AddMember("DecreaseBy", poolItem->_decreaseBy, allocator);
	jsonObj.AddMember("RemoveOn", poolItem->_removeOn, allocator);

	return jsonObj;
}

static inline PoolItem deserialize_pool_item(const rapidjson::Value& jsonObj, Warnings& warnings) noexcept
{
	PoolItem poolItem;
	if (!jsonObj.IsObject())
	{
		warnings.emplace_back("Pool Item node is not a json object");
		return poolItem;
	}

	if (parse_json_numeric_value(jsonObj, poolItem._itemID, "ItemId", warnings))
	{
		auto* collectibleConfig = g_Manager->GetItemConfig()->GetCollectible(poolItem._itemID);
		poolItem._itemID = collectibleConfig ? poolItem._itemID : COLLECTIBLE_NULL;
	}
	else if (validate_json_string(jsonObj, "Name", warnings))
	{
		std::string name = jsonObj["Name"].GetString();
		poolItem._itemID = get_collectible_by_name(name);
	}
	parse_json_numeric_value(jsonObj, poolItem._initialWeight, "InitialWeight", warnings);
	parse_json_numeric_value(jsonObj, poolItem._weight, "Weight", warnings);
	parse_json_numeric_value(jsonObj, poolItem._decreaseBy, "DecreaseBy", warnings);
	parse_json_numeric_value(jsonObj, poolItem._removeOn, "RemoveOn", warnings);

	finalize_pool_item(&poolItem);

	return poolItem;
}

void ItemPoolManager::PoolSaveState::serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) const noexcept
{
	rapidjson::Value temporaryItems(rapidjson::kArrayType);
	for (const auto& item : m_TemporaryItems)
	{
		temporaryItems.PushBack(serialize_pool_item(&item, allocator), allocator);
	}

	node.AddMember("TemporaryItems", temporaryItems, allocator);
}

void ItemPoolManager::CustomPoolSaveState::serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) const noexcept
{
	rapidjson::Value itemWeights(rapidjson::kArrayType);
	for (float weight : m_ItemWeights)
	{
		itemWeights.PushBack(rapidjson::Value(weight), allocator);
	}

	node.AddMember("ItemWeights", itemWeights, allocator);
	node.AddMember("BibleUpgrades", rapidjson::Value(m_BibleUpgrades), allocator);
	node.AddMember("RemainingGenesisItems", rapidjson::Value(m_RemainingGenesisItems), allocator);

	this->PoolSaveState::serialize(node, allocator);
}

void ItemPoolManager::PoolSaveState::deserialize(const rapidjson::Value& poolSaveState, Warnings& warnings) noexcept
{
	if (!poolSaveState.IsObject())
	{
		warnings.emplace_back("Pool Save State node is not a json object");
		m_IsUsable = false;
		return;
	}

	m_IsUsable = true;
	m_TemporaryItems.clear();

	if (validate_json_array(poolSaveState, "TemporaryItems", warnings))
	{
		warnings.emplace_back("Errors occured while deserializing \"TemporaryItems\":"); // Place it beforehand so that we do not have use insert later
		size_t warningNum = warnings.size();
		const auto& temporaryItems = poolSaveState["TemporaryItems"].GetArray();
		m_TemporaryItems.reserve(temporaryItems.Size());

		for (const auto& poolItem : temporaryItems)
		{
			m_TemporaryItems.emplace_back(deserialize_pool_item(poolItem, warnings));
		}

		if (warningNum >= warnings.size()) // Remove error warning if no errors occured
		{
			warnings.pop_back();
		}
	}
	else
	{
		warnings.emplace_back("cannot load \"TemporaryItems\".");
	}
}

void ItemPoolManager::CustomPoolSaveState::deserialize(const rapidjson::Value& poolSaveState, Warnings& warnings) noexcept
{
	if (!poolSaveState.IsObject())
	{
		warnings.emplace_back("Pool Save State is not a json object");
		m_IsUsable = false;
		return;
	}

	m_IsUsable = true;
	m_ItemWeights.clear();
	m_BibleUpgrades = 0;
	m_RemainingGenesisItems = 0;

	if (validate_json_array(poolSaveState, "ItemWeights", warnings))
	{
		warnings.emplace_back("Errors occured while loading \"ItemWeights\":"); // Place it beforehand so that we do not have use insert later
		size_t warningNum = warnings.size();
		const auto& itemWeights = poolSaveState["ItemWeights"].GetArray();
		m_ItemWeights.reserve(itemWeights.Size());

		for (const auto& weight : itemWeights)
		{
			m_ItemWeights.push_back(0.0f);
			parse_json_numeric_value(weight, m_ItemWeights.back(), nullptr, warnings);
		}

		if (warningNum >= warnings.size()) // Remove error warning if no errors occured
		{
			warnings.pop_back();
		}
	}
	else
	{
		warnings.emplace_back("cannot load \"TemporaryItems\".");
	}

	parse_json_numeric_value(poolSaveState, m_BibleUpgrades, "BibleUpgrades", warnings);
	parse_json_numeric_value(poolSaveState, m_RemainingGenesisItems, "RemainingGenesisItems", warnings);
}

static inline std::optional<rapidjson::Document> get_saved_game_state(const std::string& fileName) noexcept
{
	auto savePath = get_state_file_path(fileName);

	if (!filesystem::exists(savePath))
	{
		ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - no %s for ItemPoolManager exists, skipping load.\n", fileName.c_str());
		return std::nullopt;
	}

	Error error;
	rapidjson::Document doc = get_json_document(savePath, error);

	if (error)
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to restore %s: %s\n", fileName.c_str(), error.value().c_str());
		return std::nullopt;
	}

	if (!doc.IsObject())
	{
		error = "Root of json file is not a valid Object (actual type \"" + std::string(kTypeNames[doc.GetType()]) + ")";
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to restore %s: %s\n", fileName.c_str(), error.value().c_str());
		return std::nullopt;
	}

	return doc;
}

static inline std::vector<int> init_game_state_pool_remap() noexcept
{
	std::vector<int> poolIdRemap;
	poolIdRemap.reserve(NUM_ITEMPOOLS);

	for (size_t i = 0; i < NUM_ITEMPOOLS; i++)
	{
		poolIdRemap.push_back(i);
	}

	return poolIdRemap;
}

static inline rapidjson::Value serialize_pool_index(rapidjson::Document::AllocatorType& allocator) noexcept
{
	rapidjson::Value poolIndex(rapidjson::kArrayType);
	for (size_t i = NUM_ITEMPOOLS; i < ItemPoolManager::GetNumItemPools(); i++)
	{
		rapidjson::Value name(ItemPoolManager::GetItemPool(i)->GetName().c_str(), allocator);
		poolIndex.PushBack(name, allocator);
	}

	return poolIndex;
}

static inline bool deserialize_pool_index(const rapidjson::Value& node, std::vector<int>& poolIdRemap, Warnings& warnings) noexcept
{
	if (!validate_json_array(node, "PoolIndex", warnings))
	{
		return false;
	}

	const auto& jsonPoolIndex = node["PoolIndex"].GetArray();
	poolIdRemap.reserve(NUM_ITEMPOOLS + jsonPoolIndex.Size());

	for (const auto& poolName : jsonPoolIndex)
	{
		if (!validate_json_string(poolName, nullptr, warnings))
		{
			poolIdRemap.push_back(POOL_NULL);
			continue;
		}

		poolIdRemap.push_back(ItemPoolManager::GetPoolIdByName(poolName.GetString()));
	}

	return true;
}

rapidjson::Document ItemPoolManager::serialize_game_state(bool isRerun) noexcept
{
	rapidjson::Document doc;
	doc.SetObject();

	uint32_t stateChecksum = g_Manager->_gamestate._checksum; // This is not a mistake, for some reason the _rerunChecksum is written in the regular _checksum variable

	doc.AddMember("Version", GAMESTATE_VERSION, doc.GetAllocator());
	doc.AddMember("Checksum", stateChecksum, doc.GetAllocator());

	doc.AddMember("PoolIndex", serialize_pool_index(doc.GetAllocator()), doc.GetAllocator());

	if (isRerun)
	{
		return doc;
	}
	
	rapidjson::Value poolsData(rapidjson::kArrayType);
	for (const auto& itemPool : m_ItemPools)
	{
		rapidjson::Value poolData(rapidjson::kObjectType);
		itemPool->m_SaveStates[REPENTOGON::GameStateSlot::SAVE_FILE]->serialize(poolData, doc.GetAllocator());
		poolsData.PushBack(poolData, doc.GetAllocator());
	}

	doc.AddMember("PoolData", poolsData, doc.GetAllocator());
	return doc;
}

bool ItemPoolManager::deserialize_game_state(const rapidjson::Document& gameState, const std::string& fileName, bool isRerun, std::vector<int>& poolIdRemap) noexcept
{
	constexpr const char* errorMessage = "[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to restore %s: %s\n";

	Warnings warnings;
	uint32_t version = 0;
	uint32_t checksum = 0;
	uint32_t gameChecksum = isRerun ? g_Manager->_gamestate._rerunChecksum : g_Manager->_gamestate._checksum;

	if (!parse_json_numeric_value(gameState, version, "Version", warnings))
	{
		ZHL::Log(errorMessage, fileName.c_str(), "unable to parse gamestate version");
		log_warnings(warnings);
		return false;
	}

	if (version != GAMESTATE_VERSION)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - gamestate version mismatch in %s, gamestate will not be restored\n", fileName.c_str());
		ZHL::Log("Current Version: %u, GameState Version: %u\n", GAMESTATE_VERSION, version);
		return false;
	}

	if (!parse_json_numeric_value(gameState, checksum, "Checksum", warnings))
	{
		ZHL::Log(errorMessage, fileName.c_str(), "unable to parse checksum");
		log_warnings(warnings);
		return false;
	}

	// Compare the checksum to make sure that the game state being loaded is actually tied to the current game state, and not to some other game state
	if (checksum != gameChecksum)
	{
		ZHL::Log(errorMessage, fileName.c_str(), "checksum mismatch");
		ZHL::Log("GameState checksum: 0x%X, ItemPoolManager GameState Checksum: 0x%X\n", gameChecksum, checksum);
		return false;
	}

	if (!deserialize_pool_index(gameState, poolIdRemap, warnings))
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to deserialize pool index for %s, custom item pools cannot be restored.\n", fileName.c_str());
		log_warnings(warnings);
		warnings.clear();
	}

	if (!warnings.empty())
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - something went wrong while deserialize pool index for %s, some custom item pools may not be restored.\n", fileName.c_str());
		log_warnings(warnings);
		warnings.clear();
	}

	if (isRerun)
	{
		return true;
	}

	if (!validate_json_array(gameState, "PoolData", warnings))
	{
		ZHL::Log(errorMessage, fileName.c_str(), "unable to parse pool data");
		log_warnings(warnings);
		return false;
	}

	const auto& jsonPoolData = gameState["PoolData"].GetArray();
	if (poolIdRemap.size() != jsonPoolData.Size())
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - size mismatch between pool index (%u) and pool data (%u) while restoring %s, some pool data may not be properly restored.\n", poolIdRemap.size(), jsonPoolData.Size(), fileName.c_str());
	}

	size_t storedPools = std::min(poolIdRemap.size(), jsonPoolData.Size());
	std::vector<CustomPoolSaveState> danglingSaveStates;

	for (size_t i = 0; i < storedPools; i++)
	{
		ItemPool* pool = i < poolIdRemap.size() ? this->GetItemPool(poolIdRemap[i]) : nullptr;
		if (!pool)
		{
			danglingSaveStates.emplace_back();
			danglingSaveStates.back().deserialize(jsonPoolData[i], warnings);
			if (!warnings.empty())
			{
				ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - something went wrong while deserializing dangling save state from %s\n", fileName.c_str());
				log_warnings(warnings);
				warnings.clear();
			}

			continue;
		}

		pool->m_SaveStates[REPENTOGON::GameStateSlot::SAVE_FILE]->deserialize(jsonPoolData[i], warnings);
		if (!warnings.empty())
		{
			ZHL::Log("[ItemPoolManager] [WARN] - " __FUNCTION__ " - something went wrong while deserializing save state for pool \"%s\" from %s\n", pool->GetName().c_str(), fileName.c_str());
			log_warnings(warnings);
			warnings.clear();
		}
	}

	this->handle_dangling_save_states(danglingSaveStates);
	return true;
}

static std::optional<std::ofstream> open_game_state_file(std::filesystem::path filePath, Error& error) noexcept
{
	std::filesystem::path directory = filePath.parent_path();
	error.reset();

	std::error_code errorCode;
	if (!directory.empty() && !std::filesystem::exists(directory, errorCode))
	{
		if (errorCode)
		{
			error = "failed to check existence of game state directory: \"" + errorCode.message() + "\"";
			return std::nullopt;
		}

		std::filesystem::create_directories(directory, errorCode);
		if (errorCode)
		{
			error = "failed to create game state directory: \"" + errorCode.message() + "\"";
			return std::nullopt;
		}
	}

	std::ofstream file(filePath);
	if (!file)
	{
		error = "failed to open file for writing: \"" + filePath.string() + "\"";
		return file;
	}

	return file;
}

void ItemPoolManager::__ClearSaveState(uint32_t slot) noexcept
{
	auto& instance = ItemPoolManager::Get();
	instance.invalidate_state_slot(slot);
}

void ItemPoolManager::__SaveToDisk(const std::string& fileName, bool isRerun) noexcept
{
	Error error;
	auto& instance = ItemPoolManager::Get();

	std::filesystem::path filePath = get_state_file_path(fileName);
	auto file = open_game_state_file(filePath, error);
	if (error)
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to save %s: %s\n", fileName.c_str(), error.value().c_str());
		return;
	}

	rapidjson::Document doc = instance.serialize_game_state(isRerun);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	file.value() << buffer.GetString() << std::endl;
	ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - successfully saved %s to \"%s\"\n", fileName.c_str(), filePath.string().c_str());
}

void ItemPoolManager::__LoadFromDisk(const std::string& fileName, bool isRerun) noexcept
{
	auto& instance = ItemPoolManager::Get();

	std::vector<int> poolIdRemap = init_game_state_pool_remap();

	auto doc = get_saved_game_state(fileName);
	if (!doc)
	{
		ItemPoolManager::fix_original_game_state(poolIdRemap, isRerun);
		return;
	}

	if (instance.deserialize_game_state(doc.value(), fileName, isRerun, poolIdRemap))
	{
		ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - successfully loaded %s\n", fileName.c_str());
	}

	ItemPoolManager::fix_original_game_state(poolIdRemap, isRerun);
}

static void delete_game_state(const std::string& fileName, bool isRerun) noexcept
{
	std::filesystem::path filePath = get_state_file_path(fileName);

	std::error_code errorCode;
	if (!std::filesystem::exists(filePath, errorCode))
	{
		return;
	}

	bool status = std::filesystem::remove(filePath, errorCode);
	if (!status || errorCode)
	{
		if (errorCode)
		{
			ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to delete %s at \"%s\": %s\n", fileName.c_str(), filePath.string().c_str(), errorCode.message().c_str());
		}
		else
		{
			ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - unable to delete %s at \"%s\"\n", fileName.c_str(), filePath.string().c_str());
		}
	}
	else
	{
		ZHL::Log("[ItemPoolManager] [INFO] - " __FUNCTION__ " - successfully deleted %s at \"%s\"\n", fileName.c_str(), filePath.string().c_str());
	}
}

#pragma endregion

#pragma region Fix GameState

static inline int get_new_pool_id(int oldPoolId, const std::vector<int>& poolIdRemap) noexcept
{
	if (oldPoolId < 0 || (uint32_t)oldPoolId >= poolIdRemap.size())
	{
		return POOL_TREASURE;
	}

	return poolIdRemap[oldPoolId] != POOL_NULL ? poolIdRemap[oldPoolId] : POOL_TREASURE;
}

static void fix_entity_save_state(EntitySaveState& entity, const std::vector<int>& poolIdRemap) noexcept; // Forward declaration

static inline void fix_player_game_state(GameStatePlayer& player, std::vector<int>& poolIdRemap) noexcept
{
	for (auto& historyItem : player._history._historyItems)
	{
		historyItem._itemPoolType = get_new_pool_id(historyItem._itemPoolType, poolIdRemap);
	}

	for (auto& movingBoxEntity : player._movingBoxContents)
	{
		fix_entity_save_state(movingBoxEntity, poolIdRemap);
	}

	int* bocOutputPoolType = &player._bagOfCraftingOutput.itemPoolType;
	if (*bocOutputPoolType != POOL_NULL)
	{
		*bocOutputPoolType = get_new_pool_id(*bocOutputPoolType, poolIdRemap);
	}
}

static inline void fix_collectible_save_state(EntitySaveState& entity, const std::vector<int>& poolIdRemap) noexcept
{
	int* sourcePoolType = EntitySaveState::Collectible::GetSourcePoolType(entity);
	*sourcePoolType = get_new_pool_id(*sourcePoolType, poolIdRemap);

	EntitySaveState* flipSaveState = EntitySaveState::Collectible::GetFlipSaveState(entity);
	if (flipSaveState != nullptr)
	{
		fix_entity_save_state(*flipSaveState, poolIdRemap);
	}
}

void fix_entity_save_state(EntitySaveState& entity, const std::vector<int>& poolIdRemap) noexcept
{
	if (entity.type == 5 && entity.variant == 100 && entity.subtype != 0)
	{
		fix_collectible_save_state(entity, poolIdRemap);
	}
}

void ItemPoolManager::fix_original_game_state(std::vector<int>& poolIdRemap, bool isRerun) noexcept
{
	auto& gameState = g_Manager->_gamestate;

	// Backup players (Flipped T.Laz) are also stored in the _players array, so they are not handled separately
	for (size_t i = 0; i < gameState._playerCount; i++)
	{
		fix_player_game_state(gameState._players[i], poolIdRemap);
	}

	for (size_t i = 0; i < 4; i++)
	{
		auto& player = gameState._esauJrStates[i];
		if (player._playerType != -1) // -1 is used to mark the esauJrState as not existent
		{
			fix_player_game_state(player, poolIdRemap);
		}
	}

	if (isRerun)
	{
		return;
	}

	gameState._itemPool._lastPool = get_new_pool_id(gameState._itemPool._lastPool, poolIdRemap);

	for (size_t i = 0; i < gameState._roomCount; i++)
	{
		for (auto& entitySaveState : gameState._rooms[i].SavedEntities)
		{
			fix_entity_save_state(entitySaveState, poolIdRemap);
		}
	}
}

void ItemPoolManager::handle_dangling_save_states(std::vector<CustomPoolSaveState>& danglingSaveStates) noexcept
{
	auto& gameState = g_Manager->_gamestate;

	for (const auto& saveState : danglingSaveStates)
	{
		constexpr uint16_t max = std::numeric_limits<uint16_t>::max();
		uint16_t& treasureGenesisItems = gameState._itemPool._remainingGenesisItems[POOL_TREASURE];
		uint16_t increaseAmount = std::min(saveState.m_RemainingGenesisItems, (uint16_t)(max - treasureGenesisItems));
		treasureGenesisItems += increaseAmount;
	}
}

#pragma endregion

#pragma region Others

bool ItemPoolManager::PoolItemDesc::operator==(const PoolItem& item) const noexcept
{
	return this->itemId == item._itemID &&
		this->weight == item._initialWeight &&
		this->decreaseBy == item._decreaseBy &&
		this->removeOn == item._removeOn;
}

ItemPoolManager::ItemPool* ItemPoolManager::get_pool_by_name(const std::string& name) noexcept
{
	auto it = m_PoolByName.find(get_string_hash(name));
	return it != m_PoolByName.end() ? it->second : nullptr;
}

void ItemPoolManager::ItemPool::reset_collectible(int collectible) noexcept
{
	for (auto& poolItem : this->GetPoolData()->_poolList)
	{
		if (poolItem._itemID == collectible)
		{
			poolItem._weight = poolItem._initialWeight;
		}
	}
}

void ItemPoolManager::__ResetCollectible(int collectible) noexcept
{
	auto& instance = ItemPoolManager::Get();
	for (size_t i = NUM_ITEMPOOLS; i < instance.m_ItemPools.size(); i++)
	{
		instance.m_ItemPools[i]->reset_collectible(collectible);
	}
}

void ItemPoolManager::ItemPool::add_bible_upgrade(int amount) noexcept
{
	this->GetPoolData()->_bibleUpgrade += amount;
}

void ItemPoolManager::__AddBibleUpgrade(uint32_t poolId, int amount) noexcept
{
	auto& instance = ItemPoolManager::Get();

	if (poolId == instance.GetNumItemPools())
	{
		for (auto& itemPool : instance.m_ItemPools)
		{
			itemPool->add_bible_upgrade(amount);
		}
		return;
	}

	auto* itemPool = instance.GetItemPool(poolId);
	if (!itemPool)
	{
		ZHL::Log("[ItemPoolManager] [ERROR] - " __FUNCTION__ " - Invalid pool id: %d\n", poolId);
		return;
	}

	itemPool->add_bible_upgrade(amount);
}

uint32_t ItemPoolManager::__GetChaosPool(RNG* rng) noexcept
{
	constexpr uint32_t scaleFactor = 100;
	
	auto& instance = ItemPoolManager::Get();
	WeightedOutcomePicker picker;
	uint8_t chaosMode = g_Game->IsGreedMode() ? CHAOS_MODE_GREED : CHAOS_MODE_NORMAL;

	for (const auto& itemPool : instance.m_ItemPools)
	{
		if (!itemPool->IsChaosModeAllowed(chaosMode))
		{
			continue;
		}

		auto* poolData = itemPool->GetPoolData();
		WeightedOutcomePicker_Outcome outcome{ (uint32_t)itemPool->m_Id, (uint32_t)(poolData->_totalWeight * scaleFactor) };
		picker.AddOutcomeWeight(outcome, false);
	}

	rng->Next();

	RNG pickerRNG;
	pickerRNG.SetSeed(rng->_seed, 35);

	return picker.PickOutcome(pickerRNG);
}

void ItemPoolManager::__GetItemPoolsForCollectible(int collectible, std::vector<uint32_t>& result) noexcept
{
	auto& instance = ItemPoolManager::Get();

	for (const auto& itemPool : instance.m_ItemPools)
	{
		auto* poolData = itemPool->GetPoolData();
		for (const auto& poolItem : poolData->_poolList)
		{
			if (poolItem._itemID == collectible)
			{
				result.push_back(itemPool->GetId());
				break;
			}
		}
	}
}

uint16_t& ItemPoolManager::ItemPool::get_remaining_genesis_items() noexcept
{
	return g_Game->_itemPool._remainingGenesisItems[m_Id];
}

uint16_t& ItemPoolManager::CustomItemPool::get_remaining_genesis_items() noexcept
{
	return m_RemainingGenesisItems;
}

#pragma endregion

#pragma region Interface

enum LuaPoolItemField
{
	LUA_ITEM_ID = 0,
	LUA_ITEM_NAME = 1,
	LUA_WEIGHT = 2,
	LUA_DECREASE_BY = 3,
	LUA_REMOVE_ON = 4,

	NUM_LUA_POOL_ITEM_FIELDS
};

static const std::map<size_t, int> s_LuaPoolItemFields = {
	{get_string_hash("itemID"), LUA_ITEM_ID},
	{get_string_hash("name"), LUA_ITEM_NAME},
	{get_string_hash("weight"), LUA_WEIGHT},
	{get_string_hash("decreaseBy"), LUA_DECREASE_BY},
	{get_string_hash("removeOn"), LUA_REMOVE_ON},
};

ItemPoolManager::PoolItemDesc::PoolItemDesc(lua_State* L, int index, Warnings& warnings) noexcept
{
	int absIndex = lua_absindex(L, index);

	if (!lua_istable(L, absIndex))
	{
		warnings.emplace_back("argument is not a lua table");
		return;
	}

	std::bitset<NUM_LUA_POOL_ITEM_FIELDS> enteredFields;

	lua_pushnil(L);

	while (lua_next(L, absIndex) != 0)
	{
		if (enteredFields.all())
		{
			lua_pop(L, 2);
			break;
		}

		if (!lua_isstring(L, -2))
		{
			lua_pop(L, 1);
			continue;
		}

		size_t keyHash = get_string_hash(lua_tostring(L, -2));
		auto it = s_LuaPoolItemFields.find(keyHash);

		if (it == s_LuaPoolItemFields.end())
		{
			lua_pop(L, 1);
			continue;
		}

		if (enteredFields.test(it->second))
		{
			lua_pop(L, 1);
			continue;
		}

		std::string name;
		switch (it->second)
		{
		case LUA_ITEM_ID:
			if (!lua_isinteger(L, -1))
			{
				warnings.emplace_back("invalid parameter for \"ItemID\" (integer expected got " + std::string(lua_typename(L, lua_type(L, -1))) + ")");
				break;
			}
			this->itemId = (int)lua_tointeger(L, -1);
			enteredFields.set(LUA_ITEM_ID);
			enteredFields.set(LUA_ITEM_NAME);
			break;
		case LUA_ITEM_NAME:
			if (!lua_isstring(L, -1))
			{
				warnings.emplace_back("invalid parameter for \"Name\" (string expected got " + std::string(lua_typename(L, lua_type(L, -1))) + ")");
				break;
			}

			name = std::string(lua_tostring(L, -1));
			this->itemId = get_collectible_by_name(name);
			enteredFields.set(LUA_ITEM_ID);
			enteredFields.set(LUA_ITEM_NAME);
			break;
		case LUA_WEIGHT:
			if (!lua_isnumber(L, -1))
			{
				warnings.emplace_back("invalid parameter for \"Weight\" (number expected got " + std::string(lua_typename(L, lua_type(L, -1))) + ")");
				break;
			}
			this->weight = (float)lua_tonumber(L, -1);
			enteredFields.set(LUA_WEIGHT);
			break;
		case LUA_DECREASE_BY:
			if (!lua_isnumber(L, -1))
			{
				warnings.emplace_back("invalid parameter for \"DecreaseBy\" (number expected got " + std::string(lua_typename(L, lua_type(L, -1))) + ")");
				break;
			}
			this->decreaseBy = (float)lua_tonumber(L, -1);
			enteredFields.set(LUA_DECREASE_BY);
			break;
		case LUA_REMOVE_ON:
			if (!lua_isnumber(L, -1))
			{
				warnings.emplace_back("invalid parameter for \"RemoveOn\" (number expected got " + std::string(lua_typename(L, lua_type(L, -1))) + ")");
				break;
			}
			this->removeOn = (float)lua_tonumber(L, -1);
			enteredFields.set(LUA_REMOVE_ON);
			break;
		}
		lua_pop(L, 1);
	}
}

ItemPool_Item* ItemPoolManager::ItemPool::GetPoolData() noexcept
{
	return &g_Game->_itemPool._pools[m_Id];
}

ItemPool_Item* ItemPoolManager::CustomItemPool::GetPoolData() noexcept
{
	return &m_Pool;
}

void ItemPoolManager::ItemPool::EmplaceItem( size_t position, const ItemPoolManager::PoolItemDesc& poolItem) noexcept
{
	auto* poolData = this->GetPoolData();
	auto& poolList = poolData->_poolList;

	if (poolList.empty())
	{
		return;
	}

	position = std::min(position, poolList.size());

	poolList.emplace(poolList.begin() + position, poolItem.BuildPoolItem());
	poolData->_totalWeight += poolItem.weight;

	this->update_static_pool_list();
}

void ItemPoolManager::ItemPool::EraseItem(size_t position) noexcept
{
	auto* poolData = this->GetPoolData();
	auto& poolList = poolData->_poolList;

	if (poolList.empty())
	{
		return;
	}

	position = std::min(position, poolList.size());
	PoolItem& item = poolList[position];
	poolData->_totalWeight -= item._initialWeight;
	poolList.erase(poolList.begin() + position);

	this->update_static_pool_list();
}

uint16_t ItemPoolManager::ItemPool::GetRemainingGenesisItems() noexcept
{
	return this->get_remaining_genesis_items();
}

void ItemPoolManager::ItemPool::IncreaseRemainingGenesisItems(uint16_t amount) noexcept
{
	constexpr uint16_t max = std::numeric_limits<uint16_t>::max();
	uint16_t& value = this->get_remaining_genesis_items();
	amount = std::min(amount, (uint16_t)(max - value));
	value += amount;
}

void ItemPoolManager::ItemPool::DecreaseRemainingGenesisItems(uint16_t amount) noexcept
{
	uint16_t& value = this->get_remaining_genesis_items();
	amount = std::min(amount, value);
	value -= amount;
}

ItemPoolManager::ItemPool* ItemPoolManager::GetItemPool(uint32_t id) noexcept
{
	auto& instance = ItemPoolManager::Get();
	return id < instance.m_ItemPools.size() ? instance.m_ItemPools[id].get() : nullptr;
}

int ItemPoolManager::GetPoolIdByName(const std::string& name) noexcept
{
	auto& instance = ItemPoolManager::Get();
	auto* itemPool = instance.get_pool_by_name(name);
	return itemPool ? itemPool->m_Id : -1;
}

void ItemPoolManager::ItemPool::AddVirtualItem(const ItemPoolManager::PoolItemDesc& poolItem) noexcept
{
	auto& manager = ItemPoolManager::Get();

	if (manager.m_ItemPoolInitialized)
	{
		auto* poolData = this->GetPoolData();
		size_t insertIndex = std::min(this->get_start_of_temporary(), poolData->_poolList.size());
		this->EmplaceItem(insertIndex, poolItem);
	}

	m_VirtualItems.emplace_back(poolItem);
	if (!manager.m_AppendMode)
	{
		m_AppendIndex++;
	}
}

void ItemPoolManager::ItemPool::AddTemporaryItem(const ItemPoolManager::PoolItemDesc& poolItem, Error& error) noexcept
{
	error.reset();
	auto& instance = ItemPoolManager::Get();

	if (g_Manager->GetState() != eManagerState::STATE_GAME)
	{
		error = "Cannot add a temporary item while not in game";
		return;
	}

	if (!instance.m_ItemPoolInitialized)
	{
		error = "Cannot add a temporary item when ItemPool has not yet been initialized";
		return;
	}

	auto poolData = this->GetPoolData();
	this->EmplaceItem(poolData->_poolList.size(), poolItem);
}

void ItemPoolManager::ItemPool::RemoveTemporaryItem(const ItemPoolManager::PoolItemDesc& poolItem, Error& error) noexcept
{
	error.reset();
	auto& instance = ItemPoolManager::Get();

	if (g_Manager->GetState() != eManagerState::STATE_GAME)
	{
		error = "Cannot remove a temporary item while not in game";
		return;
	}

	if (!instance.m_ItemPoolInitialized)
	{
		error = "Cannot remove a temporary item when ItemPool has not yet been initialized";
		return;
	}

	auto* poolData = this->GetPoolData();
	auto& poolList = poolData->_poolList;

	if (poolList.empty())
	{
		return;
	}

	size_t beginIndex = this->get_start_of_temporary();
	beginIndex = std::min(beginIndex, poolList.size());

	for (size_t i = beginIndex; i < poolList.size(); i++)
	{
		if (poolItem == poolList[i])
		{
			this->EraseItem(i);
			break;
		}
	}
}

#pragma endregion

#pragma region Patches and Hooks

static int s_NumItemPools = 0; // Used by the ItemPool::GetNextGenesisPool patch
static ItemPool_Item s_NullItemPool; // Used by some patches to provide a pool if the id is invalid

#pragma region Hooks

HOOK_METHOD(ModManager, LoadConfigs, () -> void)
{
	ItemPoolManager::__Init();
	s_NumItemPools = ItemPoolManager::GetNumItemPools();
	super();
}

// Pools are loaded here since the load_pools function is called for each mod with an itempools.xml
HOOK_METHOD(ItemPool, shuffle_pools, () -> void)
{
	ItemPoolManager::__LoadPools();
	super();
	ItemPoolManager::__FinalizePools();
}

HOOK_METHOD(Game, End, (int EndingID) -> void)
{
	super(EndingID);
	ItemPoolManager::__End();
}

HOOK_METHOD(Game, SaveState, (GameState* state) -> void)
{
	super(state);

	auto slot = REPENTOGON::GetGameStateSlot(state);
	if (slot == REPENTOGON::GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - Game::SaveState Hook - could not determine Game State slot, skipping saving.\n");
		return;
	}

	ItemPoolManager::__SaveState(slot);
}

HOOK_METHOD(Game, RestoreState, (GameState* state, bool startGame) -> void)
{
	super(state, startGame);

	auto slot = REPENTOGON::GetGameStateSlot(state);
	if (slot == REPENTOGON::GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - Game::RestoreState Hook - could not determine Game State slot, skipping restore.\n");
		return;
	}

	ItemPoolManager::__RestoreState(slot);
}

HOOK_METHOD(GameState, Clear, () -> void)
{
	super();

	auto slot = REPENTOGON::GetGameStateSlot(this);
	if (slot == REPENTOGON::GameStateSlot::NULL_SLOT)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::Clear Hook - could not determine Game State slot, skipping clear.\n");
		return;
	}

	ItemPoolManager::__ClearSaveState(slot);
}

HOOK_METHOD(GameState, write, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	if (REPENTOGON::GetGameStateSlot(this) != REPENTOGON::GameStateSlot::SAVE_FILE) // This can occur when (presumably) the save state is being sent to online players joining mid-run
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::write Hook - writing non save file GameState, skipping save.\n");
		return true;
	}

	auto fileName = get_state_file_name(this, *io);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::write Hook - Unknown file name \"%s\", skipping save.\n", fileName.c_str());
		return true;
	}

	ItemPoolManager::__SaveToDisk(fileName, false);
	return true;
}

HOOK_METHOD(GameState, write_rerun, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	auto fileName = get_state_file_name(this, *io);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::write_rerun Hook - Unknown file name \"%s\", skipping save.\n", fileName.c_str());
		return true;
	}

	ItemPoolManager::__SaveToDisk(fileName, true);
	return true;
}

HOOK_METHOD(GameState, read, (GameStateIO** io, bool isLocalRun) -> bool)
{
	if (!super(io, isLocalRun))
	{
		return false;
	}

	if (!isLocalRun) // This occurs when loading a game state upon joining an already existing match.
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::read Hook - reading non save file GameState, skipping load.\n");
		return true;
	}

	auto fileName = get_state_file_name(this, *io);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::read Hook - Unknown file name \"%s\", skipping load.\n", fileName.c_str());
		return true;
	}

	ItemPoolManager::__LoadFromDisk(fileName, false);
	return true;
}

HOOK_METHOD(GameState, read_rerun, (GameStateIO** io) -> bool)
{
	if (!super(io))
	{
		return false;
	}

	auto fileName = get_state_file_name(this, *io);
	if (fileName.empty() || std::atoi(&fileName.back()) == 0)
	{
		ZHL::Log("[ItemPoolManager] [INFO] - GameState::read_rerun Hook - Unknown file name \"%s\", skipping load.\n", fileName.c_str());
		return true;
	}

	ItemPoolManager::__LoadFromDisk(fileName, true);
	return true;
}

HOOK_METHOD(GameState, Delete, () -> void)
{
	super();
	auto fileName = get_state_file_name(this, g_Manager->_currentSaveSlot, false);
	delete_game_state(fileName, false);
}

HOOK_METHOD(GameState, DeleteRerun, () -> void)
{
	super();
	auto fileName = get_state_file_name(this, g_Manager->_currentSaveSlot, true);
	delete_game_state(fileName, true);
}

HOOK_METHOD(ItemPool, get_chaos_pool, (RNG* rng) -> int)
{
	return ItemPoolManager::__GetChaosPool(rng);
}

HOOK_METHOD(ItemPool, ResetCollectible, (int collectible) -> void)
{
	super(collectible);
	ItemPoolManager::__ResetCollectible(collectible);
}

HOOK_METHOD(ItemPool, AddBibleUpgrade, (int add, int poolType) -> void)
{
	ItemPoolManager::__AddBibleUpgrade(poolType, add);
}

HOOK_METHOD(ItemPool, GetItemPoolsForCollectible, (int collectible, std_vector_uint* result) -> void)
{
	ItemPoolManager::__GetItemPoolsForCollectible(collectible, *result);
}

#pragma endregion

#pragma region Patches

constexpr const char* asmPatchMessage = "[ItemPoolManager] Patching %s at %p\n";

static void ASMPatchCheckGetCollectiblePool(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ByteBuffer byteBuffer = ByteBuffer();
	int* rawJumpOffset = (int*)byteBuffer.AddAny((char*)address + 0x5, 4).GetData();
	int jumpOffset = 0x9 + *rawJumpOffset;

	int* numItemPoolsPtr = &s_NumItemPools;

	ASMPatch patch;
	patch.AddBytes("\x3B\x35").AddBytes(ByteBuffer().AddAny((char*)&numItemPoolsPtr, sizeof(char*))) // CMP ESI, s_NumItemPools
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JAE, (char*)address + jumpOffset)
		.AddRelativeJump((char*)address + 0x9); // resume

	sASMPatcher.PatchAt(address, &patch);
}

// Pass the correct ItemPool_Item to pick_collectible
static ItemPool_Item* __stdcall asm_patch_get_item_pool_data(size_t itemPoolOffset) noexcept
{
	int itemPoolType = itemPoolOffset / sizeof(ItemPool_Item);
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (!itemPool)
	{
		&s_NullItemPool;
	}

	return itemPool->GetPoolData();
}

// This patch covers both pick_collectible and the BibleUpgrade evaluation
// The code is now similar enough to allow the same patch to work on both tries
static void ASMPatchPickCollectible(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EAX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX) // itemPoolOffset
		.AddInternalCall(asm_patch_get_item_pool_data) // call asm_patch_get_item_pool_data
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)address + 0x5);

	sASMPatcher.PatchAt(address, &patch);
}

static void __stdcall asm_patch_increase_genesis_items(uint32_t itemPoolType) noexcept
{
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (itemPool)
	{
		itemPool->IncreaseRemainingGenesisItems(1);
	}
}

static void ASMPatchPreTriggerGenesis(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX) // itemPoolType
		.AddInternalCall(asm_patch_increase_genesis_items) // call asm_patch_increase_genesis_items
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)address + 0x8); // resume
	sASMPatcher.PatchAt(address, &patch);
}

static void __stdcall asm_patch_decrease_genesis_items(uint32_t itemPoolType) noexcept
{
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (itemPool)
	{
		itemPool->DecreaseRemainingGenesisItems(1);
	}
}

static void ASMPatchDecreaseRemainingGenesisItems(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ESI) // itemPoolType
		.AddInternalCall(asm_patch_decrease_genesis_items) // call asm_patch_decrease_genesis_items
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)address + 0x8); // resume
	sASMPatcher.PatchAt(address, &patch);
}

static uint16_t __stdcall asm_patch_get_genesis_items(int itemPoolType) noexcept
{
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (!itemPool)
	{
		return 0;
	}

	return itemPool->GetRemainingGenesisItems();
}

static void ASMPatchGetNextGenesisPool(const char* getGenesisItems, const char* firstLoopCompare, const char* secondLoopCompare, const char* functionName) noexcept
{
	SigScan scannerGetGenesis(getGenesisItems);
	scannerGetGenesis.Scan();
	void* addressGetGenesis = scannerGetGenesis.GetAddress();

	SigScan scannerFirstLoopCompare(firstLoopCompare);
	scannerFirstLoopCompare.Scan();
	void* addressFirstLoopCompare = scannerFirstLoopCompare.GetAddress();

	SigScan scannerSecondLoopCompare(secondLoopCompare);
	scannerSecondLoopCompare.Scan();
	void* addressSecondLoopCompare = scannerSecondLoopCompare.GetAddress();
	
	printf("[ItemPoolManager] Patching %s at %p (getGenesisItems), %p (firstLoopCompare) and %p (secondLoopCompare)\n", functionName, addressGetGenesis, addressFirstLoopCompare, addressSecondLoopCompare);

	int* numItemPoolsPtr = &s_NumItemPools;

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::EDX, true);
	ASMPatch patchGetGenesisItems;

	patchGetGenesisItems.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::ESI) // itemPoolType
		.AddInternalCall(asm_patch_get_genesis_items) // call asm_patch_get_genesis_items
		.AddBytes("\x0F\xB7\xD0") // MOVZX EDX, AX
		.RestoreRegisters(reg)
		.AddBytes(ByteBuffer().AddAny((char*)addressGetGenesis + 0x3, 0x3)) // restore TEST DX, DX
		.AddRelativeJump((char*)addressGetGenesis + 0x6); // resume
	sASMPatcher.PatchAt(addressGetGenesis, &patchGetGenesisItems);

	ByteBuffer firstByteBuffer = ByteBuffer();
	int8_t* firstRawJumpOffset = (int8_t*)firstByteBuffer.AddAny((char*)addressFirstLoopCompare + 0x4, 1).GetData();
	int jumpOffset = 0x5 + *firstRawJumpOffset;

	ASMPatch patchFirstLoopCompare;
	patchFirstLoopCompare.AddBytes("\x3B\x35").AddBytes(ByteBuffer().AddAny((char*)&numItemPoolsPtr, sizeof(char*))) // CMP ESI, s_NumItemPools
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JC, (char*)addressFirstLoopCompare + jumpOffset)
		.AddRelativeJump((char*)addressFirstLoopCompare + 0x5); // resume
	sASMPatcher.PatchAt(addressFirstLoopCompare, &patchFirstLoopCompare);

	ByteBuffer secondByteBuffer = ByteBuffer();
	int* secondRawJumpOffset = (int*)secondByteBuffer.AddAny((char*)addressSecondLoopCompare + 0x5, 4).GetData();
	jumpOffset = 0x9 + *secondRawJumpOffset;

	ASMPatch patchSecondLoopCompare;
	patchSecondLoopCompare.AddBytes("\x3B\x05").AddBytes(ByteBuffer().AddAny((char*)&numItemPoolsPtr, sizeof(char*))) // CMP EAX, s_NumItemPools
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JA, (char*)addressSecondLoopCompare + jumpOffset)
		.AddRelativeJump((char*)addressSecondLoopCompare + 0x9); // resume
	sASMPatcher.PatchAt(addressSecondLoopCompare, &patchSecondLoopCompare);
}

static std::vector<PoolItem>* __stdcall asm_patch_get_static_pool_list(uint32_t itemPoolOffset) noexcept
{
	uint32_t itemPoolType = itemPoolOffset / sizeof(ItemPool_Item);
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (!itemPool)
	{
		return &s_NullItemPool._poolList;
	}

	return itemPool->__GetStaticPoolList();
}

static void ASMPatchBagOfCraftingPoolListIterator(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::ECX, true);
	ASMPatch patch;

	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EDX) // itemPoolOffset
		.AddInternalCall(asm_patch_get_static_pool_list) // call asm_patch_get_static_pool_list
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX) // move pool list pointer to ECX
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)address + 0x8); // resume
	sASMPatcher.PatchAt(address, &patch);
}

static void ASMPatchPoolNotFoundLogINFO(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch patch;
	ByteBuffer buffer;
	buffer.AddByte('\x90', 5);
	patch.AddBytes(buffer);
	sASMPatcher.FlatPatch(address, &patch, true);
}

const char* s_EmptyString = "";

static const char* __stdcall asm_patch_get_item_pool_name(int itemPoolType) noexcept
{
	auto* itemPool = ItemPoolManager::GetItemPool(itemPoolType);
	if (!itemPool)
	{
		return s_EmptyString;
	}

	return itemPool->GetName().c_str();
}

static void ASMPatchGetDebug12PoolName(const char* signature, const char* functionName) noexcept
{
	SigScan scanner(signature);
	scanner.Scan();
	void* address = scanner.GetAddress();

	printf(asmPatchMessage, functionName, address);

	ASMPatch::SavedRegisters reg(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS & ~ASMPatch::SavedRegisters::ECX, true);
	ASMPatch patch;
	patch.PreserveRegisters(reg)
		.Push(ASMPatch::Registers::EAX) // itemPoolType
		.AddInternalCall(asm_patch_get_item_pool_name) // call asm_patch_get_item_pool_name
		.CopyRegister(ASMPatch::Registers::ECX, ASMPatch::Registers::EAX) // mov ECX, EAX
		.RestoreRegisters(reg)
		.AddRelativeJump((char*)address + 0x5); // resume
	sASMPatcher.PatchAt(address, &patch);
}

void ASMPatches::__ItemPoolManager() noexcept
{
	ASMPatchCheckGetCollectiblePool("83fe1e0f87", "ItemPool::GetCollectible (Check Pool)");
	ASMPatchPickCollectible("83c00403c78945??e8", "ItemPool::GetCollectible (First Try)");
	ASMPatchPickCollectible("83c00403c78945??85d2", "ItemPool::GetCollectible (Retry)");
	ASMPatchPreTriggerGenesis("66ff84??????????8b5c24", "Entity_Player::PreTriggerGenesis");
	ASMPatchDecreaseRemainingGenesisItems("66ff8c", "Room::Update (Decrease Genesis Items)");
	ASMPatchGetNextGenesisPool("0fb7126685d2", "83fe1f72??80bd????????00", "83f81f0f87????????5251e8????????83c408c785????????00000000c785????????00000000c785????????00000000c787????????3c000000", "Room::Update (ItemPool::GetNextGenesisPool)");
	ASMPatchBagOfCraftingPoolListIterator("8d8e????????03ca", "Entity_Player::get_crafting_output");
}

void ASMPatches::__ItemPoolManagerExtra() noexcept
{
	ASMPatchPoolNotFoundLogINFO("e8????????c645fc0383c40c", "ItemPool::load_pools");
	ASMPatchGetDebug12PoolName("b9????????8b46??85c078??83f81e73??8b0485????????eb??b8????????5150ff76??8d85????????ff76??ff365268????????508d45??e9", "EntityPlayer::RenderDebugInfo");
	ASMPatchGetDebug12PoolName("b9????????8b46??85c078??83f81e73??8b0485????????eb??b8????????5150ff76??8d85????????ff76??ff365268????????508d45??50", "EntityPlayer::RenderDebugInfo");
}

#pragma endregion

#pragma endregion

LUA_FUNCTION(Lua_CustomItemPoolGetPoolIdByName)
{
	std::string poolName = luaL_checkstring(L, 1);
	lua_pushinteger(L, ItemPoolManager::GetPoolIdByName(poolName));
	return 1;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);
	lua::RegisterGlobalClassFunction(_state, lua::GlobalClasses::Isaac, "GetPoolIdByName", Lua_CustomItemPoolGetPoolIdByName);
}