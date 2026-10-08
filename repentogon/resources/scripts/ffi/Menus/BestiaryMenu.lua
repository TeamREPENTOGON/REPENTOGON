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
        local result = repentogon.L_BestiaryMenu_GetBestiaryMenuSprite() return result
    end,
    GetDeathScreenSprite = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetDeathScreenSprite() return result
    end,
    GetEnemySprite = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetEnemySprite() return result
    end,
    GetNumBossPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetNumBossPages() return result
    end,
    GetNumMonsterPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetNumMonsterPages() return result
    end,
    GetNumPages = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetNumPages() return result
    end,
    GetSelectedPage = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetSelectedPage() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("BestiaryMenu")
        local result = repentogon.L_BestiaryMenu_GetSelectedElement() return result
    end,
    SetSelectedPage = function(page)
        ffichecks.checkmainmenu("BestiaryMenu")
        page = ffichecks.checkinteger(1, page)
        repentogon.L_BestiaryMenu_SetSelectedPage(page);
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("BestiaryMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_BestiaryMenu_SetSelectedElement(element);
    end,
}