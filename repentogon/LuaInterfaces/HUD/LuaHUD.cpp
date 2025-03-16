#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "HookSystem.h"


LUA_FUNCTION(Lua_HUDFlashRedHearts) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	Entity_Player* player = lua::GetLuabridgeUserdata<Entity_Player*>(L, 2, lua::Metatables::ENTITY_PLAYER, "EntityPlayer");

	hud->FlashRedHearts(player);
	return 0;
}

LUA_FUNCTION(Lua_HUDGetChargeBarSprite)
{
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_chargeBarSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPickupsHUDSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_pickupHUDSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetHeartsSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_heartsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCardsPillsSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_cardsPillsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetStreakSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");

	ANM2* sprite = &hud->_messageMain._streakSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetMainMessage) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");

	HUD_Message** ud = (HUD_Message**)lua_newuserdata(L, sizeof(HUD_Message*));
	*ud = &hud->_messageMain;
	luaL_setmetatable(L, lua::metatables::HUDMessageMT);

	return 1;
}

LUA_FUNCTION(Lua_HUDGetStackedMessage) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	const int i = (int)luaL_optinteger(L, 2, 0);

	if (i < 0 || i > 5) {
		return luaL_error(L, "Invalid HUD message index %d", i);;
	}

	HUD_Message** ud = (HUD_Message**)lua_newuserdata(L, sizeof(HUD_Message*));
	*ud = &hud->_messageStack[i];
	luaL_setmetatable(L, lua::metatables::HUDMessageMT);

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPlayerMessage) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	const int i = (int)luaL_optinteger(L, 2, 0);

	if (i < 0 || i > 3) {
		return luaL_error(L, "Invalid player HUD message index %d", i);;
	}

	HUD_Message** ud = (HUD_Message**)lua_newuserdata(L, sizeof(HUD_Message*));
	*ud = &hud->_messagePlayerHUD[i];
	luaL_setmetatable(L, lua::metatables::HUDMessageMT);

	return 1;
}

LUA_FUNCTION(Lua_HUDGetFortuneSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_fortuneSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCoopMenuSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_coopMenuSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetInventorySprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_inventorySprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetCraftingSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_craftingTableSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetPoopSpellSprite) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	ANM2* sprite = &hud->_poopSpellsSprite;
	lua::luabridge::UserdataPtr::push(L, sprite, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(Lua_HUDGetBossHPBarFill) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	lua_pushnumber(L, hud->_bossHPBarFill);

	return 1;
}
LUA_FUNCTION(Lua_HUDSetBossHPBarFill) {
	HUD* hud = lua::GetLuabridgeUserdata<HUD*>(L, 1, lua::Metatables::HUD, "HUD");
	hud->_bossHPBarFill = (float)luaL_checknumber(L, 2);

	return 0;
}

LUA_FUNCTION(Lua_HUDMessageGetSprite) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);
	lua::luabridge::UserdataPtr::push(L, &message->_streakSprite, lua::GetMetatableKey(lua::Metatables::SPRITE));
	return 1;
}

LUA_FUNCTION(Lua_HUDMessageIsShowing) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);
	lua_pushboolean(L, message->_showing);
	return 1;
}

LUA_FUNCTION(Lua_HUDMessageHide) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);
	message->_showing = false;
	return 0;
}

LUA_FUNCTION(Lua_HUDMessageShow) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);
	std::string text = luaL_checkstring(L, 2);
	std::string subtext = luaL_optstring(L, 3, "");
	bool sticky = lua::luaL_optboolean(L, 4, false);
	bool curseDisplay = lua::luaL_optboolean(L, 5, false);
	message->Show(text.c_str(), subtext.c_str(), !sticky, curseDisplay);
	return 0;
}

LUA_FUNCTION(Lua_HUDMessageGetText) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);

	if (message->_text) {
		int sizeNeededTitle = WideCharToMultiByte(CP_UTF8, 0, &message->_text[0], wcslen(message->_text), NULL, 0, NULL, NULL);
		std::string strText(sizeNeededTitle, 0);
		WideCharToMultiByte(CP_UTF8, 0, message->_text, wcslen(message->_text), &strText[0], sizeNeededTitle, NULL, NULL);

		lua_pushstring(L, strText.c_str());
	} else {
		lua_pushstring(L, "");
	}

	return 1;
}

LUA_FUNCTION(Lua_HUDMessageSetText) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);

	std::string str = luaL_checkstring(L, 2);
	const int len = str.length();
	std::wstring wStr(len, 0);
	mbstowcs(&wStr[0], str.c_str(), len);

	message->LoadText(wStr.c_str(), false);
	message->UpdateTextImage();

	return 0;
}

LUA_FUNCTION(Lua_HUDMessageGetSubText) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);

	if (message->_subtext) {
		int sizeNeededTitle = WideCharToMultiByte(CP_UTF8, 0, &message->_subtext[0], wcslen(message->_subtext), NULL, 0, NULL, NULL);
		std::string strSubText(sizeNeededTitle, 0);
		WideCharToMultiByte(CP_UTF8, 0, message->_subtext, wcslen(message->_subtext), &strSubText[0], sizeNeededTitle, NULL, NULL);

		lua_pushstring(L, strSubText.c_str());
	} else {
		lua_pushstring(L, "");
	}

	return 1;
}

LUA_FUNCTION(Lua_HUDMessageSetSubText) {
	HUD_Message* message = *lua::GetRawUserdata<HUD_Message**>(L, 1, lua::metatables::HUDMessageMT);

	std::string str = luaL_checkstring(L, 2);
	const int len = str.length();
	std::wstring wStr(len, 0);
	mbstowcs(&wStr[0], str.c_str(), len);

	message->LoadText(wStr.c_str(), true);
	message->UpdateTextImage();

	return 0;
}

static void RegisterHUDMessage(lua_State* L) {
	luaL_Reg functions[] = {
		{ "GetSprite", Lua_HUDMessageGetSprite },
		{ "IsShowing", Lua_HUDMessageIsShowing },
		{ "Hide", Lua_HUDMessageHide },
		{ "Show", Lua_HUDMessageShow },
		{ "GetMainText", Lua_HUDMessageGetText },
		{ "SetMainText", Lua_HUDMessageSetText },
		{ "GetSubText", Lua_HUDMessageGetSubText },
		{ "SetSubText", Lua_HUDMessageSetSubText },
		{ NULL, NULL }
	};
	lua::RegisterNewClass(L, lua::metatables::HUDMessageMT, lua::metatables::HUDMessageMT, functions);
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua::LuaStackProtector protector(_state);

	luaL_Reg functions[] = {
		{ "FlashRedHearts", Lua_HUDFlashRedHearts },
		{ "GetChargeBarSprite", Lua_HUDGetChargeBarSprite },
		{ "GetHeartsSprite", Lua_HUDGetHeartsSprite },
		{ "GetPickupsHUDSprite", Lua_HUDGetPickupsHUDSprite },
		{ "GetCardsPillsSprite", Lua_HUDGetCardsPillsSprite },
		{ "GetStreakSprite", Lua_HUDGetStreakSprite },
		{ "GetMainMessage", Lua_HUDGetMainMessage },
		{ "GetStackedMessage", Lua_HUDGetStackedMessage },
		{ "GetCoopPlayerMessage", Lua_HUDGetPlayerMessage },
		{ "GetFortuneSprite", Lua_HUDGetFortuneSprite },
		{ "GetCoopMenuSprite", Lua_HUDGetCoopMenuSprite },
		{ "GetInventorySprite", Lua_HUDGetInventorySprite },
		{ "GetCraftingSprite", Lua_HUDGetCraftingSprite },
		{ "GetPoopSpellSprite", Lua_HUDGetPoopSpellSprite },
		{ "GetBossHPBarFill", Lua_HUDGetBossHPBarFill },
		{ "SetBossHPBarFill", Lua_HUDSetBossHPBarFill },
		{ NULL, NULL }
	};
	lua::RegisterFunctions(_state, lua::Metatables::HUD, functions);

	RegisterHUDMessage(_state);
}
