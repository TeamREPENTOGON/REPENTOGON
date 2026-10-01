ffi.cdef[[
    struct Sprite* L_TitleMenu_GetSprite();
]]

local repentogon = ffidll

TitleMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("TitleMenu")
        return repentogon.L_TitleMenu_GetSprite()
    end,
}