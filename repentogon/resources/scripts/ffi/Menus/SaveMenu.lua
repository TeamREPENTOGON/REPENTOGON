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
        local result = repentogon.L_SaveMenu_GetSaveSelectMenuSprite() return result
    end,
    GetDeleteButtonSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetDeleteButtonSprite() return result
    end,
    GetDeletePopupSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetDeletePopupSprite() return result
    end,
    GetSave1DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetSave1DrawingSprite() return result
    end,
    GetSave2DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetSave2DrawingSprite() return result
    end,
    GetSave3DrawingSprite = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetSave3DrawingSprite() return result
    end,
    GetSelectedElement = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_GetSelectedElement() return result
    end,
    IsDeleteActive = function()
        ffichecks.checkmainmenu("SaveMenu")
        local result = repentogon.L_SaveMenu_IsDeleteActive() return result
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