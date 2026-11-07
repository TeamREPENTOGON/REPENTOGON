#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_BestiaryMenu_GetBestiaryMenuSprite() {
	return &g_MenuManager->_menuBestiary.BestiaryMenuSprite;
}

MOD_EXPORT ANM2* L_BestiaryMenu_GetDeathScreenSprite() {
	return &g_MenuManager->_menuBestiary.DeathScreenSprite;
}

MOD_EXPORT ANM2* L_BestiaryMenu_GetEnemySprite() {
	return &g_MenuManager->_menuBestiary.EnemySprite;
}

MOD_EXPORT int L_BestiaryMenu_GetNumBossPages() {
	return ((g_MenuManager->_menuBestiary.unk2 - g_MenuManager->_menuBestiary.unk1) >> 2) / 4;
}

MOD_EXPORT int L_BestiaryMenu_GetNumMonsterPages() {
	return g_MenuManager->_menuBestiary.LastEnemyPageID;
}

MOD_EXPORT int L_BestiaryMenu_GetNumPages() {
	return (((g_MenuManager->_menuBestiary.unk2 - g_MenuManager->_menuBestiary.unk1) >> 2) / 4) + g_MenuManager->_menuBestiary.LastEnemyPageID;
}

MOD_EXPORT int L_BestiaryMenu_GetSelectedPage() {
	return g_MenuManager->_menuBestiary.CurrentPage;
}

MOD_EXPORT int L_BestiaryMenu_GetSelectedElement() {
	return g_MenuManager->_menuBestiary.SelectedElement;
}

MOD_EXPORT void L_BestiaryMenu_SetSelectedPage(int page) {
	g_MenuManager->_menuBestiary.CurrentPage = page;
	g_MenuManager->_menuBestiary.LoadPreview();
}

MOD_EXPORT void L_BestiaryMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuBestiary.SelectedElement = element;
	g_MenuManager->_menuBestiary.LoadPreview();
}