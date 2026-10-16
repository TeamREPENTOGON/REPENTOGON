#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_ChallengeMenu_GetSprite() {
	return &g_MenuManager->_menuChallenge.ChallengeMenuSprite;
}

MOD_EXPORT int L_ChallengeMenu_GetSelectedChallengeID() {
	return g_MenuManager->_menuChallenge.SelectedChallengeID;
}

MOD_EXPORT void L_ChallengeMenu_SetSelectedChallengeID(int challengeID) {
	g_MenuManager->_menuChallenge.SelectedChallengeID = challengeID;
}