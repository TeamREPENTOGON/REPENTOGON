ffi.cdef[[
    struct Sprite* L_ModsMenu_GetSprite();
    int L_ModsMenu_GetSelectedElement();
    void L_ModsMenu_SetSelectedElement(int);
    bool L_ModsMenu_WasListEdited();
]]

local repentogon = ffidll

ModsMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("ModsMenu")
        local result = repentogon.L_ModsMenu_GetSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("ModsMenu")
        local result = repentogon.L_ModsMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("ModsMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_ModsMenu_SetSelectedElement(element)
    end,
    WasListEdited = function()
        ffichecks.checkmainmenu("ModsMenu")
        local result = repentogon.L_ModsMenu_WasListEdited() return result
    end,
}