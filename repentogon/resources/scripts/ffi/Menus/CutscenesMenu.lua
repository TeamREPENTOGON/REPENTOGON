ffi.cdef[[
    struct Sprite* L_CutscenesMenu_GetSprite();
    int L_CutscenesMenu_GetSelectedElement();
    void L_CutscenesMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

CutscenesMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("CutscenesMenu")
        return repentogon.L_CutscenesMenu_GetSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("CutscenesMenu")
        return repentogon.L_CutscenesMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("CutscenesMenu")
        ffichecks.checkinteger(1, element)
        repentogon.L_CutscenesMenu_SetSelectedElement(element)
    end,
}