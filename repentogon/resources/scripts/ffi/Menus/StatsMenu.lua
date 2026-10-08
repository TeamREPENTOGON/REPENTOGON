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
        return repentogon.L_StatsMenu_GetSecretsMenuSprite()
    end,
    GetSecretsMenuCursorLeftSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuCursorLeftSprite()
    end,
    GetSecretsMenuCursorRightSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuCursorRightSprite()
    end,
    GetSecretsMenuMiniSprite1 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite1()
    end,
    GetSecretsMenuMiniSprite2 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite2()
    end,
    GetSecretsMenuMiniSprite3 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite3()
    end,
    GetSecretsMenuMiniSprite4 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite4()
    end,
    GetSecretsMenuMiniSprite5 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite5()
    end,
    GetSecretsMenuMiniSprite6 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite6()
    end,
    GetSecretsMenuMiniSprite7 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite7()
    end,
    GetSecretsMenuMiniSprite8 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite8()
    end,
    GetSecretsMenuMiniSprite9 = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSecretsMenuMiniSprite9()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetSelectedElement()
    end,
    GetStatsMenuSprite = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_GetStatsMenuSprite()
    end,
    IsSecretsMenuVisible = function()
        ffichecks.checkmainmenu("StatsMenu")
        return repentogon.L_StatsMenu_IsSecretsMenuVisible()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("StatsMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_StatsMenu_SetSelectedElement(element)
    end,
}