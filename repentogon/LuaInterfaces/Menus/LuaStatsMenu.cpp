#include "IsaacRepentance.h"

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuSprite() {
	return &g_MenuManager->_menuStats._achievementsSprite;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuCursorLeftSprite() {
	return &g_MenuManager->_menuStats._cursorLeftSprite;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuCursorRightSprite() {
	return &g_MenuManager->_menuStats._cursorRightSprite;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite1() {
	return &g_MenuManager->_menuStats._achievementMiniSprite1;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite2() {
	return &g_MenuManager->_menuStats._achievementMiniSprite2;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite3() {
	return &g_MenuManager->_menuStats._achievementMiniSprite3;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite4() {
	return &g_MenuManager->_menuStats._achievementMiniSprite4;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite5() {
	return &g_MenuManager->_menuStats._achievementMiniSprite5;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite6() {
	return &g_MenuManager->_menuStats._achievementMiniSprite6;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite7() {
	return &g_MenuManager->_menuStats._achievementMiniSprite7;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite8() {
	return &g_MenuManager->_menuStats._achievementMiniSprite8;
};

MOD_EXPORT ANM2* L_StatsMenu_GetSecretsMenuMiniSprite9() {
	return &g_MenuManager->_menuStats._achievementMiniSprite9;
};

MOD_EXPORT int L_StatsMenu_GetSelectedElement() {
	return g_MenuManager->_menuStats._statsMenuCurrentSelection;
}

MOD_EXPORT ANM2* L_StatsMenu_GetStatsMenuSprite() {
	return &g_MenuManager->_menuStats._StatsMenuSprite;
};

MOD_EXPORT bool L_StatsMenu_IsSecretsMenuVisible() {
	return g_MenuManager->_menuStats._isAchievementScreenVisible;
};

MOD_EXPORT void L_StatsMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuStats._statsMenuCurrentSelection = element;
}