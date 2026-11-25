ffi.cdef[[
    struct Sprite* L_CutscenesMenu_GetSprite();
    int L_CutscenesMenu_GetSelectedElement();
    void L_CutscenesMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

CutscenesMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("CutscenesMenu")
        local result = repentogon.L_CutscenesMenu_GetSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("CutscenesMenu")
        local result = repentogon.L_CutscenesMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("CutscenesMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_CutscenesMenu_SetSelectedElement(element)
    end,
}