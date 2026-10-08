ffi.cdef[[
    struct ScoreSheet {
        int StageBonus : 0x0;
        int SchwagBonus : 0x4;
        int BlueBabyBonus : 0x8;
        int LambBonus : 0xc;
        int MegaSatanBonus : 0x10;
        int MotherBonus : 0x14;
        int DeliriumBonus : 0x18;
        int BeastBonus : 0x1c;
        int RushBonus : 0x20;
        int ExplorationBonus : 0x24;
        int DamagePenalty : 0x28;
        int TimePenalty : 0x2c;
        int ItemPenalty : 0x30;
        int TotalScore : 0x34;
        int RunTimeLevel : 0x38;
        int RunTimeLevelType : 0x3c;
        uint32_t RunTime : 0x40;
        int RunEnding : 0x58;
    } : 0x90;

    struct ScoreSheet* L_ScoreSheet_Get();
    void L_ScoreSheet_AddFinishedStage(int, int, unsigned int);
    void L_ScoreSheet_Calculate();
]]

local repentogon = ffidll

local ScoreSheetMT
ScoreSheetMT = {
    __type = "ScoreSheet",
}

setmetatable(ScoreSheetMT, { __index = function() end })
ScoreSheetMT.__index = ScoreSheetMT

local ScoreSheetT = ffi.metatype("struct ScoreSheet", ScoreSheetMT)

local function GetScoreSheet()
    return repentogon.L_ScoreSheet_Get()
end

ScoreSheet = {
    AddFinishedStage = function(stage, stageType, time)
        stage = ffichecks.checkinteger(1, stage)
        stageType = ffichecks.checkinteger(2, stageType)
        time = ffichecks.checkinteger(3, time)
        repentogon.L_ScoreSheet_AddFinishedStage(stage, stageType, time)
    end,
    Calculate = function()
        repentogon.L_ScoreSheet_Calculate()
    end,
    GetBeastBonus = function()
        return GetScoreSheet().BeastBonus
    end,
    GetBlueBabyBonus = function()
        return GetScoreSheet().BlueBabyBonus
    end,
    GetDamagePenalty = function()
        return GetScoreSheet().DamagePenalty
    end,
    GetDeliriumBonus = function()
        return GetScoreSheet().DeliriumBonus
    end,
    GetExplorationBonus = function()
        return GetScoreSheet().ExplorationBonus
    end,
    GetItemPenalty = function()
        return GetScoreSheet().ItemPenalty
    end,
    GetLambBonus = function()
        return GetScoreSheet().LambBonus
    end,
    GetMegaSatanBonus = function()
        return GetScoreSheet().MegaSatanBonus
    end,
    GetMotherBonus = function()
        return GetScoreSheet().MotherBonus
    end,
    GetRunEnding = function()
        return GetScoreSheet().RunEnding
    end,
    GetRunTime = function()
        return GetScoreSheet().RunTime
    end,
    GetRunTimeLevel = function()
        return GetScoreSheet().RunTimeLevel
    end,
    GetRunTimeLevelType = function()
        return GetScoreSheet().RunTimeLevelType
    end,
    GetRushBonus = function()
        return GetScoreSheet().RushBonus
    end,
    GetSchwagBonus = function()
        return GetScoreSheet().SchwagBonus
    end,
    GetStageBonus = function()
        return GetScoreSheet().StageBonus
    end,
    GetTimePenalty = function()
        return GetScoreSheet().TimePenalty
    end,
    GetTotalScore = function()
        return GetScoreSheet().TotalScore
    end,
    SetRunEnding = function(ending)
        ending = ffichecks.checkinteger(1, ending)
        GetScoreSheet().RunEnding = ending
    end,
}