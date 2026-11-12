#include "IsaacRepentance.h"
#include "../../Patches/XMLPlayerExtras.h"

MOD_EXPORT int L_CharacterMenu_GetActiveStatus() {
	return g_MenuManager->_menuCharacter.Status;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetBigCharPageSprite() {
	return &g_MenuManager->_menuCharacter._BigCharPageSprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetBGSprite() {
	return &g_MenuManager->_menuCharacter._CharacterMenuBGSprite;
}

// Given a PlayerType, finds the corresponding CharacterMenu character ID, if any.
// I don't think the game maintains a map in this direction.
int GetCharacterMenuIDFromPlayerType(const int playerType) {
	if (playerType >= NUM_PLAYER_TYPES) {
		for (uint32_t i = 0; i < g_ModCharacterMap.size(); i++) {
			if (playerType == g_ModCharacterMap[i].normal || playerType == g_ModCharacterMap[i].tainted) {
				return i + 18;
			}
		}
	}
	else if (playerType >= 0) {
		for (uint32_t i = 1; i < 36; i++) {
			if (i != 18 && playerType == __ptr_g_MenuCharacterEntries[i].playerType) {
				return i % 18;
			}
		}
	}
	return -1;
}

MOD_EXPORT int L_CharacterMenu_GetCharacterMenuIDFromPlayerType(int playerType) {
	const int charID = GetCharacterMenuIDFromPlayerType(playerType);

	if (charID > 0) {
		return charID;
	}
	else {
		return -1;
	}
};

MOD_EXPORT ANM2* L_CharacterMenu_GetCharacterPortraitSprite() {
	return &g_MenuManager->_menuCharacter._CharacterPortraitsSprite;
}

MOD_EXPORT float L_CharacterMenu_GetCharacterWheelDepth() {
	return g_MenuManager->_menuCharacter._characterWheelDepth;
}

MOD_EXPORT float L_CharacterMenu_GetCharacterWheelWidth() {
	return g_MenuManager->_menuCharacter._characterWheelWidth;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetCompletionMarksSprite() {
	return &g_MenuManager->_menuCharacter._completionWidget.anm2;
}

MOD_EXPORT int L_CharacterMenu_GetDifficulty() {
	return g_MenuManager->_menuCharacter.Difficulty;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetDifficultyOverlaySprite() {
	return &g_MenuManager->_menuCharacter._DifficultyOverlaySprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetDifficultyPageSprite() {
	return &g_MenuManager->_menuCharacter._DifficultyPageSprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetEasterEggPageSprite() {
	return &g_MenuManager->_menuCharacter._EastereggPageSprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetGreedDecoSprite() {
	return &g_MenuManager->_menuCharacter._GreedDecoOverlaySprite;
}

MOD_EXPORT bool L_CharacterMenu_GetIsCharacterUnlocked() {
	return g_MenuManager->_menuCharacter.IsCharacterUnlocked;
}

MOD_EXPORT int L_CharacterMenu_GetNumCharacters() {
	return g_MenuManager->_menuCharacter._numCharacters;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetPageSwapWidgetSprite() {
	return &g_MenuManager->_menuCharacter._PageSwapWidgetSprite;
}

MOD_EXPORT int L_CharacterMenu_GetPlayerTypeFromCharacterMenuID(int characterMenuID, bool tainted) {
	const int playerType = Menu_Character::GetPlayerTypeFromMenuID(characterMenuID, tainted);

	if (playerType < 0) {
		return -1;
	}
	else {
		return playerType;
	}
}

MOD_EXPORT float L_CharacterMenu_GetScrollSpeed() {
	return g_MenuManager->_menuCharacter._scrollSpeed;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetSeedEntrySprite() {
	return &g_MenuManager->_menuCharacter._SeedEntrySprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetSeedPageSprite() {
	return &g_MenuManager->_menuCharacter._SeedPageSprite;
}

MOD_EXPORT ANM2* L_CharacterMenu_GetSeedUnlockPageSprite() {
	return &g_MenuManager->_menuCharacter._SeedUnlockPageSprite;
}

MOD_EXPORT int L_CharacterMenu_GetSelectedCharacterMenu() {
	return g_MenuManager->_menuCharacter._characterMenuShown;
};

MOD_EXPORT int L_CharacterMenu_GetSelectedCharacterID() {
	return g_MenuManager->_menuCharacter.SelectedCharacterID;
};

MOD_EXPORT int L_CharacterMenu_GetSelectedCharacterPlayerType() {
	const int playerType = g_MenuManager->_menuCharacter.GetSelectedPlayerType();

	if (playerType < 0) {
		return -1;
	}
	else {
		return playerType;
	}
};

MOD_EXPORT ANM2* L_CharacterMenu_GetTaintedBGDecoSprite() {
	return &g_MenuManager->_menuCharacter._TaintedMenuBGDecoSprite;
};

MOD_EXPORT ANM2* L_CharacterMenu_GetWinStreakPageSprite() {
	return &g_MenuManager->_menuCharacter._WinStreakPageSprite;
};

MOD_EXPORT void L_CharacterMenu_SetActiveStatus(int status) {
	g_MenuManager->_menuCharacter.Status = status;
};

MOD_EXPORT void L_CharacterMenu_SetCharacterWheelDepth(float value) {
	g_MenuManager->_menuCharacter._characterWheelDepth = value;
};

MOD_EXPORT void L_CharacterMenu_SetCharacterWheelWidth(float value) {
	g_MenuManager->_menuCharacter._characterWheelWidth = value;
};

MOD_EXPORT void L_CharacterMenu_SetDifficulty(int difficulty) {
	g_MenuManager->_menuCharacter.Difficulty = difficulty;
};

MOD_EXPORT void L_CharacterMenu_SetIsCharacterUnlocked(bool isUnlocked) {
	g_MenuManager->_menuCharacter.IsCharacterUnlocked = isUnlocked;
};

MOD_EXPORT void L_CharacterMenu_SetScrollSpeed(float speed) {
	g_MenuManager->_menuCharacter._scrollSpeed = speed;
};

MOD_EXPORT void L_CharacterMenu_SetSelectedCharacterMenu(int menu, bool updateGraphics) {
	if (menu == 0 || menu == 1) {
		if (updateGraphics) {
			g_MenuManager->_menuCharacter.ChangeCharacterPage(menu);
		}
		else {
			// Legacy behaviour of this function that doesn't actually visually change the menu.
			g_MenuManager->_menuCharacter._characterMenuShown = menu;
		}
	}
}

// Given a CharacterMenu character ID, returns the "index" of that character in the wheel.
// Can be affected by preceding characters being hidden/locked and such, so a bit annoying to calculate.
int GetCharacterMenuIndexFromID(const int targetCharID) {
	if (targetCharID == 0)
		return 0;

	int idx = 0;

	for (int i = 1; i < 18; i++) {
		auto* charEntry = &__ptr_g_MenuCharacterEntries[i];
		auto* taintedCharEntry = &__ptr_g_MenuCharacterEntries[i + 18];
		const int pType = charEntry->playerType;
		const bool visible = !charEntry->hidden || charEntry->IsCharacterAvailable(0);
		if (visible)
			idx++;
		if (i == targetCharID)
			return visible ? idx : -1;
	}
	for (uint32_t i = 0; i < g_ModCharacterMap.size(); i++) {
		const int pType = g_ModCharacterMap[i].normal;
		const bool visible = !g_Manager->GetEntityConfig()->GetPlayer(pType)->_hidden && !IsCharacterHiddenByAchievementRgon(pType);
		if (visible)
			idx++;
		if (i + 18 == targetCharID)
			return visible ? idx : -1;
	}

	return -1;
}

MOD_EXPORT void L_CharacterMenu_SetSelectedCharacterID(int charID, bool updateWheel, bool skipRotation) {
	if (charID >= 0 && charID < (int)g_ModCharacterMap.size() + 18) {
		if (!updateWheel) {
			// Silly legacy behaviour of this function that does no validation whatsoever nor updates the menu.
			g_MenuManager->_menuCharacter.SelectedCharacterID = charID;
		}
		else {
			const int idx = GetCharacterMenuIndexFromID(charID);
			if (idx >= 0) {
				g_MenuManager->_menuCharacter.SelectedCharacterID = charID;
				g_MenuManager->_menuCharacter._numCharacters_MINUS_SelectedEntry = (idx == 0) ? 0 : (g_MenuManager->_menuCharacter._numCharacters - idx);
				if (skipRotation) {
					g_MenuManager->_menuCharacter._horizontalScrollPosition = (g_MenuManager->_menuCharacter._numCharacters_MINUS_SelectedEntry * 360.0f) / g_MenuManager->_menuCharacter._numCharacters;
				}
				if (g_MenuManager->_menuCharacter.SelectedCharacterID < 18) {
					g_MenuManager->_menuCharacter._BigCharPageSprite.Play(__ptr_g_MenuCharacterEntries[g_MenuManager->_menuCharacter.SelectedCharacterID + g_MenuManager->_menuCharacter._characterMenuShown * 18].animationName, false);
				}
			}
		}
	}
}

MOD_EXPORT void L_CharacterMenu_SwapCharacterMenu(bool force) {
	if (g_MenuManager->_menuCharacter._PageSwapWidgetSprite._color._tint[3] != 0.0f || force) {
		g_MenuManager->_menuCharacter.DoPageSwap();
	}
}