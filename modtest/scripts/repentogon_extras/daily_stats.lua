local persistentGameData = Isaac.GetPersistentGameData()
local WinStreak=0
local TotalDailies=0

local ScheduleRefresh=true

local StreakSheet=Sprite()
local TotalSheet=Sprite()

local StreakPos=Vector(212,12)+Vector(36,24)
StreakSheet.Offset=Vector(-36,-24)

local TotalStreakPos=Vector(286,90)

local TotalStreakText="Total Runs: 0"
local TextLen=38

local AssetsLoaded=false

local font=Font()
font:Load("font/teammeatfont10.fnt")
local fontcolor=KColor(0.20,0.15,0.1,1)

local function IsGivenMenuEntry(id)              --takes MainMenu entry, returns bool, should really have a proper function instead of this smh
    local menuiddetect=(Isaac.WorldToMenuPosition(id,Vector(240,136))-Vector(Isaac.GetScreenWidth(),Isaac.GetScreenHeight())/2):LengthSquared()
    return menuiddetect<1800
end

local function RefreshDailyStats()
    ScheduleRefresh=true
end

local function LoadAssets()
    if #StreakSheet:GetDefaultAnimation()<=0 then
        StreakSheet:Load("gfx/ui/main menu/winstreakwidget.anm2",true)
        StreakSheet:SetFrame("Idle",0)

        TotalSheet:Load("gfx/ui/main menu/seedselectionwidget.anm2",true)
        TotalSheet:SetFrame("Eggs",0)
    end
    if not font:IsLoaded() then
        font:Load("font/teammeatfont10.fnt")
    end
    if font:IsLoaded() and #(#StreakSheet:GetDefaultAnimation())>0 then
        AssetsLoaded=true
        TextLen=font:GetStringWidthUTF8(TotalStreakText)/2
        Isaac.RemoveCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,LoadAssets)

local function RenderDailyStats()
    if ScheduleRefresh and IsGivenMenuEntry(MainMenu.DAILYRUN) then
        WinStreak=Isaac.GetPersistentGameData():GetEventCounter(EventCounter.DAILYS_STREAK)
        TotalDailies=Isaac.GetPersistentGameData():GetEventCounter(EventCounter.DAILYS_PLAYED)
        if WinStreak>0 then
            StreakSheet:SetFrame("Good",0)
        elseif WinStreak<0 then
            StreakSheet:SetFrame("Bad",0)
        else StreakSheet:SetFrame("Idle",0)
        end
        TotalStreakText="Total Runs: "..TotalDailies
        TextLen=font:GetStringWidthUTF8(TotalStreakText)/2
    end
    local pos=Isaac.WorldToMenuPosition(MainMenu.DAILYRUN,StreakPos)
    StreakSheet:Render(pos)
    font:DrawString(WinStreak,pos.X,pos.Y,fontcolor,0,false)
    pos=Isaac.WorldToMenuPosition(MainMenu.DAILYRUN,Vector(286,90))
    TotalSheet:RenderLayer(0,pos)
    font:DrawStringUTF8(TotalStreakText,pos.X-TextLen,pos.Y-9,fontcolor,0,false)
end

Isaac.AddCallback(REPENTOGON,ModCallbacks.MC_PRE_GAME_EXIT,RefreshDailyStats)
Isaac.AddCallback(REPENTOGON,ModCallbacks.MC_MAIN_MENU_RENDER,RenderDailyStats)
