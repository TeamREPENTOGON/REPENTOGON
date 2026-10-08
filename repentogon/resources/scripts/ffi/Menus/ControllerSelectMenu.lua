ffi.cdef[[
    struct Sprite* L_ControllerSelectMenu_GetSprite();
    int L_ControllerSelectMenu_GetSelectedElement();
    void L_ControllerSelectMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

ControllerSelectMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("ControllerSelectMenu")
        return repentogon.L_ControllerSelectMenu_GetSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("ControllerSelectMenu")
        return repentogon.L_ControllerSelectMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("ControllerSelectMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_ControllerSelectMenu_SetSelectedElement(element)
    end,
}