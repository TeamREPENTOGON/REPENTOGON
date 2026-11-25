#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_CollectionMenu_GetCollectionMenuSprite() {
	return &g_MenuManager->_menuCollection.CollectionMenuSprite;
}

MOD_EXPORT ANM2* L_CollectionMenu_GetDeathScreenSprite() {
	return &g_MenuManager->_menuCollection.DeathScreenSprite;
}

MOD_EXPORT int L_CollectionMenu_GetSelectedElement() {
	return g_MenuManager->_menuCollection.SelectedElement;
}
MOD_EXPORT int L_CollectionMenu_GetSelectedPage() {
	return g_MenuManager->_menuCollection.SelectedPage;
}

MOD_EXPORT void L_CollectionMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuCollection.SelectedElement = element;
}
MOD_EXPORT void L_CollectionMenu_SetSelectedPage(int page) {
	g_MenuManager->_menuCollection.SelectedPage = page;
}