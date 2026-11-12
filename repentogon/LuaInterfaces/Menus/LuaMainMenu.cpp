#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_MainMenu_GetGameMenuSprite() {
	return &g_MenuManager->_menuGame.GameMenuSprite;
}

MOD_EXPORT ANM2* L_MainMenu_GetContinueWidgetSprite() {
	return &g_MenuManager->_menuGame.ContinueWidgetSprite;
}

MOD_EXPORT int L_MainMenu_GetSelectedElement() {
	return g_MenuManager->_menuGame.SelectedElement;
}

MOD_EXPORT void L_MainMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuGame.SelectedElement = element;
}