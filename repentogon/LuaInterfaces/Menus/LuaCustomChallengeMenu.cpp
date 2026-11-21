#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_CustomChallengeMenu_GetSprite() {
	return &g_MenuManager->_menuCustomChallenge.CustomChallengeSprite;
}

MOD_EXPORT int L_CustomChallengeMenu_GetSelectedChallengeID() {
	return g_MenuManager->_menuCustomChallenge.SelectedElement;
}

MOD_EXPORT void L_CustomChallengeMenu_SetSelectedChallengeID(int element) {
	g_MenuManager->_menuCustomChallenge.SelectedElement = element;
}