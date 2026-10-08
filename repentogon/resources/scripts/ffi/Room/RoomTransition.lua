ffi.cdef [[
    struct Sprite* L_RoomTransition_GetPlayerExtraPortraitSprite(int);
    int L_RoomTransition_GetTransitionMode();
    struct Sprite* L_RoomTransition_GetVersusScreenSprite();
    bool L_RoomTransition_IsRenderingBossIntro();
    void L_RoomTransition_StartBossIntro(unsigned int, unsigned int);
]]
local repentogon = ffidll

RoomTransition = {
    GetPlayerExtraPortraitSprite = function(playerIndex)
        playerIndex = ffichecks.optnumber(playerIndex, 0)
        local result = repentogon.L_RoomTransition_GetPlayerExtraPortraitSprite(playerIndex) return result
    end,
    GetTransitionMode = function()
        local result = repentogon.L_RoomTransition_GetTransitionMode() return result
    end,
    GetVersusScreenSprite = function()
        local result = repentogon.L_RoomTransition_GetVersusScreenSprite() return result
    end,
    IsRenderingBossIntro = function()
        local result = repentogon.L_RoomTransition_IsRenderingBossIntro() return result
    end,
    StartBossIntro = function(BossID1, BossID2)
        BossID1 = ffichecks.checkinteger(1, BossID1)
        BossID2 = ffichecks.optnumber(BossID2, 0)
        repentogon.L_RoomTransition_StartBossIntro(BossID1, BossID2)
    end,
}