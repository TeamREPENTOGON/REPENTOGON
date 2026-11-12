#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_CutscenesMenu_GetSprite() {
	return &g_MenuManager->_menuCutscenes.CutscenesMenuSprite;
}

MOD_EXPORT int L_CutscenesMenu_GetSelectedElement() {
	return g_MenuManager->_menuCutscenes.SelectedElement;
}

MOD_EXPORT void L_CutscenesMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuCutscenes.SelectedElement = element;
}
