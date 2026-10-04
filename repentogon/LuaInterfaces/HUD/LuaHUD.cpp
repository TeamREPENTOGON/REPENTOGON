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

LUA_FUNCTION(Lua_HUDFlashChargeBar) {
	HUD* hud = LuaHUD::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::GetOpt(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);
	hud->FlashChargeBar(player, slot);
	return 0;
}

LUA_FUNCTION(Lua_HUDInvalidateActiveItem) {
	HUD* hud = LuaHUD::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::GetOpt(L, 2);
	int slot = (int)luaL_checkinteger(L, 3);
	hud->InvalidateActiveItem(player, slot);
	return 0;
}

LUA_FUNCTION(Lua_HUDInvalidateCraftingItem) {
	HUD* hud = LuaHUD::Get(L, 1);
	hud->InvalidateCraftingItem(LuaEntityPlayer::GetOpt(L, 2));
	return 0;
}

LUA_FUNCTION(Lua_HUDFlashRedHearts) {
	HUD* hud = LuaHUD::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::Get(L, 2);
	hud->FlashRedHearts(player);
	return 0;
}

LUA_FUNCTION(Lua_HUDShowItemTextPlayer) {
	HUD* hud = LuaHUD::Get(L, 1);
	Entity_Player* player = LuaEntityPlayer::Get(L, 2);
	ItemConfig_Item* item = LuaItem::Get(L, 3);
	bool stackUpText = lua_toboolean(L, 4);

	if (stackUpText)
		hud->ClearStackedItemText();
	hud->ShowItemText(player, item);
	return 0;
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

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	lua_register(_state, "__Lua_HUD_FlashChargeBar", Lua_HUDFlashChargeBar);
	lua_register(_state, "__Lua_HUD_InvalidateActiveItem", Lua_HUDInvalidateActiveItem);
	lua_register(_state, "__Lua_HUD_InvalidateCraftingItem", Lua_HUDInvalidateCraftingItem);
	lua_register(_state, "__Lua_HUD_FlashRedHearts", Lua_HUDFlashRedHearts);
	lua_register(_state, "__Lua_HUD_ShowItemTextPlayer", Lua_HUDShowItemTextPlayer);

	super();
}
