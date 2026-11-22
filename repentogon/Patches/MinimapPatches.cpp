#include "MinimapPatches.h"

#include <string>
#include <tuple>
#include <map>

#include "IsaacRepentance.h"
#include "ASMDefinition.h"
#include "ASMPatcher.hpp"
#include "HookSystem.h"
#include "Log.h"
#include "../MiscFunctions.h"
#include "CustomModManager.h"

static const MinimapIcon NULL_MINIMAP_ICON;
static MinimapIcon RUNE_MINIMAP_ICON{"runes", "IconRune", 0, 11000};

static std::map<std::tuple<int, int, int>, MinimapIcon> s_EntityMinimapIcons = {
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_ETERNAL},		{"hearts", "IconHeart", 9, 15900} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_GOLDEN},		{"hearts", "IconHeart", 8, 15800} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_BONE},		{"hearts", "IconHeart", 7, 15700} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_BLACK},		{"hearts", "IconHeart", 6, 15600} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_SOUL},		{"hearts", "IconHeart", 5, 15500} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_BLENDED},		{"hearts", "IconHeart", 4, 15400} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_HALF_SOUL},	{"hearts", "IconHeart", 3, 15300} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_ROTTEN},		{"hearts", "IconHeart", 2, 15200} },
	{ {ENTITY_PICKUP, PICKUP_HEART, -1},				{"hearts", "IconHeart", 1, 15100} },
	{ {ENTITY_PICKUP, PICKUP_HEART, HEART_HALF},		{"hearts", "IconHeart", 0, 15000} },

	{ {ENTITY_PICKUP, PICKUP_COLLECTIBLE, -1},			{"collectibles", "IconItem", 0, 14000} },
	{ {ENTITY_PICKUP, PICKUP_TRINKET, -1},				{"trinkets", "IconTrinket", 0, 13000} },
	
	{ {ENTITY_PICKUP, PICKUP_MEGACHEST, CHEST_CLOSED},		{"chests", "IconChest", 9, 12900} },
	{ {ENTITY_PICKUP, PICKUP_LOCKEDCHEST, CHEST_CLOSED},	{"chests", "IconChest", 8, 12800} },
	{ {ENTITY_PICKUP, PICKUP_ETERNALCHEST, CHEST_CLOSED},	{"chests", "IconChest", 7, 12700} },
	{ {ENTITY_PICKUP, PICKUP_WOODENCHEST, CHEST_CLOSED},	{"chests", "IconChest", 6, 12600} },
	{ {ENTITY_PICKUP, PICKUP_REDCHEST, CHEST_CLOSED},		{"chests", "IconChest", 5, 12500} },
	{ {ENTITY_PICKUP, PICKUP_CHEST, CHEST_CLOSED},			{"chests", "IconChest", 4, 12400} },
	{ {ENTITY_PICKUP, PICKUP_OLDCHEST, CHEST_CLOSED},		{"chests", "IconChest", 4, 12400} },
	{ {ENTITY_PICKUP, PICKUP_HAUNTEDCHEST, CHEST_CLOSED},	{"chests", "IconChest", 4, 12400} },
	{ {ENTITY_PICKUP, PICKUP_GRAB_BAG, 2},					{"chests", "IconChest", 3, 12300} },
	{ {ENTITY_PICKUP, PICKUP_GRAB_BAG, 1},					{"chests", "IconChest", 2, 12200} },
	{ {ENTITY_PICKUP, PICKUP_BOMBCHEST, CHEST_CLOSED},		{"chests", "IconChest", 1, 12100} },
	{ {ENTITY_PICKUP, PICKUP_MIMICCHEST, CHEST_CLOSED},		{"chests", "IconChest", 0, 12000} },
	{ {ENTITY_PICKUP, PICKUP_SPIKEDCHEST, CHEST_CLOSED},	{"chests", "IconChest", 0, 12000} },

	// Cards use their PickupSubtype value instead of the ID of the card itself
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_RUNE_LEFT},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_RUNE_RIGHT},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_RUNE_BLACK},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_ISAAC},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_MAGDALENE},		RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_CAIN},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_JUDAS},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_BLUEBABY},		RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_EVE},				RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_SAMSON},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_AZAZEL},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_LAZARUS},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_EDEN},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_THE_LOST},		RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_LILITH},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_THE_KEEPER},		RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_APOLLYON},		RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_THE_FORGOTTEN},	RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_BETHANY},			RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, CARDBACK_SOUL_JACOB_AND_ESAU},	RUNE_MINIMAP_ICON },
	{ {ENTITY_PICKUP, PICKUP_TAROTCARD, -1},							{"cards", "IconCard", 0, 10000} },

	{ {ENTITY_PICKUP, PICKUP_PILL, PILL_GOLD},			{"pills", "IconPill", 1, 9100} },
	{ {ENTITY_PICKUP, PICKUP_PILL, -1},					{"pills", "IconPill", 0, 9000} },

	{ {ENTITY_PICKUP, PICKUP_KEY, KEY_CHARGED},			{"keys", "IconKey", 2, 8200} },
	{ {ENTITY_PICKUP, PICKUP_KEY, KEY_GOLDEN},			{"keys", "IconKey", 1, 8100} },
	{ {ENTITY_PICKUP, PICKUP_KEY, -1},					{"keys", "IconKey", 0, 8000} },

	{ {ENTITY_PICKUP, PICKUP_BOMB, BOMB_GOLDEN},		{"bombs", "IconBomb", 1, 7100} },
	{ {ENTITY_PICKUP, PICKUP_BOMB, -1},					{"bombs", "IconBomb", 0, 7000} },
	{ {ENTITY_PICKUP, PICKUP_POOP, -1},					{"poops", "IconPoop", 0, 6000} },

	{ {ENTITY_PICKUP, PICKUP_COIN, COIN_GOLDEN},		{"coins", "IconCoin", 2, 5300} },
	{ {ENTITY_PICKUP, PICKUP_COIN, COIN_DIME},			{"coins", "IconCoin", 2, 5200} },
	{ {ENTITY_PICKUP, PICKUP_COIN, COIN_NICKEL},		{"coins", "IconCoin", 1, 5100} },
	{ {ENTITY_PICKUP, PICKUP_COIN, COIN_STICKYNICKEL},	{"coins", "IconCoin", 1, 5100} },
	{ {ENTITY_PICKUP, PICKUP_COIN, -1},					{"coins", "IconCoin", 0, 5000} },

	{ {ENTITY_PICKUP, PICKUP_LIL_BATTERY, BATTERY_GOLDEN},	{"batteries", "IconBattery", 1, 4100} },
	{ {ENTITY_PICKUP, PICKUP_LIL_BATTERY, -1},				{"batteries", "IconBattery", 0, 4000} },

	{ {ENTITY_SLOT, (int)SlotVariant::BATTERY_BUM, -1},		{"beggars", "IconBeggar", 5, 3500} },
	{ {ENTITY_SLOT, (int)SlotVariant::BOMB_BUM, -1},		{"beggars", "IconBeggar", 4, 3400} },
	{ {ENTITY_SLOT, (int)SlotVariant::KEY_MASTER, -1},		{"beggars", "IconBeggar", 3, 3300} },
	{ {ENTITY_SLOT, (int)SlotVariant::HELL_GAME, -1},		{"beggars", "IconBeggar", 2, 3200} },
	{ {ENTITY_SLOT, (int)SlotVariant::DEVIL_BEGGAR, -1},	{"beggars", "IconBeggar", 2, 3200} },
	{ {ENTITY_SLOT, (int)SlotVariant::ROTTEN_BEGGAR, -1},	{"beggars", "IconBeggar", 1, 3100} },
	{ {ENTITY_SLOT, (int)SlotVariant::SHELL_GAME, -1},		{"beggars", "IconBeggar", 0, 3000} },
	{ {ENTITY_SLOT, (int)SlotVariant::BEGGAR, -1},			{"beggars", "IconBeggar", 0, 3000} },

	{ {ENTITY_SLOT, (int)SlotVariant::REVIVE_MACHINE, -1},			{"slots", "IconSlot", 8, 2800} },
	{ {ENTITY_SLOT, (int)SlotVariant::CONFESSIONAL, -1},			{"slots", "IconSlot", 7, 2700} },
	{ {ENTITY_SLOT, (int)SlotVariant::SHOP_RESTOCK_MACHINE, -1},	{"slots", "IconSlot", 6, 2600} },
	{ {ENTITY_SLOT, (int)SlotVariant::CRANE_GAME, -1},				{"slots", "IconSlot", 5, 2500} },
	{ {ENTITY_SLOT, (int)SlotVariant::FORTUNE_TELLING_MACHINE, -1},	{"slots", "IconSlot", 4, 2400} },
	{ {ENTITY_SLOT, (int)SlotVariant::BLOOD_DONATION_MACHINE, -1},	{"slots", "IconSlot", 3, 2300} },
	{ {ENTITY_SLOT, -1, -1},										{"slots", "IconSlot", 2, 2200} },
	{ {ENTITY_SLOT, (int)SlotVariant::DONATION_MACHINE, -1},		{"slots", "IconSlot", 1, 2100} },
	{ {ENTITY_SLOT, (int)SlotVariant::GREED_DONATION_MACHINE, -1},	{"slots", "IconSlot", 1, 2100} },
	{ {ENTITY_SLOT, (int)SlotVariant::MOMS_DRESSING_TABLE, -1},		{"slots", "IconSlot", 0, 2000} },
	{ {ENTITY_SLOT, (int)SlotVariant::HOME_CLOSET_PLAYER, -1},		NULL_MINIMAP_ICON },

	{ {ENTITY_EFFECT, 161/*PORTAL_TELEPORT*/, 980},		{"portals", "IconPortal", 0, 0}},
};

static std::map<std::tuple<int, int>, MinimapIcon> s_GridMinimapIcons = {
	{ {GRID_STAIRS, -1}, {"ladders", "IconLadder", 0, 1000} },
};


MinimapIcon* GetEntityMinimapIcon(int id, int variant, int subtype) {
	bool isRune = false;

	// Cards use their PickupSubtype value instead of the ID of the card itself
	if (id == ENTITY_PICKUP && variant == PICKUP_TAROTCARD) {
		if (ItemConfig_Card* card = g_Manager->GetItemConfig()->GetCard(subtype)) {
			subtype = card->pickupSubtype;
			isRune = card->cardType == CARDTYPE_RUNE;
		} else {
			subtype = 0;
		}
	}

	// Type+Variant+SubType
	if (auto iter = s_EntityMinimapIcons.find({ id, variant, subtype }); iter != s_EntityMinimapIcons.end()) {
		if (iter->second.frame < 0) {
			return nullptr;
		}
		return &iter->second;
	}

	if (isRune) {
		// If no icon was defined for this rune's cardback, use the default rune one.
		return &RUNE_MINIMAP_ICON;
	}

	// Type+Variant+SubType of corresponding EntityConfig_Entity, for cases where the subtype is more dynamic
	if (EntityConfig_Entity* entity = g_Manager->GetEntityConfig()->GetEntity(id, variant, subtype)) {
		if (auto iter = s_EntityMinimapIcons.find({ entity->id, entity->variant, entity->subtype }); iter != s_EntityMinimapIcons.end()) {
			if (iter->second.frame < 0) {
				return nullptr;
			}
			return &iter->second;
		}
	}
	// Just Type+Variant
	if (auto iter = s_EntityMinimapIcons.find({ id, variant, -1 }); iter != s_EntityMinimapIcons.end()) {
		if (iter->second.frame < 0) {
			return nullptr;
		}
		return &iter->second;
	}
	// Just Type
	if (auto iter = s_EntityMinimapIcons.find({ id, -1, -1 }); iter != s_EntityMinimapIcons.end()) {
		if (iter->second.frame < 0) {
			return nullptr;
		}
		return &iter->second;
	}
	return nullptr;
}

MinimapIcon* GetEntityMinimapIcon(const EntitySaveState& entity) {
	return GetEntityMinimapIcon(entity.type, entity.variant, entity.subtype);
}

MinimapIcon* GetGridMinimapIcon(const GridEntityDesc& grid) {
	// Type+Variant
	if (auto iter = s_GridMinimapIcons.find({ grid._type, grid._variant}); iter != s_GridMinimapIcons.end()) {
		if (iter->second.frame < 0) {
			return nullptr;
		}
		return &iter->second;
	}
	// Just Type
	if (auto iter = s_GridMinimapIcons.find({ grid._type, -1 }); iter != s_GridMinimapIcons.end()) {
		if (iter->second.frame < 0) {
			return nullptr;
		}
		return &iter->second;
	}
	return nullptr;
}

void AddEntityMinimapIcon(int id, int variant, int subtype, const MinimapIcon& icon) {
	s_EntityMinimapIcons[{id, variant, subtype}] = icon;
}

void AddEntityMinimapIcon(int id, int variant, int subtype, const std::string& group, const std::string& animation, int frame, int priority, ANM2* customAnm2) {
	AddEntityMinimapIcon(id, variant, subtype, MinimapIcon{group, animation, frame, priority, customAnm2});
}

static void RenderMinimapIcon(ANM2* anm2, std::string anim, int frame, Vector pos, Vector offset) {
	if (anm2 == nullptr) {
		anm2 = &g_Game->_minimap._iconsSprite;
	}
	Vector iconPos = pos + offset - Vector(6, 6);
	iconPos.x = std::floor(iconPos.x);
	iconPos.y = std::floor(iconPos.y);
	anm2->Play(anim.c_str(), true);
	anm2->SetFrame(&anim, frame);
	Vector zerovec(0, 0);
	anm2->Render(&iconPos, &zerovec, &zerovec);
}

// Reimplemented icon rendering to enable integration of custom icons
static void __stdcall RenderMinimapIcons(MinimapConfig* map, RoomDescriptor* room, const char* roomIconName, Vector roomPos, Vector* singleIconOffset, Vector* twoIconsOffsets, Vector* fourIconsOffsets) {
	std::vector<MinimapIcon*> icons;

	if (room->DisplayFlags != 0) {
		std::unordered_map<std::string, MinimapIcon*> groupIcons;
		for (const EntitySaveState& entity : room->SavedEntities) {
			if (MinimapIcon* icon = GetEntityMinimapIcon(entity)) {
				if (groupIcons[icon->group] == nullptr || groupIcons[icon->group]->priority < icon->priority) {
					groupIcons[icon->group] = icon;
				}
			}
		}
		std::map<std::pair<int, int>, MinimapIcon*> sortedIcons;
		int i = 0;
		for (const auto& [group, icon] : groupIcons) {
			sortedIcons[{-icon->priority, i++}] = icon;
		}

		for (const auto& [key, icon] : sortedIcons) {
			icons.push_back(icon);
		}
	}

	int numIcons = icons.size();

	if (roomIconName) {
		numIcons++;
	}

	if (numIcons > map->_minimapIconNum) {
		numIcons = map->_minimapIconNum;
	}

	Vector* iconOffsets = singleIconOffset;
	if (numIcons == 2) {
		iconOffsets = twoIconsOffsets;
	} else if (numIcons > 2) {
		iconOffsets = fourIconsOffsets;
		if (numIcons > 4) {
			numIcons = 4;
		}
	}

	int i = 0;

	if (roomIconName) {
		RenderMinimapIcon(nullptr, roomIconName, 0, roomPos, iconOffsets[i]);
		i++;
	}

	for (MinimapIcon* icon : icons) {
		if (i >= numIcons) break;
		RenderMinimapIcon(icon->anm2, icon->animation, icon->frame, roomPos, iconOffsets[i]);
		i++;
	}
}
static void patch_minimap_config_render_pre_render_icons() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Minimap_Config_Render_PreIcons);
	void* exitAddr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Minimap_Config_Render_PostIcons);
	ZHL::Log("[REPENTOGON] Patching Minimap::Config::Render for minimap icons at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0x24, ASMPatch::Registers::EBX).Push(ASMPatch::Registers::EBX)  // Vector* fourIconsOffsets
		.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0xc8, ASMPatch::Registers::EBX).Push(ASMPatch::Registers::EBX)  // Vector* twoIconsOffsets
		.LoadEffectiveAddress(ASMPatch::Registers::EBP, -0xd0, ASMPatch::Registers::EBX).Push(ASMPatch::Registers::EBX)  // Vector* singleIconOffset
		.Push(ASMPatch::Registers::EBP, -0x74)  // float y
		.Push(ASMPatch::Registers::EBP, -0x38)  // float x
		.Push(ASMPatch::Registers::EAX)  // char* roomIconName
		.Push(ASMPatch::Registers::EDX)  // RoomDescriptor*
		.Push(ASMPatch::Registers::EBP, -0x44)  // MinimapConfig*
		.AddInternalCall(RenderMinimapIcons)
		.RestoreRegisters(savedRegisters)
		.AddRelativeJump(exitAddr);
	sASMPatcher.PatchAt(addr, &patch);
}

/**
 * This patch properly sets the shader to ColorOffset right before the minimap image is rendered.
 * Currently we patch right after the border is rendered, but any place before the image render is valid.
 * 
 * The issue stems from the fact that the question mark render does not set the shader prior to the
 * image render. This ends up not being a problem in vanilla because AnimationLayer::RenderFrame ends up leaving
 * the last used shader as the currently set shader as a side effect.
 * 
 * However due to the changes made to RenderFrame, which aims to make the function side effect free,
 * besides queuing render operations, this causes issues if the shader does not end up being ColorOffset.
 */
static void patch_set_shader_pre_render_image()
{
    ASMPatch patch;

    intptr_t addr = (intptr_t)sASMDefinitionHolder->GetDefinition(&AsmDefinitions::Minimap_Render_SetShaderPreImageRender);
    ZHL::Log("[REPENTOGON] Patching Minimap::Render for SetShaderPreImageRender at %p\n", addr);

    intptr_t resumeAddr = addr + 5;
    intptr_t managerShaderAddr = (intptr_t)&(g_KAGE_Graphics_Manager._shader);
    KAGE_Graphics_Shader* colorOffsetPtr = __ptr_g_AllShaders[ShaderType::SHADER_COLOR_OFFSET];
    int32_t restoreCall_rel32 = *(int32_t*)(addr + 1);
    intptr_t restoreCallAddr = addr + 5 + restoreCall_rel32;

    patch.AddInternalCall((void*)restoreCallAddr)
        .AddBytes("\xC7\x05") // MOV [imm32], imm32
        .AddBytes(ByteBuffer().AddAny((void*)&managerShaderAddr, sizeof(managerShaderAddr)))
        .AddBytes(ByteBuffer().AddAny((void*)&colorOffsetPtr, sizeof(colorOffsetPtr)))
        .AddRelativeJump((void*)resumeAddr);

    sASMPatcher.PatchAt((void*)addr, &patch);
}

void MinimapPatches::ApplyPatches() {
	patch_minimap_config_render_pre_render_icons();
	patch_set_shader_pre_render_image();
}

struct BelowMapIcon {
	ANM2* anm2;
	std::string anim;
	int frame;
	bool isCurse;
	bool isVanilla;
	int priority;

	int GetSortingBucket() const {
		if (priority < 0 && !isCurse) {
			// After curse icons
			return 2;
		} else if (isCurse) {
			// Curse icons
			return 1;
		}
		// Before curse icons (map icons)
		return 0;
	}

	bool operator<(const BelowMapIcon& other) const {
		int bucket = this->GetSortingBucket();
		int bucket_other = other.GetSortingBucket();
		// Bucket (Ascending), Priority (Descending), Modded before Vanilla, Filename+Animation (Ascending), Frame (Ascending)
		// Reminder that icons render from right to left, so the "first" / "highest priority" icon is the rightmost one visually.
		return std::tie(bucket, other.priority, this->isVanilla, this->anm2->_filename, this->anim, this->frame)
			< std::tie(bucket_other, this->priority, other.isVanilla, other.anm2->_filename, other.anim, other.frame);
	}
};

static void AddBelowMapIcon(std::vector<BelowMapIcon>& icons, ANM2* anm2, const std::string& anim, const int frame, const bool isCurse, const bool isVanilla, const int priority) {
	if (!anm2) return;
	icons.push_back({ anm2, anim, frame, isCurse, isVanilla, priority });
}

constexpr uint32_t MAX_MINIMAP_ICONS = 7;
constexpr float BASE_ICON_X_OFFSET = -16;
constexpr char VANILLA_ICONS_ANIM[] = "icons";
constexpr char CURSES_ANIM[] = "curses";
constexpr uint32_t NUM_VANILLA_CURSES = 8;

HOOK_METHOD(Minimap, RenderIcons, (Vector* position, float alpha) -> void) {
	PlayerManager* playerManager = g_Game->GetPlayerManager();
	RNG* rng = &playerManager->_rng;

	std::vector<BelowMapIcon> icons;

	// Vanilla icons
	if (playerManager->FirstCollectibleOwner(COLLECTIBLE_MIND, &rng, true)) {
		AddBelowMapIcon(icons, &this->_itemIconsSprite, VANILLA_ICONS_ANIM, 5, /*isCurse=*/false, /*isVanilla=*/true, 500);
	} else {
		if (playerManager->FirstCollectibleOwner(COLLECTIBLE_COMPASS, &rng, true)) {
			AddBelowMapIcon(icons, &this->_itemIconsSprite, VANILLA_ICONS_ANIM, 0, /*isCurse=*/false, /*isVanilla=*/true, 400);
		}

		if (playerManager->FirstCollectibleOwner(COLLECTIBLE_BLUE_MAP, &rng, true)) {
			AddBelowMapIcon(icons, &this->_itemIconsSprite, VANILLA_ICONS_ANIM, 1, /*isCurse=*/false, /*isVanilla=*/true, 300);
		}

		if (playerManager->FirstCollectibleOwner(COLLECTIBLE_TREASURE_MAP, &rng, true)) {
			AddBelowMapIcon(icons, &this->_itemIconsSprite, VANILLA_ICONS_ANIM, 2, /*isCurse=*/false, /*isVanilla=*/true, 200);
		}
	}
	if (playerManager->FirstCollectibleOwner(COLLECTIBLE_RESTOCK, &rng, true)) {
		AddBelowMapIcon(icons, &this->_itemIconsSprite, VANILLA_ICONS_ANIM, 4, /*isCurse=*/false, /*isVanilla=*/true, 100);
	}

	// Curses
	for (const RoomConfig_Curse& curse : g_Game->GetRoomConfig()->_curses) {
		uint32_t curseID = curse._idMaybe;
		if (curseID == 0) continue;
		uint32_t curseFlag = 1 << (curseID - 1);
		bool isVanilla = curseID <= NUM_VANILLA_CURSES;
		if (g_Game->GetLevel()->GetCurses() & curseFlag) {
			if (curseID <= NUM_VANILLA_CURSES) {
				// Vanilla curse
				AddBelowMapIcon(icons, &this->_itemIconsSprite, CURSES_ANIM, curseID - 1, /*isCurse=*/true, /*isVanilla=*/true, -curseID);
			} else if (auto it = CustomModManager::GetCurseSpriteMap().find(curseID); it != CustomModManager::GetCurseSpriteMap().end()) {
				// Modded curse
				const CurseSpriteData& curseIcon = it->second;
				AddBelowMapIcon(icons, curseIcon.customANM2, CURSES_ANIM, curseIcon.frameNum, /*isCurse=*/true, /*isVanilla=*/false, -curseID);
			}
		}
	}

	// Test
	// AddBelowMapIcon(icons, &g_Game->_minimap._iconsSprite, "IconAngelRoom", 0, false, false, -1);

	// Sort the icons by priority, etc
	std::sort(icons.begin(), icons.end());

	// Rendering
	int numRendered = 0;
	Vector renderPos(position->x, position->y);
	Vector zerovec(0, 0);

	Vector offset(BASE_ICON_X_OFFSET, 0);
	if (icons.size() >= 4) {
		offset.x += std::min(icons.size(), MAX_MINIMAP_ICONS) - 3.0f;
	}

	for (BelowMapIcon& icon : icons) {
		if (numRendered >= MAX_MINIMAP_ICONS) {
			break;
		}

		icon.anm2->Play(icon.anim.c_str(), true);
		icon.anm2->SetFrame(&icon.anim, icon.frame);

		icon.anm2->_color._tint[3] = alpha;
		icon.anm2->Render(&renderPos, &zerovec, &zerovec);

		renderPos += offset;
		numRendered++;
	}
}
