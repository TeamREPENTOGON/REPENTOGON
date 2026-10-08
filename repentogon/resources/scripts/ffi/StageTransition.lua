ffi.cdef[[
    struct StageTransition {
    bool SameStage : 0x10;
    } : 0x20;

    struct StageTransition* L_StageTransition_Get();
]]

local repentogon = ffidll

local StageTransitionMT
StageTransitionMT = {
    __type = "StageTransition",
}

setmetatable(StageTransitionMT, { __index = function() end })
StageTransitionMT.__index = StageTransitionMT

local StageTransitionT = ffi.metatype("struct StageTransition", StageTransitionMT)

local function GetStageTransition()
    return repentogon.L_StageTransition_Get()
end

StageTransition = {
    GetSameStage = function()
        return GetStageTransition().SameStage;
    end,
    SetSameStage = function(value)
        value = ffichecks.checkboolean(1, value)
        GetStageTransition().SameStage = value;
    end,
}