ffi.cdef[[
    struct Sprite* L_SpecialSeedsMenu_GetSprite();
    int L_SpecialSeedsMenu_GetSelectedElement();
    void L_SpecialSeedsMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

SpecialSeedsMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        return repentogon.L_SpecialSeedsMenu_GetSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        return repentogon.L_SpecialSeedsMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_SpecialSeedsMenu_SetSelectedElement(element)
    end,
}