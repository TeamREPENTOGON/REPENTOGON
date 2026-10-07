#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../LuaClasses.h"
#include "HookSystem.h"
#include <list>

MOD_EXPORT void L_HUD_AssignPlayerHUDs(HUD* hud) {
	hud->AssignPlayerHUDs();
}

MOD_EXPORT void L_HUD_Update(HUD* hud) {
	hud->Update();
}

MOD_EXPORT void L_HUD_PostUpdate(HUD* hud) {
	hud->PostUpdate();
}

MOD_EXPORT void L_HUD_Render(HUD* hud) {
	hud->Render();
}

MOD_EXPORT void L_HUD_ShowFortuneText(HUD* hud, const char** lines, int count) {
	std::list<std::string> text;
	for (int i = 0; i < count; i++) {
		text.push_back(lines[i]);
	}
	hud->ShowFortuneText((int**)&text);
}

MOD_EXPORT void L_HUD_ShowStackedItemText(HUD* hud, const char* mainString, const char* secondaryString, bool isCurseDisplay, bool stackUpText) {
	if (stackUpText)
		hud->ClearStackedItemText();
	hud->ShowStackedItemTextCustomUTF8(const_cast<char*>(mainString), const_cast<char*>(secondaryString), false, isCurseDisplay);
}

MOD_EXPORT void L_HUD_FlashChargeBar(HUD* hud, Entity_Player* player, int slot) {
	hud->FlashChargeBar(player, slot);
}

MOD_EXPORT void L_HUD_FlashRedHearts(HUD* hud, Entity_Player* player) {
	hud->FlashRedHearts(player);
}

MOD_EXPORT void L_HUD_InvalidateActiveItem(HUD* hud, Entity_Player* player, int slot) {
	hud->InvalidateActiveItem(player, slot);
}

MOD_EXPORT void L_HUD_InvalidateCraftingItem(HUD* hud, Entity_Player* player) {
	hud->InvalidateCraftingItem(player);
}

MOD_EXPORT void L_HUD_ShowItemTextPlayer(HUD* hud, Entity_Player* player, ItemConfig_Item* item, bool stackUpText) {
	if (stackUpText)
		hud->ClearStackedItemText();
	hud->ShowItemText(player, item);
}

MOD_EXPORT void L_HUDMessage_Show(HUD_Message* message, const char* text, const char* subtext, bool sticky, bool curseDisplay) {
	message->Show(text, subtext, !sticky, curseDisplay);
}

static std::string HUDMessageText;

static const char* WideToUTF8(const wchar_t* text) {
	if (!text) {
		return "";
	}
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, text, wcslen(text), NULL, 0, NULL, NULL);
	HUDMessageText.assign(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, text, wcslen(text), &HUDMessageText[0], sizeNeeded, NULL, NULL);
	return HUDMessageText.c_str();
}

static void SetText(HUD_Message* message, const char* text, bool asSubText) {
	std::string str = text;
	const int len = str.length();
	std::wstring wStr(len, 0);
	mbstowcs(&wStr[0], str.c_str(), len);

	message->LoadText(wStr.c_str(), asSubText);
	message->UpdateTextImage();
}

MOD_EXPORT const char* L_HUDMessage_GetMainText(HUD_Message* message) {
	return WideToUTF8(message->_text);
}

MOD_EXPORT void L_HUDMessage_SetMainText(HUD_Message* message, const char* text) {
	SetText(message, text, false);
}

MOD_EXPORT const char* L_HUDMessage_GetSubText(HUD_Message* message) {
	return WideToUTF8(message->_subtext);
}

MOD_EXPORT void L_HUDMessage_SetSubText(HUD_Message* message, const char* text) {
	SetText(message, text, true);
}
