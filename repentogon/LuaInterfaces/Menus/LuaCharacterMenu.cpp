#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"
#include "../../Patches/XMLPlayerExtras.h"

LUA_FUNCTION(lua_CharMenu_GetBigCharPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetBigCharPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetDifficultyOverlaySprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetDifficultyOverlaySprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetGreedDecoSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetGreedDecoSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetBGSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetBGSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetDifficultyPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetDifficultyPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetSeedPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetSeedPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetWinStreakPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetWinStreakPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetEastereggPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetEastereggPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetSeedUnlockPageSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetSeedUnlockPageSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetCharacterPortraitSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetCharacterPortraitSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetSeedEntrySprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetSeedEntrySprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetPageSwapWidgetSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetPageSwapWidgetSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetTaintedBGDecoSprite)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	ANM2* anm2 = menu->GetTaintedBGDecoSprite();
	lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetNumCharacters)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->_numCharacters);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetSelectedCharacterMenu)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->_characterMenuShown);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetSelectedCharacterMenu)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const int page = (int)luaL_checkinteger(L, 1);
	const bool updateGraphics = lua::luaL_optboolean(L, 2, false);

	if (page == 0 || page == 1) {
		if (updateGraphics) {
			menu->ChangeCharacterPage(page);
		} else {
			// Legacy behaviour of this function that doesn't actually visually change the menu.
			menu->_characterMenuShown = page;
		}
	}

	return 0;
}

LUA_FUNCTION(lua_CharMenu_SwapCharacterMenu)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const bool force = lua::luaL_optboolean(L, 1, false);

	if (menu->_PageSwapWidgetSprite._color._tint[3] != 0.0f || force) {
		menu->DoPageSwap();
	}

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetIsCharacterUnlocked)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushboolean(L, menu->IsCharacterUnlocked);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetIsCharacterUnlocked)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->IsCharacterUnlocked = lua::luaL_checkboolean(L, 1);

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetDifficulty)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->Difficulty);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetDifficulty)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->Difficulty = (int)luaL_checkinteger(L, 1);

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetSelectedCharacterID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->SelectedCharacterID);

	return 1;
}

// Given a CharacterMenu character ID, returns the "index" of that character in the wheel.
// Can be affected by preceding characters being hidden/locked and such, so a bit annoying to calculate.
int GetCharacterMenuIndexFromID(const int targetCharID) {
	if (targetCharID == 0)
		return 0;

	int idx = 0;

	for (int i = 1; i < 18; i++) {
		auto* charEntry = &__ptr_g_MenuCharacterEntries[i];
		auto* taintedCharEntry = &__ptr_g_MenuCharacterEntries[i + 18];
		const int pType = charEntry->playerType;
		const bool visible = !charEntry->hidden || charEntry->IsCharacterAvailable(0);
		if (visible)
			idx++;
		if (i == targetCharID)
			return visible ? idx : -1;
	}
	for (uint32_t i = 0; i < g_ModCharacterMap.size(); i++) {
		const int pType = g_ModCharacterMap[i].normal;
		const bool visible = !g_Manager->GetEntityConfig()->GetPlayer(pType)->_hidden && !IsCharacterHiddenByAchievementRgon(pType);
		if (visible)
			idx++;
		if (i + 18 == targetCharID)
			return visible ? idx : -1;
	}

	return -1;
}

LUA_FUNCTION(lua_CharMenu_SetSelectedCharacterID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const int charID = (int)luaL_checkinteger(L, 1);
	const bool updateWheel = lua::luaL_optboolean(L, 2, false);
	const bool skipRotation = lua::luaL_optboolean(L, 3, false);

	if (charID >= 0 && charID < (int)g_ModCharacterMap.size() + 18) {
		if (!updateWheel) {
			// Silly legacy behaviour of this function that does no validation whatsoever nor updates the menu.
			menu->SelectedCharacterID = charID;
		} else {
			const int idx = GetCharacterMenuIndexFromID(charID);
			if (idx >= 0) {
				menu->SelectedCharacterID = charID;
				menu->_numCharacters_MINUS_SelectedEntry = (idx == 0) ? 0 : (menu->_numCharacters - idx);
				if (skipRotation) {
					menu->_horizontalScrollPosition = (menu->_numCharacters_MINUS_SelectedEntry * 360.0f) / menu->_numCharacters;
				}
				if (menu->SelectedCharacterID < 18) {
					menu->_BigCharPageSprite.Play(__ptr_g_MenuCharacterEntries[menu->SelectedCharacterID + menu->_characterMenuShown * 18].animationName, false);
				}
			}
		}
	}

	return 0;
}

// Given a CharacterMenu character ID, returns the corresponding PlayerType.
int GetPlayerTypeFromCharacterMenuID(const int charID, const bool taintedMenu) {
	if (charID > 0) {
		if (charID < 18) {
			return __ptr_g_MenuCharacterEntries[taintedMenu ? (charID + 18) : charID].playerType;
		} else if (charID < (int)g_ModCharacterMap.size() + 18) {
			const auto& modChar = g_ModCharacterMap[charID - 18];
			return taintedMenu ? modChar.tainted : modChar.normal;
		}
	}
	return -1;
}

LUA_FUNCTION(lua_CharMenu_GetPlayerTypeFromCharacterMenuID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const int charID = (int)luaL_checkinteger(L, 1);
	const bool taintedMenu = lua::luaL_optboolean(L, 2, menu->GetSelectedCharacterMenu() == 1);
	const int playerType = GetPlayerTypeFromCharacterMenuID(charID, taintedMenu);

	if (playerType < 0) {
		lua_pushnil(L);
	} else {
		lua_pushinteger(L, playerType);
	}

	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetSelectedCharacterPlayerType)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const bool taintedMenu = menu->GetSelectedCharacterMenu() == 1;
	const int playerType = GetPlayerTypeFromCharacterMenuID(menu->SelectedCharacterID, taintedMenu);

	if (playerType < 0) {
		lua_pushnil(L);
	} else {
		lua_pushinteger(L, playerType);
	}

	return 1;
}

// Given a PlayerType, finds the corresponding CharacterMenu character ID, if any.
// I don't think the game maintains a map in this direction.
int GetCharacterMenuIDFromPlayerType(const int playerType) {
	if (playerType >= NUM_PLAYER_TYPES) {
		for (uint32_t i = 0; i < g_ModCharacterMap.size(); i++) {
			if (playerType == g_ModCharacterMap[i].normal || playerType == g_ModCharacterMap[i].tainted) {
				return i + 18;
			}
		}
	} else if (playerType >= 0) {
		for (uint32_t i = 1; i < 36; i++) {
			if (playerType == __ptr_g_MenuCharacterEntries[i].playerType) {
				return i % 18;
			}
		}
	}
	return -1;
}

LUA_FUNCTION(lua_CharMenu_GetCharacterMenuIDFromPlayerType) {
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();

	const int playerType = (int)luaL_checkinteger(L, 1);
	const int charID = GetCharacterMenuIDFromPlayerType(playerType);

	if (charID > 0) {
		lua_pushinteger(L, charID);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

LUA_FUNCTION(lua_CharMenu_GetCharacterWheelDepth)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushnumber(L, menu->_characterWheelDepth);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetCharacterWheelDepth)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->_characterWheelDepth = (float)luaL_checknumber(L, 1);

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetScrollSpeed)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushnumber(L, menu->_scrollSpeed);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetScrollSpeed)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->_scrollSpeed = (float)luaL_checknumber(L, 1);

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetCharacterWheelWidth)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushnumber(L, menu->_characterWheelWidth);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetCharacterWheelWidth)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->_characterWheelWidth = (float)luaL_checknumber(L, 1);

	return 0;
}

LUA_FUNCTION(Lua_CharMenu_GetActiveStatus) {
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->Status);
	return 1;
}

LUA_FUNCTION(Lua_CharMenu_SetActiveStatus) {
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->Status = (int)luaL_checkinteger(L, 1);
	return 0;
}

static void RegisterStatsMenuGame(lua_State* L)
{
	lua_newtable(L);
	lua::TableAssoc(L, "GetBigCharPageSprite", lua_CharMenu_GetBigCharPageSprite);
	lua::TableAssoc(L, "GetDifficultyOverlaySprite", lua_CharMenu_GetDifficultyOverlaySprite);
	lua::TableAssoc(L, "GetGreedDecoSprite", lua_CharMenu_GetGreedDecoSprite);
	lua::TableAssoc(L, "GetBGSprite", lua_CharMenu_GetBGSprite);
	lua::TableAssoc(L, "GetDifficultyPageSprite", lua_CharMenu_GetDifficultyPageSprite);
	lua::TableAssoc(L, "GetSeedPageSprite", lua_CharMenu_GetSeedPageSprite);
	lua::TableAssoc(L, "GetWinStreakPageSprite", lua_CharMenu_GetWinStreakPageSprite);
	lua::TableAssoc(L, "GetEasterEggPageSprite", lua_CharMenu_GetEastereggPageSprite);
	lua::TableAssoc(L, "GetEastereggPageSprite", lua_CharMenu_GetEastereggPageSprite); //deprecated
	lua::TableAssoc(L, "GetSeedUnlockPageSprite", lua_CharMenu_GetSeedUnlockPageSprite);
	lua::TableAssoc(L, "GetSeedUnlockPageSprite", lua_CharMenu_GetSeedUnlockPageSprite);
	lua::TableAssoc(L, "GetCharacterPortraitSprite", lua_CharMenu_GetCharacterPortraitSprite);
	lua::TableAssoc(L, "GetSeedEntrySprite", lua_CharMenu_GetSeedEntrySprite);
	lua::TableAssoc(L, "GetPageSwapWidgetSprite", lua_CharMenu_GetPageSwapWidgetSprite);
	lua::TableAssoc(L, "GetTaintedBGDecoSprite", lua_CharMenu_GetTaintedBGDecoSprite);
	lua::TableAssoc(L, "GetNumCharacters", lua_CharMenu_GetNumCharacters);
	lua::TableAssoc(L, "GetSelectedCharacterMenu", lua_CharMenu_GetSelectedCharacterMenu);
	lua::TableAssoc(L, "SetSelectedCharacterMenu", lua_CharMenu_SetSelectedCharacterMenu);
	lua::TableAssoc(L, "SwapCharacterMenu", lua_CharMenu_SwapCharacterMenu);
	lua::TableAssoc(L, "GetIsCharacterUnlocked", lua_CharMenu_GetIsCharacterUnlocked);
	lua::TableAssoc(L, "SetIsCharacterUnlocked", lua_CharMenu_SetIsCharacterUnlocked);
	lua::TableAssoc(L, "GetDifficulty", lua_CharMenu_GetDifficulty);
	lua::TableAssoc(L, "SetDifficulty", lua_CharMenu_SetDifficulty);
	lua::TableAssoc(L, "GetSelectedCharacterID", lua_CharMenu_GetSelectedCharacterID);
	lua::TableAssoc(L, "SetSelectedCharacterID", lua_CharMenu_SetSelectedCharacterID);
	lua::TableAssoc(L, "GetSelectedCharacterPlayerType", lua_CharMenu_GetSelectedCharacterPlayerType);
	lua::TableAssoc(L, "GetPlayerTypeFromCharacterMenuID", lua_CharMenu_GetPlayerTypeFromCharacterMenuID);
	lua::TableAssoc(L, "GetCharacterMenuIDFromPlayerType", lua_CharMenu_GetCharacterMenuIDFromPlayerType);
	lua::TableAssoc(L, "GetCharacterWheelDepth", lua_CharMenu_GetCharacterWheelDepth);
	lua::TableAssoc(L, "SetCharacterWheelDepth", lua_CharMenu_SetCharacterWheelDepth);
	lua::TableAssoc(L, "GetScrollSpeed", lua_CharMenu_GetScrollSpeed);
	lua::TableAssoc(L, "SetScrollSpeed", lua_CharMenu_SetScrollSpeed);
	lua::TableAssoc(L, "GetCharacterWheelWidth", lua_CharMenu_GetCharacterWheelWidth);
	lua::TableAssoc(L, "SetCharacterWheelWidth", lua_CharMenu_SetCharacterWheelWidth);
	lua::TableAssoc(L, "GetActiveStatus", Lua_CharMenu_GetActiveStatus);
	lua::TableAssoc(L, "SetActiveStatus", Lua_CharMenu_SetActiveStatus);
	lua_setglobal(L, lua::metatables::CharacterMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterStatsMenuGame(state);
}
