#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../LuaClasses.h"
#include "../Patches/XMLData.h"
#include "../Patches/ItemConfigEx.h"

XMLItem* GetItemXML(const ItemConfig_Item* config) {
	if (config->type == 0) {
		return XMLStuff.NullItemData;
	}
	else if (config->type == 2) {
		return XMLStuff.TrinketData;
	}
	return XMLStuff.ItemData;
}

MOD_EXPORT ItemConfig* L_ItemConfig_Get() {
	return g_Manager->GetItemConfig();
}

MOD_EXPORT ItemConfig_Item* L_ItemConfig_GetCollectible(ItemConfig* config, int id) {
	return config->GetCollectible(id);
}

MOD_EXPORT ItemConfig_Item* L_ItemConfig_GetNullItem(ItemConfig* config, int id) {
	return config->GetNullItem(id);
}

MOD_EXPORT ItemConfig_Item* L_ItemConfig_GetTrinket(ItemConfig* config, int id) {
	return config->GetTrinket(id);
}

MOD_EXPORT ItemConfig_Card* L_ItemConfig_GetCard(ItemConfig* config, int id) {
	return config->GetCard(id);
}

MOD_EXPORT ItemConfig_Pill* L_ItemConfig_GetPillEffect(ItemConfig* config, int id) {
	std::vector<ItemConfig_Pill*>* pills = config->GetPillEffects();
	if (id < 0 || id >= (int)pills->size()) {
		return nullptr;
	}
	return (*pills)[id];
}

MOD_EXPORT std::vector<ItemConfig_Item*>* L_ItemConfig_GetTaggedItems(ItemConfig* config, uint64_t tags) {
	return &config->GetTaggedItems(tags);
}

MOD_EXPORT int L_ItemConfig_GetItemsWithCustomTag(ItemConfig* config, const char* tag, ItemConfig_Item** out) {
	const std::string tagString = tag;
	int count = 0;
	const auto add = [&](ItemConfig_Item* item) {
		if (item) {
			if (out) {
				out[count] = item;
			}
			++count;
		}
	};

	for (const int id : ItemConfigEx::GetCollectiblesWithCustomTag(tagString)) {
		add(config->GetCollectible(id));
	}
	for (const int id : ItemConfigEx::GetTrinketsWithCustomTag(tagString)) {
		add(config->GetTrinket(id));
	}
	for (const int id : ItemConfigEx::GetNullItemsWithCustomTag(tagString)) {
		add(config->GetNullItem(id));
	}
	return count;
}

// Legacy compat for a player function in ItemConfig
MOD_EXPORT bool L_ItemConfig_CanRerollCollectible(int id) {
	if (!g_Game->_playerManager._playerList.empty() && g_Game->GetPlayer(0) && g_Game->GetPlayer(0)->_exists) {
		return g_Game->GetPlayer(0)->CanRerollCollectible(id, false);
	}
	return !g_Manager->GetItemConfig()->IsQuestItem(id);
}
