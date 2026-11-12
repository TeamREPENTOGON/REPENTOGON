#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_KeyConfigMenu_GetSprite() {
	return &g_MenuManager->_menuKeyConfig.KeyConfigSprite;
}

MOD_EXPORT int L_KeyConfigMenu_GetSelectedColumn() {
	return g_MenuManager->_menuKeyConfig.SelectedColumn;
}

MOD_EXPORT int L_KeyConfigMenu_GetSelectedElement() {
	return g_MenuManager->_menuKeyConfig.SelectedElement;
}

MOD_EXPORT bool L_KeyConfigMenu_IsEditActive() {
	return g_MenuManager->_menuKeyConfig.State == 1;
}

MOD_EXPORT void L_KeyConfigMenu_SetSelectedColumn(int column) {
	g_MenuManager->_menuKeyConfig.SelectedColumn = column;
}

MOD_EXPORT void L_KeyConfigMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuKeyConfig.SelectedElement = element;
}