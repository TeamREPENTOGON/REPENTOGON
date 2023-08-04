#include <lua.hpp>

#include "HookSystem.h"
#include "IsaacRepentance.h"
#include "LuaCore.h"

static constexpr const char* CharacterMenuMT = "CharacterMenu";


static int lua_CharMenu_GetBigCharPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetBigCharPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyOverlaySprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetDifficultyOverlaySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetGreedDecoSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetGreedDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetBGSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetBGSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetDifficultyPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetDifficultyPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetSeedPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetWinStreakPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetWinStreakPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetEastereggPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetEastereggPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedUnlockPageSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetSeedUnlockPageSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetCharacterPortraitSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetCharacterPortraitSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetSeedEntrySprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetSeedEntrySprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetPageSwapWidgetSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetPageSwapWidgetSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetTaintedBGDecoSprite(lua_State* L)
{
    Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
    ANM2* anm2 = menu->GetTaintedBGDecoSprite();
    lua::luabridge::UserdataPtr::push(L, anm2, lua::GetMetatableKey(lua::Metatables::SPRITE));

    return 1;
}

static int lua_CharMenu_GetNumCharacters(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  lua_pushinteger(L, menu->GetNumCharacters());

  return 1;
}

static int lua_CharMenu_GetSelectedCharacterMenu(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  lua_pushinteger(L, menu->GetSelectedCharacterMenu());

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterMenu(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  menu->SetSelectedCharacterMenu((int)luaL_checkinteger(L, 2));

  return 0;
}

static int lua_CharMenu_GetIsCharacterUnlocked(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  lua_pushboolean(L, menu->IsCharacterUnlocked);

  return 1;
}

static int lua_CharMenu_SetIsCharacterUnlocked(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  menu->IsCharacterUnlocked = lua_toboolean(L, 2);

  return 0;
}

static int lua_CharMenu_GetDifficulty(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  lua_pushinteger(L, menu->Difficulty);

  return 1;
}

static int lua_CharMenu_SetDifficulty(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  menu->Difficulty = (int)luaL_checkinteger(L, 2);

  return 0;
}

static int lua_CharMenu_GetSelectedCharacterID(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  lua_pushinteger(L, menu->SelectedCharacterID);

  return 1;
}

static int lua_CharMenu_SetSelectedCharacterID(lua_State* L)
{
  Menu_Character* menu = *lua::GetUserdata<Menu_Character**>(L, 1, CharacterMenuMT);
  menu->SelectedCharacterID = (int)luaL_checkinteger(L, 2);

  return 0;
}

static void RegisterStatsMenuGame(lua_State* L)
{
    luaL_newmetatable(L, CharacterMenuMT);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_Reg functions[] = {
        { "GetBigCharPageSprite", lua_CharMenu_GetBigCharPageSprite },
        { "GetDifficultyOverlaySprite", lua_CharMenu_GetDifficultyOverlaySprite },
        { "GetGreedDecoSprite", lua_CharMenu_GetGreedDecoSprite },
        { "GetBGSprite", lua_CharMenu_GetBGSprite },
        { "GetDifficultyPageSprite", lua_CharMenu_GetDifficultyPageSprite },
        { "GetSeedPageSprite", lua_CharMenu_GetSeedPageSprite },
        { "GetWinStreakPageSprite", lua_CharMenu_GetWinStreakPageSprite },
        { "GetEastereggPageSprite", lua_CharMenu_GetEastereggPageSprite },
        { "GetSeedUnlockPageSprite", lua_CharMenu_GetSeedUnlockPageSprite },
        { "GetCharacterPortraitSprite", lua_CharMenu_GetCharacterPortraitSprite },
        { "GetSeedEntrySprite", lua_CharMenu_GetSeedEntrySprite },
        { "GetPageSwapWidgetSprite", lua_CharMenu_GetPageSwapWidgetSprite },
        { "GetTaintedBGDecoSprite", lua_CharMenu_GetTaintedBGDecoSprite },

        { "GetNumCharacters", lua_CharMenu_GetNumCharacters },
        { "GetSelectedCharacterMenu", lua_CharMenu_GetSelectedCharacterMenu },
        //{ "SetSelectedCharacterMenu", lua_CharMenu_SetSelectedCharacterMenu }, // does not update sprites and crashes with value = 2
        { "GetIsCharacterUnlocked", lua_CharMenu_GetIsCharacterUnlocked },
        { "SetIsCharacterUnlocked", lua_CharMenu_SetIsCharacterUnlocked },
        { "GetDifficulty", lua_CharMenu_GetDifficulty },
        { "SetDifficulty", lua_CharMenu_SetDifficulty },
        { "GetSelectedCharacterID", lua_CharMenu_GetSelectedCharacterID },
        //{ "SetSelectedCharacterID", lua_CharMenu_SetSelectedCharacterID }, // does not rotate the wheel correctly
        { NULL, NULL }
    };

    luaL_setfuncs(L, functions, 0);
    lua_pop(L, 1);
}

HOOK_METHOD(LuaEngine, RegisterClasses, ()->void)
{
    super();
    lua_State* state = g_LuaEngine->_state;
    lua::LuaStackProtector protector(state);
    RegisterStatsMenuGame(state);
}
