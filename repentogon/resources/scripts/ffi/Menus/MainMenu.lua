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
        return repentogon.L_MainMenu_GetGameMenuSprite()
    end,
    GetContinueWidgetSprite = function()
        ffichecks.checkmainmenu("MainMenu")
        return repentogon.L_MainMenu_GetContinueWidgetSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("MainMenu")
        return repentogon.L_MainMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("MainMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_MainMenu_SetSelectedElement(element)
    end,
}