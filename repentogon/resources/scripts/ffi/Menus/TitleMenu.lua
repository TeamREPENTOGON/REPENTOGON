ffi.cdef[[
    struct Sprite* L_TitleMenu_GetSprite();
]]

local repentogon = ffidll

TitleMenu = {
    GetSprite = function()
        ffichecks.checkmainmenu("TitleMenu")
        local result = repentogon.L_TitleMenu_GetSprite() return result
    end,
}