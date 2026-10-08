ffi.cdef[[
    struct Sprite* L_OptionsMenu_GetOptionsMenuSprite();
    struct Sprite* L_OptionsMenu_GetGammaWidgetSprite();
    int L_OptionsMenu_GetSelectedElement();
    void L_OptionsMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

OptionsMenu = {
    GetOptionsMenuSprite = function()
        return repentogon.L_OptionsMenu_GetOptionsMenuSprite()
    end,
    GetGammaWidgetSprite = function()
        return repentogon.L_OptionsMenu_GetGammaWidgetSprite()
    end,
    GetSelectedElement = function()
        return repentogon.L_OptionsMenu_GetSelectedElement()
    end,
    SetSelectedElement = function(element)
        element = ffichecks.checkinteger(1, element)
        repentogon.L_OptionsMenu_SetSelectedElement(element)
    end,
}