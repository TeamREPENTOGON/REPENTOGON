#include "IsaacRepentance.h"
#include "../Patches/ItemConfigEx.h"

extern "C" {
	__declspec(dllexport) const char* L_ItemConfigItem_GetDesc(ItemConfig_Item* item) {
		return item->desc.c_str();
	}

	__declspec(dllexport) void L_ItemConfigItem_SetDesc(ItemConfig_Item* item, const char* desc) {
		item->desc = desc;
	}

	__declspec(dllexport) const char* L_ItemConfigItem_GetName(ItemConfig_Item* item) {
		return item->name.c_str();
	}

	__declspec(dllexport) void L_ItemConfigItem_SetName(ItemConfig_Item* item, const char* name) {
		item->name = name;
	}

	__declspec(dllexport) const char* L_ItemConfigItem_GetGfxFileName(ItemConfig_Item* item) {
		return item->gfxFilename.c_str();
	}
	
	__declspec(dllexport) void L_ItemConfigItem_SetGfxFileName(ItemConfig_Item* item, const char* name) {
		item->gfxFilename = name;
	}

	__declspec(dllexport) bool L_ItemConfigItem_IsAvailable(ItemConfig_Item* item) {
		return item->IsAvailable(0);
	}

	__declspec(dllexport) int L_ItemConfigItem_GetCustomTags(ItemConfig_Item* item, const char** out) {
		ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item);
		if (!ex) return 0;

		const auto& tags = ex->GetCustomTags();
		if (out) {
			int i = 0;
			for (auto const& tag : tags)
				out[i++] = tag.c_str();
		}
		return (int)tags.size();
	}

	__declspec(dllexport) bool L_ItemConfigItem_HasCustomTag(ItemConfig_Item* item, const char* tag) {
		ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item);
		return ex && ex->HasCustomTag(tag);
	}

	__declspec(dllexport) void L_ItemConfigItem_AddCustomTag(ItemConfig_Item* item, const char* tag) {
		if (ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item)) {
			ex->AddCustomTag(tag);
		};
	}
	__declspec(dllexport) void L_ItemConfigItem_RemoveCustomTag(ItemConfig_Item* item, const char* tag) {
		if (ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item)) {
			ex->RemoveCustomTag(tag);
		};
	}

	__declspec(dllexport) int L_ItemConfigItem_GetCustomCacheTags(ItemConfig_Item* item, const char** out) {
		ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item);
		if (!ex) return 0;

		const auto& tags = ex->GetCustomCaches();
		if (out) {
			int i = 0;
			for (auto const& tag : tags)
				out[i++] = tag.c_str();
		}
		return (int)tags.size();
	}

	__declspec(dllexport) bool L_ItemConfigItem_HasCustomCacheTag(ItemConfig_Item* item, const char* tag) {
		ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item);
		return ex && ex->HasCustomCache(tag);
	}

	__declspec(dllexport) void L_ItemConfigItem_AddCustomCacheTag(ItemConfig_Item* item, const char* tag) {
		if (ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item)) {
			ex->AddCustomCache(tag);
		};
	}

	__declspec(dllexport) void L_ItemConfigItem_RemoveCustomCacheTag(ItemConfig_Item* item, const char* tag) {
		if (ItemConfigEx::ItemEx* ex = ItemConfigEx::GetItemEx(item)) {
			ex->RemoveCustomCache(tag);
		};
	}
}