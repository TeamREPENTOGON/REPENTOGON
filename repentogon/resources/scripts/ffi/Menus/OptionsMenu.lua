ffi.cdef[[
    struct Sprite* L_OptionsMenu_GetOptionsMenuSprite();
    struct Sprite* L_OptionsMenu_GetGammaWidgetSprite();
    int L_OptionsMenu_GetSelectedElement();
    void L_OptionsMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

OptionsMenu = {
    GetOptionsMenuSprite = function()
        local result = repentogon.L_OptionsMenu_GetOptionsMenuSprite() return result
    end,
    GetGammaWidgetSprite = function()
        local result = repentogon.L_OptionsMenu_GetGammaWidgetSprite() return result
    end,
    GetSelectedElement = function()
        local result = repentogon.L_OptionsMenu_GetSelectedElement() return result
    end,
    SetSelectedElement = function(element)
        element = ffichecks.checkinteger(1, element)
        repentogon.L_OptionsMenu_SetSelectedElement(element)
    end,
}