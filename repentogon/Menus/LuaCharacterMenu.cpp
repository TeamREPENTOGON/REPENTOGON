#include <lua.hpp>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static constexpr const char* CharacterMenuMT = "CharacterMenu";


static int lua_CharMenu_GetBigCharPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetBigCharPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyOverlaySprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetDifficultyOverlaySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetGreedDecoSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetGreedDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetBGSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetBGSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetDifficultyPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetWinStreakPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetWinStreakPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetEastereggPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetEastereggPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedUnlockPageSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedUnlockPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetCharacterPortraitSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetCharacterPortraitSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedEntrySprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetSeedEntrySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetPageSwapWidgetSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetPageSwapWidgetSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetTaintedBGDecoSprite(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
    ANM2* anm2 = menu->GetTaintedBGDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetNumCharacters(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->GetNumCharacters());

  return 1;
}

static int lua_CharMenu_GetSelectedCharacterMenu(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->GetSelectedCharacterMenu());

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterMenu(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->SetSelectedCharacterMenu((int)luaL_checkinteger(L, 1));

  return 0;
}

static int lua_CharMenu_GetIsCharacterUnlocked(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushboolean(L, menu->IsCharacterUnlocked);

  return 1;
}

static int lua_CharMenu_SetIsCharacterUnlocked(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->IsCharacterUnlocked = lua_toboolean(L, 1);

  return 0;
}

static int lua_CharMenu_GetDifficulty(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->Difficulty);

  return 1;
}

static int lua_CharMenu_SetDifficulty(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->Difficulty = (int)luaL_checkinteger(L, 1);

  return 0;
}

static int lua_CharMenu_GetSelectedCharacterID(lua_State* L)
{
  Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  lua_pushinteger(L, menu->SelectedCharacterID);

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterID(lua_State* L)
{
    Menu_Character* menu = g_MenuManager->GetMenuCharacter();
  menu->SelectedCharacterID = (int)luaL_checkinteger(L, 1);

  return 0;
}

static void RegisterStatsMenuGame(lua_State* L)
{
    lua::LuaStackProtector protector(L);
    lua_newtable(L);
    lua_pushstring(L, "GetBigCharPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetBigCharPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetDifficultyOverlaySprite");
    lua_pushcfunction(L, lua_CharMenu_GetDifficultyOverlaySprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetGreedDecoSprite");
    lua_pushcfunction(L, lua_CharMenu_GetGreedDecoSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetBGSprite");
    lua_pushcfunction(L, lua_CharMenu_GetBGSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetDifficultyPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetDifficultyPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetSeedPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetSeedPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetWinStreakPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetWinStreakPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetEastereggPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetEastereggPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetSeedUnlockPageSprite");
    lua_pushcfunction(L, lua_CharMenu_GetSeedUnlockPageSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetCharacterPortraitSprite");
    lua_pushcfunction(L, lua_CharMenu_GetCharacterPortraitSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetSeedEntrySprite");
    lua_pushcfunction(L, lua_CharMenu_GetSeedEntrySprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetPageSwapWidgetSprite");
    lua_pushcfunction(L, lua_CharMenu_GetPageSwapWidgetSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetPageSwapWidgetSprite");
    lua_pushcfunction(L, lua_CharMenu_GetPageSwapWidgetSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetTaintedBGDecoSprite");
    lua_pushcfunction(L, lua_CharMenu_GetTaintedBGDecoSprite);
    lua_settable(L, -3);
    lua_pushstring(L, "GetNumCharacters");
    lua_pushcfunction(L, lua_CharMenu_GetNumCharacters);
    lua_settable(L, -3);
    lua_pushstring(L, "GetSelectedCharacterMenu");
    lua_pushcfunction(L, lua_CharMenu_GetSelectedCharacterMenu);
    lua_settable(L, -3);
    lua_pushstring(L, "GetIsCharacterUnlocked");
    lua_pushcfunction(L, lua_CharMenu_GetIsCharacterUnlocked);
    lua_settable(L, -3);
    lua_pushstring(L, "SetIsCharacterUnlocked");
    lua_pushcfunction(L, lua_CharMenu_SetIsCharacterUnlocked);
    lua_settable(L, -3);
    lua_pushstring(L, "GetDifficulty");
    lua_pushcfunction(L, lua_CharMenu_GetDifficulty);
    lua_settable(L, -3);
    lua_pushstring(L, "SetDifficulty");
    lua_pushcfunction(L, lua_CharMenu_SetDifficulty);
    lua_settable(L, -3);
    lua_pushstring(L, "GetSelectedCharacterID");
    lua_pushcfunction(L, lua_CharMenu_GetSelectedCharacterID);
    lua_settable(L, -3);
    lua_setglobal(L, "CharacterMenu");
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
    super();
    lua_State* state = g_LuaEngine->_state;
    lua::LuaStackProtector protector(state);
    RegisterStatsMenuGame(state);
}
