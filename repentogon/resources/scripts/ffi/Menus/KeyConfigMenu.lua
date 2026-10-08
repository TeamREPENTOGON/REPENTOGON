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
        local result = repentogon.L_KeyConfigMenu_GetSprite() return result
    end,
    GetSelectedColumn = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        local result = repentogon.L_KeyConfigMenu_GetSelectedColumn() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        local result = repentogon.L_KeyConfigMenu_GetSelectedElement() return result
    end,
    IsEditActive = function()
        ffichecks.checkmainmenu("KeyConfigMenu")
        local result = repentogon.L_KeyConfigMenu_IsEditActive() return result
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