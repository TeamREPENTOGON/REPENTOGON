ffi.cdef[[
    struct Sprite* L_PauseMenu_GetCompletionMarksSprite();
    struct Sprite* L_PauseMenu_GetMyStuffSprite();
    int L_PauseMenu_GetSelectedElement();
    struct Sprite* L_PauseMenu_GetSprite();
    int L_PauseMenu_GetState();
    struct Sprite* L_PauseMenu_GetStatsSprite();
    void L_PauseMenu_SetSelectedElement(int);
    void L_PauseMenu_SetState(int);
]]

local repentogon = ffidll

PauseMenu = {
    GetCompletionMarksSprite = function()
        local result = repentogon.L_PauseMenu_GetCompletionMarksSprite() return result
    end,
    GetMyStuffSprite = function()
        local result = repentogon.L_PauseMenu_GetMyStuffSprite() return result
    end,
    GetSelectedElement = function()
        local result = repentogon.L_PauseMenu_GetSelectedElement() return result
    end,
    GetSprite = function()
        local result = repentogon.L_PauseMenu_GetSprite() return result
    end,
    GetState = function()
        local result = repentogon.L_PauseMenu_GetState() return result
    end,
    GetStatsSprite = function()
        local result = repentogon.L_PauseMenu_GetStatsSprite() return result
    end,
    SetSelectedElement = function(element)
        element = ffichecks.checkinteger(1, element)
        repentogon.L_PauseMenu_SetSelectedElement(element)
    end,
    SetState = function(state)
        state = ffichecks.checkinteger(1, state)
        repentogon.L_PauseMenu_SetState(state)
    end,
}