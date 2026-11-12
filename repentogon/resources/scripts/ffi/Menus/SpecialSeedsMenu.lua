ffi.cdef[[
    struct Sprite* L_SpecialSeedsMenu_GetSprite();
    int L_SpecialSeedsMenu_GetSelectedElement();
    void L_SpecialSeedsMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

SpecialSeedsMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        local result = repentogon.L_SpecialSeedsMenu_GetSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        local result = repentogon.L_SpecialSeedsMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("SpecialSeedsMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_SpecialSeedsMenu_SetSelectedElement(element)
    end,
}