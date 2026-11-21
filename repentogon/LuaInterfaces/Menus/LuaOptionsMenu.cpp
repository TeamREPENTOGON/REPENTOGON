#include "IsaacRepentance.h"

Menu_Options* GetAppropiateMenu() {
	if (g_MenuManager == NULL) {
		return &g_Game->GetPauseMenu()->menuoptions;
	}
	else {
		return g_MenuManager->GetMenuOptions();
	}
}

MOD_EXPORT ANM2* L_OptionsMenu_GetOptionsMenuSprite() {
	Menu_Options* menu = GetAppropiateMenu();
	return &menu->OptionsSprite;
}

MOD_EXPORT ANM2* L_OptionsMenu_GetGammaWidgetSprite() {
	Menu_Options* menu = GetAppropiateMenu();
	return &menu->GammaMenuSprite;
}

MOD_EXPORT int L_OptionsMenu_GetSelectedElement() {
	Menu_Options* menu = GetAppropiateMenu();
	return menu->SelectedElement;
}

MOD_EXPORT void L_OptionsMenu_SetSelectedElement(int element) {
	Menu_Options* menu = GetAppropiateMenu();
	menu->SelectedElement = element;
}