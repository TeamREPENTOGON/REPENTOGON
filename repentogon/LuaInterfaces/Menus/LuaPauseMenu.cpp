#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_PauseMenu_GetCompletionMarksSprite() {
	return &g_Game->_pauseScreen.completionWidget.anm2;
}

MOD_EXPORT ANM2* L_PauseMenu_GetMyStuffSprite() {
	return &g_Game->_pauseScreen.mystuffminispritebase;
}

MOD_EXPORT int L_PauseMenu_GetSelectedElement() {
	return g_Game->_pauseScreen.selectedelement;
}

MOD_EXPORT ANM2* L_PauseMenu_GetSprite() {
	return &g_Game->_pauseScreen.mainsprite;
}

MOD_EXPORT int L_PauseMenu_GetState() {
	return g_Game->_pauseScreen.state;
}

MOD_EXPORT ANM2* L_PauseMenu_GetStatsSprite() {
	return &g_Game->_pauseScreen.statssprite;
}

MOD_EXPORT void L_PauseMenu_SetSelectedElement(int element) {
	g_Game->_pauseScreen.selectedelement = element;
}

MOD_EXPORT void L_PauseMenu_SetState(int state) {
	g_Game->_pauseScreen.state = state;
}