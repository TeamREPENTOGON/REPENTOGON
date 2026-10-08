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
        return repentogon.L_PauseMenu_GetCompletionMarksSprite()
    end,
    GetMyStuffSprite = function()
        return repentogon.L_PauseMenu_GetMyStuffSprite()
    end,
    GetSelectedElement = function()
        return repentogon.L_PauseMenu_GetSelectedElement()
    end,
    GetSprite = function()
        return repentogon.L_PauseMenu_GetSprite()
    end,
    GetState = function()
        return repentogon.L_PauseMenu_GetState()
    end,
    GetStatsSprite = function()
        return repentogon.L_PauseMenu_GetStatsSprite()
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