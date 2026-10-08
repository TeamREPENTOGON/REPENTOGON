ffi.cdef[[
    int L_CharacterMenu_GetActiveStatus();
    struct Sprite* L_CharacterMenu_GetBigCharPageSprite();
    struct Sprite* L_CharacterMenu_GetBGSprite();
    int L_CharacterMenu_GetCharacterMenuIDFromPlayerType(int);
    struct Sprite* L_CharacterMenu_GetCharacterPortraitSprite();
    float L_CharacterMenu_GetCharacterWheelDepth();
    float L_CharacterMenu_GetCharacterWheelWidth();
    struct Sprite* L_CharacterMenu_GetCompletionMarksSprite();
    int L_CharacterMenu_GetDifficulty();
    struct Sprite* L_CharacterMenu_GetDifficultyOverlaySprite();
    struct Sprite* L_CharacterMenu_GetDifficultyPageSprite();
    struct Sprite* L_CharacterMenu_GetEasterEggPageSprite();
    struct Sprite* L_CharacterMenu_GetGreedDecoSprite();
    bool L_CharacterMenu_GetIsCharacterUnlocked();
    int L_CharacterMenu_GetNumCharacters();
    struct Sprite* L_CharacterMenu_GetPageSwapWidgetSprite();
    int L_CharacterMenu_GetPlayerTypeFromCharacterMenuID(int, bool);
    float L_CharacterMenu_GetScrollSpeed();
    struct Sprite* L_CharacterMenu_GetSeedEntrySprite();
    struct Sprite* L_CharacterMenu_GetSeedPageSprite();
    struct Sprite* L_CharacterMenu_GetSeedUnlockPageSprite();
    int L_CharacterMenu_GetSelectedCharacterMenu();
    int L_CharacterMenu_GetSelectedCharacterID();
    int L_CharacterMenu_GetSelectedCharacterPlayerType();
    struct Sprite* L_CharacterMenu_GetTaintedBGDecoSprite();
    struct Sprite* L_CharacterMenu_GetWinStreakPageSprite();
    void L_CharacterMenu_SetActiveStatus(int);
    void L_CharacterMenu_SetCharacterWheelDepth(float);
    void L_CharacterMenu_SetCharacterWheelWidth(float);
    void L_CharacterMenu_SetDifficulty(int);
    void L_CharacterMenu_SetIsCharacterUnlocked(bool);
    void L_CharacterMenu_SetScrollSpeed(float);
    void L_CharacterMenu_SetSelectedCharacterMenu(int, bool);
    void L_CharacterMenu_SetSelectedCharacterID(int, bool, bool);
    void L_CharacterMenu_SwapCharacterMenu(bool);
]]

local repentogon = ffidll

CharacterMenu = {
    GetActiveStatus = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetActiveStatus()
    end,
    GetBigCharPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetBigCharPageSprite()
    end,
    GetBGSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetBGSprite()
    end,
    GetCharacterMenuIDFromPlayerType = function(character)
        ffichecks.checkmainmenu("CharacterMenu")
        character = ffichecks.checkinteger(1, character)
        local ret = repentogon.L_CharacterMenu_GetCharacterMenuIDFromPlayerType(character)
        if ret == -1 then 
            return nil 
        end
        return ret
    end,
    GetCharacterPortraitSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetCharacterPortraitSprite()
    end,
    GetCharacterWheelDepth = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetCharacterWheelDepth()
    end,
    GetCharacterWheelWidth = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetCharacterWheelWidth()
    end,
    GetCompletionMarksSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetCompletionMarksSprite()
    end,
    GetDifficulty = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetDifficulty()
    end,
    GetDifficultyPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetDifficultyPageSprite()
    end,
    GetDifficultyOverlaySprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetDifficultyOverlaySprite()
    end,
    GetEasterEggPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetEasterEggPageSprite()
    end,
    GetIsCharacterUnlocked = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetIsCharacterUnlocked()
    end,
    GetGreedDecoSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetGreedDecoSprite()
    end,
    GetNumCharacters = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetNumCharacters()
    end,
    GetPageSwapWidgetSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetPageSwapWidgetSprite()
    end,
    GetPlayerTypeFromCharacterMenuID = function(characterMenuID, tainted)
        ffichecks.checkmainmenu("CharacterMenu")
        characterMenuID = ffichecks.checkinteger(1, characterMenuID)
        tainted = ffichecks.optboolean(tainted, repentogon.L_CharacterMenu_GetSelectedCharacterMenu() == 1)
        local ret = repentogon.L_CharacterMenu_GetPlayerTypeFromCharacterMenuID(characterMenuID, tainted)
        if ret == -1 then
            return nil
        end
        return ret
    end,
    GetScrollSpeed = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetScrollSpeed()
    end,
    GetSeedEntrySprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetSeedEntrySprite()
    end,
    GetSeedPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetSeedPageSprite()
    end,
    GetSeedUnlockPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetSeedUnlockPageSprite()
    end,
    GetSelectedCharacterMenu = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetSelectedCharacterMenu()
    end,
    GetSelectedCharacterID = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetSelectedCharacterID()
    end,
    GetSelectedCharacterPlayerType = function()
        ffichecks.checkmainmenu("CharacterMenu")
        local ret = repentogon.L_CharacterMenu_GetSelectedCharacterPlayerType()
        if ret == -1 then
            return nil
        end
        return ret
    end,
    GetTaintedBGDecoSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetTaintedBGDecoSprite()
    end,
    GetWinStreakPageSprite = function()
        ffichecks.checkmainmenu("CharacterMenu")
        return repentogon.L_CharacterMenu_GetWinStreakPageSprite()
    end,
    SetActiveStatus = function(status)
        ffichecks.checkmainmenu("CharacterMenu")
        status = ffichecks.checkinteger(1, status)
        repentogon.L_CharacterMenu_SetActiveStatus(status)
    end,
    SetCharacterWheelDepth = function(value)
        ffichecks.checkmainmenu("CharacterMenu")
        value = ffichecks.checknumber(1, value)
        repentogon.L_CharacterMenu_SetCharacterWheelDepth(value)
    end,
    SetCharacterWheelWidth = function(value)
        ffichecks.checkmainmenu("CharacterMenu")
        value = ffichecks.checknumber(1, value)
        repentogon.L_CharacterMenu_SetCharacterWheelWidth(value)
    end,
    SetDifficulty = function(difficulty)
        ffichecks.checkmainmenu("CharacterMenu")
        difficulty = ffichecks.checkinteger(1, difficulty)
        repentogon.L_CharacterMenu_SetDifficulty(difficulty)
    end,
    SetIsCharacterUnlocked = function(isUnlocked)
        ffichecks.checkmainmenu("CharacterMenu")
        isUnlocked = ffichecks.checkboolean(1, isUnlocked)
        repentogon.L_CharacterMenu_SetIsCharacterUnlocked(isUnlocked)
    end,
    SetScrollSpeed = function(speed)
        ffichecks.checkmainmenu("CharacterMenu")
        speed = ffichecks.checknumber(1, speed)
        repentogon.L_CharacterMenu_SetScrollSpeed(speed)
    end,
    SetSelectedCharacterMenu = function(menu, updateGraphics) 
        ffichecks.checkmainmenu("CharacterMenu")
        menu = ffichecks.checkinteger(1, menu)
        updateGraphics = ffichecks.optboolean(updateGraphics, false)
        repentogon.L_CharacterMenu_SetSelectedCharacterMenu(menu, updateGraphics)
    end,
    SetSelectedCharacterID = function(charID, updateWheel, skipRotation) 
        ffichecks.checkmainmenu("CharacterMenu")
        charID = ffichecks.checkinteger(1, charID)
        updateWheel = ffichecks.optboolean(updateWheel, false)
        skipRotation = ffichecks.optboolean(skipRotation, false)
        repentogon.L_CharacterMenu_SetSelectedCharacterID(charID, updateWheel, skipRotation)
    end,
    SwapCharacterMenu = function(force) 
        ffichecks.checkmainmenu("CharacterMenu")
        force = ffichecks.optboolean(force, false)
        repentogon.L_CharacterMenu_SwapCharacterMenu(force)
    end
}

CharacterMenu.GetEastereggPageSprite = CharacterMenu.GetEasterEggPageSprite -- deprecated