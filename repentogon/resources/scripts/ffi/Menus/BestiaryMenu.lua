ffi.cdef[[
    struct Sprite* L_BestiaryMenu_GetBestiaryMenuSprite();
    struct Sprite* L_BestiaryMenu_GetDeathScreenSprite();
    struct Sprite* L_BestiaryMenu_GetEnemySprite();
    int L_BestiaryMenu_GetNumBossPages();
    int L_BestiaryMenu_GetNumMonsterPages();
    int L_BestiaryMenu_GetNumPages();
    int L_BestiaryMenu_GetSelectedPage();
    int L_BestiaryMenu_GetSelectedElement();
    void L_BestiaryMenu_SetSelectedPage(int);
    void L_BestiaryMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

BestiaryMenu = {
    GetBestiaryMenuSprite = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetBestiaryMenuSprite();
    end,
    GetDeathScreenSprite = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetDeathScreenSprite();
    end,
    GetEnemySprite = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetEnemySprite();
    end,
    GetNumBossPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetNumBossPages();
    end,
    GetNumMonsterPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetNumMonsterPages();
    end,
    GetNumPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetNumPages();
    end,
    GetSelectedPage = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetSelectedPage();
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        return repentogon.L_BestiaryMenu_GetSelectedElement();
    end,
    SetSelectedPage = function(page)
        ffichecks.checkmainmenu("BestiaryMenu")
        ffichecks.checkinteger(1, page)
        repentogon.L_BestiaryMenu_SetSelectedPage(page);
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("BestiaryMenu")
        ffichecks.checkinteger(1, element)
        repentogon.L_BestiaryMenu_SetSelectedElement(element);
    end,
}