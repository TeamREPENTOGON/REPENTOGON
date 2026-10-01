#include "IsaacRepentance.h"
#include "HookSystem.h"

MOD_EXPORT ANM2* L_SaveMenu_GetSaveSelectMenuSprite() {
	return &g_MenuManager->_menuSave.SaveSelectMenuSprite;
}

MOD_EXPORT ANM2* L_SaveMenu_GetDeleteButtonSprite() {
	return &g_MenuManager->_menuSave.DeleteButtonSprite;
}

MOD_EXPORT ANM2* L_SaveMenu_GetDeletePopupSprite() {
	return &g_MenuManager->_menuSave.DeletePopupSprite;
}

MOD_EXPORT ANM2* L_SaveMenu_GetSave1DrawingSprite() {
	return &g_MenuManager->_menuSave.Save1DrawingSprite;
}

MOD_EXPORT ANM2* L_SaveMenu_GetSave2DrawingSprite() {
	return &g_MenuManager->_menuSave.Save2DrawingSprite;
}

MOD_EXPORT ANM2* L_SaveMenu_GetSave3DrawingSprite() {
	return &g_MenuManager->_menuSave.Save3DrawingSprite;
}

MOD_EXPORT int L_SaveMenu_GetSelectedElement() {
	return g_MenuManager->_menuSave.SelectedSave;
}

MOD_EXPORT bool L_SaveMenu_IsDeleteActive() {
	return g_MenuManager->_menuSave.State == 1;
}

static std::string CustomSaveSlotSprite1;
static std::string CustomSaveSlotSprite2;
static std::string CustomSaveSlotSprite3;

void TryReplaceSlotGraphic(ANM2* sprite, std::string& customSprite) {
	if (customSprite.empty()) {
		return;
	}

	bool success = sprite->ReplaceSpritesheet(0, customSprite);
	if (!success) {
		return;
	}

	for (int i = 1; i < sprite->GetLayerCount(); i++) {
		LayerState* layer = sprite->GetLayer(i);
		if (layer != nullptr) {
			layer->_visible = false;
		}
	}

	sprite->LoadGraphics(false);
}

MOD_EXPORT void L_SaveMenu_SetSlotSpritesheet(int slot, const char* customSprite) {
	switch (slot) {
	case 1:
		CustomSaveSlotSprite1 = customSprite;
		TryReplaceSlotGraphic(&g_MenuManager->_menuSave.Save1DrawingSprite, CustomSaveSlotSprite1);
		break;
	case 2:
		CustomSaveSlotSprite2 = customSprite;
		TryReplaceSlotGraphic(&g_MenuManager->_menuSave.Save2DrawingSprite, CustomSaveSlotSprite2);
		break;
	case 3:
		CustomSaveSlotSprite3 = customSprite;
		TryReplaceSlotGraphic(&g_MenuManager->_menuSave.Save3DrawingSprite, CustomSaveSlotSprite3);
	}
}

HOOK_METHOD(Menu_Save, replace_slot_graphics, (ANM2* sprite) -> void) {
	super(sprite);

	if (g_Manager->_currentSaveSlot == 1) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite1);
	}
	else if (g_Manager->_currentSaveSlot == 2) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite2);
	}
	else if (g_Manager->_currentSaveSlot == 3) {
		TryReplaceSlotGraphic(sprite, CustomSaveSlotSprite3);
	}
}

MOD_EXPORT void L_SaveMenu_SetSelectedElement(int element) {
	g_MenuManager->_menuSave.SelectedSave = element;
}