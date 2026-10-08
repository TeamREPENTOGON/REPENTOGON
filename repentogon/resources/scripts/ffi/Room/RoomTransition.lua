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
        return repentogon.L_RoomTransition_GetPlayerExtraPortraitSprite(playerIndex)
    end,
    GetTransitionMode = function()
        return repentogon.L_RoomTransition_GetTransitionMode()
    end,
    GetVersusScreenSprite = function()
        return repentogon.L_RoomTransition_GetVersusScreenSprite()
    end,
    IsRenderingBossIntro = function()
        return repentogon.L_RoomTransition_IsRenderingBossIntro()
    end,
    StartBossIntro = function(BossID1, BossID2)
        BossID1 = ffichecks.checkinteger(1, BossID1)
        BossID2 = ffichecks.optnumber(BossID2, 0)
        repentogon.L_RoomTransition_StartBossIntro(BossID1, BossID2)
    end,
}