#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "AchievementsStuff.h"
#include "ASMDefinition.h"
#include "ASMPatches.h"
#include "ASMPatcher.hpp"
#include "PlayerFeatures.h"
#include "../ImGuiFeatures/LogViewer.h"

#include <tuple>

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {

	super(type, variant, subtype, initSeed);
	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(subtype);
	std::map<std::string, int*> intValues = {
		std::pair<std::string, int*> {"heartcontainers", this->GetMaxHearts()},
		std::pair<std::string, int*> {"redhearts", this->GetRedHearts()},
		std::pair<std::string, int*> {"bonehearts", this->GetBoneHearts()},
		std::pair<std::string, int*> {"soulhearts", this->GetSoulHearts()},
		std::pair<std::string, int*> {"eternalheart", this->GetEternalHearts()},
		std::pair<std::string, int*> {"goldenhearts", this->GetGoldenHearts()},
		std::pair<std::string, int*> {"brokenhearts", this->GetBrokenHearts()},
	};

	for (std::pair<std::string, int*> value : intValues) {
		if (!playerXML[value.first].empty())
			*value.second = stoi(playerXML[value.first]);
	}
	// Special cases, where direct setting doesn't work right

	if (!playerXML["blackhearts"].empty()) {
		this->AddBlackHearts(stoi(playerXML["blackhearts"]));
	}

	if (!playerXML["rottenhearts"].empty()) {
		// WHY does HALF A HEART equal HALF A ROTTEN HEART? WHAT?????
		this->AddRottenHearts(stoi(playerXML["rottenhearts"]) * 2, false);
	}

	if (!playerXML["gigabombs"].empty()) {
		this->AddBombs(stoi(playerXML["gigabombs"]));
		this->AddGigaBombs(stoi(playerXML["gigabombs"]));
	}
	this->update_red_hearts(); 
	this->update_golden_hearts(false); 
	this->update_bone_hearts(); 
}

// GetHealthType hook merged into the one in CustomCallbacks.cpp

namespace PlayerStats {
	float modCharacterSpeed = 0;
	float modCharacterFireDelay = 0;
	float modCharacterDamage = 0;
	float modCharacterRange = 0;
	float modCharacterShotSpeed = 0;
	float modCharacterLuck = 0;
}

HOOK_METHOD(Entity_Player, EvaluateItems, () -> void) {

	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(this->GetPlayerType());

	std::tuple<std::string, float*, float> statValues[] = {
	{"speedmodifier", &PlayerStats::modCharacterSpeed, 1.f},
	{"firedelaymodifier", &PlayerStats::modCharacterFireDelay, 1.f},
	{"damagemodifier", &PlayerStats::modCharacterDamage, 1.f},
	{"rangemodifier", &PlayerStats::modCharacterRange, 40.f},
	{"shotspeedmodifier", &PlayerStats::modCharacterShotSpeed, 1.f},
	{"luckmodifier", &PlayerStats::modCharacterLuck, 1.f},
	};

	for (auto& value : statValues) {
		*std::get<1>(value) = 0;

		if (!playerXML[std::get<0>(value)].empty())
			*std::get<1>(value) = stof(playerXML[std::get<0>(value)]) * std::get<2>(value);
	}
	super();
}

HOOK_METHOD_PRIORITY(Entity_Player, GetHealthLimit, 100, (bool keeper) -> int) {
	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(this->GetPlayerType());

	int orig = super(keeper);

	if (!playerXML["healthlimit"].empty()) {
		return stoi(playerXML["healthlimit"]);
	}

	return orig;
}


bool IsCharacterUnlockedRgon(const int playerType) {
	XMLAttributes att = XMLStuff.PlayerData->GetNodeById(playerType);
	const std::string& achievement = att["achievement"];
	if (!achievement.empty()) {
		return IsAchievementUnlockedByXmlString(achievement);
	}
	return true;
}

bool IsCharacterHiddenByAchievementRgon(const int playerType) {
	if (playerType < NUM_PLAYER_TYPES) {
		return false;
	}
	EntityConfig_Player* player = g_Manager->GetEntityConfig()->GetPlayer(playerType);
	if (!player->_bSkinParentName.empty()) {
		// This is a tainted character. Need to check their non-tainted counterpart.
		for (uint32_t i = NUM_PLAYER_TYPES; i < g_Manager->GetEntityConfig()->GetPlayers()->size(); i++) {
			EntityConfig_Player* otherPlayer = g_Manager->GetEntityConfig()->GetPlayer(i);
			if (otherPlayer && otherPlayer->_id != player->_id && otherPlayer->_name == player->_bSkinParentName && otherPlayer->_bSkinParentName.empty()) {
				return IsCharacterHiddenByAchievementRgon(otherPlayer->_id);
			}
		}
		return false;
	}
	XMLAttributes att = XMLStuff.PlayerData->GetNodeById(playerType);
	const std::string& achievement = att["hideachievement"];
	if (!achievement.empty()) {
		return !IsAchievementUnlockedByXmlString(att["hideachievement"]);
	}
	return false;
}

HOOK_METHOD(ModManager, LoadConfigs, () -> void) {
	super();

	// Players with the "hideachievement" attribute (hides them from menus until an achievement is earned) cannot also be ""hidden"".
	// They mean different things (for example, The Forgotten never has the ""hidden"" attribute).
	for (EntityConfig_Player& player : *g_Manager->GetEntityConfig()->GetPlayers()) {
		if (player._hidden && !XMLStuff.PlayerData->GetNodeById(player._id)["hideachievement"].empty()) {
			player._hidden = false;
		}
	}
}


HOOK_METHOD(ModManager, RenderCustomCharacterPortraits, (int id, Vector* pos, ColorMod* color, Vector* scale) -> void) {
	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(id);

	ANM2** portrait = g_Manager->GetPlayerConfig()->at(id).GetModdedMenuPortraitANM2();
	if ((*portrait) != nullptr) {
		(*portrait)->Play(playerXML["name"].c_str(), true);
		(*portrait)->SetLayerFrame(0, !IsCharacterUnlockedRgon(id) ? 1 : 0);
		(*portrait)->_color = *color;
		(*portrait)->_scale = *scale;
		(*portrait)->Render_Wrapper(pos, &Vector(0, 0), &Vector(0, 0));
	}
}

HOOK_STATIC_PRIORITY(ModManager, RenderCustomCharacterMenu, -100, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {

	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(CharacterId);
	bool disableState;
	std::vector<const char*> layersToDisable = {
		"Item Name",
		"Item Name 2",
		"Item Name 3",
		"Item Icon",
		"Item Icon 2",
		"Item Icon 3",
		"Power Stat",
		"Speed Stat",
		"Life Stat",
		"Power Icon",
		"Speed Icon",
		"Life Icon"
	};

	ANM2** background = g_Manager->GetPlayerConfig()->at(CharacterId).GetModdedMenuBackgroundANM2();

	if (*background != nullptr) {
		if (!IsCharacterUnlockedRgon(CharacterId))
			disableState = false;
		else
			disableState = true;


		for (const char* layer : layersToDisable) {
			LayerState* layerState = (*background)->GetLayer(layer);
			if (layerState != nullptr) { 
				*layerState->IsVisible() = disableState;
			}
		}

		LayerState* unlockedByLayer = (*background)->GetLayer("Unlocked By");
		if (unlockedByLayer != nullptr) {
			*unlockedByLayer->IsVisible() = !disableState;
		}
	}

	super(CharacterId, RenderPos, DefaultSprite);

}

HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	super(CharacterId, RenderPos, DefaultSprite);

	XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(CharacterId);

	if (!IsCharacterUnlockedRgon(CharacterId))
		g_MenuManager->GetMenuCharacter()->IsCharacterUnlocked = false;
}

HOOK_METHOD(Menu_Character, UpdateRotations, () -> void) {

	super();

	// The tainted menu is very naive and hides itself if... the page swap sprite is invisible.
	// If no modded tainted characters are locked, hide the sprite.
	bool hideTaintedMenu = true;

	for (unsigned int i = PLAYER_ISAAC_B; i < g_Manager->GetPlayerConfig()->size(); ++i) {
		EntityConfig_Player* player = &g_Manager->GetPlayerConfig()->at(i);

		if (i < NUM_PLAYER_TYPES) {
			if (player->_hidden) {
				continue;
			}

			if ((player->_achievement == -1) || ((player->_achievement >= 0 && g_Manager->GetPersistentGameData()->Unlocked(player->_achievement)) &&
				((player->_id != PLAYER_EDEN_B) || g_Manager->GetPersistentGameData()->GetEventCounter(21) > 0))) {
				hideTaintedMenu = false;
			}
		}
		else {
			if (player->_hidden || player->_bSkinParentName.empty()) {
				continue;
			}
			if (IsCharacterUnlockedRgon(player->_id)) {
				hideTaintedMenu = false;
			}
		}
	}
	if (hideTaintedMenu)
		this->_PageSwapWidgetSprite._color._tint[3] = 0.0f;	
};

HOOK_METHOD(Menu_Character, SelectRandomChar, () -> void) {
	std::vector<std::pair<int, EntityConfig_Player>> allowedCharacters;
	std::vector<std::string> bSkinParents;
	// These hardcoded arrays should only stick around until we have the properties for these actually figured out.
	int bannedIds[7] = { PLAYER_LAZARUS2, PLAYER_BLACKJUDAS, PLAYER_THESOUL, PLAYER_ESAU, PLAYER_LAZARUS2_B, PLAYER_JACOB2_B, PLAYER_THESOUL_B }; // Until we have character configs fully mapped out, this is a hardcoded list of banned ID's.
	int hiddenIds[4] = { PLAYER_BLUEBABY, PLAYER_THELOST, PLAYER_KEEPER, PLAYER_THEFORGOTTEN }; // Will be hidden when not unlocked, so this needs to be accounted for.
	int hiddenTainteds[4] = { PLAYER_BLUEBABY_B, PLAYER_THELOST_B, PLAYER_KEEPER_B, PLAYER_THEFORGOTTEN_B }; // Their tainted forms, too

	// First, for modded characters, build a list of all bSkinParents
	for (unsigned int i = NUM_PLAYER_TYPES; i < g_Manager->GetPlayerConfig()->size(); ++i) {
		XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(g_Manager->GetPlayerConfig()->at(i)._id);
		if (!playerXML["bskinparent"].empty()) {
			logViewer.AddLog("[REPENTOGON]", "Pushing back %s as a b-skin parent\n", playerXML["bskinparent"].c_str());
			bSkinParents.push_back(playerXML["bskinparent"]);
		}
	}
	int offset = 1;

	for (unsigned int i = 0; i < g_Manager->GetPlayerConfig()->size(); ++i) {
		EntityConfig_Player player = g_Manager->GetPlayerConfig()->at(i);
		std::string name = player.GetDisplayName(nullptr);


		if (player._id < NUM_PLAYER_TYPES) { // Vanilla character handling
			int achievement = *player.GetAchievement();

			// If a character ID isn't valid for the current character menu, skip
			if (this->GetSelectedCharacterMenu() == 1 && player._id <= PLAYER_ESAU || this->GetSelectedCharacterMenu() == 0 && player._id > PLAYER_ESAU) { // TODO probably an enum for this, 0 = normal 1 = tainted
				continue;
			}

			// If a character is always hidden, skip
			if (std::find(std::begin(bannedIds), std::end(bannedIds), player._id) != std::end(bannedIds)) {
				continue;
			}

			// If the character is locked and not hidden when locked, increment offset and skip, otherwise just skip
			if (achievement > 0 && !g_Manager->GetPersistentGameData()->Unlocked(achievement)) {
				if(std::find(std::begin(hiddenIds), std::end(hiddenIds), player._id) == std::end(hiddenIds) && std::find(std::begin(hiddenTainteds), std::end(hiddenTainteds), player._id) == std::end(hiddenTainteds)) {

					logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a NON HIDDEN locked character at offset %d\n", player._id, name.c_str(), offset);
					offset++;
				}
				else {
					logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a HIDDEN locked character at offset %d\n", player._id, name.c_str(), offset);
				}
				continue;
			}

			// If the character is unlocked, and we have enough Eden tokens if we're Eden, add as a valid option
			if (achievement < 0 || g_Manager->GetPersistentGameData()->Unlocked(achievement) &&
				(player._id != PLAYER_EDEN && player._id != PLAYER_EDEN_B || g_Manager->GetPersistentGameData()->GetEventCounter(21))) { //TODO: enum for eden tokens
				logViewer.AddLog("[REPENTOGON]", "Adding %d (%s) as an option at offset %d\n", player._id, name.c_str(), offset);
				allowedCharacters.push_back(std::pair<int, EntityConfig_Player>(offset, player));
			}
			offset++;
		}
		else {
			XMLAttributes playerXML = XMLStuff.PlayerData->GetNodeById(player._id);

			// This character is invalid for the current menu, skip
			if ((GetSelectedCharacterMenu() == 0 && !playerXML["bskinparent"].empty()) ||
				((GetSelectedCharacterMenu() == 1 && playerXML["bskinparent"].empty()) && std::find(bSkinParents.begin(), bSkinParents.end(), name) != bSkinParents.end())) {
				continue;
			}

			// This is a hidden modded character, skip
			if (player._hidden) {
				logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a HIDDEN modded character at offset %d\n", player._id, name.c_str(), offset);
				continue;
			}

			// This is a modded character hidden from the menu by an achievement, skip
			if (IsCharacterHiddenByAchievementRgon(player._id)) {
				logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a modded character \"hidden\" by an achievement at offset %d\n", player._id, name.c_str(), offset);
				continue;
			}

			// This is a visible, but locked modded character, increment offset and skip
			if (!IsCharacterUnlockedRgon(player._id)) {
				logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a locked modded character at offset %d\n", player._id, name.c_str(), offset);
				offset++;
				continue;
			}

			// Allow the character if it's valid for the current menu, or if it isn't the bSkinParent of anything
			if ((GetSelectedCharacterMenu() == 0 && playerXML["bskinparent"].empty()) || (GetSelectedCharacterMenu() == 1 && !playerXML["bskinparent"].empty()) ||
				std::find(bSkinParents.begin(), bSkinParents.end(), name) == bSkinParents.end()) {

				logViewer.AddLog("[REPENTOGON]", "Adding %d (%s) as an option at offset %d\n", player._id, name.c_str(), offset);
				allowedCharacters.push_back(std::pair<int, EntityConfig_Player>(offset, player));
			}
			offset++;
		}
	}

	// Avoids a crash if no characters are available (all tainteds locked). This is not an elegant solution but will work until we have the tainted menu hidden in this case.
	if (allowedCharacters.empty()) { 
		g_Manager->_sfxManager.Play(187, 1.0, 0, false, 1, 0);
		this->Status = 0;
		return;
	}

	unsigned int seed = Isaac::genrand_int32();
	int randomId =  seed % allowedCharacters.size();
	std::pair<int, EntityConfig_Player> chosenCharacter = allowedCharacters[randomId];
	std::string name = chosenCharacter.second.GetDisplayName(nullptr);
	logViewer.AddLog("[REPENTOGON]", "I have chosen: %d (%s, menu slot %d, seed %d, randomId %d/%d)\n", chosenCharacter.second._id, name.c_str(), chosenCharacter.first, seed, randomId+1, allowedCharacters.size());
	this->_chosenRandomCharacter = chosenCharacter.second._id;
	this->_randomRotationAmount = chosenCharacter.first + this->GetNumCharacters() * 2 + this->GetNumCharacters();
	this->_randomRotationVelocity = (360.0f / this->GetNumCharacters()) / 30.0f;

}

// In Menu_Character::Reset, the game reconstructs the set of characters that should appear on the main menu.
// Normally the only condition to not include a modded character on the menu would be for it to be hidden.
// This patch introduces an additional check to see if it should be "hidden" until some achievement is unlocked.
bool __stdcall HideModdedCharacterInMenu(EntityConfig_Player* playerConf) {
	return playerConf->_hidden || IsCharacterHiddenByAchievementRgon(playerConf->_id);
}
void PatchModdedCharacterHiddenByAchievementInMenu() {
	void* addr = sASMDefinitionHolder->GetDefinition(&AsmDefinitions::HideModdedCharacterByAchievement);

	const int jumpOffset = 0x9 + *(uint8_t*)((char*)addr + 0x8);

	printf("[REPENTOGON] Patching Menu_Character::Reset to support hidden-until-locked modded characters at %p\n", addr);

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::GP_REGISTERS_STACKLESS, true);
	ASMPatch patch;
	patch.PreserveRegisters(savedRegisters)
		.Push(ASMPatch::Registers::EAX)  // EntityConfig_Player*
		.AddInternalCall(HideModdedCharacterInMenu)
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, (char*)addr + jumpOffset)  // Jump for true
		.AddRelativeJump((char*)addr + 0x9);  // Jump for false
	sASMPatcher.PatchAt(addr, &patch);
}
