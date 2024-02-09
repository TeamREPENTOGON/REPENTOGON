ffi.cdef[[
void L_StageTransition_SetSame(bool);
bool L_StageTransition_GetSame();
]]

local repentogon = ffidll

StageTransition = {}

StageTransition.SetSameStage = function(sameStage)
	ffichecks.checkboolean(1, sameStage)
	
	repentogon.L_StageTransition_SetSame(sameStage)
end

StageTransition.GetSameStage = function() return repentogon.L_StageTransition_GetSame() end