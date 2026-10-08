ffi.cdef[[
    struct Sprite* L_KeyConfigMenu_GetSprite();
    int L_KeyConfigMenu_GetSelectedColumn();
    int L_KeyConfigMenu_GetSelectedElement();
    bool L_KeyConfigMenu_IsEditActive();
    void L_KeyConfigMenu_SetSelectedColumn(int);
    void L_KeyConfigMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

KeyConfigMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        return repentogon.L_KeyConfigMenu_GetSprite()
    end,
    GetSelectedColumn = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        return repentogon.L_KeyConfigMenu_GetSelectedColumn()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        return repentogon.L_KeyConfigMenu_GetSelectedElement()
    end,
    IsEditActive = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        return repentogon.L_KeyConfigMenu_IsEditActive()
    end,
    SetSelectedColumn = function(element)
        ffichecks.checkmainmenu("KeyConfigMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_KeyConfigMenu_SetSelectedColumn(element)
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("KeyConfigMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_KeyConfigMenu_SetSelectedElement(element)
    end,
}