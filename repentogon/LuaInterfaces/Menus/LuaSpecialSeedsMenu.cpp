#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_SpecialSeedsMenu_GetSprite() {
	return &g_MenuManager->_menuSpecialSeeds.SpecialSeedsSprite;
}

MOD_EXPORT int L_SpecialSeedsMenu_GetSelectedElement() {
	return g_MenuManager->_menuSpecialSeeds.SelectedElement;
}

MOD_EXPORT void L_SpecialSeedsMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuSpecialSeeds.SelectedElement = element;
}
