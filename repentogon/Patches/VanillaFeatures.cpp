#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "../REPENTOGONOptions.h"
#include "VanillaFeatures.h"
#include <cctype>
#include <string>
#include <unordered_map>

// These hooks allow for reverting Repentance+ changes to 1.7.9b behavior.

// ----- ITEM METADATA OVERRIDE SYSTEM -----
// This allows us to edit items_metadata.xml dynamically while still letting mod values take precedent.
static constexpr uint64_t TAG_OFFENSIVE = 1ull << 20;

struct ItemMetadataOverride {
	bool REPENTOGONOptions::* toggle; // Rep+ behavior while true; the override applies while false
	int id;
	uint64_t addTags = 0;
	uint64_t removeTags = 0;
	int quality = -1; // -1 leaves the quality alone
};

static const ItemMetadataOverride itemMetadataOverrides[] = {
	// 1.9.7.11 removes Glowing Hourglass and Book of Shadows from Tainted Lost's pool.
	{ &REPENTOGONOptions::vfTaintedLostPoolNoHourglassShadows, COLLECTIBLE_BOOK_OF_SHADOWS, TAG_OFFENSIVE },
	{ &REPENTOGONOptions::vfTaintedLostPoolNoHourglassShadows, COLLECTIBLE_GLOWING_HOUR_GLASS, TAG_OFFENSIVE },
};

struct ItemMetadataState {
	uint64_t addedTags = 0;
	uint64_t removedTags = 0;
	bool qualityApplied = false;
	int originalQuality = 0;
	int appliedQuality = 0;
	bool modTags = false;
	bool modQuality = false;
};

static std::unordered_map<int, ItemMetadataState> itemMetadataStates;

static void UndoItemMetadata(ItemConfig_Item* item, ItemMetadataState& state) {
	item->tags = (item->tags & ~state.addedTags) | state.removedTags;
	state.addedTags = state.removedTags = 0;

	if (state.qualityApplied && item->quality == state.appliedQuality) {
		item->quality = state.originalQuality;
	}
	state.qualityApplied = false;
}

static void ApplyItemMetadata(int id, ItemConfig_Item* item, ItemMetadataState& state) {
	uint64_t addTags = 0;
	uint64_t removeTags = 0;
	int quality = -1;
	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		if (entry.id != id || repentogonOptions.*entry.toggle) {
			continue;
		}
		addTags |= entry.addTags;
		removeTags |= entry.removeTags;
		if (entry.quality >= 0) {
			quality = entry.quality;
		}
	}

	if (!state.modTags) {
		state.addedTags = addTags & ~item->tags;
		state.removedTags = removeTags & item->tags;
		item->tags = (item->tags | addTags) & ~removeTags;
	}

	if (!state.modQuality && quality >= 0) {
		state.qualityApplied = true;
		state.originalQuality = item->quality;
		state.appliedQuality = quality;
		item->quality = quality;
	}
}

static void ForEachOverriddenItem(ItemConfig* config, void (*fn)(int, ItemConfig_Item*, ItemMetadataState&)) {
	for (auto& [id, state] : itemMetadataStates) {
		if (ItemConfig_Item* item = config->GetCollectible(id)) {
			fn(id, item, state);
		}
	}
}

void ApplyVanillaItemMetadata() {
	ForEachOverriddenItem(g_Manager->GetItemConfig(), [](int id, ItemConfig_Item* item, ItemMetadataState& state) {
		UndoItemMetadata(item, state);
		ApplyItemMetadata(id, item, state);
	});
}

HOOK_METHOD(ItemConfig, LoadMetadata, (char* xmlpath, ModEntry* modentry) -> void) {
	std::string path = xmlpath ? xmlpath : "";
	for (char& c : path) {
		c = (c == '\\') ? '/' : (char)std::tolower((unsigned char)c);
	}
	const bool fromMod = path.find("/mods/") != std::string::npos;

	if (fromMod) {
		ForEachOverriddenItem(this, [](int, ItemConfig_Item* item, ItemMetadataState& state) { UndoItemMetadata(item, state); });
	}
	else {
		itemMetadataStates.clear();
	}

	std::unordered_map<int, std::pair<uint64_t, int>> before;
	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		if (ItemConfig_Item* item = this->GetCollectible(entry.id)) {
			before[entry.id] = { item->tags, item->quality };
		}
	}

	super(xmlpath, modentry);

	for (const ItemMetadataOverride& entry : itemMetadataOverrides) {
		ItemConfig_Item* item = this->GetCollectible(entry.id);
		if (!item) {
			continue;
		}

		ItemMetadataState& state = itemMetadataStates[entry.id];
		if (fromMod) {
			auto previous = before.find(entry.id);
			if (previous == before.end()) {
				state.modTags = state.modQuality = true;
			}
			else {
				state.modTags |= item->tags != previous->second.first;
				state.modQuality |= item->quality != previous->second.second;
			}
		}
	}

	ForEachOverriddenItem(this, ApplyItemMetadata);
}
// ----- END ITEM METADATA OVERRIDE -----

// 1.9.7.11 makes The Siren unable to charm Isaac's Heart, and Lilith's Incubus.
HOOK_METHOD_PRIORITY(Entity_Familiar, CanCharm, 1000, () -> bool) {
	if (!repentogonOptions.vfSirenCharmHeartIncubus && (_variant == 62 || _variant == 80)) {
		return true;
	}
	
	return super();
}