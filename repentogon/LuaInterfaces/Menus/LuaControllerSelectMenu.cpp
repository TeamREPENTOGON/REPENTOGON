#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_ControllerSelectMenu_GetSprite() {
	return &g_MenuManager->_menuControllerSelect.ControllerSelectSprite;
}

MOD_EXPORT int L_ControllerSelectMenu_GetSelectedElement() {
	return g_MenuManager->_menuControllerSelect.SelectedElement;
}

MOD_EXPORT void L_ControllerSelectMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuControllerSelect.SelectedElement = element;
}
