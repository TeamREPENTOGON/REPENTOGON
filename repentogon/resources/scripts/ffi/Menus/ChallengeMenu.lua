ffi.cdef[[
    struct Sprite* L_ChallengeMenu_GetSprite();
    int L_ChallengeMenu_GetSelectedChallengeID();
    void L_ChallengeMenu_SetSelectedChallengeID(int);
]]

local repentogon = ffidll

ChallengeMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("ChallengeMenu")
        return repentogon.L_ChallengeMenu_GetSprite();
    end,
    GetSelectedChallengeID = function()
        ffichecks.checkmainmenu("ChallengeMenu")
        return repentogon.L_ChallengeMenu_GetSelectedChallengeID();
    end,
    SetSelectedChallengeID = function(challengeID)
        ffichecks.checkmainmenu("ChallengeMenu")
        challengeID = ffichecks.checkinteger(1, challengeID)
        repentogon.L_ChallengeMenu_SetSelectedChallengeID(challengeID)
    end,
}