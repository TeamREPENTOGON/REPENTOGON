#pragma once

#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "document.h" // rapidjson

#include <cstdint>
#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include <optional>

class ItemPoolManager
{
public:
	enum eChaosMode {
		CHAOS_MODE_NORMAL = 0,
		CHAOS_MODE_GREED = 1,
		NUM_CHAOS_MODES
	};

	typedef std::vector<std::string> Warnings;
	typedef std::optional<std::string> Error;

	struct PoolItemDesc;
	class ItemPool;
	class CustomItemPool;
	struct PoolSaveState;
	struct CustomPoolSaveState;

private:
	/*
	Append mode is permanently entered on the first ItemPool::Init of the session.

	When this mode is active all virtual items that are added must be marked as "appended"
	so that they may only be placed at the end of the _poolList when loaded in, and not shuffled.
	 
	This mechanism serves two main purposes:
	- Prevent crafting recipes from changing mid-session.
	- Maintain the deterministic nature of the Item Pool on seeded runs (assuming the items in the pool are the same).
	*/
	bool m_AppendMode = false;
	/*
	Keeps track of whether or not the game's ItemPool has been intialized.
	Used to make sure that temporary items are not added when the pool is in an invalid state.
	*/
	bool m_ItemPoolInitialized = false;
	std::vector<std::unique_ptr<ItemPool>> m_ItemPools;
	std::map<size_t, ItemPool*> m_PoolByName;
	RNG m_RNG = RNG();

public:
	static ItemPoolManager& Get()
	{
		static ItemPoolManager s_Instance;
		return s_Instance;
	}

	static void __Init();
	static void __LoadPools();
	static void __MarkItemPoolNotInitialized() { Get().m_ItemPoolInitialized = false; }
	static void __FinalizePools();
	static void __ResetCollectible(int collectible);
	static void __AddBibleUpgrade(uint32_t poolId, int amount);
	static uint32_t __GetChaosPool(RNG* rng);
	static void __GetItemPoolsForCollectible(int collectible, std::vector<uint32_t>& result);
	static void __SaveState(uint32_t slot);
	static void __RestoreState(uint32_t slot);
	static void __ClearSaveState(uint32_t slot);
	static void __SaveToDisk(const std::string& fileName, bool isRerun);
	static void __LoadFromDisk(const std::string& fileName, bool isRerun);
	static void __DeleteGameState(const std::string& fileName);
	static bool IsItemPoolInitialized() { return Get().m_ItemPoolInitialized; }
	static ItemPool* GetItemPool(uint32_t id);
	static RNG& GetRNG() { return Get().m_RNG; }
	static const std::vector<std::unique_ptr<ItemPool>>& GetItemPools() { return Get().m_ItemPools; }
	/* not marked as as it causes a crash in some lua functions when erroring
	   due to a weird + longjmp interactions when the function is inlined
	*/
	static size_t GetNumItemPools() { return Get().m_ItemPools.size(); }
	static int GetPoolIdByName(const std::string& name);

private:
	void reset();
	ItemPool* add_pool(const std::string& name, bool isCustom);
	void add_base_pool(const std::string& name, bool isGreedMode);
	void add_base_pools();
	void add_modded_pool(const std::string& name, rapidxml::xml_node<>* xmlNode, Warnings& warnings);
	void add_modded_pools();
	void parse_xml_pool_names(const std::string& filePath, const ModEntry* mod);
	ItemPool* get_pool_by_name(const std::string& name);
	void invalidate_state_slot(uint32_t slot);
	rapidjson::Document ItemPoolManager::serialize_game_state(bool isRerun);
	bool deserialize_game_state(const rapidjson::Document& gameState, const std::string& fileName, bool isRerun, std::vector<int>& poolIdRemap);
	static void fix_original_game_state(std::vector<int>& poolIdRemap, bool isRerun);
	void handle_dangling_save_states(std::vector<CustomPoolSaveState>& danglingSaveStates);
private:
	ItemPoolManager() = default;
	ItemPoolManager(const ItemPoolManager&) = delete;
};

struct ItemPoolManager::PoolItemDesc
{
	int itemId = COLLECTIBLE_NULL;
	float weight = 1.0f;
	float decreaseBy = 0.5f;
	float removeOn = 0.1f;

	PoolItem BuildPoolItem() const;
	PoolItemDesc() = default;
	PoolItemDesc(lua_State* L, int index, Warnings& warnings);
	bool operator==(const PoolItem& item) const;
};

class ItemPoolManager::ItemPool
{
protected:
	int m_Id = POOL_NULL;
	std::string m_Name;
	std::bitset<NUM_CHAOS_MODES> m_ChaosMode = 1 << CHAOS_MODE_NORMAL;
	bool m_IsChaosModeDefined = false;

	/// A container for permanent pool items that have not been added through XML.
	/// Any item that has an index >= than m_AppendIndex is marked as "appended".
	std::vector<PoolItemDesc> m_VirtualItems;
	uint32_t m_AppendIndex = 0; /// The index of the first item in m_VirtualItems that is considered "appended".
	uint32_t m_NumRegularItems = 0; /// The size of the _poolList before the introduction of the VirtualItems.

	std::unique_ptr<PoolSaveState> m_SaveStates[3];
	PoolItem* m_StaticPoolList[3] = { 0 }; // A manually created "vector" that ends at start of append (this is only used for ASMPatches)

protected:
	void reset();
	virtual RNG& get_global_rng() { return g_Game->_itemPool._itemPoolRNG; }
	virtual void load_pool();
	void shuffle_pool(MTRNG& rng);
	void finalize_pool();
	/// Returns the index in _poolList where appended items start to be stored (this includes temporary items).
	inline size_t get_start_of_append() const { return m_NumRegularItems + m_AppendIndex; }
	/// Returns the index in _poolList where temporary items start to be stored.
	inline size_t get_start_of_temporary() const { return m_NumRegularItems + m_VirtualItems.size(); }
	void update_static_pool_list();
	void reset_collectible(int collectible);
	void add_bible_upgrade(int amount);
	virtual uint16_t& get_remaining_genesis_items();
	virtual void store_state(size_t saveSlot);
	virtual void restore_state(size_t saveSlot);
	void invalidate_state(size_t slot);
public:
	ItemPool(int id, const std::string& name);
	virtual ~ItemPool() = default;
	inline size_t GetId() const { return m_Id; }
	inline const std::string& GetName() const { return m_Name; }
	inline bool IsChaosModeAllowed(uint32_t mode) const { return mode < NUM_CHAOS_MODES ? m_ChaosMode.test(mode) : false; }
	virtual ItemPool_Item* GetPoolData();
	uint16_t GetRemainingGenesisItems();
	void IncreaseRemainingGenesisItems(uint16_t amount);
	void DecreaseRemainingGenesisItems(uint16_t amount);
	/// A helper function to add an item to the _poolList at the specified position.
	/// Performs all the necessary operations needed to make the itempool function properly,
	/// and should be preferred over manually adding items to the _poolList.
	void EmplaceItem(size_t position, const PoolItemDesc& poolItem);
	/// A helper function to remove the item at the specified position from the _poolList.
	/// Performs all the necessary operations needed to make the itempool function properly,
	/// and should be preferred over manually removing items from the _poolList.
	void EraseItem(size_t position);
	void AddVirtualItem(const PoolItemDesc& poolItem);
	void AddTemporaryItem(const PoolItemDesc& poolItem, Error& error);
	void RemoveTemporaryItem(const PoolItemDesc& poolItem, Error& error);
	inline std::vector<PoolItem>* __GetStaticPoolList() { this->update_static_pool_list(); return (std::vector<PoolItem>*)& m_StaticPoolList; }

	friend class ItemPoolManager;
};

class ItemPoolManager::CustomItemPool : public ItemPoolManager::ItemPool
{
private:
	ItemPool_Item m_Pool;
	uint16_t m_RemainingGenesisItems = 0;

private:
	virtual RNG& get_global_rng() override { return ItemPoolManager::GetRNG(); }
	virtual void load_pool() override;
	virtual uint16_t& get_remaining_genesis_items() override;
	virtual void store_state(size_t saveSlot) override;
	virtual void restore_state(size_t saveSlot) override;
public:
	CustomItemPool(int id, const std::string& name);
	virtual ~CustomItemPool() override = default;
	virtual ItemPool_Item* GetPoolData() override;

	friend class ItemPoolManager;
};

struct ItemPoolManager::PoolSaveState
{
protected:
	bool m_IsUsable = false;
private:
	std::vector<PoolItem> m_TemporaryItems;

protected:
	virtual void serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) const;
	virtual void deserialize(const rapidjson::Value& poolSaveState, Warnings& warnings);

	friend class ItemPoolManager;
	friend class ItemPool;
};

struct ItemPoolManager::CustomPoolSaveState : public ItemPoolManager::PoolSaveState
{
private:
	std::vector<float> m_ItemWeights;
	uint32_t m_BibleUpgrades = 0;
	uint16_t m_RemainingGenesisItems = 0;

private:
	virtual void serialize(rapidjson::Value& node, rapidjson::Document::AllocatorType& allocator) const override;
	virtual void deserialize(const rapidjson::Value& poolSaveState, Warnings& warnings) override;

	friend class ItemPoolManager;
	friend class CustomItemPool;
};

namespace ASMPatches
{
	void __ItemPoolManager();
	void __ItemPoolManagerExtra();
}