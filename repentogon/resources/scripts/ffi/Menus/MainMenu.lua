ffi.cdef[[
    struct Sprite* L_MainMenu_GetGameMenuSprite();
    struct Sprite* L_MainMenu_GetContinueWidgetSprite();
    int L_MainMenu_GetSelectedElement();
    void L_MainMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

MainMenu = {
    GetGameMenuSprite = function()
        ffichecks.checkmainmenu("MainMenu")
        local result = repentogon.L_MainMenu_GetGameMenuSprite() return result
    end,
    GetContinueWidgetSprite = function()
        ffichecks.checkmainmenu("MainMenu")
        local result = repentogon.L_MainMenu_GetContinueWidgetSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("MainMenu")
        local result = repentogon.L_MainMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("MainMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_MainMenu_SetSelectedElement(element)
    end,
}