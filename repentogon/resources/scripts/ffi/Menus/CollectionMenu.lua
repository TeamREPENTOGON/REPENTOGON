ffi.cdef[[
    struct Sprite* L_CollectionMenu_GetCollectionMenuSprite();
    struct Sprite* L_CollectionMenu_GetDeathScreenSprite();
    int L_CollectionMenu_GetSelectedElement();
    int L_CollectionMenu_GetSelectedPage();
    void L_CollectionMenu_SetSelectedElement(int);
    void L_CollectionMenu_SetSelectedPage(int page);
]]

local repentogon = ffidll

CollectionMenu = {
    GetCollectionMenuSprite = function()
        ffichecks.checkmainmenu("CollectionMenu")
        return repentogon.L_CollectionMenu_GetCollectionMenuSprite()
    end,
    GetDeathScreenSprite = function()
        ffichecks.checkmainmenu("CollectionMenu")
        return repentogon.L_CollectionMenu_GetDeathScreenSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("CollectionMenu")
        return repentogon.L_CollectionMenu_GetSelectedElement()
    end,
    GetSelectedPage = function()
        ffichecks.checkmainmenu("CollectionMenu")
        return repentogon.L_CollectionMenu_GetSelectedPage()
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("CollectionMenu")
        ffichecks.checkinteger(1, element)
        repentogon.L_CollectionMenu_SetSelectedElement(element)
    end,
    SetSelectedPage = function(page)
        ffichecks.checkmainmenu("CollectionMenu")
        ffichecks.checkinteger(1, page)
        repentogon.L_CollectionMenu_SetSelectedPage(page)
    end,
}