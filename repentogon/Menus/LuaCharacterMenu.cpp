#include <lua.hpp>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static constexpr const char* CharacterMenuMT = "CharacterMenu";


static int lua_CharMenu_GetBigCharPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetBigCharPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyOverlaySprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetDifficultyOverlaySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetGreedDecoSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetGreedDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetBGSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetBGSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetDifficultyPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetWinStreakPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetWinStreakPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetEastereggPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetEastereggPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedUnlockPageSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedUnlockPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetCharacterPortraitSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetCharacterPortraitSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedEntrySprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedEntrySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetPageSwapWidgetSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetPageSwapWidgetSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetTaintedBGDecoSprite(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetTaintedBGDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetNumCharacters(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->GetNumCharacters());

  return 1;
}

static int lua_CharMenu_GetSelectedCharacterMenu(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->GetSelectedCharacterMenu());

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterMenu(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->SetSelectedCharacterMenu((int)luaL_checkinteger(L, 1));

  return 0;
}

static int lua_CharMenu_GetIsCharacterUnlocked(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushboolean(L, menu->IsCharacterUnlocked);

  return 1;
}

static int lua_CharMenu_SetIsCharacterUnlocked(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->IsCharacterUnlocked = lua_toboolean(L, 1);

  return 0;
}

static int lua_CharMenu_GetDifficulty(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->Difficulty);

  return 1;
}

static int lua_CharMenu_SetDifficulty(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->Difficulty = (int)luaL_checkinteger(L, 1);

  return 0;
}

static int lua_CharMenu_GetSelectedCharacterID(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
  Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->SelectedCharacterID);

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterID(lua_State* L)
{
    if (g_MenuManager == NULL) { return luaL_error(L, "CharacterMenu functions can only be used in the main menu"); }
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->SelectedCharacterID = (int)luaL_checkinteger(L, 1);

  return 0;
}

static void RegisterStatsMenuGame(lua_State* L)
{
    lua::LuaStackProtector protector(L);
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
    lua::TableAssoc(L, "GetIsCharacterUnlocked", lua_CharMenu_GetIsCharacterUnlocked);
    lua::TableAssoc(L, "SetIsCharacterUnlocked", lua_CharMenu_SetIsCharacterUnlocked);
    lua::TableAssoc(L, "GetDifficulty", lua_CharMenu_GetDifficulty);
    lua::TableAssoc(L, "SetDifficulty", lua_CharMenu_SetDifficulty);
    lua::TableAssoc(L, "GetSelectedCharacterID", lua_CharMenu_GetSelectedCharacterID);
    lua_setglobal(L, "CharacterMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
    super();
    lua_State* state = g_LuaEngine->_state;
    lua::LuaStackProtector protector(state);
    RegisterStatsMenuGame(state);
}
