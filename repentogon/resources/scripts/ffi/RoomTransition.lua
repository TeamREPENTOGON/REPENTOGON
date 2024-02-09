ffi.cdef[[
ANM2* L_RoomTransition_GetVersusScreenSprite();
void L_RoomTransition_StartBossIntro(int, int);
int L_RoomTransition_GetTransitionMode();
bool L_RoomTransition_IsRenderingBossIntro();
]]

local repentogon = ffidll

RoomTransition = {}

RoomTransition.GetVersusScreenSprite = function() return repentogon.L_RoomTransition_GetVersusScreenSprite() end

RoomTransition.StartBossIntro = function(bossID1, bossID2)
	ffichecks.checknumber(1, bossID1)
	ffichecks.optnumber(2, bossID2, 0)
	
	repentogon.L_RoomTransirtion_StartBossIntro(bossID1, bossID2)
end

RoomTransition.GetTransitionMode = function() return repentogon.L_RoomTransition_GetTransitionMode() end

RoomTransition.IsRenderingBossIntro = function() return repentogon.L_RoomTransition_IsRenderingBossIntro() end