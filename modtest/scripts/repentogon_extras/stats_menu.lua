-- data structure: {category name, {stat name, enum, current value, value text width/2}}
local stats = {
    {
        "GAME STATS", {
        { "deaths",        EventCounter.DEATHS,          0, 0 },
        { "items",         function() return "TODO" end, 0, 0 },
        { "mom kills",     EventCounter.MOM_KILLS,       0, 0 },
        { "secrets",       function() return "TODO" end, 0, 0 },
        { "best streak",   EventCounter.BEST_STREAK,     0, 0 },
        { "",              function() return "" end,     0, 0 }, -- spacer
        { "eden tokens",   EventCounter.EDEN_TOKENS,     0, 0 },
        { "",              function() return "" end,     0, 0 }, -- spacer
        { "dailys played", EventCounter.DAILYS_PLAYED,   0, 0 },
        { "dailys won",   EventCounter.DAILYS_WON,      0, 0 },
    } },
    {
        "BOSS KILLS", {
        { "mom kills",          EventCounter.MOM_KILLS,         0, 0 },
        { "isaac kills",        EventCounter.ISAAC_KILLS,       0, 0 },
        { "satan kills",        EventCounter.SATAN_KILLS,       0, 0 },
        { "blue baby kills",    EventCounter.BLUE_BABY_KILLS,   0, 0 },
        { "lamb kills",         EventCounter.LAMB_KILLS,        0, 0 },
        { "hush kills",         EventCounter.HUSH_KILLS,        0, 0 },
        { "delirium kills",     EventCounter.DELIRIUM_KILLS,    0, 0 },
        { "mega satan kills",   EventCounter.MEGA_SATAN_KILLS,  0, 0 },
        { "mother kills",       EventCounter.MOTHER_KILLS,      0, 0 },
        { "beast kills",        EventCounter.BEAST_KILLS,       0, 0 },
        { "baby plum kills",    EventCounter.BABY_PLUM_KILLS,   0, 0 },
        { "boss rushs cleared", EventCounter.BOSSRUSHS_CLEARED, 0, 0 },
    } },
    {
        "SHOPPING", {
        { "shopkeeper killed",                EventCounter.SHOPKEEPER_KILLED,              0, 0 },
        { "items bought",                     EventCounter.SHOP_ITEMS_BOUGHT,              0, 0 },
        { "angel deals taken",                EventCounter.ANGEL_DEALS_TAKEN,              0, 0 },
        { "devil deals taken",                EventCounter.DEVIL_DEALS_TAKEN,              0, 0 },
        { "",                                 function() return "" end,                    0, 0 }, -- spacer
        { { "donation machine", "coins" },    EventCounter.DONATION_MACHINE_COUNTER,       0, 0 },
        { { "greed donation", "machine coins" }, EventCounter.GREED_DONATION_MACHINE_COUNTER, 0, 0 },
    } },
    {
        "GAMBLING", {
        { "arcades entered",                     EventCounter.ARCADES_ENTERED,                 0, 0 },
        { "shellgames played",                   EventCounter.SHELLGAMES_PLAYED,               0, 0 },
        { { "blood donation", "machine used" },  EventCounter.BLOOD_DONATION_MACHINE_USED,     0, 0 },
        { { "slot machines", "broken" },         EventCounter.SLOT_MACHINES_BROKEN,            0, 0 },
        { { "golden chests", "opened" },         EventCounter.CHESTS_OPENED_WITH_KEY,          0, 0 },
        { { "secret room", "walls broken" },     EventCounter.SECRET_ROOMS_WALLS_OPENED,       0, 0 },
    } },
    {
        "CHAOS", {
        { "rocks destroyed",            EventCounter.ROCKS_DESTROYED,        0, 0 },
        { { "tinted rocks", "destroyed" }, EventCounter.TINTED_ROCKS_DESTROYED, 0, 0 },
        { "poop destroyed",             EventCounter.POOP_DESTROYED,         0, 0 },
        { { "rainbow poop", "destroyed" }, EventCounter.RAINBOW_POOP_DESTROYED, 0, 0 },
        { "pills eaten",                EventCounter.PILLS_EATEN,            0, 0 },
        { "cards used",                 EventCounter.CARDS_USED,             0, 0 },
        { "batteries collected",        EventCounter.BATTERIES_COLLECTED,    0, 0 },
        { "battery bums killed",        EventCounter.BATTERY_BUMS_KILLED,    0, 0 },
    } },
    {
        "MISC", {
        { "beds used",                  EventCounter.BEDS_USED,              0, 0 },
        { { "battery bum", "collectible payouts" }, EventCounter.BATTERY_BUM_COLLECTIBLE_PAYOUTS, 0, 0 },
    } },
}
local StatsPos = Vector(35, 34)
local statPageAngleOffsetX = -2
local statPageNumberOffsetX = 120
local StatSheet = Sprite()
local StatSheetPos = Vector(-39, -15)
local ArrowSheet = Sprite()
local LeftArrowSheetPos = Vector(-93, -33)
local RightArrowSheetPos = Vector(-169, -25)
local ScheduleRefresh = true
local CurrentlyDisplayedCategoryID = 1
local MaxCategoryID = #stats + 1

local font = Font()
font:Load("font/teammeatfont10.fnt")
local fontcolor = KColor(0.20, 0.15, 0.1, 1)

local MaxPollCIdx = 3
local function IsActionTriggeredAll(action)
    for i = 0, MaxPollCIdx do
        if Input.IsActionTriggered(action, i) then
            return true
        end
    end
    return false
end

local function IsGivenMenuEntry(id) --takes MainMenu entry, returns bool, should really have a proper function instead of this smh
    local menuiddetect = (Isaac.WorldToMenuPosition(id, Vector(240, 136)) - Vector(Isaac.GetScreenWidth(), Isaac.GetScreenHeight()) / 2)
        :LengthSquared()
    return menuiddetect < 1800
end

local function RefreshExtraStats()
    ScheduleRefresh = true
end

local function LoadAssets()
    if #StatSheet:GetDefaultAnimation() <= 0 then
        StatSheet:Load("gfx/ui/main menu/statsmenu.anm2", true)
        StatSheet:SetFrame("Idle", 0)

        ArrowSheet:Load("gfx/ui/leaderboardmenu.anm2", true)
        ArrowSheet:SetFrame("Idle", 0)
    end
    if not font:IsLoaded() then
        font:Load("font/teammeatfont10.fnt")
    end
    if font:IsLoaded() and #(StatSheet:GetDefaultAnimation()) > 0 then
        Isaac.RemoveCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)
    end
end

Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, LoadAssets)


local function RenderExtraStats()
    if ScheduleRefresh and IsGivenMenuEntry(MainMenu.STATS) then
        local gameData = Isaac.GetPersistentGameData()
        for i, categoryData in ipairs(stats) do
            for j, stat in ipairs(categoryData[2]) do
                if type(stat[2]) == "function" then
                    stats[i][2][j][3] = stat[2]()
                else
                    stats[i][2][j][3] = gameData:GetEventCounter(stat[2])
                end
                stats[i][2][j][4] = font:GetStringWidthUTF8(tostring(stats[i][2][j][3])) / 2
            end
        end
    end

    if IsActionTriggeredAll(ButtonAction.ACTION_MENULEFT) then
        SFXManager():Play(SoundEffect.SOUND_PAPER_IN)
        CurrentlyDisplayedCategoryID = (CurrentlyDisplayedCategoryID - 1) % MaxCategoryID
        if CurrentlyDisplayedCategoryID == 0 then
            CurrentlyDisplayedCategoryID = MaxCategoryID - 1
        end
    elseif IsActionTriggeredAll(ButtonAction.ACTION_MENURIGHT) then
        SFXManager():Play(SoundEffect.SOUND_PAPER_IN)
        CurrentlyDisplayedCategoryID = math.max((CurrentlyDisplayedCategoryID + 1) % MaxCategoryID, 1)
    end

    -- render stat page over original stat page, to allow for custom content
    local posSheet = Isaac.WorldToMenuPosition(MainMenu.STATS, StatSheetPos)
    StatSheet:RenderLayer(1, posSheet)
    local posLeftArrow = Isaac.WorldToMenuPosition(MainMenu.STATS, LeftArrowSheetPos)
    ArrowSheet:RenderLayer(6, posLeftArrow)
    local posRightArrow = Isaac.WorldToMenuPosition(MainMenu.STATS, RightArrowSheetPos)
    ArrowSheet:RenderLayer(7, posRightArrow)

    local nextLineOffsetX = 0
    local nextLineOffsetY = 0

    local pos = Isaac.WorldToMenuPosition(MainMenu.STATS, StatsPos)

    local categoryData = stats[CurrentlyDisplayedCategoryID]
    font:DrawStringUTF8(categoryData[1], pos.X, pos.Y + nextLineOffsetY, fontcolor, 0, false)
    nextLineOffsetY = nextLineOffsetY + 12
    for _, stat in ipairs(categoryData[2]) do
        -- text
        if type(stat[1]) == "table" then
            nextLineOffsetY = nextLineOffsetY + 2 -- some padding aboth
            for i, text in ipairs(stat[1]) do
                font:DrawStringUTF8(text, pos.X + nextLineOffsetX, pos.Y + nextLineOffsetY, fontcolor, 0, false)
                if i ~= #stat[1] then
                    nextLineOffsetY = nextLineOffsetY + 12
                    nextLineOffsetX = nextLineOffsetX + statPageAngleOffsetX
                end
            end
            -- value
            font:DrawStringUTF8(tostring(stat[3]), pos.X + nextLineOffsetX + statPageNumberOffsetX - stat[4],
                pos.Y + nextLineOffsetY - 5, fontcolor, 0, false)

            nextLineOffsetY = nextLineOffsetY + 2 -- some padding below
        else
            font:DrawStringUTF8(stat[1], pos.X + nextLineOffsetX, pos.Y + nextLineOffsetY, fontcolor, 0, false)
            -- value
            font:DrawStringUTF8(tostring(stat[3]), pos.X + nextLineOffsetX + statPageNumberOffsetX - stat[4],
                pos.Y + nextLineOffsetY + 2, fontcolor, 0, false)
        end

        nextLineOffsetY = nextLineOffsetY + 12
        nextLineOffsetX = nextLineOffsetX + statPageAngleOffsetX
    end
end

Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_PRE_GAME_EXIT, RefreshExtraStats)
Isaac.AddCallback(REPENTOGON, ModCallbacks.MC_MAIN_MENU_RENDER, RenderExtraStats)
