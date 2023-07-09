#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "XMLData.h"
#include "LuaCore.h"

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
HOOK_STATIC(ModManager, RenderCustomCharacterMenu, (int CharacterId, Vector* RenderPos, ANM2* DefaultSprite) -> void, __stdcall) {

	XMLAttributes playerXML = XMLStuff.PlayerData->nodes[CharacterId];

	super(CharacterId, RenderPos, DefaultSprite);

	// This gets run every render frame... which works out for us! Locked flags get reset every render frame. This is certianly efficient, thanks Nicalis!
	if (playerXML["needsunlock"] == "true") {
		characterUnlockData[CharacterId] = false;
		g_MenuManager->GetMenuCharacter()->lockedflags = CharacterLockedFlag::LOCKED;

		// Run a callback here. Let Lua mods determine if they need to do unlocks. Once this is done, we'll worry about archetypes.
		int callbackid = 1140;
		if (!CallbackState.test(callbackid - 1000)) {
			return;
		}
		lua_State* L = g_LuaEngine->_state;
		lua::LuaStackProtector protector(L);

		lua_rawgeti(L, LUA_REGISTRYINDEX, g_LuaEngine->runCallbackRegistry->key);

		lua::LuaResults result = lua::LuaCaller(L).push(callbackid)
			.push(CharacterId)
			.push(CharacterId)
			.call(1);

		if (!result) {
		    if (lua_isboolean(L, -1)) {
				if (lua_toboolean(L, -1)) {
					characterUnlockData[CharacterId] = true;
					g_MenuManager->GetMenuCharacter()->lockedflags = CharacterLockedFlag::UNLOCKED;
				}
			}
		}
	}

}