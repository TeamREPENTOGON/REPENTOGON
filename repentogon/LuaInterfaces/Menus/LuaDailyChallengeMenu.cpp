#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_DailyChallengeMenu_GetSprite() {
	return &g_MenuManager->_menuDailyChallenge._DailyRunSprite;
}

MOD_EXPORT ANM2* L_DailyChallengeMenu_GetLeaderboardSprite() {
	return &g_MenuManager->_menuDailyChallenge._leaderboard._leaderboardMenuSprite;
}

MOD_EXPORT ANM2* L_DailyChallengeMenu_GetLeaderboardScoreMenuSprite() {
	return &g_MenuManager->_menuDailyChallenge._leaderboard._scoreMenuSprite;
}

MOD_EXPORT int L_DailyChallengeMenu_GetSelectedElement() {
	return g_MenuManager->_menuDailyChallenge.SelectedElement;
}

MOD_EXPORT int L_DailyChallengeMenu_GetState() {
	return g_MenuManager->_menuDailyChallenge.State;
}

MOD_EXPORT int L_DailyChallengeMenu_GetTimeLeftHours() {
	return g_MenuManager->_menuDailyChallenge._timeHoursLeft;
}

MOD_EXPORT int L_DailyChallengeMenu_GetTimeLeftMinutes() {
	return g_MenuManager->_menuDailyChallenge._timeMinutesLeft;
}

MOD_EXPORT int L_DailyChallengeMenu_GetTimeLeftSeconds() {
	return g_MenuManager->_menuDailyChallenge._timeSecondsLeft;
}

MOD_EXPORT bool L_DailyChallengeMenu_IsLeaderboardVisible() {
	return g_MenuManager->_menuDailyChallenge._leaderboard._displayState > 0;
}

MOD_EXPORT void L_DailyChallengeMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuDailyChallenge.SelectedElement = element;
}

MOD_EXPORT void L_DailyChallengeMenu_SetState(int state) {
	g_MenuManager->_menuDailyChallenge.State = state;
}