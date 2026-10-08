ffi.cdef[[
    struct Sprite* L_SaveMenu_GetSaveSelectMenuSprite();
    struct Sprite* L_SaveMenu_GetDeleteButtonSprite();
    struct Sprite* L_SaveMenu_GetDeletePopupSprite();
    struct Sprite* L_SaveMenu_GetSave1DrawingSprite();
    struct Sprite* L_SaveMenu_GetSave2DrawingSprite();
    struct Sprite* L_SaveMenu_GetSave3DrawingSprite();
    int L_SaveMenu_GetSelectedElement();
    bool L_SaveMenu_IsDeleteActive();
    void L_SaveMenu_SetSlotSpritesheet(int, const char*);
    void L_SaveMenu_SetSelectedElement(int);
]]

local repentogon = ffidll

SaveMenu = {
    GetSaveSelectMenuSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetSaveSelectMenuSprite()
    end,
    GetDeleteButtonSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetDeleteButtonSprite()
    end,
    GetDeletePopupSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetDeletePopupSprite()
    end,
    GetSave1DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetSave1DrawingSprite()
    end,
    GetSave2DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetSave2DrawingSprite()
    end,
    GetSave3DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetSave3DrawingSprite()
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_GetSelectedElement()
    end,
    IsDeleteActive = function()
        ffichecks.checkmainmenu("SaveMenu")
        return repentogon.L_SaveMenu_IsDeleteActive()
    end,
    SetSlotSpritesheet = function(slot, spritesheet)
        ffichecks.checkmainmenu("SaveMenu")
        slot = ffichecks.checkinteger(1, slot)
        spritesheet = ffichecks.checkstring(2, spritesheet)
        repentogon.L_SaveMenu_SetSlotSpritesheet(slot, spritesheet)
    end,
    SetSelectedElement = function(element)
        ffichecks.checkmainmenu("SaveMenu")
        element = ffichecks.checkinteger(1, element)
        repentogon.L_SaveMenu_SetSelectedElement(element)
    end,
}