ffi.cdef[[
    struct Sprite* L_StatsMenu_GetSecretsMenuSprite();
    struct Sprite* L_StatsMenu_GetSecretsMenuCursorLeftSprite();
    struct Sprite* L_StatsMenu_GetSecretsMenuCursorRightSprite();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite1();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite2();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite3();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite4();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite5();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite6();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite7();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite8();
    struct Sprite* L_StatsMenu_GetSecretsMenuMiniSprite9();
    int L_StatsMenu_GetSelectedElement();
    struct Sprite* L_StatsMenu_GetStatsMenuSprite();
    bool L_StatsMenu_IsSecretsMenuVisible();
    void L_StatsMenu_SetSelectedElement(int element);
]]

local repentogon = ffidll

StatsMenu = {
    GetSecretsMenuSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuSprite() return result
    end,
    GetSecretsMenuCursorLeftSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuCursorLeftSprite() return result
    end,
    GetSecretsMenuCursorRightSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuCursorRightSprite() return result
    end,
    GetSecretsMenuMiniSprite1 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite1() return result
    end,
    GetSecretsMenuMiniSprite2 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite2() return result
    end,
    GetSecretsMenuMiniSprite3 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite3() return result
    end,
    GetSecretsMenuMiniSprite4 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite4() return result
    end,
    GetSecretsMenuMiniSprite5 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite5() return result
    end,
    GetSecretsMenuMiniSprite6 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite6() return result
    end,
    GetSecretsMenuMiniSprite7 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite7() return result
    end,
    GetSecretsMenuMiniSprite8 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite8() return result
    end,
    GetSecretsMenuMiniSprite9 = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSecretsMenuMiniSprite9() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetSelectedElement() return result
    end,
    GetStatsMenuSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_GetStatsMenuSprite() return result
    end,
    IsSecretsMenuVisible = function()
        ffichecks.checkmainmenu("StatsMenu")
        local result = repentogon.L_StatsMenu_IsSecretsMenuVisible() return result
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("StatsMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_StatsMenu_SetSelectedElement(element)
    end,
}