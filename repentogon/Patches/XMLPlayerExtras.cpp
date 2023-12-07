#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "XMLData.h"
#include "LuaCore.h"
#include "../ImGuiFeatures/LogViewer.h"

std::map<int, bool> characterUnlockData;

HOOK_METHOD(Entity_Player, Init, (unsigned int type, unsigned int variant, unsigned int subtype, unsigned int initSeed) -> void) {

	super(type, variant, subtype, initSeed);
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[subtype];
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
}

HOOK_METHOD_PRIORITY(Entity_Player, GetHealthType, 100, () -> int) {
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[this->GetPlayerType()];

	int orig = super();

	if (!playerXML["healthtype"].empty()) {
		int healthType = stoi(playerXML["healthtype"]);
		if (healthType > 4 || healthType < 0) { // INVALID
			return orig;
		}
		return healthType;
	}
	return orig;
}

bool playerStatOverride = true;
int playerStatOverrideId;

HOOK_METHOD(Entity_Player, EvaluateItems, () -> void) {
	int id = this->GetPlayerType();

	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[id];
	playerStatOverride = false;
	std::map<std::string, float*> damageValues = {
	std::pair<std::string, float*> {"speedmodifier", this->GetEdenSpeed()},
	std::pair<std::string, float*> {"firedelaymodifier", this->GetEdenFireDelay()},
	std::pair<std::string, float*> {"damagemodifier", this->GetEdenDamage()},
	std::pair<std::string, float*> {"rangemodifier", this->GetEdenRange()},
	std::pair<std::string, float*> {"shotspeedmodifier", this->GetEdenShotSpeed()},
	std::pair<std::string, float*> {"luckmodifier", this->GetEdenLuck()},

	};

	for (std::pair<std::string, float*> value : damageValues) {
		if (!playerXML[value.first].empty()) {
			playerStatOverride = true;
			playerStatOverrideId = id;
			*value.second = stof(playerXML[value.first]);
		}
			
	}

	if (playerStatOverride) {
		*this->GetPlayerTypeMutable() = 9; // EDEN
	}

	super();

	if (playerStatOverride) {
		*this->GetPlayerTypeMutable() = id;
	}
}

HOOK_METHOD(LuaEngine, EvaluateItems, (Entity_Player* player, int cacheFlags) -> void) {
	if (playerStatOverride) {
		*player->GetPlayerTypeMutable() = playerStatOverrideId;
	}

	super(player, cacheFlags);

	if (playerStatOverride) {
		*player->GetPlayerTypeMutable() = 9;
	}
}


HOOK_METHOD_PRIORITY(Entity_Player, GetHealthLimit, 100, (bool keeper) -> int) {
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[this->GetPlayerType()];

	int orig = super(keeper);

	if (!playerXML["healthlimit"].empty()) {
		return stoi(playerXML["healthlimit"]);
	}

	return orig;
}

extern std::bitset<500> CallbackState;

HOOK_METHOD(ModManager, RenderCustomCharacterPortraits, (int id, Vector* pos, ColorMod* color, Vector* scale) -> void) {
	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[id];

	if (playerXML["needsunlock"] == "true" && characterUnlockData[id] == false) {
		ANM2** portrait = g_Manager->GetPlayerConfig()->at(id).GetModdedMenuPortraitANM2();
		if ((*portrait) != nullptr) {
			(*portrait)->Play(playerXML["name"].c_str(), false);
			(*portrait)->SetLayerFrame(0, 1);
			(*portrait)->_color = *color;
			(*portrait)->_scale = *scale;
			(*portrait)->Render_Wrapper(pos, &Vector(0, 0), &Vector(0, 0));
		}
	}
	else
		super(id, pos, color, scale);
}

HOOK_STATIC_PRIORITY(ModManager, RenderCustomCharacterMenu, -100, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {

	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[CharacterId];
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
		if (playerXML["needsunlock"] == "true" && characterUnlockData[CharacterId] == false)
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

// Unlock checking
HOOK_METHOD(Menu_Character, Render, () -> void) {

	// This gets run every render frame... which works out for us! Locked status reset every render frame. This is certainly efficient, thanks Nicalis!
	for (std::pair<int, bool> character : characterUnlockData) {
		XMLAttributes playerXML = XMLStuff.PlayerData->nodes[character.first];

		if (playerXML["needsunlock"] == "true") {
			characterUnlockData[character.first] = false;

			// Run a callback here. Let Lua mods determine if they need to do unlocks. Once this is done, we'll worry about archetypes.
			int callbackid = 1140;
			if (CallbackState.test(callbackid - 1000)) {

				lua_State* L = g_LuaEngine->_state;
				lua::LuaStackProtector protector(L);

				lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

				lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
					.push(character.first)
					.push(character.first)
					.call(1);

				if (!result) {
					if (lua_isboolean(L, -1)) {
						if (lua_toboolean(L, -1)) {
							characterUnlockData[character.first] = true;
						}
					}
				}
			}
		}
	}

	super();
}

HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {
	super(CharacterId, RenderPos, DefaultSprite);

	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[CharacterId];

	if (playerXML["needsunlock"] == "true" && characterUnlockData[CharacterId] == false)
		g_MenuManager->GetMenuCharacter()->IsCharacterUnlocked = false;
}

HOOK_METHOD(Menu_Character, SelectRandomChar, () -> void) {
	std::vector<std::pair<int, EntityConfig_Player>> allowedCharacters;
	std::vector<std::string> bSkinParents;
	// These hardcoded arrays should only stick around until we have the properties for these actually figured out.
	int bannedIds[7] = { 11, 12, 17, 20, 38, 39, 40 }; // Until we have character configs fully mapped out, this is a hardcoded list of banned ID's.
	int hiddenIds[4] = { 4, 10, 14, 16 }; // Blue Baby, Keeper, The Lost and The Forgotten will be hidden when not unlocked, and this needs to be accounted for.
	int hiddenTainteds[4] = { 25, 31, 33, 35 }; // Their tainted forms, too

	// First, for modded characters, build a list of all bSkinParents
	for (unsigned int i = 41; i < g_Manager->GetPlayerConfig()->size(); ++i) {
		XMLAttributes playerXML = XMLStuff.PlayerData->nodes[g_Manager->GetPlayerConfig()->at(i)._id];
		if (!playerXML["bskinparent"].empty()) {
			logViewer.AddLog("[REPENTOGON]", "Pushing back %s as a b-skin parent\n", playerXML["bskinparent"].c_str());
			bSkinParents.push_back(playerXML["bskinparent"]);
		}
	}
	int offset = 1;

	for (unsigned int i = 0; i < g_Manager->GetPlayerConfig()->size(); ++i) {
		EntityConfig_Player player = g_Manager->GetPlayerConfig()->at(i);
		std::string name = player.GetDisplayName(nullptr);


		if (player._id < 41) { // Vanilla character handling
			int achievement = *player.GetAchievement();

			// If a character ID isn't valid for the current character menu, skip
			if (this->GetSelectedCharacterMenu() == 1 && player._id <= 20 || this->GetSelectedCharacterMenu() == 0 && player._id > 20) { // TODO probably an enum for this, 0 = normal 1 = tainted
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
				(player._id != 9 && player._id != 30 || g_Manager->GetPersistentGameData()->GetEventCounter(21))) { //TODO: enums, but this is eden and tainted eden, and eden tokens
				logViewer.AddLog("[REPENTOGON]", "Adding %d (%s) as an option at offset %d\n", player._id, name.c_str(), offset);
				allowedCharacters.push_back(std::pair<int, EntityConfig_Player>(offset, player));
			}
			offset++;
		}
		else {
			XMLAttributes playerXML = XMLStuff.PlayerData->nodes[player._id];

			// This character is invalid for the current menu, skip
			if ((GetSelectedCharacterMenu() == 0 && !playerXML["bskinparent"].empty()) ||
				((GetSelectedCharacterMenu() == 1 && playerXML["bskinparent"].empty()) && std::find(bSkinParents.begin(), bSkinParents.end(), name) != bSkinParents.end())) {
				continue;
			}


			// This is a locked modded character, increment offset and skip
			if (playerXML["needsunlock"] == "true" && characterUnlockData[player._id] == false) {
				logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a locked character at offset %d\n", player._id, name.c_str(), offset);
				offset++;
				continue;
			}

			// This is a hidden modded character, skip
			if (!playerXML["hidden"].empty()) {
				logViewer.AddLog("[REPENTOGON]", "Skipping %d (%s) as a HIDDEN modded character at offset %d\n", player._id, name.c_str(), offset);
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

	int randomId = (Isaac::genrand_int32() % allowedCharacters.size());
	std::pair<int, EntityConfig_Player> chosenCharacter = allowedCharacters[randomId];
	std::string name = chosenCharacter.second.GetDisplayName(nullptr);
	logViewer.AddLog("[REPENTOGON]", "I have chosen: %d (%s, slot %d on the menu)\n", chosenCharacter.second._id, name.c_str(), chosenCharacter.first);
	this->_chosenRandomCharacter = chosenCharacter.second._id;
	this->_randomRotationAmount = chosenCharacter.first + this->GetNumCharacters() * 2 + this->GetNumCharacters();
	this->_randomRotationVelocity = (360.0f / this->GetNumCharacters()) / 30.0f;

}