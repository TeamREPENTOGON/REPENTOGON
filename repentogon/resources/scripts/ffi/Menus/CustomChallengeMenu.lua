ffi.cdef[[
    struct Sprite* L_CustomChallengeMenu_GetSprite();
    int L_CustomChallengeMenu_GetSelectedChallengeID();
    void L_CustomChallengeMenu_SetSelectedChallengeID(int);
]]

local repentogon = ffidll

CustomChallengeMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("CustomChallengeMenu")
        local result = repentogon.L_CustomChallengeMenu_GetSprite() return result
    end,
    GetSelectedChallengeID = function()
        ffichecks.checkmainmenu("CustomChallengeMenu")
        local result = repentogon.L_CustomChallengeMenu_GetSelectedChallengeID() return result
    end,
    SetSelectedChallengeID = function(element)
        ffichecks.checkmainmenu("CustomChallengeMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_CustomChallengeMenu_SetSelectedChallengeID(element)
    end,
}