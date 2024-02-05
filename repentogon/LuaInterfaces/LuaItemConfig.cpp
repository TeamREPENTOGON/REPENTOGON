#include <LuaJIT/src/lua.hpp>

#include "IsaacRepentance.h"

extern "C" {
	// ItemConfig
	ItemConfig_Item* L_ItemConfig_GetCollectible(const int id) {
		return g_Manager->GetItemConfig()->GetCollectible(id);
	}
	ItemConfig_Item* L_ItemConfig_GetNullItem(const int id) {
		if (id <= 0) return nullptr;
		return g_Manager->GetItemConfig()->GetNullItem(id);
	}
	ItemConfig_Item* L_ItemConfig_GetTrinket(const int id) {
		if (id <= 0) return nullptr;
		return g_Manager->GetItemConfig()->GetTrinket(id);
	}
	ItemConfig_Card* L_ItemConfig_GetCard(const int id) {
		auto* cards = g_Manager->GetItemConfig()->GetCards();
		if (id <= 0 || id >= cards->size()) {
			return nullptr;
		}
		return cards->at(id);
	}
	bool L_ItemConfig_IsValidCollectible(const int id) {
		return g_Manager->GetItemConfig()->IsValidCollectible(id);
	}

	// ItemConfig_Item
	bool L_ItemConfigItem_HasTags(ItemConfig_Item* self, const uint64_t tags) {
		return (self->tags & tags) == tags;
	}
	bool L_ItemConfigItem_IsAvailable(ItemConfig_Item* self) {
		// I still dunno exactly what the parameters are, but it seems like
		// `self->IsAvailable(0)` is the same as `self->IsAvailableEx(0xFFFFFFFF)`
		// and the game usually calls them like that (observed in Ghidra and debugger).
		return self->IsAvailable(0);
	}
	const char* L_ItemConfigItem_GetName(ItemConfig_Item* self) {
		return self->name.c_str();
	}
	void L_ItemConfigItem_SetName(ItemConfig_Item* self, const char* str) {
		self->name = std::string(str);
	}
	const char* L_ItemConfigItem_GetDescription(ItemConfig_Item* self) {
		return self->desc.c_str();
	}
	void L_ItemConfigItem_SetDescription(ItemConfig_Item* self, const char* str) {
		self->desc = std::string(str);
	}
	const char* L_ItemConfigItem_GetGfxFileName(ItemConfig_Item* self) {
		return self->gfxFilename.c_str();
	}
	void L_ItemConfigItem_SetGfxFileName(ItemConfig_Item* self, const char* str) {
		self->gfxFilename = std::string(str);
	}

	// ItemConfig_Costume
	const char* L_ItemConfigCostume_GetAnm2Path(ItemConfig_Costume* self) {
		return self->anm2Path.c_str();
	}
	void L_ItemConfigCostume_SetAnm2Path(ItemConfig_Costume* self, const char* str) {
		self->anm2Path = std::string(str);
	}

	// ItemConfig_Card
	bool L_ItemConfigCard_IsAvailable(ItemConfig_Card* self) {
		return self->IsAvailable();
	}
	const char* L_ItemConfigCard_GetName(ItemConfig_Card* self) {
		return self->name.c_str();
	}
	void L_ItemConfigCard_SetName(ItemConfig_Card* self, const char* str) {
		self->name = std::string(str);
	}
	const char* L_ItemConfigCard_GetDescription(ItemConfig_Card* self) {
		return self->desc.c_str();
	}
	void L_ItemConfigCard_SetDescription(ItemConfig_Card* self, const char* str) {
		self->desc = std::string(str);
	}
	const char* L_ItemConfigCard_GetHudAnim(ItemConfig_Card* self) {
		return self->hudAnim.c_str();
	}
	void L_ItemConfigCard_SetHudAnim(ItemConfig_Card* self, const char* str) {
		self->hudAnim = std::string(str);
	}
}
