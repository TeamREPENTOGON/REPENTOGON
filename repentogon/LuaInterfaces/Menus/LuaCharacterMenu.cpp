#include <lua.hpp>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

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
	menu->_characterMenuShown = (int)luaL_checkinteger(L, 2);

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
	menu->IsCharacterUnlocked = lua_toboolean(L, 2);

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
	menu->Difficulty = (int)luaL_checkinteger(L, 2);

	return 0;
}

LUA_FUNCTION(lua_CharMenu_GetSelectedCharacterID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	lua_pushinteger(L, menu->SelectedCharacterID);

	return 1;
}

LUA_FUNCTION(lua_CharMenu_SetSelectedCharacterID)
{
	lua::LuaCheckMainMenuExists(L, lua::metatables::CharacterMenuMT);
	Menu_Character* menu = g_MenuManager->GetMenuCharacter();
	menu->SelectedCharacterID = (int)luaL_checkinteger(L, 2);

	return 0;
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
	menu->_characterWheelDepth = (float)luaL_checknumber(L, 2);

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
	menu->_scrollSpeed = (float)luaL_checknumber(L, 2);

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
	menu->_characterWheelWidth = (float)luaL_checknumber(L, 2);

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
	lua::TableAssoc(L, "GetEastereggPageSprite", lua_CharMenu_GetEastereggPageSprite);
	lua::TableAssoc(L, "GetSeedUnlockPageSprite", lua_CharMenu_GetSeedUnlockPageSprite);
	lua::TableAssoc(L, "GetSeedUnlockPageSprite", lua_CharMenu_GetSeedUnlockPageSprite);
	lua::TableAssoc(L, "GetCharacterPortraitSprite", lua_CharMenu_GetCharacterPortraitSprite);
	lua::TableAssoc(L, "GetSeedEntrySprite", lua_CharMenu_GetSeedEntrySprite);
	lua::TableAssoc(L, "GetPageSwapWidgetSprite", lua_CharMenu_GetPageSwapWidgetSprite);
	lua::TableAssoc(L, "GetTaintedBGDecoSprite", lua_CharMenu_GetTaintedBGDecoSprite);
	lua::TableAssoc(L, "GetNumCharacters", lua_CharMenu_GetNumCharacters);
	lua::TableAssoc(L, "GetSelectedCharacterMenu", lua_CharMenu_GetSelectedCharacterMenu);
	lua::TableAssoc(L, "SetSelectedCharacterMenu", lua_CharMenu_SetSelectedCharacterMenu);
	lua::TableAssoc(L, "GetIsCharacterUnlocked", lua_CharMenu_GetIsCharacterUnlocked);
	lua::TableAssoc(L, "SetIsCharacterUnlocked", lua_CharMenu_SetIsCharacterUnlocked);
	lua::TableAssoc(L, "GetDifficulty", lua_CharMenu_GetDifficulty);
	lua::TableAssoc(L, "SetDifficulty", lua_CharMenu_SetDifficulty);
	lua::TableAssoc(L, "GetSelectedCharacterID", lua_CharMenu_GetSelectedCharacterID);
	lua::TableAssoc(L, "SetSelectedCharacterID", lua_CharMenu_SetSelectedCharacterID);
	lua::TableAssoc(L, "GetCharacterWheelDepth", lua_CharMenu_GetCharacterWheelDepth);
	lua::TableAssoc(L, "SetCharacterWheelDepth", lua_CharMenu_SetCharacterWheelDepth);
	lua::TableAssoc(L, "GetScrollSpeed", lua_CharMenu_GetScrollSpeed);
	lua::TableAssoc(L, "SetScrollSpeed", lua_CharMenu_SetScrollSpeed);
	lua::TableAssoc(L, "GetCharacterWheelWidth", lua_CharMenu_GetCharacterWheelWidth);
	lua::TableAssoc(L, "SetCharacterWheelWidth", lua_CharMenu_SetCharacterWheelWidth);
	lua_setglobal(L, lua::metatables::CharacterMenuMT);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
	super();
	lua_State* state = g_LuaEngine->_state;
	lua::LuaStackProtector protector(state);
	RegisterStatsMenuGame(state);
}
