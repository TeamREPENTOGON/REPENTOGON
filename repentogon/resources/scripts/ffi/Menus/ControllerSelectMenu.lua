ffi.cdef[[
    struct Sprite* L_ControllerSelectMenu_GetSprite();
    int L_ControllerSelectMenu_GetSelectedElement();
    void L_ControllerSelectMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

ControllerSelectMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("ControllerSelectMenu")
        local result = repentogon.L_ControllerSelectMenu_GetSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("ControllerSelectMenu")
        local result = repentogon.L_ControllerSelectMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("ControllerSelectMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_ControllerSelectMenu_SetSelectedElement(element)
    end,
}