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
        local result = repentogon.L_DailyChallengeMenu_GetSprite() return result
    end,
    GetLeaderboardSprite = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetLeaderboardSprite() return result
    end,
    GetLeaderboardScoreMenuSprite = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetLeaderboardScoreMenuSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetSelectedElement() return result
    end,
    GetState = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetState() return result
    end,
    GetTimeLeftHours = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetTimeLeftHours() return result
    end,
    GetTimeLeftMinutes = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetTimeLeftMinutes() return result
    end,
    GetTimeLeftSeconds = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_GetTimeLeftSeconds() return result
    end,
    IsLeaderboardVisible = function()
        ffichecks.checkmainmenu("DailyChallengeMenu")
        local result = repentogon.L_DailyChallengeMenu_IsLeaderboardVisible() return result
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