ffi.cdef[[
    struct Sprite* L_DailyChallengeMenu_GetSprite();
    struct Sprite* L_DailyChallengeMenu_GetLeaderboardSprite();
    struct Sprite* L_DailyChallengeMenu_GetLeaderboardScoreMenuSprite();
    int L_DailyChallengeMenu_GetSelectedElement();
    int L_DailyChallengeMenu_GetState();
    int L_DailyChallengeMenu_GetTimeLeftHours();
    int L_DailyChallengeMenu_GetTimeLeftMinutes();
    int L_DailyChallengeMenu_GetTimeLeftSeconds();
    bool L_DailyChallengeMenu_IsLeaderboardVisible();
    void L_DailyChallengeMenu_SetSelectedElement(int);
    void L_DailyChallengeMenu_SetState(int);
]]

local repentogon = ffidll

DailyChallengeMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetSprite()
    end,
    GetLeaderboardSprite = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetLeaderboardSprite()
    end,
    GetLeaderboardScoreMenuSprite = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetLeaderboardScoreMenuSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetSelectedElement()
    end,
    GetState = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetState()
    end,
    GetTimeLeftHours = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetTimeLeftHours()
    end,
    GetTimeLeftMinutes = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetTimeLeftMinutes()
    end,
    GetTimeLeftSeconds = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_GetTimeLeftSeconds()
    end,
    IsLeaderboardVisible = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        return repentogon.L_DailyChallengeMenu_IsLeaderboardVisible()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("DailyChallengeMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_DailyChallengeMenu_SetSelectedElement(element)
    end,
    SetState = function(state)
        ffichecks.checkmainmenu("DailyChallengeMenu")
        state = ffichecks.checkinteger(1, state)
        repentogon.L_DailyChallengeMenu_SetState(state)
    end,
}