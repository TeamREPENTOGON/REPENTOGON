-- save copy of original enums, to make it harder for mods to mess with the repentogon features
local _EventCounter = EventCounter
local _ModCallbacks = ModCallbacks
local _MainMenuType = MainMenuType

local WinStreak = 0
local TotalDailies = 0
local GetStageGoal = 0
local isAltPath = false
local isHardMode = false
local isMegaSatan = false
local diffOffsetFrameNum = 0

local ScheduleRefresh = true

local StreakSheet = Sprite()
local TotalSheet = Sprite()
--local GoalDestinationIcon = Sprite()
--local HardModeIcon = Sprite()

local LeaderboardSprite = Sprite()

local StreakPos = Vector(428, 110) + Vector(36, 24)
StreakSheet.Offset = Vector(-36, -24)

local TotalStreakText = "Total Runs: 0"
local TextLen = 38

local font = Font()
font:Load("font/teammeatex/teammeatex10.fnt")
local fontcolor = KColor(0.20, 0.15, 0.1, 1)

local function RefreshDailyStats()
    ScheduleRefresh = true
end

local function GetGoalDestination(stage, altPath)
    local goalMap = {
        [6] = 0,
        [8] = 1,
        [10] = {
            [false] = 2,
            [true] = 3
        },
        [11] = {
            [false] = 4,
            [true] = 5,
        },
    }

    if goalMap[stage] then
        if type(goalMap[stage]) == "table" then
            if goalMap[stage][altPath] then
                return goalMap[stage][altPath]
            end
        else
            return goalMap[stage]
        end
    end
    return 0
end

local function LoadAssets()
    if #StreakSheet:GetDefaultAnimation() <= 0 then
        StreakSheet:Load("gfx/ui/main menu/winstreakwidget.anm2", true)
        StreakSheet:SetFrame("Idle", 0)

        TotalSheet:Load("gfx/ui/main menu/seedselectionwidget.anm2", true)
        TotalSheet:SetFrame("Eggs", 0)

        --GoalDestinationIcon:Load("gfx/ui/hudpickups.anm2", true)
        --GoalDestinationIcon:SetAnimation("Destination", true)
        --GoalDestinationIcon:SetFrame(7)

        --HardModeIcon:Load("gfx/ui/hudpickups.anm2", true)
        --HardModeIcon:SetAnimation("Idle", true)
        --HardModeIcon:SetFrame(4)
		
		LeaderboardSprite:Load("gfx/ui/daily_destinations.anm2", true)
		LeaderboardSprite:SetAnimation("Destination", true)
		LeaderboardSprite:SetFrame(0)
    end
    if not font:IsLoaded() then
        font:Load("font/teammeatex/teammeatex10.fnt")
    end
    if font:IsLoaded() and #(StreakSheet:GetDefaultAnimation()) > 0 then
        TextLen = font:GetStringWidthUTF8(TotalStreakText) / 2
        Isaac.RemoveCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)

local function RenderDailyStats()
    if ScheduleRefresh and MenuManager:GetActiveMenu() == _MainMenuType.DAILYRUN then
        local dailyChallenge = DailyChallenge
		local challengeParam = dailyChallenge.GetChallengeParams()
        WinStreak = Isaac.GetPersistentGameData():GetEventCounter(_EventCounter.DAILYS_STREAK)
        TotalDailies = Isaac.GetPersistentGameData():GetEventCounter(_EventCounter.DAILYS_PLAYED)
        GetStageGoal = challengeParam:GetEndStage()
        isAltPath = challengeParam:IsAltPath()

        if WinStreak > 0 then
            StreakSheet:SetFrame("Good", 0)
        elseif WinStreak < 0 then
            StreakSheet:SetFrame("Bad", 0)
        else
            StreakSheet:SetFrame("Idle", 0)
        end
        TotalStreakText = "Total Runs: " .. TotalDailies
        TextLen = font:GetStringWidthUTF8(TotalStreakText) / 2
        --StageGoalText = "Goal Stage: "..GetStageGoal
        isHardMode = challengeParam:GetDifficulty() == 1
        isMegaSatan = challengeParam:IsMegaSatanRun()
		diffOffsetFrameNum = isHardMode and 7 or 0
    end
    if DailyChallengeMenu:IsLeaderboardVisible() then
        return
    end
    local pos = Isaac.WorldToMenuPosition(_MainMenuType.DAILYRUN, StreakPos)
    StreakSheet:Render(pos)
    font:DrawString(WinStreak, pos.X, pos.Y, fontcolor, 0, false)
    pos = Isaac.WorldToMenuPosition(_MainMenuType.DAILYRUN, Vector(470, 205))
    TotalSheet:RenderLayer(0, pos)
    
    local goalPosition = Isaac.WorldToMenuPosition(_MainMenuType.DAILYRUN, Vector(258, 80))
    local goalFrame = GetGoalDestination(GetStageGoal, isAltPath) + diffOffsetFrameNum

    if isMegaSatan then
        LeaderboardSprite:SetFrame(6 + diffOffsetFrameNum)
        LeaderboardSprite:RenderLayer(0, goalPosition)
        LeaderboardSprite:SetFrame(goalFrame)
        LeaderboardSprite:RenderLayer(0, goalPosition - Vector(16, 0)) --alr variant -8,48
    else
        LeaderboardSprite:SetFrame(goalFrame)
        LeaderboardSprite:RenderLayer(0, goalPosition)
    end
    --GoalDestinationIcon:RenderLayer(0, Vector(pos.X + 69, pos.Y - 32))
	
    --[[if isHardMode then
        HardModeIcon:RenderLayer(0, Vector(pos.X + 75, pos.Y - 82))
    end]]
    font:DrawStringUTF8(TotalStreakText, pos.X - TextLen, pos.Y - 9, fontcolor, 0, false)
end

Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_PRE_GAME_EXIT, RefreshDailyStats)
Isaac.AddCallback(REPENTOGON, _ModCallbacks.MC_MAIN_MENU_RENDER, RenderDailyStats)
