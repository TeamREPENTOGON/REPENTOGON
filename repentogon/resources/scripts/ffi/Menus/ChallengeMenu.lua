ffi.cdef[[
    struct Sprite* L_ChallengeMenu_GetSprite();
    int L_ChallengeMenu_GetSelectedChallengeID();
    void L_ChallengeMenu_SetSelectedChallengeID(int);
]]

local repentogon = ffidll

ChallengeMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("ChallengeMenu")
        local result = repentogon.L_ChallengeMenu_GetSprite() return result
    end,
    GetSelectedChallengeID = function()
        ffichecks.checkmainmenu("ChallengeMenu")
        local result = repentogon.L_ChallengeMenu_GetSelectedChallengeID() return result
    end,
    SetSelectedChallengeID = function(challengeID)
        ffichecks.checkmainmenu("ChallengeMenu")
        challengeID = ffichecks.checkinteger(1, challengeID)
        repentogon.L_ChallengeMenu_SetSelectedChallengeID(challengeID)
    end,
}