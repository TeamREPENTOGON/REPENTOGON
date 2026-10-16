#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_ModsMenu_GetSprite() {
	return &g_MenuManager->_menuMods.ModsMenuSprite;
}

MOD_EXPORT int L_ModsMenu_GetSelectedElement() {
	return g_MenuManager->_menuMods.SelectedElement + 1;
}

MOD_EXPORT void L_ModsMenu_SetSelectedElement(int element) {
	int newPosition = element - 1;

	if (newPosition >= (int)g_Manager->_modManager._mods.size()) {
		newPosition = g_Manager->GetModManager()->_mods.size() - 1;
	}

	if (newPosition < 0)
		newPosition = 0;

	g_MenuManager->_menuMods._pointerToSelectedMod = *(ModEntry**)g_Manager->GetModManager() + (newPosition * 4);
	g_MenuManager->_menuMods.SelectedElement = newPosition;
}

MOD_EXPORT bool L_ModsMenu_WasListEdited() {
	return g_MenuManager->_menuMods.State == 1;
}