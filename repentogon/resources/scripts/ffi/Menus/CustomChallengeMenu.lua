ffi.cdef[[
    struct Sprite* L_CustomChallengeMenu_GetSprite();
    int L_CustomChallengeMenu_GetSelectedChallengeID();
    void L_CustomChallengeMenu_SetSelectedChallengeID(int);
]]

local repentogon = ffidll

CustomChallengeMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("CustomChallengeMenu")
        return repentogon.L_CustomChallengeMenu_GetSprite()
    end,
    GetSelectedChallengeID = function()
        ffichecks.checkmainmenu("CustomChallengeMenu")
        return repentogon.L_CustomChallengeMenu_GetSelectedChallengeID()
    end,
    SetSelectedChallengeID = function(element)
        ffichecks.checkmainmenu("CustomChallengeMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_CustomChallengeMenu_SetSelectedChallengeID(element)
    end,
}