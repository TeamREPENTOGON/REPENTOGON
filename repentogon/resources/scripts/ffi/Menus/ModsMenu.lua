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
        return repentogon.L_ModsMenu_GetSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("ModsMenu")
        return repentogon.L_ModsMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("ModsMenu")
        ffichecks.checkinteger(1, element)
        repentogon.L_ModsMenu_SetSelectedElement(element)
    end,
    WasListEdited = function()
        ffichecks.checkmainmenu("ModsMenu")
        return repentogon.L_ModsMenu_WasListEdited()
    end,
}